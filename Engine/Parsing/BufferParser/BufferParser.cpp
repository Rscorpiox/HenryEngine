#include "BufferParser.hpp"

#include <Windows.h>
#include <sstream>

namespace Henry
{


BufferParser::BufferParser() : m_fileSize(0), m_buffer(nullptr), m_scan(nullptr), m_end(nullptr)
{
}


BufferParser::BufferParser(const std::string& filePath) : m_fileSize(0), m_buffer(nullptr), m_scan(nullptr), m_end(nullptr)
{
	LoadFile(filePath);
}


BufferParser::~BufferParser(void)
{
	if(m_buffer)
		delete[] m_buffer;
}


bool BufferParser::LoadFile(const std::string& filePath)
{
	if (m_buffer)
	{
		delete[] m_buffer;
		m_buffer = nullptr;
	}

	FILE *file;
	fopen_s(&file,filePath.c_str(),"rb");

	if(file == NULL)
	{
		std::string errorInfo("Can't find the file : \n\t");
		errorInfo.append(filePath);
		errorInfo.append("\n\nMake sure the file already been cooked.");
		errorInfo.append("\n\nThe Program Will Close Automatically.");
		MessageBoxA( NULL , errorInfo.c_str(), "Failed to loading files", MB_ICONERROR | MB_OK );
		return false;
	}

	fseek(file, 0, SEEK_END);
	long fsize = ftell(file);
	fseek(file, 0, SEEK_SET);

	m_buffer = new unsigned char[fsize];
	fread(m_buffer, sizeof(unsigned char), fsize, file);
	fclose(file);

	m_fileSize = fsize;
	m_scan = m_buffer;
	m_end = m_buffer + fsize * sizeof(unsigned char);
	return true;
}


bool BufferParser::ReadBool(bool& out_bool)
{
	out_bool = *((bool*)m_scan);
	m_scan += sizeof(bool);
	return true;
}


bool BufferParser::ReadFloat(float& out_float)
{
	out_float = *((float*)m_scan);
	m_scan += sizeof(float);
	return true;
}


bool BufferParser::ReadInt(int& out_int)
{
	out_int = *((int*)m_scan);
	m_scan += sizeof(int);
	return true;
}


bool BufferParser::ReadUInt(unsigned int& out_int)
{
	out_int = *((unsigned int*)m_scan);
	m_scan += sizeof(unsigned int);
	return true;
}


bool BufferParser::ReadString(std::string& out_string)
{
	char data;
	ReadChar(data);
	while(data != '\0')
	{
		out_string.push_back(data);
		ReadChar(data);
	}
	return true;
}


bool BufferParser::ReadChar(char& out_char)
{
	out_char = *((char*)m_scan);
	m_scan += sizeof(char);
	return true;
}


bool BufferParser::ReadUChar(unsigned char& our_char)
{
	our_char = *((unsigned char*)m_scan);
	m_scan += sizeof(unsigned char);
	return true;
}


void BufferParser::Rewind()
{
	m_scan = m_buffer;
}

bool BufferParser::WriteFloat(std::vector<unsigned char>& buffer,const float data)
{
	for(int index=0; index < sizeof(float); index++)
		buffer.push_back(((unsigned char*)&data)[index]);
	
	return true;
}


bool BufferParser::WriteChar(std::vector<unsigned char>& buffer,const char data)
{
	for(int index=0; index < sizeof(char); index++)
		buffer.push_back(((unsigned char*)&data)[index]);

	return true;
}


bool BufferParser::WriteInt(std::vector<unsigned char>& buffer,const int data)
{
	for(int index=0; index < sizeof(int); index++)
		buffer.push_back(((unsigned char*)&data)[index]);

	return true;
}


bool BufferParser::WriteUInt(std::vector<unsigned char>& buffer,const unsigned int data)
{
	for(int index=0; index < sizeof(unsigned int); index++)
		buffer.push_back(((unsigned char*)&data)[index]);

	return true;
}


bool BufferParser::WriteBool(std::vector<unsigned char>& buffer,const bool data)
{
	for(int index=0; index < sizeof(bool); index++)
		buffer.push_back(((unsigned char*)&data)[index]);

	return true;
}


bool BufferParser::WriteString(std::vector<unsigned char>& buffer,const std::string& data)
{
	for(size_t index=0; index < data.length(); index++)
		buffer.push_back(data[index]);
	buffer.push_back('\0');

	return true;
}


bool BufferParser::WriteUChar(std::vector<unsigned char>& buffer,const unsigned char data)
{
	for(int index=0; index < sizeof(unsigned char); index++)
		buffer.push_back(((unsigned char*)&data)[index]);
	
	return true;
}


};