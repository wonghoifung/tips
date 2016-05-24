-- coding:utf-8

local c = require('commands')
local pm = require('peer_manager')

local _M = {}

function _M.process(msg, conn)
	local me = getpeerid(conn)

	local content = msg:read_cstring()
	
	local om = outmsg()
	om:begin(c.cmd_chat2all)
	om:write_int(me)
	om:write_cstring(content)
	om:complete()

	for peerid, conn in pairs(pm.peers) do
		sendmsg(conn, om)
	end
end

return _M
