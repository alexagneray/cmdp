#include "pch.h"
#include "Kernel.h"

namespace cmdp
{
	int Kernel::ExecutionRoutine()
	{
		while (true)
		{
			Sleep(10);
			std::unique_lock<std::mutex> lock(m_mutQuCmd);
			/**
			* On utilise unique lock avant le wait pour eviter les race conditions
			* race condition : plusieurs threads modifient une même variable
			* et le résultat dépend de l'ordre d'exécution des threads => imprévisible 
			* Exemple : plusieurs threads font x++, 
			*/


			// la capture de this permet à la lambda d'accéder aux membres de l'instance
			m_cvAvailableCmd.wait(lock, [this]() {return !m_quCmd.empty(); });

			
			std::unique_ptr<Cmd> cmd = std::move(m_quCmd.front());
			m_quCmd.pop();

			std::unique_lock<std::mutex> lock2(m_mutLstWorker);
			m_lstWorker.push_back(
			std::async(
				[this](std::unique_ptr<Cmd> cmd) -> int
					{
						if (cmd)
						{
							cmd.get()->Do();
						}
						return 0;
					},
					std::move(cmd)));
		}
		return 0;
	}
	int Kernel::CleaningRoutine()
	{
		std::list<std::future<int>>::iterator itPrev;
		bool bDelPrev = false;
		while (true)
		{
			Sleep(1000);
			bDelPrev = false;
			for (auto it = m_lstWorker.begin(); it!=m_lstWorker.end(); ++it)
			{
				if (bDelPrev)
				{
					bDelPrev = false;
					std::unique_lock<std::mutex> lock(m_mutLstWorker);
					m_lstWorker.erase(itPrev);
				}
				


				std::future_status status = it->wait_for(std::chrono::milliseconds(10));
				if (status == std::future_status::ready)
				{
					bDelPrev = true;
				}
				
				itPrev = it;
			}
		}
		return 0;
	}
	Kernel::Kernel() :
		m_thdExecRoutine(&Kernel::ExecutionRoutine, this),
		m_thdCleanRoutine(&Kernel::CleaningRoutine, this)
	{
	}
	int Kernel::AddCmd(std::unique_ptr<Cmd> cmd)
	{
		std::unique_lock<std::mutex> lock(m_mutQuCmd);
		m_quCmd.push(std::move(cmd));
		m_cvAvailableCmd.notify_one();
		return 0;
	}

	int Kernel::Run()
	{

		return 0;
	}

	int Kernel::GetExecCount() const
	{
		return m_lstWorker.size();
	}

	int Kernel::GetCmdCount() const
	{
		return m_quCmd.size();
	}



}
