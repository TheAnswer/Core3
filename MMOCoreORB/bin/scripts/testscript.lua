function printf(...) io.write(string.format(unpack({...}))) end

local creature = LuaCreatureObject(nil)

function runScript(creatureObject)
	creature:_setObject(creatureObject)
  	
  	--creature:setHAM(0, math.random(99))
  	credits = creature:getBankCredits()  
end
