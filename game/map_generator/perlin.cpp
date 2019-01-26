#include "perlin.h"
#include "game/utils/math_utils.h"

namespace noise_generator
{
	Perlin::Perlin(const double& aFrequency, const double& aLacunarity, const double& aPersistence, const int& aOctaveCount, const int& aSeed)
		:ModuleBase(0)
	{
		mFrequency = aFrequency;
		mLacunarity = aLacunarity;
		mPersistence = aPersistence;

		mOctaveCount = aOctaveCount;
		mSeed = aSeed;
	}

	Perlin::~Perlin()
	{
	}

	double Perlin::getValue(const double& x, const double& y, const double& z)
	{
		auto nX = x * mFrequency;
		auto nY = y * mFrequency;
		auto nZ = z * mFrequency;

		double value = 0.0;
		double cp = 1.0;

		for (int index = 0; index < mOctaveCount; index++)
		{
			auto seed = (mSeed + index) & 0xffffffff;
			auto signal = math_utils::gradientCoherentNoise3D(nX, nY, nZ, seed);
			value += signal * cp;

			nX *= mLacunarity;
			nY *= mLacunarity;
			nZ *= mLacunarity;
			cp *= mPersistence;
		}

		return value;
	}
}