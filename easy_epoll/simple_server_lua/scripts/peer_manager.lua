-- coding:utf-8

local _M = {}

_M.peers = {}

function _M.peer_come(peerid, conn)
	print("lua peer come:"..peerid)
	_M.peers[peerid] = conn
end

function _M.peer_go(peerid)
	print("lua peer go:"..peerid)
	-- table.remove(_M.peers, peerid)
	_M.peers[peerid] = nil
end

function _M.peer_conn(peerid)
	return _M.peers[peerid]
end

return _M
