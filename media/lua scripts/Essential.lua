
function myprint(...)
	local str
	str = ''

	for i = 1, arg.n do
		if str ~= '' then str = str .. '\t' end
		str = str .. tostring( arg[i] )
	end

	str = str .. '\n'

	interpreterOutput( str )
end

oldprint = print
print = myprint



--print the number of managed particles to console
function NumOfManagedParticles()
	print(ParticleManager:getNumManagedParticles().." managed particles active")
end

--print number of unmanaged particles
function NumOfUnManagedParticles()
	print(ParticleManager:getNumUnManagedParticles().." unmanaged particles active")
end

--print total number of particles
function NumOfParticlesTotal()
	print(ParticleManager:getNumUnManagedParticles().." particles active total")
end

--toggle PhysX visual Debug
function PhysXDebug()
	PhysicsSys:toggleDebugRender()
end

dofile("../../media/lua scripts/ActorList.lua")
