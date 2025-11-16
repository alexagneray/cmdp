#include "pch.h"
#include "Command.h"
#include "Display.h"

namespace cmdp
{
	int TestCmd::Do()
	{
		display.Set("text", std::any(std::string("Hello world")));
		return 0;
	}
}
