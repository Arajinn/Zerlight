#pragma once

#include "game/map_generator/modulebase.h"

namespace noise_generator
{
	class RiggedMultifractal : public ModuleBase
	{
	public:
		RiggedMultifractal(const double& aFrequency, const double& aLacunarity, const int& aOctaveCount, const int& aSeed);
		~RiggedMultifractal();

		double getValue(const double& x, const double& y, const double& z);
	private:
		double mFrequency;
		double mLacunarity;
		int mOctaveCount;
		int mSeed;
		std::vector<double> mWeights;
		void updateWeights();
	};
}
