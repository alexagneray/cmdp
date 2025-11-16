#pragma once
#include <vector>

namespace cmdp
{
	template<typename T>
	class CmdTpl
	{

	};

	class Cmd
	{
	public:
		virtual int Do() = 0;
	};

	class TestCmd : public Cmd
	{
	public:
		int Do() override;
	};
}