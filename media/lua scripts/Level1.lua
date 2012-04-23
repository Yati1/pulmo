--TestScece
--This file defines Level1
Level1 = {}

Level1["HieghtMap"] = "HeightMap.jpg"

Level1["Scene"] = "TestScene.scene"

Level1["Load"] = function()
	print("Loading "..Level1["Scene"])
end

Level1["Unload"] = function()
	print("Unloading "..Level1["Scene"])
end
