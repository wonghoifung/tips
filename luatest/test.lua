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