#include "scale.h"

namespace noise_generator
{
	Scale::Scale(const double& aX, const double& aY, const double& aZ, std::shared_ptr<ModuleBase> input)
		:ModuleBase(1)
	{
		mModules[0] = input;

		mX = aX;
		mY = aY;
		mZ = aZ;
	}

	Scale::~Scale()
	{
	}

	double Scale::getValue(const double& x, const double& y, const double& z)
	{
		return mModules.at(0)->getValue(x*mX, y*mY, z*mZ);
	}
}