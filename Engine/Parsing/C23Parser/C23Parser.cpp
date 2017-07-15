#include "C23Parser.hpp"
#include "..\..\Core\HenryFunctions.hpp"


namespace Henry
{
	C23Parser::C23Parser() : m_version(1)
	{

	}

	C23Parser::~C23Parser()
	{

	}

	bool C23Parser::LoadModel(const char* filePath)
	{
		m_bufferParser.LoadFile(filePath);
		
		char fileExtention[5];
		char subType;
		char versionNum;
		std::string commentText;
		int numOfVertex;

		memset(fileExtention, 0, 5);
		for (int index = 0; index < 4; index++)
			m_bufferParser.ReadChar(fileExtention[index]);
		
		if (strcmp(fileExtention, "GC23") != 0)
			return false;

		m_bufferParser.ReadChar(subType);
		m_bufferParser.ReadChar(versionNum);
		m_bufferParser.ReadString(commentText);

		if (subType == 2)
		{
			int numOfindex;
			m_bufferParser.ReadInt(numOfindex);
			m_modelIndices.reserve(numOfindex);
			int indices;

			for (int index = 0; index < numOfindex; index++)
			{
				m_bufferParser.ReadInt(indices);
				m_modelIndices.push_back(indices);
			}
		}

		m_bufferParser.ReadInt(numOfVertex);
		m_modelVertices.reserve(numOfVertex);
		Vertex_PCTNB source;	//FIX: should have load model function for rigged and non-rigged model

		if (subType == 1)
		{
			m_modelIndices.reserve(numOfVertex);

			for (int index = 0; index < numOfVertex; index++)
			{
				m_modelIndices.push_back(index);
			}
		}

		for (int index = 0; index < numOfVertex; index++)
		{
			m_bufferParser.ReadFloat(source.position.x);
			m_bufferParser.ReadFloat(source.position.y);
			m_bufferParser.ReadFloat(source.position.z);
			
			m_bufferParser.ReadUChar(source.color.r);
			m_bufferParser.ReadUChar(source.color.g);
			m_bufferParser.ReadUChar(source.color.b);
			m_bufferParser.ReadUChar(source.color.a);

			m_bufferParser.ReadFloat(source.texCoords.x);
			m_bufferParser.ReadFloat(source.texCoords.y);

			m_bufferParser.ReadFloat(source.normals.normal.x);
			m_bufferParser.ReadFloat(source.normals.normal.y);
			m_bufferParser.ReadFloat(source.normals.normal.z);

			m_bufferParser.ReadFloat(source.normals.tangent.x);
			m_bufferParser.ReadFloat(source.normals.tangent.y);
			m_bufferParser.ReadFloat(source.normals.tangent.z);

			m_bufferParser.ReadFloat(source.normals.biTangent.x);
			m_bufferParser.ReadFloat(source.normals.biTangent.y);
			m_bufferParser.ReadFloat(source.normals.biTangent.z);

			for (int b = 0; b < MAX_BONE_ATTACH_SUPPORT; ++b)
			{
				m_bufferParser.ReadInt(source.boneIdWeights.ids[b]);
				m_bufferParser.ReadFloat(source.boneIdWeights.weights[b]);
			}

			m_modelVertices.push_back(source);
		}
		return true;
	}

	
//bool C23Parser::LoadAnim(const char* filePath/*, std::vector<std::vector<BoneData>>& keyframeData*/)
//{
//	std::map<int, int> maxBoneIdToVertexIdMap;
//	m_bufferParser.LoadFile(filePath);
//
//	char fileExtention[5];
//	char subType;
//	char versionNum;
//	std::string commentText;
//
//	memset(fileExtention, 0, 5);
//	for (int index = 0; index < 4; index++)
//		m_bufferParser.ReadChar(fileExtention[index]);
//
//	if (strcmp(fileExtention, "GC23") != 0)
//		return false;
//
//	m_bufferParser.ReadChar(subType);
//	m_bufferParser.ReadChar(versionNum);
//	m_bufferParser.ReadString(commentText);
//
//	int numOfBones;
//	m_bufferParser.ReadInt(numOfBones);
//
//	for (int index = 0; index < numOfBones; ++index)
//	{
//		BoneData bone;
//
//		m_bufferParser.ReadInt(bone.id);
//		m_bufferParser.ReadString(bone.name);
//
//		m_bufferParser.ReadFloat(bone.translation.x);
//		m_bufferParser.ReadFloat(bone.translation.y);
//		m_bufferParser.ReadFloat(bone.translation.z);
//
//		m_bufferParser.ReadFloat(bone.orientation.x);
//		m_bufferParser.ReadFloat(bone.orientation.y);
//		m_bufferParser.ReadFloat(bone.orientation.z);
//
//		m_bufferParser.ReadFloat(bone.scale.x);
//		m_bufferParser.ReadFloat(bone.scale.y);
//		m_bufferParser.ReadFloat(bone.scale.z);
//
//		m_bufferParser.ReadUChar(bone.color.r);
//		m_bufferParser.ReadUChar(bone.color.g);
//		m_bufferParser.ReadUChar(bone.color.b);
//
//		m_bufferParser.ReadInt(bone.parentId);
//
//		// create a node of this bone, and store it to the map
//		BoneNode* bn = new BoneNode();
//		bn->id = bone.id;
//		bn->parentId = bone.parentId;
//		bn->name = bone.name;
//		
//		bn->matrix.ApplyTransform(bone.translation.x, bone.translation.y, bone.translation.z);
//		bn->matrix.ApplyRotateRadians(bone.orientation.z, 0.0f, 0.0f, 1.0f);
//		bn->matrix.ApplyRotateRadians(bone.orientation.x, 0.0f, 1.0f, 0.0f);
//		bn->matrix.ApplyRotateRadians(bone.orientation.y, 1.0f, 0.0f, 0.0f);
//		bn->matrix.ApplyScale(bone.scale.x, bone.scale.y, bone.scale.z);
//		
//		bn->translation = bone.translation;
//		bn->orientation = bone.orientation;
//		bn->scale = bone.scale;
//		bn->color = bone.color;
//
//		m_boneIdToNodeMap[bone.id] = bn;
//		m_boneNameToNodeMap[bone.name] = bn;
//	}
//
//	//hook up the parent and child relationship
//	std::map<int, BoneNode*>::iterator it = m_boneIdToNodeMap.begin();
//	while (it != m_boneIdToNodeMap.end())
//	{
//		BoneNode* bn = it->second;
//		int parentId = bn->parentId;
//		if (parentId != -1)
//		{
//			if (m_boneIdToNodeMap.find(parentId) == m_boneIdToNodeMap.end())
//				return false;
//
//			bn->parent = m_boneIdToNodeMap[parentId];
//			m_boneIdToNodeMap[parentId]->childs.push_back(bn);
//		}
//		else
//		{
//			m_rootBones.push_back(bn);
//		}
//		++it;
//	}
//
//	//load keyframe data
//	int numOfKeyframe;
//	int numOfBonesInThisFrame;
//	m_bufferParser.ReadInt(numOfKeyframe);
//	for (int frameIndex = 0; frameIndex < numOfKeyframe; ++frameIndex)
//	{
//		m_bufferParser.ReadInt(numOfBonesInThisFrame);
//		std::vector<BoneData> bonesForThisFrame;
//		for (int boneIndex = 0; boneIndex < numOfBonesInThisFrame; ++boneIndex)
//		{
//			BoneData bone;
//			m_bufferParser.ReadInt(bone.id);
//
//			m_bufferParser.ReadFloat(bone.translation.x);
//			m_bufferParser.ReadFloat(bone.translation.y);
//			m_bufferParser.ReadFloat(bone.translation.z);
//
//			m_bufferParser.ReadFloat(bone.orientation.x);
//			m_bufferParser.ReadFloat(bone.orientation.y);
//			m_bufferParser.ReadFloat(bone.orientation.z);
//
//			m_bufferParser.ReadFloat(bone.scale.x);
//			m_bufferParser.ReadFloat(bone.scale.y);
//			m_bufferParser.ReadFloat(bone.scale.z);
//
//			m_bufferParser.ReadInt(bone.parentId);
//			
//			//correct bone's id
//			if (maxBoneIdToVertexIdMap.find(bone.id) == maxBoneIdToVertexIdMap.end() || 
//				(maxBoneIdToVertexIdMap.find(bone.parentId) == maxBoneIdToVertexIdMap.end() && bone.parentId != -1))
//				return false;
//			else
//			{
//				bone.id = maxBoneIdToVertexIdMap[bone.id];
//				bone.parentId = bone.parentId == -1 ? -1 : maxBoneIdToVertexIdMap[bone.parentId];
//			}
//
//			bonesForThisFrame.push_back(bone);
//		}
//		keyframeData.push_back(bonesForThisFrame);
//	}
//
//	return true;
//}


bool C23Parser::WriteModel(const char* filePath, const std::vector<Vertex_PCTNB>& verticesToWrite, const std::vector<int>& indicesToWrite)
{
	std::vector<unsigned char> writeBuffer;
	writeBuffer.reserve(verticesToWrite.size() * sizeof(Vertex_PCTN) + 1024);

	m_bufferParser.WriteChar(writeBuffer, 'G');
	m_bufferParser.WriteChar(writeBuffer, 'C');
	m_bufferParser.WriteChar(writeBuffer, '2');
	m_bufferParser.WriteChar(writeBuffer, '3');
	m_bufferParser.WriteChar(writeBuffer, 2);
	m_bufferParser.WriteChar(writeBuffer, m_version);
	m_bufferParser.WriteString(writeBuffer, "This is a model file.");
	m_bufferParser.WriteUInt(writeBuffer, (unsigned int)indicesToWrite.size());

	for (size_t index = 0; index < indicesToWrite.size(); index++)
	{
		m_bufferParser.WriteUInt(writeBuffer, indicesToWrite[index]);
	}

	m_bufferParser.WriteUInt(writeBuffer, (unsigned int)verticesToWrite.size());

	for (size_t index = 0; index < verticesToWrite.size(); index++)
	{
		m_bufferParser.WriteFloat(writeBuffer, verticesToWrite[index].position.x);
		m_bufferParser.WriteFloat(writeBuffer, verticesToWrite[index].position.y);
		m_bufferParser.WriteFloat(writeBuffer, verticesToWrite[index].position.z);

		m_bufferParser.WriteUChar(writeBuffer, verticesToWrite[index].color.r);
		m_bufferParser.WriteUChar(writeBuffer, verticesToWrite[index].color.g);
		m_bufferParser.WriteUChar(writeBuffer, verticesToWrite[index].color.b);
		m_bufferParser.WriteUChar(writeBuffer, verticesToWrite[index].color.a);
		
		m_bufferParser.WriteFloat(writeBuffer, verticesToWrite[index].texCoords.x);
		m_bufferParser.WriteFloat(writeBuffer, verticesToWrite[index].texCoords.y);
		
		m_bufferParser.WriteFloat(writeBuffer, verticesToWrite[index].normals.normal.x);
		m_bufferParser.WriteFloat(writeBuffer, verticesToWrite[index].normals.normal.y);
		m_bufferParser.WriteFloat(writeBuffer, verticesToWrite[index].normals.normal.z);
		
		m_bufferParser.WriteFloat(writeBuffer, verticesToWrite[index].normals.tangent.x);
		m_bufferParser.WriteFloat(writeBuffer, verticesToWrite[index].normals.tangent.y);
		m_bufferParser.WriteFloat(writeBuffer, verticesToWrite[index].normals.tangent.z);
	
		m_bufferParser.WriteFloat(writeBuffer, verticesToWrite[index].normals.biTangent.x);
		m_bufferParser.WriteFloat(writeBuffer, verticesToWrite[index].normals.biTangent.y);
		m_bufferParser.WriteFloat(writeBuffer, verticesToWrite[index].normals.biTangent.z);

		for (int b = 0; b < MAX_BONE_ATTACH_SUPPORT; ++b)
		{
			m_bufferParser.WriteInt(writeBuffer, verticesToWrite[index].boneIdWeights.ids[b]);
			m_bufferParser.WriteFloat(writeBuffer, verticesToWrite[index].boneIdWeights.weights[b]);
		}
	}

	FILE *file;
	fopen_s(&file, filePath, "wb");
	fwrite(writeBuffer.data(), sizeof(unsigned char), writeBuffer.size(), file);
	fclose(file);

	return true;
}



bool C23Parser::WriteAnim(const char* filePath, const std::vector<BoneData>& bonesToWrite, std::map<int, std::vector<BoneData>>& keyframeDataToWrite)
{
	std::vector<unsigned char> writeBuffer;
	writeBuffer.reserve(bonesToWrite.size() * sizeof(BoneData) + 1024);

	m_bufferParser.WriteChar(writeBuffer, 'G');
	m_bufferParser.WriteChar(writeBuffer, 'C');
	m_bufferParser.WriteChar(writeBuffer, '2');
	m_bufferParser.WriteChar(writeBuffer, '3');
	m_bufferParser.WriteChar(writeBuffer, 1);
	m_bufferParser.WriteChar(writeBuffer, m_version);
	m_bufferParser.WriteString(writeBuffer, "This is a skeletal file.");
	m_bufferParser.WriteUInt(writeBuffer, (unsigned int)bonesToWrite.size());

	for (size_t index = 0; index < bonesToWrite.size(); index++)
	{
		m_bufferParser.WriteInt(writeBuffer, bonesToWrite[index].id);
		m_bufferParser.WriteString(writeBuffer, bonesToWrite[index].name);
		
		m_bufferParser.WriteFloat(writeBuffer, bonesToWrite[index].translation.x);
		m_bufferParser.WriteFloat(writeBuffer, bonesToWrite[index].translation.y);
		m_bufferParser.WriteFloat(writeBuffer, bonesToWrite[index].translation.z);
		
		m_bufferParser.WriteFloat(writeBuffer, bonesToWrite[index].orientation.x);
		m_bufferParser.WriteFloat(writeBuffer, bonesToWrite[index].orientation.y);
		m_bufferParser.WriteFloat(writeBuffer, bonesToWrite[index].orientation.z);
		
		m_bufferParser.WriteFloat(writeBuffer, bonesToWrite[index].scale.x);
		m_bufferParser.WriteFloat(writeBuffer, bonesToWrite[index].scale.y);
		m_bufferParser.WriteFloat(writeBuffer, bonesToWrite[index].scale.z);

		m_bufferParser.WriteChar(writeBuffer, bonesToWrite[index].color.r);
		m_bufferParser.WriteChar(writeBuffer, bonesToWrite[index].color.g);
		m_bufferParser.WriteChar(writeBuffer, bonesToWrite[index].color.b);
		
		m_bufferParser.WriteInt(writeBuffer, bonesToWrite[index].parentId);
	}

	//skip match table for now
	//skip size of animations for now
	//skip animation name for now
	m_bufferParser.WriteInt(writeBuffer, (unsigned int)keyframeDataToWrite.size());
	for (size_t frameIndex = 0; frameIndex < keyframeDataToWrite.size(); ++frameIndex)
	{
		m_bufferParser.WriteInt(writeBuffer, (unsigned int)keyframeDataToWrite[frameIndex].size());
		for (size_t boneIndex = 0; boneIndex < keyframeDataToWrite[frameIndex].size(); ++boneIndex)
		{
			m_bufferParser.WriteInt(writeBuffer, keyframeDataToWrite[frameIndex][boneIndex].id);

			m_bufferParser.WriteFloat(writeBuffer, keyframeDataToWrite[frameIndex][boneIndex].translation.x);
			m_bufferParser.WriteFloat(writeBuffer, keyframeDataToWrite[frameIndex][boneIndex].translation.y);
			m_bufferParser.WriteFloat(writeBuffer, keyframeDataToWrite[frameIndex][boneIndex].translation.z);

			m_bufferParser.WriteFloat(writeBuffer, keyframeDataToWrite[frameIndex][boneIndex].orientation.x);
			m_bufferParser.WriteFloat(writeBuffer, keyframeDataToWrite[frameIndex][boneIndex].orientation.y);
			m_bufferParser.WriteFloat(writeBuffer, keyframeDataToWrite[frameIndex][boneIndex].orientation.z);

			m_bufferParser.WriteFloat(writeBuffer, keyframeDataToWrite[frameIndex][boneIndex].scale.x);
			m_bufferParser.WriteFloat(writeBuffer, keyframeDataToWrite[frameIndex][boneIndex].scale.y);
			m_bufferParser.WriteFloat(writeBuffer, keyframeDataToWrite[frameIndex][boneIndex].scale.z);

			m_bufferParser.WriteInt(writeBuffer, keyframeDataToWrite[frameIndex][boneIndex].parentId);
		}
	}
	//skip keyframe time tag (% of whole animation), maybe not needed?
	//skip curve method for now


	FILE *file;
	fopen_s(&file, filePath, "wb");
	fwrite(writeBuffer.data(), sizeof(unsigned char), writeBuffer.size(), file);
	fclose(file);

	return true;
}

};