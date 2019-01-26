#pragma once

#include "game/map_generator/modulebase.h"

namespace noise_generator
{
	class ScaleBias : public ModuleBase
	{
	public:
		ScaleBias(const double& aScale, const double& aBias, std::shared_ptr<ModuleBase> input);
		~ScaleBias();

		double getValue(const double& x, const double& y, const double& z);
	private:
		double mScale;
		double mBias;
	};
}
