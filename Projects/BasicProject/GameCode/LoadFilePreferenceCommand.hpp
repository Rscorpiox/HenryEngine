#pragma once

#ifndef LOADFILEPREFERENCECOMMAND_HPP 
#define LOADFILEPREFERENCECOMMAND_HPP

#include "Engine\Commandlet\CommandletRegistration.hpp"

namespace Henry
{

class LoadFilePreferenceCommand : Commandlet
{
public:
	LoadFilePreferenceCommand(const CommandletArguments* args);
	~LoadFilePreferenceCommand(void);
	bool Execute();
	static Commandlet* CreateAndGetCommand(const CommandletArguments* args);

public:
	static CommandletRegistration s_generateRegistration;
};

};


#endif