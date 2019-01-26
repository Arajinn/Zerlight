#pragma once

#include "modulebase.h"

namespace noise_generator
{
	class Multiply : public ModuleBase
	{
	public:
		Multiply(std::shared_ptr<ModuleBase> lhs, std::shared_ptr<ModuleBase> rhs);
		~Multiply();

		double getValue(const double& x, const double& y, const double& z);
	};
}