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

local cmd_callbacks = {}

cmd_upper = 5

local function handle_upper(msg, conn)
	local ss = msg:read_cstring()
	-- print(string.upper(s))
	local outmm = outmsg()
	outmm:begin(cmd_upper)
	outmm:write_cstring(string.upper(ss))
	outmm:complete()
	print("handle_upper:"..outmm:size())

	local inmm = inmsg()
	inmm:copy(outmm:buffer(), outmm:size())
	local sss = inmm:read_cstring()
	print("handle_upper:"..sss)

	sendmsg(conn, outmm)
end

local function init_callbacks()
    cmd_callbacks[cmd_upper] = handle_upper
end

function test_c(i1,i2,i3,lud)
	print("i1:"..i1)
	print("i2:"..i2)
	print("i3:"..i3)
	-- return lud
	print("i4:"..lud)
	return string.upper(lud)
end

function dispatch_message(msg, conn)
	cmd = msg:command()
	if cmd_callbacks[cmd] then
		cmd_callbacks[cmd](msg, conn)
	end
end

test(1,2,3)
init_callbacks()
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


