#pragma once

#include "modulebase.h"

namespace noise_generator
{
	class Const : public ModuleBase
	{
	public:
		Const(double value);
		~Const();

		double getValue(const double& x, const double& y, const double& z);
	private:
		double mValue;
	};
}
