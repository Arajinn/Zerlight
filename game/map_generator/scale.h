#pragma once

#include "game/map_generator/modulebase.h"

namespace noise_generator
{
	class Scale : public ModuleBase
	{
	public:
		Scale(const double& aX, const double& aY, const double& aZ, std::shared_ptr<ModuleBase> input);
		~Scale();

		double getValue(const double& x, const double& y, const double& z);
	private:
		double mX, mY, mZ;
	};
}
