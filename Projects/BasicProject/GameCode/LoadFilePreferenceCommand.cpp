#include "LoadFilePreferenceCommand.hpp"
#include "GameCommon.hpp"

#include <Windows.h>
#include <algorithm>


namespace Henry
{

CommandletRegistration LoadFilePreferenceCommand::s_generateRegistration("LoadPreference",&LoadFilePreferenceCommand::CreateAndGetCommand);


LoadFilePreferenceCommand::LoadFilePreferenceCommand(const CommandletArguments* args) 
	: Commandlet( args )
{
	m_exitAfterExecuted = false;
}


LoadFilePreferenceCommand::~LoadFilePreferenceCommand(void)
{
}


bool LoadFilePreferenceCommand::Execute()
{
	if( m_commandletArgs->arguments.size() == 1 )
	{
		std::string preference = m_commandletArgs->arguments[0];
		std::transform(preference.begin(), preference.end(), preference.begin(), ::tolower);
		if( preference == "data_only" )
			g_loadPreference = DATA_ONLY;
		else if( preference == "zip_only" )
			g_loadPreference = ZIP_ONLY;
		else if( preference == "prefer_data" )
			g_loadPreference = PREFER_DATA;
		else
			g_loadPreference = PREFER_ZIP;
	}
	else
	{
		std::string errorInfo;
		errorInfo = "Failed to execute command : " + m_commandletArgs->command + 
					"\r\n\r\nWrong arguments." +
					"\r\n\r\nThe program will ends now.";

		MessageBoxA( NULL , errorInfo.c_str(), "Failed to Execute Command", MB_ICONERROR | MB_OK );
		exit(0);
		//assertion
	}

	return m_exitAfterExecuted;
}


Commandlet* LoadFilePreferenceCommand::CreateAndGetCommand(const CommandletArguments* args)
{
	return ( new LoadFilePreferenceCommand(args) );
}


};