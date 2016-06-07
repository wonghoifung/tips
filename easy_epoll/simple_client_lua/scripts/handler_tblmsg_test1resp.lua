-- coding:utf-8

local c = require('commands')

local _M = {}

function _M.process(cmd, tblmsg, conn)
	print("=======> tblmsg test1resp")
	print("lua receive 1: "..tblmsg[1])
	print("lua receive 2: "..tblmsg[2])
	print("lua receive 3: "..tblmsg[3])
	print("lua receive 4: "..tblmsg[4])
	print("lua receive 5: "..tblmsg[5])
end

return _M
