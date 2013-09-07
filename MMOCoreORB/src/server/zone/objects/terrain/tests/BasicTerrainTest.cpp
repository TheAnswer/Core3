/*
 * BasicTerrainTest.cpp
 *
 *  Created on: 06/09/2013
 *      Author: victor
 */

#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include "server/zone/managers/templates/DataArchiveStore.h"
#include "server/zone/objects/terrain/ProceduralTerrainAppearance.h"
#include "server/zone/objects/terrain/TargaBitmap.h"

class BasicTerrainTest : public ::testing::Test {
public:

	BasicTerrainTest() {
		// Perform creation setup here.
	}

	~BasicTerrainTest() {
		// Clean up.
	}

	void SetUp() {
		// Perform setup of common constructs here.
		//DataArchiveStore
	}

	void TearDown() {
		// Perform clean up of common constructs here.
	}
};

TEST_F(BasicTerrainTest, LoadTestTerrainTest) {
	IffStream* stream = DataArchiveStore::instance()->openIffFile("terrain/test_terrain.trn");

	ASSERT_TRUE(stream != NULL);

	ProceduralTerrainAppearance terrain;

	terrain.readObject(stream);

	delete stream;

	float x = -238;
	float y = -166;
	float res = terrain.getHeight(x, y);

	EXPECT_NEAR(res, 30.367649, 0.01);

	//printf("height at %f, %f is:%f\n", x, y, res);
}
