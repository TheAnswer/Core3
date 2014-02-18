local SithShadowEncounter = require("managers.jedi.village.sith_shadow_encounter")
local DirectorManagerMocks = require("screenplays.mocks.director_manager_mocks")
local QuestManagerMocks = require("managers.quest.mocks.quest_manager_mocks")
local SpawnMobilesMocks = require("utils.mocks.spawn_mobiles_mocks")
local OldManEncounterMocks = require("managers.jedi.village.mocks.old_man_encounter_mocks")

LOOTCREATURE = 53

describe("Sith Shadow Encounter", function()
	local pCreatureObject = { "creatureObjectPointer" }
	local pFirstSithShadow = { "firstSithShadowPointer" }
	local pSecondSithShadow = { "secondSithShadowPointer" }
	local spawnedSithShadowList = {
		pFirstSithShadow,
		pSecondSithShadow
	}
	local creatureObject
	local firstSithShadowObject
	local secondSithShadowObject
	local pInventory = { "inventoryPointer" }
	local firstSithShadowId = 12345
	local secondSithShadowId = 23456

	setup(function()
		DirectorManagerMocks.mocks.setup()
		SpawnMobilesMocks.mocks.setup()
		OldManEncounterMocks.mocks.setup()
		QuestManagerMocks.mocks.setup()
	end)

	teardown(function()
		DirectorManagerMocks.mocks.teardown()
		SpawnMobilesMocks.mocks.teardown()
		OldManEncounterMocks.mocks.teardown()
		QuestManagerMocks.mocks.teardown()
	end)

	before_each(function()
		DirectorManagerMocks.mocks.before_each()
		SpawnMobilesMocks.mocks.before_each()
		OldManEncounterMocks.mocks.before_each()
		QuestManagerMocks.mocks.before_each()

		creatureObject = {}
		DirectorManagerMocks.creatureObjects[pCreatureObject] = creatureObject

		firstSithShadowObject = {}
		firstSithShadowObject.getObjectID = spy.new(function() return firstSithShadowId end)
		firstSithShadowObject.getSlottedObject = spy.new(function() return pInventory end)
		DirectorManagerMocks.creatureObjects[pFirstSithShadow] = firstSithShadowObject

		secondSithShadowObject = {}
		secondSithShadowObject.getObjectID = spy.new(function() return secondSithShadowId end)
		DirectorManagerMocks.creatureObjects[pSecondSithShadow] = secondSithShadowObject
	end)

	describe("onEncounterSpawned", function()
		describe("When called with a pointer to a player and a list of spawned sith shadows", function()
			it("Should register an observer for LOOTCREATURE on the first sith shadow in the sith shadow list.", function()
				SithShadowEncounter:onEncounterSpawned(pCreatureObject, spawnedSithShadowList)

				assert.spy(createObserver).was.called_with(LOOTCREATURE, SithShadowEncounter.taskName, "onLoot", pFirstSithShadow)
			end)

			it("Should register an observer for OBJECTDESTRUCTION on the player.", function()
				SithShadowEncounter:onEncounterSpawned(pCreatureObject, spawnedSithShadowList)

				assert.spy(createObserver).was.called_with(OBJECTDESTRUCTION, SithShadowEncounter.taskName, "onPlayerKilled", pCreatureObject)
			end)
		end)
	end)

	describe("onLoot", function()
		describe("When called with a pointer to a creature and a pointer to the looter", function()
			it("Should get the list of spawned sith shadows for the looter.", function()
				SithShadowEncounter:onLoot(pFirstSithShadow, pCreatureObject, 0)

				assert.spy(SpawnMobilesMocks.getSpawnedMobiles).was.called_with(pCreatureObject, SithShadowEncounter.taskName)
			end)

			describe("and the player has a list of spawned sith shadows", function()
				before_each(function()
					SpawnMobilesMocks.getSpawnedMobiles = spy.new(function() return spawnedSithShadowList end)
				end)

				it("Should get the id of the first sith shadow in the list", function()
					SithShadowEncounter:onLoot(pSecondSithShadow, pCreatureObject, 0)

					assert.spy(firstSithShadowObject.getObjectID).was.called_with(firstSithShadowObject)
				end)

				it("Should get the id of the looted creature", function()
					SithShadowEncounter:onLoot(pSecondSithShadow, pCreatureObject, 0)

					assert.spy(secondSithShadowObject.getObjectID).was.called_with(secondSithShadowObject)
				end)

				describe("and both ids are identical", function()
					it("Should create loot in the inventory of the sith shadow.", function()
						SithShadowEncounter:onLoot(pFirstSithShadow, pCreatureObject, 0)

						assert.spy(createLoot).was.called_with(pInventory, "sith_shadow_encounter_datapad", 0, true)
					end)

					it("Should return 1 to remove the observer.", function()
						assert.same(1, SithShadowEncounter:onLoot(pFirstSithShadow, pCreatureObject, 0))
					end)
				end)

				describe("and both ids are not identical", function()
					it("Should not create loot in the inventory of the looted sith shadow.", function()
						SithShadowEncounter:onLoot(pSecondSithShadow, pCreatureObject, 0)

						assert.spy(createLoot).was.not_called()
					end)

					it("Should return 0 to keep the observer.", function()
						assert.same(0, SithShadowEncounter:onLoot(pSecondSithShadow, pCreatureObject, 0))
					end)
				end)
			end)

			describe("and the player has no spawned sith shadows", function()
				before_each(function()
					SpawnMobilesMocks.getSpawnedMobiles = spy.new(function() return nil end)
				end)

				it("Should not create loot in the inventory of the looted sith shadow.", function()
					SithShadowEncounter:onLoot(pFirstSithShadow, pCreatureObject, 0)

					assert.spy(createLoot).was.not_called()
				end)

				it("Should return 0 to keep the observer.", function()
					assert.same(0, SithShadowEncounter:onLoot(pFirstSithShadow, pCreatureObject, 0))
				end)
			end)
		end)
	end)

	describe("onPlayerKilled", function()
		local realFinish

		setup(function()
			realFinish = SithShadowEncounter.finish
		end)

		teardown(function()
			SithShadowEncounter.finish = realFinish
		end)

		before_each(function()
			SithShadowEncounter.finish = spy.new(function() end)
		end)

		it("Should check if the killer is from the sith shadow spawn of the player.", function()
			SithShadowEncounter:onPlayerKilled(pCreatureObject, pFirstSithShadow, 0)

			assert.spy(SpawnMobilesMocks.isFromSpawn).was.called_with(pCreatureObject, SithShadowEncounter.taskName, pFirstSithShadow)
		end)

		describe("and the killer is one of the sith shadows of the player", function()
			before_each(function()
				SpawnMobilesMocks.isFromSpawn = spy.new(function() return true end)
			end)

			it("Should remove the crystal from the player.", function()
				SithShadowEncounter:onPlayerKilled(pCreatureObject, pFirstSithShadow, 0)

				assert.spy(OldManEncounterMocks.removeForceCrystalFromPlayer).was.called_with(OldManEncounterMocks, pCreatureObject)
			end)

			it("Should restart the old man encounter.", function()
				SithShadowEncounter:onPlayerKilled(pCreatureObject, pFirstSithShadow, 0)

				assert.spy(OldManEncounterMocks.start).was.called_with(OldManEncounterMocks, pCreatureObject)
			end)

			it("Should return 1 to remove the observer.", function()
				assert.same(1, SithShadowEncounter:onPlayerKilled(pCreatureObject, pFirstSithShadow, 0))
			end)
		end)

		describe("and the killer is not one of the sith shadows of the player", function()
			it("Should not remove the crystal from the player.", function()
				SithShadowEncounter:onPlayerKilled(pCreatureObject, pFirstSithShadow, 0)

				assert.spy(OldManEncounterMocks.removeForceCrystalFromPlayer).was.not_called()
			end)

			it("Should not restart the old man encounter.", function()
				SithShadowEncounter:onPlayerKilled(pCreatureObject, pFirstSithShadow, 0)

				assert.spy(OldManEncounterMocks.start).was.not_called()
			end)

			it("Should return 0 to keep the observer.", function()
				assert.same(0, SithShadowEncounter:onPlayerKilled(pCreatureObject, pFirstSithShadow, 0))
			end)
		end)
	end)

	describe("isEncounterFinished", function()
		describe("When called with a player object", function()
			it("Should check if the player has the force crystal or not.", function()
				SithShadowEncounter:isEncounterFinished(pCreatureObject)

				assert.spy(QuestManagerMocks.hasCompletedQuest).was.called(1)
			end)

			describe("and the player has lost the force crystal", function()
				before_each(function()
					QuestManagerMocks.hasCompletedQuest = spy.new(function() return false end)
				end)

				it("Should return true", function()
					assert.is_true(SithShadowEncounter:isEncounterFinished(pCreatureObject))
				end)
			end)

			describe("and the player has not lost the force crystal", function()
				before_each(function()
					QuestManagerMocks.hasCompletedQuest = spy.new(function() return true end)
				end)

				it("Should return false", function()
					assert.is_false(SithShadowEncounter:isEncounterFinished(pCreatureObject))
				end)
			end)
		end)
	end)
end)
