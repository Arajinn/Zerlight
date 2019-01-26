#include "gradient.h"
#include <cmath>

namespace noise_generator
{
	Gradient::Gradient(const double& aPointX, const double& aPointY, const double& aPointZ,
		const double& aDirectionX, const double& aDirectionY, const double& aDirectionZ)
		:ModuleBase(0)
	{
		mPointX = aPointX;
		mPointY = aPointY;
		mPointZ = aPointZ;

		mDirectionX = aDirectionX;
		mDirectionY = aDirectionY;
		mDirectionZ = aDirectionZ;

		mLength = std::sqrt(aDirectionX*aDirectionX+ aDirectionY* aDirectionY+ aDirectionZ* aDirectionZ);
	}
	
	Gradient::~Gradient()
	{
	}

	double Gradient::getValue(const double& x, const double& y, const double& z)
	{
		auto dX = x - mPointX;
		auto dY = y - mPointY;
		auto dZ = z - mPointZ;

		auto dot_product = dX * mDirectionX + dY * mDirectionY + dZ * mDirectionZ;

		auto value = dot_product / mLength;
		if (value < 0.0)
			value = 0.0;
		if (value > 1.0)
			value = 1.0;

		return 1.0 - value;
	}
}