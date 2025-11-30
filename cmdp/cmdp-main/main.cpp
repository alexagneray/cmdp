#include "pch.h"

#include "Kernel.h"
#include "Display.h"
int main()
{
	cmdp::Kernel kernel;
	//kernel.Run();

	for(int i=0; i<100;++i)
	{ 
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
		kernel.AddCmd(std::make_unique<cmdp::TestCmd>());
		display.Set("WorkerCount", std::any(kernel.GetExecCount()));
		display.Set("CmdCount", std::any(kernel.GetCmdCount()));
		display.Set("i", std::any(i));
	}

	while (true)
	{
		display.Set("WorkerCount", std::any(kernel.GetExecCount()));
		display.Set("CmdCount", std::any(kernel.GetCmdCount()));
	}
}