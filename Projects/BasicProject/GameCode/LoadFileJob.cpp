#include "LoadFileJob.hpp"
#include "Engine\Parsing\ZipUtils\ZipHelper.hpp"

#include <Windows.h>
#include <mutex>


namespace Henry
{

LoadFileJob::LoadFileJob(std::string name, JobPriority priority , const std::string& filePath, const std::string& zipPath, LoadPreference preference)
	: Job( name , priority )
	, m_filePath(filePath)
	, m_zipPath(zipPath)
	, m_prefer(preference)
	, m_contentBuffer(nullptr)
{

}


LoadFileJob::~LoadFileJob()
{
	if (m_contentBuffer)
	{
		delete[] m_contentBuffer;
		m_contentBuffer = nullptr;
	}
}


void LoadFileJob::Execute()
{
	if (m_finished)
		return;

	if (m_prefer == DATA_ONLY)
	{
		LoadFromData();
	}

	if (m_prefer == ZIP_ONLY)
	{
		LoadFromZip();
	}

	if (m_prefer == PREFER_DATA)
	{
		bool success = LoadFromData();
		if (!success)
			success = LoadFromZip();
	}

	if (m_prefer == PREFER_ZIP)
	{
		bool success = LoadFromZip();
		if (!success)
			success = LoadFromData();
	}

	m_finished = true;
}


bool LoadFileJob::LoadFromData()
{
	bool success = true;
	FILE *file;
	fopen_s(&file, m_filePath.c_str(), "rb");
	if (file == NULL)
	{
		std::string errorInfo(m_filePath.c_str());
		errorInfo = "Failed to load file : " + errorInfo;
		MessageBoxA(NULL, errorInfo.c_str(), "Failed to loading files", MB_ICONERROR | MB_OK);
		success = false;
	}

	fseek(file, 0, SEEK_END);
	long fsize = ftell(file);
	fseek(file, 0, SEEK_SET);

	int bytesRead = fsize;
	m_contentBuffer = new unsigned char[bytesRead + 1];
	fread(m_contentBuffer, sizeof(unsigned char), bytesRead, file);
	fclose(file);
	m_contentBuffer[bytesRead] = '\0';

	return success;
}


bool LoadFileJob::LoadFromZip()
{
	bool success = true;
	int bufferLength;
	ZipHelper zipHelper;
	m_contentBuffer = (unsigned char*)zipHelper.GetContentInZip(m_zipPath, m_filePath, "c23", &bufferLength, &success);
	return success;
}


};