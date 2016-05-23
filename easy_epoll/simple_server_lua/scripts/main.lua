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

log_debug("debug from lua script...........")
log_error("error from lua script...........")

local server = SimpleServer()
if not server:init_server(6464) then
	print("cannot init server")
	return
end
if not server:init() then
	print("cannot init")
	return
end
server:run()