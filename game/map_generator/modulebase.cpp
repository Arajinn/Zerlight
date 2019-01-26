#include "modulebase.h"

namespace noise_generator
{
	ModuleBase::ModuleBase(const int& size)
	{
		mModules.resize(size);
	}


	ModuleBase::~ModuleBase()
	{
	}

}