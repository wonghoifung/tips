t = {}
m = {a='and', b='a', c='b'}
setmetatable(t,{__index=m})

for k,v in pairs(t) do
	print(k,v)
end

print("-----------")

print(t.a,t.b,t.c)

function add( t1, t2 )
	assert(#t1 == #t2)
	local length = #t1
	for i=1,length do
		t1[i] = t1[i] + t2[i]
	end
	return t1
end

t1 = setmetatable({1,2,3},{__add=add})
t2 = setmetatable({10,20,30},{__add=add})
t1 = t1 + t2
for i=1,#t1 do
	print(t1[i])
end

-------------------------------------------- based on prototype

Robot = {name='Bond',id=007}

function Robot:New( extension )
	local t = setmetatable(extension or {}, self)
	self.__index = self
	return t
end

function Robot:SetName( name )
	self.name = name
end

function Robot:GetName(  )
	return self.name
end

function Robot:SetId( id )
	self.id = id
end

function Robot:GetId(  )
	return self.id
end

robot = Robot:New()
print("robot ",robot:GetName(),robot:GetId())

print('--------------')

FootballRobot = Robot:New({position="right back"})

function FootballRobot:SetPosition( p )
	self.position = p
end

function FootballRobot:GetPosition(  )
	return self.position
end

fr = FootballRobot:New()

print("fr ", fr:GetPosition(), fr:GetName(), fr:GetId())

print('--------------')

fr:SetName("James")
print("fr ", fr:GetPosition(), fr:GetName(), fr:GetId())
print("robot ",robot:GetName(),robot:GetId())