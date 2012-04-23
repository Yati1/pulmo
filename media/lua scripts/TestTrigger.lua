--init table
TestTrigger = {}

TestTrigger["Trigger_OnEnter"] = function(actor)
	assert(actor,"Actor is NIL!")
	vec = PhysicsSys:convertOgreVec3(Vector3(0,1000,0))
	bod = actor:getBody()
	bod:addForceImpulse(vec)
	--GameApp:startState(MENU_STATE)
	print("GOT Triggered2!!")
end
