#pragma once

#ifndef COMMANDGENERATEFILES_HPP 
#define COMMANDGENERATEFILES_HPP

#include "Engine\Commandlet\CommandletRegistration.hpp"

namespace Henry
{

class GenerateFilesCommand : public Commandlet
{
public:
	GenerateFilesCommand(const CommandletArguments* args);
	~GenerateFilesCommand(void);
	bool Execute();
	static Commandlet* CreateAndGetCommand(const CommandletArguments* args);

public:
	static CommandletRegistration s_generateRegistration;

private:
	void GenerateFile( const std::string& fileName , int kb );
};

}

#endif