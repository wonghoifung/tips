function producer(  )
	return coroutine.create(
			function ( salt )
				local t = {1,2,3}
				for i=1,#t do
					salt = coroutine.yield(t[i] + salt)
				end
			end
		)
end

function consumer( prod )
	local salt = 10
	while true do
		local running, product = coroutine.resume(prod, salt)
		salt = salt * salt
		if running then
			print(product or "end")
		else
			break
		end
	end
end

consumer(producer())

------------------------------------

function instream(  )
	return coroutine.wrap(
			function (  )
				while true do
					local line = io.read("*l") -- read the next line
					if line then 
						coroutine.yield(line)
					else
						break
					end
				end
			end
		)
end

function filter( ins )
	return coroutine.wrap(
			function (  )
				while true do
					local line = ins()
					if line then
						line = "** "..line.." **"
						coroutine.yield(line)
					else
						break
					end
				end
			end
		)
end

function outstream( ins )
	while true do
		local line = ins()
		if line then
			print(line)
		else 
			break
		end
	end
end

--outstream(filter(instream()))

------------------------------------

function enum( array )
	return coroutine.wrap(
			function (  )
				local len = #array
				for i=1,len do
					coroutine.yield(array[i])
				end
			end
		)
end

function foreach( array, action )
	for element in enum(array) do
		action(element)
	end
end

foreach({1,2,3},print)