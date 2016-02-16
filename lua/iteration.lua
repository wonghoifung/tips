function enum( array )
	local index = 1
	return function (  )
		local ret = array[index]
		index = index + 1
		return ret
	end
end

function foreach( array, action )
	for element in enum(array) do
		action(element)
	end
end

foreach({1,2,3},print)