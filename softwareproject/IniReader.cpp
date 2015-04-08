#include "IniReader.h"

IniReader::IniReader()
{
	char szFileName[] = "Resources/settings/customgamesettings.ini";
	memset(m_szFileName, 0x00, 255);
	memcpy(m_szFileName, szFileName, strlen(szFileName));
}

IniReader::IniReader(char* szFileName)
{
 memset(m_szFileName, 0x00, 255);
 memcpy(m_szFileName, szFileName, strlen(szFileName));
}

int IniReader::ReadInteger(char* szSection, char* szKey, int iDefaultValue)
{
	int iResult = 0;
	iResult = GetPrivateProfileIntA(szSection,  szKey, iDefaultValue, m_szFileName); 
	return iResult;
}

float IniReader::ReadFloat(char* szSection, char* szKey, float fltDefaultValue)
{
	char szResult[255];
	char szDefault[255];
	float fltResult = 0.f;
	sprintf(szDefault, "%f",fltDefaultValue);

	GetPrivateProfileStringA(szSection, szKey, szDefault, szResult, 255, m_szFileName);
	fltResult = std::atof(szResult);
	return fltResult;
}

bool IniReader::ReadBoolean(char* szSection, char* szKey, bool bolDefaultValue)
{
	char szResult[255];
	char szDefault[255];
	bool bolResult;
	sprintf(szDefault, "%s", bolDefaultValue? "True" : "False");
	GetPrivateProfileStringA(szSection, szKey, szDefault, szResult, 255, m_szFileName); 
	bolResult =  (strcmp(szResult, "True") == 0 || 
		strcmp(szResult, "true") == 0) ? true : false;
	return bolResult;
}

char* IniReader::ReadString(char* szSection, char* szKey, const char* szDefaultValue)
{
	char szResult[255];
	memset(szResult, 0x00, 255);
	GetPrivateProfileStringA(szSection,  szKey, szDefaultValue, szResult, 255, m_szFileName); 
	return szResult;
}

//convertCharArrayToLPCWSTR
wchar_t* IniReader::AToL(const char* charArray)
{
	wchar_t* wString=new wchar_t[4096];
	MultiByteToWideChar(CP_ACP, 0, charArray, -1, wString, 4096);
	return wString;
}