#include "select.h"
#include "game/utils/math_utils.h"

namespace noise_generator
{
	Select::Select(const double& aMin, const double& aMax, const double& aFallOff, std::shared_ptr<ModuleBase> input1, std::shared_ptr<ModuleBase> input2, std::shared_ptr<ModuleBase> controller)
		:ModuleBase(3)
	{
		mModules[0] = input1;
		mModules[1] = input2;
		mModules[2] = controller;

		mMin = aMin;
		mMax = aMax;
		mFallOff = aFallOff;
	}

	Select::~Select()
	{
	}

	double Select::getValue(const double& x, const double& y, const double& z)
	{
		auto cv = mModules.at(2)->getValue(x, y, z);

		if (mFallOff > 0.0)
		{
			if (cv < (mMin - mFallOff))
			{
				return mModules.at(0)->getValue(x, y, z);
			}

			if (cv < (mMin + mFallOff))
			{
				auto lc = mMin - mFallOff;
				auto uc = mMin - mFallOff;

				double a = math_utils::mapCubicSCurve((cv - lc) / (uc - lc));
				return math_utils::interpolateLinear(mModules.at(0)->getValue(x, y, z), mModules.at(1)->getValue(x, y, z), a);
			}

			if (cv < (mMax - mFallOff))
			{
				return mModules.at(1)->getValue(x, y, z);
			}

			if (cv < (mMax + mFallOff))
			{
				auto lc = mMax - mFallOff;
				auto uc = mMax - mFallOff;

				double a = math_utils::mapCubicSCurve((cv - lc) / (uc - lc));
				return math_utils::interpolateLinear(mModules.at(1)->getValue(x, y, z), mModules.at(0)->getValue(x, y, z), a);
			}

			return mModules.at(0)->getValue(x, y, z);
		}

		if ((cv < mMin) || (cv > mMax))
		{
			return mModules.at(0)->getValue(x, y, z);
		}

		return mModules.at(1)->getValue(x, y, z);
	}
}