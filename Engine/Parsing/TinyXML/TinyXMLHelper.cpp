#include "TinyXMLHelper.hpp"

#include <string>
#include <Windows.h>

namespace Henry
{

TinyXMLHelper::TinyXMLHelper(void) 
:	m_metaDoc(nullptr)
,	m_root(nullptr)
{
}


TinyXMLHelper::~TinyXMLHelper(void)
{
	if(m_metaDoc)
	{
		delete m_metaDoc;
		m_metaDoc = nullptr;
	}
}


bool TinyXMLHelper::LoadFile(const char* filePath, bool quitProgramIfNotExists)
{
	m_metaDoc = new	TiXmlDocument(filePath);
	bool loadSuccessful = m_metaDoc->LoadFile();
	if(!loadSuccessful)
	{
		std::string errorInfo(filePath);
		errorInfo = "Failed to load file : " + errorInfo;
		MessageBoxA( NULL , errorInfo.c_str(), "Failed to loading files", MB_ICONERROR | MB_OK );
		if (quitProgramIfNotExists)
			exit(0);
	}

	if (loadSuccessful && m_metaDoc->ErrorId() > 0)
	{
		std::string errorInfo(filePath);
		errorInfo = "Error , can't analysis meta-data file : " + errorInfo + "\n\n";
		MessageBoxA( NULL , (LPCSTR)errorInfo.c_str() , "Load Failed!", MB_ICONERROR | MB_OK );
		loadSuccessful = false;
	}

	m_root = m_metaDoc->RootElement();
	if (loadSuccessful && !m_root)
	{
		std::string errorInfo(filePath);
		errorInfo = "Error , can't find root element of meta-data file : " + errorInfo + "\n\n";
		MessageBoxA( NULL , (LPCSTR)errorInfo.c_str() , "Load Failed!", MB_ICONERROR | MB_OK );
		loadSuccessful = false;
	}

	return loadSuccessful;
}


int TinyXMLHelper::ToInt(const TiXmlElement* elem, const char* attributeName)
{
	return (elem->Attribute(attributeName) == NULL ? 0 : atoi(elem->Attribute(attributeName)));
}


float TinyXMLHelper::ToFloat(const TiXmlElement* elem, const char* attributeName)
{
	return (elem->Attribute(attributeName) == NULL ? 0.0f : (float)atof(elem->Attribute(attributeName)));
}


char* TinyXMLHelper::ToString(const TiXmlElement* elem, const char* attributeName)
{
	return (elem->Attribute(attributeName) == NULL ? "" : elem->Attribute(attributeName));
}


bool TinyXMLHelper::IsValid(const TiXmlElement* elem, const char* attributeName)
{
	if (elem->Attribute(attributeName) == NULL)
		return false;
	return true;
}


};