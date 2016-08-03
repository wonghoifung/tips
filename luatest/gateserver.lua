local gateserver = {}

local i=0
function gateserver.hello()
	i = i + 1
	print('hello',i)
end

return gateserver
