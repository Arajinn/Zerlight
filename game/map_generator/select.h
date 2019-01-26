#pragma once

#include "game/map_generator/modulebase.h"

namespace noise_generator
{
	class Select : public ModuleBase
	{
	public:
		Select(const double& aMin, const double& aMax, const double& aFallOff, std::shared_ptr<ModuleBase> input1, std::shared_ptr<ModuleBase> input2, std::shared_ptr<ModuleBase> controller);
		~Select();

		double getValue(const double& x, const double& y, const double& z);
	private:
		double mFallOff;
		double mRaw;
		double mMin, mMax;
	};
}
