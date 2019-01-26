#pragma once

#include "modulebase.h"

namespace noise_generator
{
	class Cache : public ModuleBase
	{
	public:
		Cache(std::shared_ptr<ModuleBase> input);
		~Cache();

		double getValue(const double& x, const double& y, const double& z);
	private:
		double mValue;

		bool mCached;

		double mX, mY, mZ;
	};
}
