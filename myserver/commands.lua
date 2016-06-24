-- coding:utf-8

local _M = {}

_M.cmd_peer_login = 0
_M.cmd_upper = 5
_M.cmd_chat2all = 6

_M.tblmsg_test1 = 7
_M.tblmsg_test2 = 8

_M.tblmsg_format = {}
_M.tblmsg_format[_M.cmd_peer_login] = { 1 }
_M.tblmsg_format[_M.tblmsg_test1] = { 1, 2, 2, 2, 1 } -- 1 for int; 2 for str
_M.tblmsg_format[_M.tblmsg_test2] = { 2, 1, 1, 1, 2 }

return _M
