-- coding:utf-8

-----------------------------------------> script path
local p = "./scripts/"
local p2 = "../shared/"
local oldp = package.path
package.path = string.format("%s?.lua;%s?.lua;%s",p,p2,oldp)

local c = require('commands')
local tblmsg_test1resp_handler = require('handler_tblmsg_test1resp')
local tblmsg_test2resp_handler = require('handler_tblmsg_test2resp')

local cmd_callbacks = {}

local function init_callbacks()
    cmd_callbacks[c.tblmsg_test1] = tblmsg_test1resp_handler
    cmd_callbacks[c.tblmsg_test2] = tblmsg_test2resp_handler
end

function dispatch_tblmsg(cmd, tblmsg, conn)
	if cmd_callbacks[cmd] then
		cmd_callbacks[cmd].process(cmd, tblmsg, conn)
	end
end

-----------------------------------------> init
if not skeleton_init("SimpleClientLua", 7, 1024) then
	print("lua cannot init skeleton")
	return
end
init_callbacks()
load_tblmsg_formats(c.tblmsg_format) -- test1 test2 request format is same with response

local client = simpleclient_create()
simpleclient_connect(client, "127.0.0.1", "8989")
--simpleclient_connect(client, "127.0.0.1", "6464")

-----------------------------------------> run loop
skeleton_run()

-----------------------------------------> clear
simpleclient_destroy(client)
