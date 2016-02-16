function create( name, id )
	local obj = {name=name, id=id}
	
	function obj:SetName( name )
		self.name = name
	end

	function obj:GetName(  )
		return self.name
	end

	function obj.SetId( self, id )
		self.id = id
	end

	function obj.GetId( self )
		return self.id
	end

	return obj
end

o1 = create("wong",1)
print("o1: ",o1:GetName(),o1:GetId())
o1:SetId(2)
o1:SetName('albert')
print("o1: ",o1.GetName(o1),o1.GetId(o1))

-------------------------------------------- inherit

function createRobot( name, id )
	local obj = {name=name, id=id}
	
	function obj:SetName( name )
		self.name = name
	end

	function obj:GetName(  )
		return self.name
	end

	function obj:GetId(  )
		return self.id
	end

	return obj
end

function createFootballRobot( name, id, position )
	local obj = createRobot(name,id)
	obj.position = "right back"

	function obj:SetPosition( p )
		self.position = p
	end

	function obj:GetPosition(  )
		return self.position
	end

	return obj
end

-------------------------------------------- closure

function createCountdownTimer( second )
	local ms = second * 1000

	local function countDown(  )
		ms = ms - 1
		return ms
	end

	return countDown
end

timer1 = createCountdownTimer(1)
for i=1,3 do
	print(timer1())
end

timer2 = createCountdownTimer(1)
for i=1,3 do
	print(timer2())
end

-------------------------------------------- based on object

function create2( name, id )
	local data = {name=name, id=id}
	local obj = {}

	function obj.SetName( name )
		data.name = name
	end

	function obj.GetName(  )
		return data.name
	end

	function obj.SetId( id )
		data.id = id
	end

	function obj.GetId(  )
		return data.id
	end

	return obj
end

print("==========")
o1 = create2("hhf",123)
o2 = create2("whf",234)
o1.SetId(100)
print("o1 ",o1.GetId(),o1.GetName())
print("o2 ",o2.GetId(),o2.GetName())
o2.SetName('wonghoifung')
print("o2 ",o2.GetId(),o2.GetName())
