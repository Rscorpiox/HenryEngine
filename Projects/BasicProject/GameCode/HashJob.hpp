#pragma once
#include "Engine_vs2013\Thread\Job.hpp"

#include <string>


namespace Henry
{

class HashJob :	public Job
{
public:
	HashJob(const std::string& name , JobPriority priority , char* data , int length);
	~HashJob();
	void Execute();

public:
	int m_hashValue;

private:
	char* m_buffer;
	int m_length;
};

};