#pragma once

class CIniFile
{
public:
	CIniFile();
	virtual ~CIniFile();

	BOOL Open(LPCTSTR fileName);
	BOOL Close();

	BOOL SetValue(LPCTSTR keyName, LPCTSTR valueName, LPCTSTR value);
	BOOL SetValue(LPCTSTR keyName, LPCTSTR valueName, DWORD value);
	BOOL SetValue(LPCTSTR keyName, LPCTSTR valueName, FLOAT value);

	BOOL GetValue(LPCTSTR keyName, LPCTSTR valueName, LPTSTR value, LPDWORD bufferLength);
	BOOL GetValue(LPCTSTR keyName, LPCTSTR valueName, LPDWORD value);
	BOOL GetValue(LPCTSTR keyName, LPCTSTR valueName, FLOAT *value);

private:
	TCHAR mFileName[MAX_PATH];
};
