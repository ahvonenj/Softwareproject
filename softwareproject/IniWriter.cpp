#include "IniWriter.h"

IniWriter::IniWriter(char* szFileName)
{
 memset(m_szFileName, 0x00, 255);
 memcpy(m_szFileName, szFileName, strlen(szFileName));
}

void IniWriter::WriteInteger(char* szSection, char* szKey, int iValue)
{
 char szValue[255];
 sprintf(szValue, "%d", iValue);
 WritePrivateProfileStringA(szSection,  szKey, szValue, m_szFileName); 
}

void IniWriter::WriteFloat(char* szSection, char* szKey, float fltValue)
{
 char szValue[255];
 sprintf(szValue, "%f", fltValue);
 WritePrivateProfileStringA(szSection,  szKey, szValue, m_szFileName); 
}

void IniWriter::WriteBoolean(char* szSection, char* szKey, bool bolValue)
{
 char szValue[255];
 sprintf(szValue, "%s", bolValue ? "True" : "False");
 WritePrivateProfileStringA(szSection,  szKey, szValue, m_szFileName); 
}

void IniWriter::WriteString(char* szSection, char* szKey, char* szValue)
{
 WritePrivateProfileStringA(szSection,  szKey, szValue, m_szFileName);
}

//convertCharArrayToLPCWSTR
wchar_t* IniWriter::AToL(const char* charArray)
{
    wchar_t* wString=new wchar_t[4096];
    MultiByteToWideChar(CP_ACP, 0, charArray, -1, wString, 4096);
    return wString;
}
