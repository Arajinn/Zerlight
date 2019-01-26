#pragma once

#include <memory>
#include <vector>

namespace noise_generator
{
	class ModuleBase
	{
	public:
		ModuleBase(const int& size);
		~ModuleBase();

		virtual double getValue(const double& x, const double& y, const double& z) = 0;
	protected:
		std::vector<std::shared_ptr<ModuleBase>> mModules;
	};
}
