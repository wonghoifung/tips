local skynet = require "skynet"
require "skynet.manager"	-- import skynet.register
local netpack = require "netpack"
local socket = require "socket"

local p = "./myserver/"
local oldp = package.path
package.path = string.format("%s?.lua;%s",p,oldp)
local c = require "commands"

local CMD = {}
-- _M.tblmsg_format[_M.tblmsg_test1] = { 1, 2, 2, 2, 1 } -- 1 for int; 2 for str
-- _M.tblmsg_format[_M.tblmsg_test2] = { 2, 1, 1, 1, 2 }
function CMD.message(clientfd, message)
	-- print("[echo.lua] message", clientfd, message)
	-- socket.write(clientfd, message)
	local cmd, tbl = netpack.totable(message, string.len(message))
	print("[echo.lua] cmd", cmd)
	if not tbl then
		print("[echo.lua] find no format for cmd", cmd)
		return
	end

	if cmd == c.cmd_peer_login then
		local resp = { 0 }
		local pack, size = netpack.topack(cmd, resp) -- socket_server_send will free the buffer
		socket.write(clientfd, skynet.tostring(pack,size))
		return
	end
	
	if cmd == c.tblmsg_test1 then
		print("=======> tbl test1")
		print("lua receive 1: "..tbl[1])
		print("lua receive 2: "..tbl[2])
		print("lua receive 3: "..tbl[3])
		print("lua receive 4: "..tbl[4])
		print("lua receive 5: "..tbl[5])
		local resp = { tbl[5], tbl[4], tbl[3], tbl[2], tbl[1] }
		local pack, size = netpack.topack(cmd, resp) -- socket_server_send will free the buffer
		socket.write(clientfd, skynet.tostring(pack,size))
	end

	if cmd == c.tblmsg_test2 then
		print("=======> tbl test2")
		print("lua receive 1: "..tbl[1])
		print("lua receive 2: "..tbl[2])
		print("lua receive 3: "..tbl[3])
		print("lua receive 4: "..tbl[4])
		print("lua receive 5: "..tbl[5])
		local resp = { string.upper(tbl[5]), tbl[4], tbl[3], tbl[2], string.upper(tbl[1]) }
		local pack, size = netpack.topack(cmd, resp) -- socket_server_send will free the buffer
		socket.write(clientfd, skynet.tostring(pack,size))
	end
end

function CMD.disconn(clientfd)
	print("[echo.lua] disconn", clientfd)
end

skynet.start(function()
	skynet.dispatch("lua", function(session, address, cmd, ...)
		local f = CMD[cmd]
		skynet.ret(skynet.pack(f(...)))
	end)
	
	skynet.register "echo"
end)
