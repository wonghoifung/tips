-- coding:utf-8

local p = "./scripts/"
local oldp = package.path
package.path = string.format("%s?.lua;%s",p,oldp)

local cmd_callbacks = {}

local pm = require('peer_manager')
local c = require('commands')
local upper_handler = require('handler_upper')
local chat2all_handler = require('handler_chat2all')

local function init_callbacks()
    cmd_callbacks[c.cmd_upper] = upper_handler
    cmd_callbacks[c.cmd_chat2all] = chat2all_handler
end

function dispatch_message(msg, conn)
	cmd = msg:command()
	if cmd_callbacks[cmd] then
		cmd_callbacks[cmd].process(msg, conn)
	end
end

function peer_come(peerid, conn)
	pm.peer_come(peerid, conn)
end

function peer_go(peerid)
	pm.peer_go(peerid)
end

init_callbacks()

local server = SimpleServer()

if not server:init_server(6464) then
	print("cannot init server")
	return
end

if not server:init() then
	print("cannot init")
	return
end

server:run()


