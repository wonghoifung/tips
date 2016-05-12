local curjsonstr = redis.call('hget', KEYS[1], ARGV[1])

if not curjsonstr then
	return nil
end

local curjson = cjson.decode(curjsonstr)

for k,v in pairs(curjson) do 
	if k == ARGV[2] then
		return v
	end
end

return nil
