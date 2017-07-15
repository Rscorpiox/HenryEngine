#include "HashJob.hpp"


namespace Henry
{

HashJob::HashJob(const std::string& name, JobPriority priority, char* data , int length) 
	: Job(name,priority)
	, m_hashValue(0)
	, m_buffer(data)
	, m_length(length)
{
	
}


HashJob::~HashJob()
{
}


void HashJob::Execute()
{
	if (m_finished)
		return;

	long multipleNum = 1;
	for (int index = 0; index < m_length; ++index)
	{
		m_hashValue += m_buffer[m_length] * multipleNum;
		multipleNum += 37;
	}

	m_finished = true;
}




};