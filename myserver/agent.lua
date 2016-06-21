local skynet = require "skynet"
local netpack = require "netpack"
local socket = require "socket"

local CMD = {}

local clientfd

local function mym_send(v)
	socket.write(clientfd, v)
end

skynet.register_protocol {
	name = "client",
	id = skynet.PTYPE_CLIENT,
	unpack = function (msg, sz)
		return skynet.tostring(msg,sz)
	end,
	dispatch = function (session, address, text)
		data = text
		pcall(skynet.call, "echo", "lua", "message", clientfd, data)
	end
}

skynet.register_protocol {
	name = "mym",
	id = 12,
	pack = skynet.pack,
	unpack = skynet.unpack,
	dispatch = function (session, address, text)
		print("[agent.lua]", os.date("%m-%d-%Y %X", skynet.starttime()), text, skynet.address(address))
		mym_send("welcome to myserver\n")
		skynet.retpack(text)
	end
}

function CMD.start(gate , fd)
	clientfd = fd
	skynet.call(gate, "lua", "forward", fd)
end

skynet.start(function()
	skynet.dispatch("lua", function(_,_, command, ...)
		local f = CMD[command]
		skynet.ret(skynet.pack(f(...)))
	end)
end)
