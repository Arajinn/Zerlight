#pragma once

#include "modulebase.h"

namespace noise_generator
{
	class Gradient : public ModuleBase
	{
	public:
		Gradient(const double& aPointX, const double& aPointY, const double& aPointZ,
			const double& aDirectionX, const double& aDirectionY, const double& aDirectionZ);
		~Gradient();

		double getValue(const double& x, const double& y, const double& z);
	private:
		double mPointX, mPointY, mPointZ;
		double mDirectionX, mDirectionY, mDirectionZ;
		double mLength;
	};
}
