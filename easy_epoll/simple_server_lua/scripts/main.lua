-- coding:utf-8

-----------------------------------------> script path
local p = "./scripts/"
local oldp = package.path
package.path = string.format("%s?.lua;%s",p,oldp)

local cmd_callbacks = {}

local pm = require('peer_manager')
local c = require('commands')
local upper_handler = require('handler_upper')
local chat2all_handler = require('handler_chat2all')

local tblmsg_test1_handler = require('handler_tblmsg_test1')
local tblmsg_test2_handler = require('handler_tblmsg_test2')

local function init_callbacks()
    cmd_callbacks[c.cmd_upper] = upper_handler
    cmd_callbacks[c.cmd_chat2all] = chat2all_handler

    cmd_callbacks[c.tblmsg_test1] = tblmsg_test1_handler
    cmd_callbacks[c.tblmsg_test2] = tblmsg_test2_handler
end

-----------------------------------------> global functions for c
function dispatch_message(msg, conn)
	cmd = msg:command()
	if cmd_callbacks[cmd] then
		cmd_callbacks[cmd].process(msg, conn)
	end
end

function dispatch_tblmsg(cmd, tblmsg, conn)
	if cmd_callbacks[cmd] then
		cmd_callbacks[cmd].process(cmd, tblmsg, conn)
	end
end

function peer_come(peerid, conn)
	pm.peer_come(peerid, conn)
end

function peer_go(peerid)
	pm.peer_go(peerid)
end

-----------------------------------------> init
if not skeleton_init("SimpleServerLua", 7, 100000) then
	print("lua cannot init skeleton")
	return
end

init_callbacks()
load_tblmsg_formats(c.tblmsg_format)

print("")
print("")

-----------------------------------------> test timer
local testtimer = create_timer('on_testtimer_timerout')
function on_testtimer_timerout(timer)
	print('lua on_testtimer_timerout')
	start_timer(3, testtimer)
end
-- start_timer(3, testtimer)

-----------------------------------------> test table
function printtable(t, n)
	if "table" ~= type(t) then
		return 0
	end
	n = n or 0
	local spaces = ""
	for i = 1, n do
		spaces = spaces.."  "
	end
	print(spaces.."{")
	for k, v in pairs(t) do
		local str_k_v = spaces.."  ["..tostring(k).."] = "
		if "table" == type(v) then
			print(str_k_v)
			printtable(v, n + 1)
		else
			str_k_v = str_k_v..tostring(v)
			print(str_k_v);
		end
	end
	print(spaces.."}")
end

local tt = {
	gdp = 1234,
	info = "to be or not to be, this is a question!",
	task = {12, 23, 34, 45},
};
local tr = testtable(tt)
printtable(tr)
tr.key.hello()

print("")
print("")

local tt2 = { 123, "456", "abc", 789, "adddddddddddddz"}
sendtable(tt2)

print("")
print("")

-----------------------------------------> test server
local server = SimpleServer()

if not server:init(6464) then
	print("cannot init server")
	return
end

-----------------------------------------> run
skeleton_run()


