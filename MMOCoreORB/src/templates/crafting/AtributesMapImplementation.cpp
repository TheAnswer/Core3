/*
				Copyright <SWGEmu>
		See file COPYING for copying conditions.

		Author: Hakry
		3/5/2023
*/

#include "AttributesMap.h"

float AttributesMap::VALUENOTFOUND = -999999;
const String AttributesMap::EMPTY;

// #define DEBUG_ATTRIBUTES_MAP

/*
	The Attributes Map Constsist of the following:

	- attributes Vector - String vector of attributes
	- visibleGroups Vector - String vector of attribute groups visible during crafting session

	- attributeGroups VectorMap
		Contains the property and the group that it is effected by. Example <maxdamage, expDamage>

	- attributeValues VectorMap
		Contains the property and the Value Object to store the data for that property.
*/

void AttributesMap::addExperimentalAttribute(const String& attribute, const String& group, const float min, const float max, const int precision, const bool filler, const int combine) {
#ifdef DEBUG_ATTRIBUTES_MAP
	info(true) << "AttributesMap::addExperimentalAttribute called for: " << attribute << " Group: " << group;
#endif

	Locker lock(&mutex);

	// The attribute is not on the map, add it.
	if (!attributes.contains(attribute)) {
		addAttribute(attribute);
		attributeGroups.put(attribute, group);

		Values* value = new Values(attribute, min, max, precision, filler, combine);
		attributeValues.put(attribute, value);
	} else {
		// The attribute is already on the map, we only want to update its values
		Reference<Values*> attValues = attributeValues.get(attribute);

		if (attValues == nullptr) {
			error() << "Values are nullptr for Experimental Attribute: " << attribute << " with group: " << group;
		}

		attValues->setMinValue(min);
		attValues->setMaxValue(max);
		attValues->setPrecision(precision);
		attValues->setFiller(filler);
		attValues->setCombineType(combine);
	}

	if (!filler) {
		addVisibleGroup(group);
	}
}

const String& AttributesMap::getAttribute(const int i) const {
	if (attributes.size() <= i)
		return EMPTY;

	return attributes.get(i);
}

const String& AttributesMap::getAttributeGroup(const String& attribute) const {
	if (attributeGroups.contains(attribute))
		return attributeGroups.get(attribute);

	return EMPTY;
}

const String& AttributesMap::getVisibleAttributeGroup(const int i) const {
	return visibleGroups.get(i);
}

int AttributesMap::getTotalVisibleAttributeGroups() const {
	return visibleGroups.size();
}

bool AttributesMap::isHidden(const String& attribute) const {
	Reference<Values*> values = attributeValues.get(attribute);

	if (values == nullptr)
		return false;

	return values->isFiller();
}

void AttributesMap::setHidden(const String& attribute) {
	Locker lock(&mutex);

	Reference<Values*> values = attributeValues.get(attribute);

	if (values == nullptr)
		return;

	values->setFiller(true);

	String group;

	if (visibleGroups.contains(attribute)) {
		visibleGroups.drop(attribute);
	}
}

void AttributesMap::unsetHidden(const String& attribute) {
	Locker lock(&mutex);

	Reference<Values*> values = attributeValues.get(attribute);

	if (values == nullptr)
		return;

	values->setFiller(false);
}

short AttributesMap::getCombineType(const String& attribute) const {
	Reference<Values*> values = attributeValues.get(attribute);

	if (values == nullptr)
		return (int)VALUENOTFOUND;

	return values->getCombineType();
}

void AttributesMap::setCurrentValue(const String& attribute, const float value) {
	Locker lock(&mutex);

	Reference<Values*> values = attributeValues.get(attribute);

	if (values == nullptr)
		return;

	values->setValue(value);
}

void AttributesMap::setCurrentValue(const String& attribute, const float value, const float min, const float max) {
	Locker lock(&mutex);

	Reference<Values*> values = attributeValues.get(attribute);

	if (values == nullptr)
		return;

	values->setValue(value);
	values->setMaxValue(max);
	values->setMinValue(min);
}

float AttributesMap::getCurrentValue(const String& attribute) const {
	Reference<Values*> values = attributeValues.get(attribute);

	if (values == nullptr)
		return VALUENOTFOUND;

#ifdef DEBUG_ATTRIBUTES_MAP
	info(true) << " AttributesMap::getCurrentValue called for " << attribute << " with a value of " << values->getValue();
#endif

	return values->getValue();
}

float AttributesMap::getCurrentValue(const int i) const {
	const Values* value = attributeValues.get(i);

	if (value == nullptr)
		return VALUENOTFOUND;

	return value->getValue();
}

void AttributesMap::lockValue(const String& attribute) {\
	Locker lock(&mutex);

	Reference<Values*> value = attributeValues.get(attribute);

	if (value == nullptr)
		return;

	value->lockValue();
}

void AttributesMap::unlockValue(const String& attribute) {
	Locker lock(&mutex);

	Reference<Values*> value = attributeValues.get(attribute);

	if (value == nullptr)
		return;

	value->unlockValue();
}

void AttributesMap::resetValue(const String& attribute) {
	Locker lock(&mutex);

	Reference<Values*> value = attributeValues.get(attribute);

	if (value == nullptr)
		return;

	value->resetValue();
}

void AttributesMap::setCurrentPercentage(const String& attribute, float amount) {
	Locker lock(&mutex);

	Reference<Values*> value = attributeValues.get(attribute);

#ifdef DEBUG_ATTRIBUTES_MAP
	info(true) << "---------- AttributesMap::setCurrentPercentage 1 for " << attribute << " setting amount: " << amount;
#endif

	if (value == nullptr)
		return;

	float max = value->getMaxPercentage();

	if (amount > max)
		amount = max;

#ifdef DEBUG_ATTRIBUTES_MAP
	info(true) << "Current Percentage Set: " << amount << " With a max of " << max;
#endif

	value->setPercentage(amount);

#ifdef DEBUG_ATTRIBUTES_MAP
	info(true) << "---------- END AttributesMap::setCurrentPercentage 1";
#endif
}

void AttributesMap::setCurrentPercentage(const String& attribute, float amount, float max) {
	Locker lock(&mutex);

#ifdef DEBUG_ATTRIBUTES_MAP
	info(true) << "---------- AttributesMap::setCurrentPercentage 2 for " << attribute << " ----------";
#endif

	Reference<Values*> value = attributeValues.get(attribute);

	if (value == nullptr)
		return;

#ifdef DEBUG_ATTRIBUTES_MAP
	info(true) << "setCurrentPercentage 2 for " << attribute << " setting Percentage: " << amount << " with a max percentage set: " << max;
#endif

	if (amount > max)
		amount = max;

	value->setMaxPercentage(max);
	value->setPercentage(amount);

#ifdef DEBUG_ATTRIBUTES_MAP
	info(true) << "---------- END AttributesMap::setCurrentPercentage 2 ----------";
#endif
}

float AttributesMap::getCurrentPercentage(const String& attribute) const {
	const Reference<Values*> value = attributeValues.get(attribute);

	if (value == nullptr)
		return VALUENOTFOUND;

	return value->getPercentage();
}

float AttributesMap::getCurrentPercentage(const int i) const {
	const Reference<Values*> value = attributeValues.get(i);

	if (value == nullptr)
		return VALUENOTFOUND;

	return value->getPercentage();
}

float AttributesMap::getCurrentVisiblePercentage(const String group) const {
	float percentage = 0.f;

#ifdef DEBUG_ATTRIBUTES_MAP
	info(true) << "---------- AttributesMap::getCurrentVisiblePercentage called for group: " << group << " with Attribute Size: " << attributes.size() << " ----------";
#endif

	// shouldnt this show the avg so with 1 item who cares, but more than 1 we wanna should avg of all not the LAST one
	for (int i = 0; i < attributes.size(); ++i) {
		const String attribute = attributes.get(i);
		const String atttributeGroup = getAttributeGroup(attribute);

#ifdef DEBUG_ATTRIBUTES_MAP
		info(true) << "getCurrentVisiblePercentage -- Checking attribute: " << attribute << " with group: " << atttributeGroup;
#endif

		if (group != atttributeGroup)
			continue;

		const Values* values = attributeValues.get(attribute);

		if (values == nullptr || values->isFiller())
			continue;

		if (values->getMinValue() != values->getMaxValue() && values->getMaxPercentage() <= 1.0f) {
			float currentPercentage = values->getPercentage();

			if (currentPercentage > percentage)
				percentage = currentPercentage;
		}
	}

#ifdef DEBUG_ATTRIBUTES_MAP
	info(true) << "---------- END AttributesMap::getCurrentVisiblePercentage returning " << percentage << " ----------";
#endif

	return percentage;
}

void AttributesMap::setMaxPercentage(const String& attribute, float amount) {
	Locker lock(&mutex);

	Reference<Values*> value = attributeValues.get(attribute);

	if (value == nullptr)
		return;

	value->setMaxPercentage(amount);
}

float AttributesMap::getMaxPercentage(const String& attribute) const {
	const Reference<Values*> value = attributeValues.get(attribute);

	if (value == nullptr)
		return VALUENOTFOUND;

	float maxPercent = value->getMaxPercentage();

#ifdef DEBUG_ATTRIBUTES_MAP
	info(true) << "----- AttributesMap::getMaxPercentage for " << attribute << " returning max percentage " << maxPercent;
#endif

	return maxPercent;
}

float AttributesMap::getMaxPercentage(const int i) const {
	const Reference<Values*> value = attributeValues.get(i);

	if (value == nullptr)
		return VALUENOTFOUND;

	return value->getMaxPercentage();
}

float AttributesMap::getMaxVisiblePercentage(const int i) const {
	float value = 0;
	String group = getVisibleAttributeGroup(i);

#ifdef DEBUG_ATTRIBUTES_MAP
	info(true) << "---------- AttributesMap::getMaxVisiblePercentage called for group: " << group << " with attribute value size: " << attributeValues.size() << " ----------";
#endif

	for (int j = 0; j < attributeValues.size(); ++j) {
		const Values* values = attributeValues.get(j);

		if (values == nullptr || values->isFiller())
			continue;

		if ((values->getMinValue() != values->getMaxValue()) && (values->getMaxPercentage() <= 1.0f) && (values->getMaxPercentage() > value)) {
			value = values->getMaxPercentage();
		}
	}

#ifdef DEBUG_ATTRIBUTES_MAP
	info(true) << "---------- END AttributesMap::getMaxVisiblePercentage returning: " << value << " ----------";
#endif

	return value;
}

float AttributesMap::getMinValue(const String& attribute) const {
	const Reference<Values*> value = attributeValues.get(attribute);

	if (value == nullptr)
		return VALUENOTFOUND;

#ifdef DEBUG_ATTRIBUTES_MAP
	info(true) << " AttributesMap::getMinValue called for " << attribute << " with a value of " << value->getMinValue();
#endif

	return value->getMinValue();
}

float AttributesMap::getMaxValue(const String& attribute) const {
	const Reference<Values*> value = attributeValues.get(attribute);

	if (value == nullptr)
		return VALUENOTFOUND;

#ifdef DEBUG_ATTRIBUTES_MAP
	info(true) << " AttributesMap::getMaxValue called for " << attribute << " with a value of " << value->getMaxValue();
#endif

	return value->getMaxValue();
}

void AttributesMap::setMinValue(const String& attribute, const float min) {
	Locker lock(&mutex);

	Reference<Values*> value = attributeValues.get(attribute);

	if (value == nullptr)
		return;

#ifdef DEBUG_ATTRIBUTES_MAP
	info(true) << " AttributesMap::setMaxValue for " << attribute << " setting a value of " << min;
#endif

	value->setMinValue(min);
}

void AttributesMap::setMaxValue(const String& attribute, const float max) {
	Locker lock(&mutex);

	Reference<Values*> value = attributeValues.get(attribute);

	if (value == nullptr)
		return;

#ifdef DEBUG_ATTRIBUTES_MAP
	info(true) << " AttributesMap::setMaxValue for " << attribute << " setting a value of " << max;
#endif

	value->setMaxValue(max);
}

int AttributesMap::getPrecision(const String& attribute) const {
	const Reference<Values*> value = attributeValues.get(attribute);

	if (value == nullptr)
		return (int)VALUENOTFOUND;

	return value->getPrecision();
}

void AttributesMap::setPrecision(const String& attribute, const int amount) {
	Locker lock(&mutex);

	Reference<Values*> value = attributeValues.get(attribute);

	if (value == nullptr)
		return;

	value->setPrecision(amount);
}
