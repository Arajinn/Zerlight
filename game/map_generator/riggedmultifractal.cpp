#include "riggedmultifractal.h"
#include "game/utils/math_utils.h"

namespace noise_generator
{
	RiggedMultifractal::RiggedMultifractal(const double& aFrequency, const double& aLacunarity, const int& aOctaveCount, const int& aSeed)
		:ModuleBase(0)
	{
		mFrequency = aFrequency;
		mLacunarity = aLacunarity;
		mOctaveCount = aOctaveCount;
		mSeed = aSeed;

		mWeights.resize(30);
		updateWeights();
	}

	RiggedMultifractal::~RiggedMultifractal()
	{
	}

	void RiggedMultifractal::updateWeights()
	{
		double value = 1.0;
		for (int i = 0, isize = mWeights.size(); i < isize; i++)
		{
			mWeights[i] = 1.0 / value;
			value *= mLacunarity;
		}
	}

	double RiggedMultifractal::getValue(const double& x, const double& y, const double& z)
	{
		auto nX = x * mFrequency;
		auto nY = y * mFrequency;
		auto nZ = z * mFrequency;

		double value = 0.0;
		double weight = 1.0;
		double offset = 1.0;
		double gain = 2.0;

		for (int index = 0; index < mOctaveCount; index++)
		{
			auto seed = (mSeed + index) & 0x7fffffff;
			auto signal = math_utils::gradientCoherentNoise3D(nX, nY, nZ, seed);
			signal = abs(signal);
			signal = offset - signal;
			signal *= signal;
			signal *= weight;
			weight = signal * gain;

			if (weight < 0.0)
				weight = 0.0;

			if (weight > 1.0)
				weight = 1.0;

			value += (signal*mWeights.at(index));

			nX *= mLacunarity;
			nY *= mLacunarity;
			nZ *= mLacunarity;
		}

		return value * 1.25 - 1.0;
	}	
}