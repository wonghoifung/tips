width = 888

print('bbb')

print(msg)

ta = {
	name='albert',
	age=32
}

print(wo.name..','..wo.age)

function onEvent(e, obj)
	print('event '..e)
	print(obj.name..' '..obj.age)
	-- print(aa..bb)
	-- return 'ok'
	local re = {ret='ok'}
	return re
end

function ferror(e)
	print('error '..e)
	return 'failed'
end

local ret = c_fun1('world', 123, false, {'a1','a2','a3'}, {name='jack',age=23})
print('c ret: '..ret)

ret = c_fun2()
print(ret.name..' '..ret.age)

local cppcurl = require "cppcurl"
cppcurl.http_post()
cppcurl.http_get()


