SuiEventType = {
	SET_none = 0x00,
	SET_onButton = 0x01,
	SET_onCheckbox = 0x02,
	SET_onEnabledChanged = 0x03,
	SET_onGenericSelection = 0x04,
	SET_onSliderbar = 0x05,
	SET_onTabbedPane = 0x06,
	SET_onTextbox = 0x07,
	SET_onVisibilityChanged = 0x08,
	SET_onClosedOk = 0x09, -- OK Button
	SET_onClosedCancel = 0x0A, -- Cancel button
	SET_numEventTypes = 0x0B,
}

SuiTemplate = {}
SuiTemplate.new = function (templateName)
	local self = {}
	local luaCallback = {}

	local suiPageData = LuaSuiPageData(templateName)

	--self.getId = function () return suiPageData.id end

	self.getUsingObject = function ()
		--return suiPageData.usingObject
		return 0
	end

	self.setUsingObject = function (object)
	--suiPageData.usingObject = object
	end

	self.setForceCloseDistance = function (forceCloseDistance)
	--suiPageData.forceCloseDistance = forceCloseDistance
	end

	self.setProperty = function (widget, property, value)
		suiPageData:setProperty(widget, property, value)
	end

	self.addDataItem = function (widget, property, value)
		suiPageData:addDataItem(widget, property, value)
	end

	self.clearDataSource = function (dataSource)
		suiPageData:clearDataSource(dataSource)
	end

	self.addDataSourceContainer = function (parent, name)
		suiPageData:addDataSourceContainer(parent, name)
	end

	self.clearDataSourceContainer = function (dataSourceContainer)
		suiPageData:clearDataSourceContainer(dataSourceContainer)
	end

	self.addChildWidget = function (parent, type, name)
		suiPageData:addChildWidget(parent, type, name)
	end

	self.subscribeToEvent = function (eventType, parent, callback)
		suiPageData:subscribeToEvent(eventType, parent, callback)
	end

	self.setDefaultCallback = function (play, callback)
		luaCallback.play = play
		luaCallback.callback = callback
		suiPageData:setDefaultCallback(play .. ":" .. callback)
	end

	self.subscribeToPropertyForEvent = function (eventType, widget, property)
		suiPageData:subscribeToPropertyForEvent(eventType, widget, property)
	end

	self.sendTo = function (pCreatureObject)
		if (luaCallback.play == nil or luaCallback.callback == nil) then
			printf("Error in SuiTemplate:sendTo, nil callback data.\n")
		end

		local pPageData = suiPageData:_getObject()

		if (pPageData == nil) then
			printf("Error in SuiTemplate:sendTo, nil page data.\n")
		end

		local suiManager = LuaSuiManager()
		suiManager:sendSuiPage(pCreatureObject, pPageData, luaCallback.play, luaCallback.callback)
	end

	return self
end
