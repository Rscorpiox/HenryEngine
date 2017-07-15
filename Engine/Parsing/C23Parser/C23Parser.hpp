#pragma once

#ifndef C23PARSER_HPP
#define C23PARSER_HPP

#include <vector>
#include <map>

#include "Engine\Parsing\BufferParser\BufferParser.hpp"
#include "Engine\Math\Matrix4.hpp"

namespace Henry
{

struct BoneData
{
	int id;
	int parentId;
	std::string name;
	Henry::Vec3f translation;
	Henry::Vec3f orientation;
	Henry::Vec3f scale;
	RGBA color;
	std::vector<int> childsId;
};


class C23Parser
{
public:
	C23Parser();
	~C23Parser();
	bool LoadModel(const char* filePath);
	//bool LoadAnim(const char* filePath/*, std::vector<std::vector<BoneData>>& keyframeData*/);
	bool WriteModel(const char* filePath, const std::vector<Vertex_PCTNB>& verticesToWrite, const std::vector<int>& indicesToWrite);
	bool WriteAnim(const char* filePath, const std::vector<BoneData>& bonesToWrite, std::map<int, std::vector<Henry::BoneData>>& keyframeDataToWrite);
	
public:
	unsigned char m_version;
	std::vector<Vertex_PCTNB> m_modelVertices;
	std::vector<unsigned int> m_modelIndices;

private:
	BufferParser m_bufferParser;
};

};

#endif