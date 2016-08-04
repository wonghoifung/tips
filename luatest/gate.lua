local gateserver = require "gateserver"

gateserver.hello()
gateserver.hello()
gateserver.hello()
gateserver.hello()
gateserver.hello()

require "gate2"

local q={}
table.insert(q,1)
table.insert(q,2)
table.insert(q,3)
function pop(queue)
	e = table.remove(queue)
	return e
end
for e in pop,q do
	print(e)
end
