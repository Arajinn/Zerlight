#include "const.h"

namespace noise_generator
{
	Const::Const(double value)
		:ModuleBase(0)
		,mValue(value)
	{
	}

	Const::~Const()
	{
	}

	double Const::getValue(const double& x, const double& y, const double& z)
	{
		return mValue;
	}
}