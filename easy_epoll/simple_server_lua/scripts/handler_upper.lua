-- coding:utf-8

local c = require('commands')

local _M = {}

function _M.process(msg, conn)
	local ss = msg:read_cstring()

	local outmm = outmsg()
	outmm:begin(c.cmd_upper)
	outmm:write_cstring(string.upper(ss))
	outmm:complete()

	sendmsg(conn, outmm)
end

return _M
