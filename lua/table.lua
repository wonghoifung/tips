a={}
b={
	x=1,
	["bbc.."]="hihihi"
}
a[1]=123
a['ddd']=b

function foo(  )
	
end

function bar(  )
	
end

a[foo]=bar

for k,v in pairs(a) do
	print(k,'=>',v)
end

print('--------------------')

for k,v in pairs(b) do
	print(k,'=>',v)
end

print('--------------------')

c={1,2,['bbc']='kkk',3,4,5}

for k,v in ipairs(c) do
	print(k,'=>',v)
end

print('--------------------')

for k,v in pairs(c) do
	print(k,'=>',v)
end

print('--------------------')

c[1]=nil
for k,v in pairs(c) do
	print(k,'=>',v)
end