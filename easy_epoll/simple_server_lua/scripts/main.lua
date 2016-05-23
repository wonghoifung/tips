print("in lua script file...")

local outm = outmsg()
outm:begin(123)
outm:write_int(456789)
outm:write_cstring("hello world")
outm:complete()

print(outm:size())

local inm = inmsg()
inm:copy(outm:buffer(), outm:size())
local cmd = inm:command()
local i = inm:read_int()
local s = inm:read_cstring()
print("cmd:"..cmd)
print("i:"..i)
print("s:"..s)