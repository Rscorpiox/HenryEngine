#include "GenerateFilesCommand.hpp"

#include <windows.h>
#include <sstream>
#include <iomanip>


namespace Henry
{

CommandletRegistration GenerateFilesCommand::s_generateRegistration("GenerateFiles",&GenerateFilesCommand::CreateAndGetCommand);


GenerateFilesCommand::GenerateFilesCommand(const CommandletArguments* args) 
	: Commandlet( args )
{
	m_exitAfterExecuted = false;
}


GenerateFilesCommand::~GenerateFilesCommand(void)
{

}


Commandlet* GenerateFilesCommand::CreateAndGetCommand(const CommandletArguments* args)
{
	return ( new GenerateFilesCommand(args) );
}


bool GenerateFilesCommand::Execute()
{
	if( m_commandletArgs->arguments.size() == 2 )
	{
		int numberOfFile = std::stoi( m_commandletArgs->arguments[0] );
		int sizeOfEachFile = std::stoi( m_commandletArgs->arguments[1] );
		std::stringstream name;

		for( int index = 0; index < numberOfFile; ++index )
		{
			name << "generated_" << sizeOfEachFile << "kb_file_" << std::setw(3) << std::setfill('0') << (index+1);
			GenerateFile(name.str(),sizeOfEachFile);
			name.str(std::string());
		}
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


void GenerateFilesCommand::GenerateFile(const std::string& fileName , int kb)
{
	FILE *file;
	std::string filepath;
	filepath += "Data/";
	filepath += fileName;
	filepath += ".dat";

	fopen_s(&file , filepath.c_str() , "wb" );

	int numOfBytes = kb * 1024;
	std::vector<char> buffer;
	buffer.reserve( numOfBytes );
	int index = 0;
	while(index < numOfBytes)
	{
		char randomByte = rand() & 255;
		buffer.push_back(randomByte);
		++index;
	}

	fwrite (buffer.data(), sizeof(char), buffer.size(), file);
	fclose (file);
}


};