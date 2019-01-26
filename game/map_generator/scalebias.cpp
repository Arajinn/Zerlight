#include "scalebias.h"

namespace noise_generator
{
	ScaleBias::ScaleBias(const double& aScale, const double& aBias, std::shared_ptr<ModuleBase> input)
		:ModuleBase(1)
	{
		mModules[0] = input;

		mScale = aScale;
		mBias = aBias;
	}
	
	ScaleBias::~ScaleBias()
	{
	}

	double ScaleBias::getValue(const double& x, const double& y, const double& z)
	{
		return mModules.at(0)->getValue(x, y, z)*mScale + mBias;
	}
}