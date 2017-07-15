#pragma once

#include "Engine_vs2013\Thread\Job.hpp"
#include "GameCommon.hpp"

#include <string>


namespace Henry
{

class LoadFileJob : public Job
{
public:
	LoadFileJob(std::string name, JobPriority priority = AVERAGE_PRIORITY, const std::string& filePath = "", const std::string& zipPath = "", LoadPreference preference = DATA_ONLY);
	~LoadFileJob();
	void Execute();

private:
	bool LoadFromData();
	bool LoadFromZip();

private:
	std::string m_filePath;
	std::string m_zipPath;
	LoadPreference m_prefer;
	unsigned char* m_contentBuffer;
};

};