-- print('to require')
local tc = require "testlib.counter"
print('hello')
local f = tc.newCounter()
print(f())
print(f())
print(f())

local tlh = require "testlib.luahello"
tlh.hello()
print(tlh.three())

-- weak table 
wt = {}
wt[1] = function() print("first call") end
wt[2] = function() print("second call") end
wt[3] = {1,2,3}
setmetatable(wt,{__mode="kv"})
print(#wt)
ele=wt[1]
collectgarbage()
print(#wt)
ele=nil
collectgarbage()
print(#wt)

-- create table with default value, only one metatable, but weak table is large
local defaults={}
setmetatable(defaults, {__mode="k"})
local mt = {__index=function(t) return defaults[t] end}
function setDefault( t, d )
	defaults[t] = d
	setmetatable(t, mt)
end
local ttt={}
setDefault(ttt, 123)
print(ttt[1])
print(ttt[2])

-- create table with default value 2, many metatable, but weak table is small
metas = {}
setmetatable(metas, {__mode="v"})
setDefault2 = function ( t, d )
	local mt = metas[d]
	if mt == nil then
		mt = {__index = function() return d end}
		metas[d] = mt
	end
	setmetatable(t, mt)
end
local ttt2={}
setDefault2(ttt2, 456)
print(ttt2[1])
print(ttt2[33])

-- coroutine profile
local profile = require "testlib.profile"
local coroutine_resume=profile.resume
local coroutine_yield=profile.yield
function producer(  )
	return coroutine.create(function ( salt )
		profile.start()
		local t = {1,2,3}
		for i=1,#t do
			-- salt=coroutine.yield(t[i]+salt)
			salt=coroutine_yield(t[i]+salt)
		end
		profile.stop()
	end)
end
function consumer( prod )
	local salt=10
	while true do
		-- local running,product=coroutine.resume(prod,salt)
		local running,product=coroutine_resume(prod,salt)
		salt=salt*salt
		if running then
			print(product or "end")
		else
			break
		end
	end
end
profile.start()
consumer(producer()) -- 11 102 10003 end
profile.stop()

local queue={}
print('queue size:',#queue)
table.insert(queue,11)
table.insert(queue,22)
table.insert(queue,33)
print('queue size:',#queue)
while true do
	local key,val = next(queue)
	if val == nil then
		break
	end
	queue[key]=nil
	print('queue:',key,val)
end
print('queue size:',#queue)

-- error('TESTERROR')
-- print('after error')

local co = coroutine.create(function () print('coco',coroutine.running()) end)
coroutine.resume(co)
print(debug.traceback(co,'==='),coroutine.running())
-- error(debug.traceback(co,'~~~'))
-- print('after error')

function response()
	return 'response'
end
pack = response
print(pack)

-- only false and nil is condition FALSE
if 1 then print('true') else print('false') end
if 0 then print('true') else print('false') end
if nil then print('true') else print('false') end
if false then print('true') else print('false') end

local ws = {}
ws['a'] = true
ws['b'] = true
ws['c'] = false
ws['d'] = nil
ws['e'] = true
local s = next(ws)
while s do
	print('###',s,ws[s])
	ws[s] = nil
	s = next(ws)
end
