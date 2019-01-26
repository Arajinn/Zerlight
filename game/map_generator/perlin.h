#pragma once

#include "game/map_generator/modulebase.h"

namespace noise_generator
{
	class Perlin : public ModuleBase
	{
	public:
		Perlin(const double& aFrequency, const double& aLacunarity, const double& aPersistence, const int& aOctaveCount, const int& aSeed);
		~Perlin();

		double getValue(const double& x, const double& y, const double& z);
	private:
		double mFrequency;
		double mLacunarity;
		double mPersistence;
		int mOctaveCount;
		int mSeed;
	};
}
