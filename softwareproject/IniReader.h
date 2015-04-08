#pragma once
#include <iostream>
#include <Windows.h>
#include <stdlib.h>

class IniReader
{

public:
	IniReader();
	IniReader(char* szFileName); 
	int ReadInteger(char* szSection, char* szKey, int iDefaultValue);
	float ReadFloat(char* szSection, char* szKey, float fltDefaultValue);
	bool ReadBoolean(char* szSection, char* szKey, bool bolDefaultValue);
	char* ReadString(char* szSection, char* szKey, const char* szDefaultValue);

private:
	char m_szFileName[255];
	wchar_t* AToL(const char* charArray);

};