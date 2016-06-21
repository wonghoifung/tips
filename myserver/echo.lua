local skynet = require "skynet"
require "skynet.manager"	-- import skynet.register
local netpack = require "netpack"
local socket = require "socket"

local CMD = {}

function CMD.message(clientfd, message)
	print("[echo.lua] message", clientfd, message)
	socket.write(clientfd, message)
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
