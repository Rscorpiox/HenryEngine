#pragma once

#ifndef BUFFERPARSER_HPP
#define BUFFERPARSER_HPP

#include "Engine\Core\VertexStruct.hpp"

#include <string>
#include <vector>

namespace Henry
{

class BufferParser
{
public:
	BufferParser();
	BufferParser(const std::string& filePath);
	~BufferParser(void);
	bool LoadFile(const std::string& filePath);
	void Rewind();
	bool ReadInt(int& out_int);
	bool ReadUInt(unsigned int& out_int);
	bool ReadFloat(float& out_float);
	bool ReadBool(bool& out_bool);
	bool ReadString(std::string& out_string);
	bool ReadChar(char& out_char);
	bool ReadUChar(unsigned char& our_char);
	//bool IsEOF();		//FIX:use for checked if it's EOF beofer every read function.

	bool WriteFloat(std::vector<unsigned char>& buffer, const float data);
	bool WriteChar(std::vector<unsigned char>& buffer, const char data);
	bool WriteInt(std::vector<unsigned char>& buffer, const int data);
	bool WriteUInt(std::vector<unsigned char>& buffer, const unsigned int data);
	bool WriteBool(std::vector<unsigned char>& buffer, const bool data);
	bool WriteString(std::vector<unsigned char>& buffer, const std::string& data);
	bool WriteUChar(std::vector<unsigned char>& buffer, const unsigned char data);

public:
	const unsigned char* m_scan;
	unsigned const char* m_end;

private:
	unsigned char* m_buffer;
	int m_fileSize;
	//int m_writeIndex;
};

};

#endif