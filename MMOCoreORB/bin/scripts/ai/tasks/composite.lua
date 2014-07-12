require("ai.ai")

--Composite = Ai:new {}
CompositeBase = createClass(Ai)

function CompositeBase:doAction(pAgent)
	if (pAgent ~= nil) then
		local agent = LuaAiAgent(pAgent)
		local res = agent:getBehaviorStatus()
		return res
	else
		return BEHAVIOR_INVALID
	end
end

Composite = createClass(CompositeBase, Interrupt)
CompositeDefault = createClass(CompositeBase, DefaultInterrupt)
CompositePack = createClass(CompositeBase, PackInterrupt)