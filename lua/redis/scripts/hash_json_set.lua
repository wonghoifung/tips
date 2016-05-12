local curjsonstr = redis.call('hget', KEYS[1], ARGV[1])

local curjson = {}
if curjsonstr then
	curjson = cjson.decode(curjsonstr)
end

local newjson = cjson.decode(ARGV[2])

for key,value in pairs(newjson) do 
	curjson[key] = value
end

curjsonstr = cjson.encode(curjson)

return redis.call('hset', KEYS[1], ARGV[1], curjsonstr)
