#include "cache.h"

namespace noise_generator
{
	Cache::Cache(std::shared_ptr<ModuleBase> input)
		:ModuleBase(1)
	{
		mModules[0] = input;

		mCached = false;

		mValue = 0.0;

		mX = 0.0;
		mY = 0.0;
		mZ = 0.0;
	}

	Cache::~Cache()
	{
	}

	double Cache::getValue(const double& x, const double& y, const double& z)
	{
		if ((!mCached) || (mX != x) || (mY != y) || (mZ != z))
		{
			mValue = mModules.at(0)->getValue(x, y, z);
			mX = x;
			mY = y;
			mZ = z;
		}

		mCached = true;
		return mValue;
	}
}