#include "turbulence.h"

namespace noise_generator
{
	const double X0 = (12414.0 / 65536.0);
	const double Y0 = (65124.0 / 65536.0);
	const double Z0 = (31337.0 / 65536.0);
	const double X1 = (26519.0 / 65536.0);
	const double Y1 = (18128.0 / 65536.0);
	const double Z1 = (60493.0 / 65536.0);
	const double X2 = (53820.0 / 65536.0);
	const double Y2 = (11213.0 / 65536.0);
	const double Z2 = (44845.0 / 65536.0);

	Turbulence::Turbulence(std::shared_ptr<ModuleBase> x, std::shared_ptr<ModuleBase> y, std::shared_ptr<ModuleBase> z, 
		const double& aXPower, const double& aYPower, const double& aZPower, std::shared_ptr<ModuleBase> input)
		:ModuleBase(1)
	{
		mModules[0] = input;

		mXDistort = x;
		mYDistort = y;
		mZDistort = z;

		mXPower = aXPower;
		mYPower = aYPower;
		mZPower = aZPower;
	}

	Turbulence::~Turbulence()
	{
	}

	double Turbulence::getValue(const double& x, const double& y, const double& z)
	{
		double xd, yd, zd;

		if (mXDistort == nullptr)
			xd = x;
		else
			xd = x + mXDistort->getValue(x + X0, y + Y0, z + Z0)*mXPower;

		if (mYDistort == nullptr)
			yd = y;
		else
			yd = y + mYDistort->getValue(x + X1, y + Y1, z + Z1)*mYPower;

		if (mZDistort == nullptr)
			zd = z;
		else
			zd = z + mZDistort->getValue(x + X2, y + Y2, z + Z2)*mZPower;

		return mModules.at(0)->getValue(xd, yd, zd);
	}
}