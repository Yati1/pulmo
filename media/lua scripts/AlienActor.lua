--init state tables
Alien_State_GetInPosition = {}
Alien_State_ShootStation = {}
Alien_State_ShootPlayer = {}

-----------------
--GetInPosition
-----------------

--Enter
Alien_State_GetInPosition["OnEnter"] = function(actor)
	print("Entering Alien_State_GetInPosition")
end

--Update
Alien_State_GetInPosition["OnUpdate"] = function(actor,elapsedTime)
	print("Updating Alien_State_GetInPosition "..elapsedTime.." ms")
end

--Exit
Alien_State_GetInPosition["OnExit"] = function(actor)
	print("Exiting Alien_State_GetInPosition")
end

-------------------
--ShootStation
-------------------
--Enter
Alien_State_ShootStation["OnEnter"] = function(actor)
	print("Entering Alien_State_ShootStation")
end

--Update
Alien_State_ShootStation["OnUpdate"] = function(actor,elapsedTime)
	print("Updating Alien_State_ShootStation with "..elapsedTime.." ms")
end

--Exit
Alien_State_ShootStation["OnExit"] = function(actor)
	print("Entering Alien_State_ShootStation")
end

--------------
--shootPlayer
--------------

--Enter
Alien_State_ShootPlayer["OnEnter"] = function(actor)
	print("Entering Alien_State_shootPlayer")
end

--Update
Alien_State_ShootPlayer["OnUpdate"] = function(actor,elapsedTime)
	print("Updating Alien_State_ShootPlayer with "..elapsedTime.." ms")
end

--Exit
Alien_State_ShootPlayer["OnExit"] = function(actor)
	print("Entering Alien_State_ShootPlayer")
end
