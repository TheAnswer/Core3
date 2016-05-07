/*
 * AffectorRiver.cpp
 *
 *  Created on: 4/26/2016
 *      Author: gslomin
 */

#include "AffectorRiver.h"
#include "../../TerrainGenerator.h"

void AffectorRiver::process(float x, float y, float transformValue, float& baseValue, TerrainGenerator* terrainGenerator) {
	info("Process: " + String::valueOf(x) + " , " + String::valueOf(y), true);
	if (mfrc == NULL) {
		mfrc = terrainGenerator->getMfrc(0);
		mfrc->setGainValue(0.9f);
		mfrc->setOctavesParam(3);
	}
	//
	
	if (transformValue > 0.f)
	{
		if (isWithinBounds (x, y))
		{
			const float  width_2        = width * 0.5f;
			const float originalHeight = baseValue;
			
			//FindData result;
			float resultDistanceToCenter, resultHeight, distanceAlongPath;
			if (getHeight (x, y, width_2, resultDistanceToCenter, resultHeight, distanceAlongPath)) {
				const float distanceToCenter = fabsf (resultDistanceToCenter);
				const float desiredHeight    = resultHeight;
				const float subWidth         = width_2 * mfrc->getNoise(distanceAlongPath);
				const float feather          = subWidth * (1.f - featheringAmount);

				if (distanceToCenter <= feather)
				{
					baseValue = desiredHeight;
				}
				else
				{
					if (distanceToCenter <= subWidth)
					{
						//-- height
						const float t = distanceToCenter / subWidth;
						baseValue = Segment::linearInterpolate (desiredHeight, originalHeight, t*t);
						
					}
				}
				
			}
		}
	}
}
