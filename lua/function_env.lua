function foo(  )
	print(g or "no g found")
end

foo()

setfenv(foo, {g=100,print=print})

foo()

print(g or "no g found")