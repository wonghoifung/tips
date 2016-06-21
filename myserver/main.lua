local skynet = require "skynet"

local max_client = 64

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
