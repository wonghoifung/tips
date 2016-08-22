local skynet = require "skynet"
local socket = require "socket"
local boxproto = require "boxproto"

local mode , id = ...

local function echo(id)
	local pt = boxproto.createparseinfotable() -- one parseinfo table each vm 
	local tmp = setmetatable({}, { __gc = function() boxproto.freeparseinfotable(pt) end })
	tmp[1] = 1 -- make use of gc
	boxproto.init(pt, id)
	print("[server] id:"..id)

	socket.start(id)

	local msg = boxproto.bmbegin(1)
	boxproto.bmwritecstring(msg, "start")
	boxproto.bmend(msg)
	socket.write(id, boxproto.bmtolstring(msg))
	boxproto.freemsg(msg)

	while true do
		local str = socket.read(id)
		if str then
			local chain, error = boxproto.parse(pt, id, str)
			while chain == nil do -- error ? -1
				str = socket.read(id)
				chain, error = boxproto.parse(pt, id, str)
			end
			local msg, left = boxproto.takefront(chain)
			while msg ~= nil do
				local cmd = boxproto.bmcmd(msg)
				local version = boxproto.bmversion(msg)
				local flag = boxproto.bmflag(msg)
				local sid = boxproto.bmsid(msg)
				local i32 = boxproto.bmreadi32(msg)
				local cstr = boxproto.bmreadcstring(msg)
				print("[server] cmd:"..cmd..", version:"..version..", flag:"..flag..", sid:"..sid..", i32:"..i32..", cstr:"..cstr)
				local resp = boxproto.bmbegin(cmd)
				boxproto.bmsetversion(resp, version)
				boxproto.bmsetflag(resp, flag)
				boxproto.bmsetsid(resp, sid)
				boxproto.bmwritei32(resp, i32)
				boxproto.bmwritecstring(resp, cstr)
				boxproto.bmend(resp)
				socket.write(id, boxproto.bmtolstring(resp))
				boxproto.freemsg(resp)
				boxproto.freechainnode(msg) 
				msg, left = boxproto.takefront(left)
			end
		else
			socket.close(id)
			boxproto.fini(pt, id)
			return
		end
	end
end

if mode == "agent" then
	id = tonumber(id)

	skynet.start(function()
		skynet.fork(function()
			echo(id)
			skynet.exit()
		end)
	end)
else
	local pt = boxproto.createparseinfotable() -- one parseinfo table each vm 
	local tmp = setmetatable({}, { __gc = function() boxproto.freeparseinfotable(pt) end })
	tmp[1] = 1 -- make use of gc

	local function accept(id)
		socket.start(id)
		skynet.newservice(SERVICE_NAME, "agent", id)
		-- notice: Some data on this connection(id) may lost before new service start.
		-- So, be careful when you want to use start / abandon / start .
		socket.abandon(id)
	end

	skynet.start(function()
		local id = socket.listen("127.0.0.1", 8001)
		print("[server] listen socket :", "127.0.0.1", 8001)

		socket.start(id , function(id, addr)
			print("[server] connect from " .. addr .. " " .. id)
			-- you have choices :
			-- 1. skynet.newservice("testsocket", "agent", id)
			-- 2. skynet.fork(echo, id)
			-- 3. accept(id)
			accept(id)
		end)
	end)

	-- client
	skynet.fork(function()
		local cid = socket.open("127.0.0.1", 8001)
		boxproto.init(pt, cid)
		print("----> cid:"..cid)
		print("----> connect to 8001")
		
		local str = socket.read(cid)
		if str then
			local chain, error = boxproto.parse(pt, cid, str)
			while chain == nil do -- error ? -1
				str = socket.read(cid)
				chain, error = boxproto.parse(pt, cid, str)
				local msg, left = boxproto.takefront(chain)
				if msg ~= nil then
					local cmd = boxproto.bmcmd(msg)
					local cstr = boxproto.bmreadcstring(msg)
					if cmd == 1 then
						print("----> greeting: "..cstr)
					end
					boxproto.freechainnode(msg) 
					break
				end
			end
		else
			socket.close(cid)
			boxproto.fini(pt, cid)
			return
		end

		local counter = 0
		while true do
			-- print("----> send time: "..skynet.time())
			counter = counter + 1
			if counter > 5 then
				break
			end
			
			local req = boxproto.bmbegin(2)
			boxproto.bmsetversion(req, 2)
			boxproto.bmsetflag(req, 1)
			boxproto.bmsetsid(req, 123)
			boxproto.bmwritei32(req, 999)
			boxproto.bmwritecstring(req, "helloworld")
			boxproto.bmend(req)
			socket.write(cid, boxproto.bmtolstring(req))
			boxproto.freemsg(req)

			local str = socket.read(cid)
			if str then
				local chain, error = boxproto.parse(pt, cid, str)
				while chain == nil do -- error ? -1
					str = socket.read(cid)
					chain, error = boxproto.parse(pt, cid, str)
					local msg, left = boxproto.takefront(chain)
					while msg ~= nil do
						local cmd = boxproto.bmcmd(msg)
						local version = boxproto.bmversion(msg)
						local flag = boxproto.bmflag(msg)
						local sid = boxproto.bmsid(msg)
						local i32 = boxproto.bmreadi32(msg)
						local cstr = boxproto.bmreadcstring(msg)
						print("----> cmd:"..cmd..", version:"..version..", flag:"..flag..", sid:"..sid..", i32:"..i32..", cstr:"..cstr)
						boxproto.freechainnode(msg) 
						msg, left = boxproto.takefront(left)
					end
				end
				skynet.sleep(100)
			else
				socket.close(cid)
				break
			end
		end
		boxproto.fini(pt, cid)
	end)
end
