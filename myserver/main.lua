local skynet = require "skynet"

local p = "./myserver/"
local oldp = package.path
package.path = string.format("%s?.lua;%s",p,oldp)
local c = require "commands"

local netpack = require "netpack"

local max_client = 64

netpack.loadtblmsgformats(c.tblmsg_format)

skynet.start(function()
	print("[main.lua]", os.date("%m-%d-%Y %X", skynet.starttime()), "server start")
	skynet.newservice("echo")
	local watchdog = skynet.newservice("watchdog")
	skynet.call(watchdog, "lua", "start", {
		port = 8989,
		maxclient = max_client,
	})

	skynet.exit()
end)
