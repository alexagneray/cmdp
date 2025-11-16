#pragma once
#include "Command.h"

namespace cmdp
{

class Kernel
{
private:
	// Liste des commandes à exécuter
	std::mutex m_mutQuCmd;
	std::queue<std::unique_ptr<Cmd>> m_quCmd;
	
	// Routine d'exécution des commandes
	std::thread m_thdExecRoutine;
	int ExecutionRoutine();

	// Routine de nettoyage des threads
	std::thread m_thdCleanRoutine;
	int CleaningRoutine();

	// Disponibilité d'une commande
	std::condition_variable m_cvAvailableCmd;


	// Liste de workers
	std::mutex m_mutLstWorker;
	std::list<std::future<int>> m_lstWorker;
public:
	Kernel();
	int AddCmd(std::unique_ptr<Cmd> cmd);
	int Run();
	int GetExecCount() const;
	int GetCmdCount() const;

};

}
