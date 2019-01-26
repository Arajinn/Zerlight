#pragma once

#include "game/map_generator/modulebase.h"

namespace noise_generator
{
	class Turbulence : public ModuleBase
	{
	public:
		Turbulence(std::shared_ptr<ModuleBase> x, std::shared_ptr<ModuleBase> y, std::shared_ptr<ModuleBase> z, 
			const double& aXPower, const double& aYPower, const double& aZPower,
			std::shared_ptr<ModuleBase> input);
		~Turbulence();

		double getValue(const double& x, const double& y, const double& z);
	private:
		double mXPower, mYPower, mZPower;
		std::shared_ptr<ModuleBase> mXDistort, mYDistort, mZDistort;
	};
}
