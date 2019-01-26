#include "multiply.h"

namespace noise_generator
{
	Multiply::Multiply(std::shared_ptr<ModuleBase> lhs, std::shared_ptr<ModuleBase> rhs)
		:ModuleBase(2)
	{
		mModules[0] = lhs;
		mModules[1] = rhs;
	}

	Multiply::~Multiply()
	{
	}

	double Multiply::getValue(const double& x, const double& y, const double& z)
	{
		return mModules.at(0)->getValue(x, y, z)*mModules.at(1)->getValue(x, y, z);
	}
}