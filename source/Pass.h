#pragma once
#include "base/FXCommon.h"

namespace FX 
{
	using PassName = std::string;

	class Pass
	{
	public:
		virtual void Execute() = 0;

	};




}