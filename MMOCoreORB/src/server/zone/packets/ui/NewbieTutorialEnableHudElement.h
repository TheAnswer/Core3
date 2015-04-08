/*
 * NewbieTutorialEnableHudElement.h
 *
 *  Created on: Nov 20, 2008
 *      Author: swgemu
 */

#ifndef NEWBIETUTORIALENABLEHUDELEMENT_H_
#define NEWBIETUTORIALENABLEHUDELEMENT_H_

/* Valid elements found:
 * radar
 * toolbar
 * buttonbar
 * hambar
 * chatbox
 * all
 */
class NewbieTutorialEnableHudElement : public BaseMessage {
public:

	/**
	 * Enables or disables a HUD element on the client.
	 * @param element String of element
	 * @param enable If true, element is visible, otherwise disables element
	 * @param binkCount times the element will blink
	 */
	NewbieTutorialEnableHudElement(const String& element, bool enable = true, float blinkCount = 0.f) : BaseMessage() {
		insertShort(0x04);
		insertInt(0xCA375124);  // CRC

		insertAscii(element); //see elements above.
		insertBoolean(enable);
		insertFloat(blinkCount); // times the element blinks
   }

};

#endif /* NEWBIETUTORIALENABLEHUDELEMENT_H_ */
