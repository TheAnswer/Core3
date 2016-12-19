SwgemuChristmasScreenPlay = ScreenPlay:new {
	treeLocs = {
		{ template = "object/swgemu/christmas/swgemu_christmas_tree_02_nomusic.iff", x = -4854.6, z = 6.5, y = 4177.1, planet = "naboo" },
		{ template = "object/swgemu/christmas/swgemu_christmas_tree_02_nomusic.iff", x = -4846.6, z = 6.5, y = 4170.2, planet = "naboo" },

		{ template = "object/swgemu/christmas/swgemu_christmas_tree_02_nomusic.iff", x = -135.4, z = 28, y = 4716.7, planet = "corellia" },
		{ template = "object/swgemu/christmas/swgemu_christmas_tree_02_nomusic.iff", x = -136.9, z = 28, y = 4728.6, planet = "corellia" },
	}
}

function SwgemuChristmasScreenPlay:start()
	self:spawnSceneObjects()
end

function SwgemuChristmasScreenPlay:spawnSceneObjects()
	for i = 1, #self.treeLocs, 1 do
		local treeData = self.treeLocs[i]
		if (isZoneEnabled(treeData.planet)) then
			spawnSceneObject(treeData.planet, treeData.template, treeData.x, treeData.z, treeData.y, 0, 0)
		end
	end
end


registerScreenPlay("SwgemuChristmasScreenPlay", true)
