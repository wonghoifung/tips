#include "stdafx.h"

#include "CriticalSection.h"
#include "Registry.h"
#include "Log.h"

#define MAX_STRING_LENGTH		409600

typedef struct _PERFORMANCE_COUNTER
{
	DWORD					Index;
	TCHAR					Name[32];
	DWORD					Value;
	PERF_COUNTER_DEFINITION	Data;
} PERFORMANCE_COUNTER;

static std::vector<PERFORMANCE_COUNTER> gCounterVector;
static CCriticalSection					gSync;
static PERF_OBJECT_TYPE					gHeader;

static TCHAR							gReadString[MAX_STRING_LENGTH]			= {0,}; 
static DWORD							gReadStringLength						= sizeof(gReadString);

static TCHAR							gReadHelpString[MAX_STRING_LENGTH]		= {0,};
static DWORD							gReadHelpStringLength					= sizeof(gReadHelpString);

static TCHAR							gObjectListString[MAX_STRING_LENGTH]	= {0,};

static DWORD							gLastIndex								= 0;

static BYTE								gEnd[2]									= {0, 0};

BOOL AddString(LPTSTR source, LPTSTR addItem, DWORD &length)
{
	if (!source || !addItem)
		return FALSE;

	CopyMemory(source + length / sizeof(TCHAR) - 1, addItem, _tcslen(addItem) * sizeof(TCHAR));
	length += (DWORD) _tcslen(addItem) * sizeof(TCHAR);

	CopyMemory(source + length / sizeof(TCHAR), gEnd, sizeof(gEnd));
	length += sizeof(gEnd);

	return TRUE;
}

BOOL AddDWORD(LPTSTR source, DWORD addItem, DWORD &length)
{
	if (!source)
		return FALSE;

	TCHAR AddItem[16] = {0,};
	_sntprintf(AddItem, 16, _T("%d"), addItem);

	CopyMemory(source + length / sizeof(TCHAR) - 1, AddItem, _tcslen(AddItem) * sizeof(TCHAR));
	length += (DWORD) _tcslen(AddItem) * sizeof(TCHAR);

	CopyMemory(source + length / sizeof(TCHAR), gEnd, sizeof(gEnd));
	length += sizeof(gEnd);

	return TRUE;
}

BOOL GetIndex(LPTSTR source, LPTSTR counterName, DWORD &index)
{
	TCHAR *PreviosString	= NULL;
	DWORD Length			= (DWORD) _tcslen(source) + 1;

	if (!_tcscmp(source, counterName))
		return FALSE;

	while (Length)
	{
		if (!_tcscmp(source + Length, counterName))
		{
			index = _ttoi(PreviosString);

			return TRUE;
		}

		PreviosString	= source + Length;
		Length			+= (DWORD) _tcslen(source + Length) + 1;
	}

	return FALSE;
}

BOOL GetName(LPTSTR source, DWORD index, LPTSTR counterName)
{
	TCHAR	*PreviosString	= NULL;
	DWORD	Length			= (DWORD) _tcslen(source) + 1;
	TCHAR	Index[16]		= {0,};

	_itot(index, Index, 10);

	if (!_tcscmp(source, Index))
		return FALSE;

	while (Length)
	{
		if (!_tcscmp(source + Length, Index))
		{
			Length		+= (DWORD) _tcslen(source + Length) + 1;
			
			_tcscpy(counterName, source + Length);

			return TRUE;
		}

		PreviosString	= source + Length;
		Length			+= (DWORD) _tcslen(source + Length) + 1;
	}

	return FALSE;
}

extern "C"
{
	BOOL __declspec(dllexport) Begin(LPTSTR dllPath)
	{
		gSync.Enter();

		if (!dllPath)
		{
			gSync.Leave();

			return FALSE;
		}

		ZeroMemory(&gHeader, sizeof(gHeader));

		CRegistry Registry;

		Registry.Open(HKEY_LOCAL_MACHINE, _T("SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Perflib\\012"));
		Registry.GetValue(_T("Counter"), gReadString, &gReadStringLength);
		Registry.GetValue(_T("Help"), gReadHelpString, &gReadHelpStringLength);
		Registry.Close();

		Registry.Open(HKEY_LOCAL_MACHINE, _T("SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Perflib"));
		Registry.GetValue(_T("Last Counter"), &gLastIndex);
		Registry.Close();

		if (!Registry.Open(HKEY_LOCAL_MACHINE, _T("SYSTEM\\CurrentControlSet\\Services\\Performance Provider\\Performance")))
		{
			Registry.CreateKey(HKEY_LOCAL_MACHINE, _T("SYSTEM\\CurrentControlSet\\Services\\Performance Provider\\Performance"));

			_sntprintf(gObjectListString, MAX_STRING_LENGTH, _T("%d"), gLastIndex + 2);
			Registry.SetValue(_T("Object List"), gObjectListString);

			Registry.SetValue(_T("Open"), _T("PP_Open"));
			Registry.SetValue(_T("Close"), _T("PP_Close"));
			Registry.SetValue(_T("Collect"), _T("PP_Collect"));
			Registry.SetValue(_T("Library"), dllPath);

			Registry.Close();

			Registry.Open(HKEY_LOCAL_MACHINE, _T("SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Perflib"));
			Registry.SetValue(_T("Last Counter"), gLastIndex + 2);
			Registry.SetValue(_T("Last Help"), gLastIndex + 3);
			Registry.Close();

			AddDWORD(gReadString, gLastIndex + 2, gReadStringLength);
			AddString(gReadString, _T("Performance Provider"), gReadStringLength);

			AddDWORD(gReadHelpString, gLastIndex + 3, gReadHelpStringLength);
			AddString(gReadHelpString, _T("Performance Provider Help"), gReadHelpStringLength);

			Registry.Open(HKEY_LOCAL_MACHINE, _T("SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Perflib\\012"));
			Registry.SetValueForMultiSz(_T("Counter"), gReadString, gReadStringLength);
			Registry.SetValueForMultiSz(_T("Help"), gReadHelpString, gReadHelpStringLength);
			Registry.Close();

			gLastIndex	= gLastIndex + 2;
		}
		else
		{
			DWORD MaxStringLength = MAX_STRING_LENGTH;

			Registry.GetValue(_T("Object List"), gObjectListString, &MaxStringLength);

			Registry.Close();
		}

		TCHAR	Seperator[]				= _T(" ");
		LPTSTR	Token					= NULL;

		Token = _tcstok(gObjectListString, Seperator);

		DWORD FirstIndex				= _ttoi(Token);

		gHeader.TotalByteLength			= (DWORD) gCounterVector.size() * sizeof(PERF_COUNTER_DEFINITION) + sizeof(gHeader) + sizeof(DWORD) * gCounterVector.size() + sizeof(DWORD);
		gHeader.DefinitionLength		= (DWORD) gCounterVector.size() * sizeof(PERF_COUNTER_DEFINITION) + sizeof(gHeader);
		gHeader.HeaderLength			= sizeof(gHeader);
		gHeader.ObjectNameTitleIndex	= FirstIndex;
		gHeader.ObjectNameTitle			= NULL;
		gHeader.ObjectHelpTitleIndex	= FirstIndex + 1;
		gHeader.ObjectHelpTitle			= NULL;
		gHeader.DetailLevel				= PERF_DETAIL_NOVICE;
		gHeader.NumCounters				= (DWORD) gCounterVector.size();
		gHeader.DefaultCounter			= 0;
		gHeader.NumInstances			= PERF_NO_INSTANCES;
		gHeader.CodePage				= 0;

		ZeroMemory(&gHeader.PerfTime, sizeof(gHeader.PerfTime));
		ZeroMemory(&gHeader.PerfFreq, sizeof(gHeader.PerfFreq));

		Token = _tcstok(NULL, Seperator);

		while (Token)
		{
			DWORD Index = _ttoi(Token);

			PERFORMANCE_COUNTER pc;

			ZeroMemory(&pc, sizeof(pc));

			pc.Index						= Index;
			pc.Value						= 0;

			pc.Data.ByteLength				= sizeof(pc.Data);
			pc.Data.CounterNameTitleIndex	= Index;
			pc.Data.CounterNameTitle		= NULL;
			pc.Data.CounterHelpTitleIndex	= Index + 1;
			pc.Data.CounterHelpTitle		= NULL;
			pc.Data.DefaultScale			= 0;
			pc.Data.DetailLevel				= PERF_DETAIL_NOVICE;
			pc.Data.CounterType				= PERF_COUNTER_RAWCOUNT;
			pc.Data.CounterSize				= sizeof(DWORD);
			pc.Data.CounterOffset			= sizeof(DWORD);

			GetName(gReadString, Index, pc.Name);

			gCounterVector.push_back(pc);

			Token = _tcstok(NULL, Seperator);
		}

		gHeader.TotalByteLength			= (DWORD) gCounterVector.size() * sizeof(PERF_COUNTER_DEFINITION) + sizeof(gHeader) + sizeof(DWORD) * gCounterVector.size() + sizeof(DWORD);
		gHeader.DefinitionLength		= (DWORD) gCounterVector.size() * sizeof(PERF_COUNTER_DEFINITION) + sizeof(gHeader);
		gHeader.NumCounters				= (DWORD) gCounterVector.size();

		gSync.Leave();

		return TRUE;
	}

	BOOL __declspec(dllexport) End(VOID)
	{
		return TRUE;
	}

	BOOL __declspec(dllexport) AddCounter(LPTSTR name)
	{
		gSync.Enter();

		if (!name)
		{
			gSync.Leave();

			return FALSE;
		}

		for (DWORD i=0;i<gCounterVector.size();i++)
		{
			if (!_tcscmp(gCounterVector[i].Name, name))
			{
				gSync.Leave();

				return FALSE;
			}
		}

		CRegistry Registry;
		Registry.Open(HKEY_LOCAL_MACHINE, _T("SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Perflib"));
		Registry.GetValue(_T("Last Counter"), &gLastIndex);
		Registry.Close();

		AddDWORD(gReadString, gLastIndex + 2, gReadStringLength);
		AddString(gReadString, name, gReadStringLength);

		AddDWORD(gReadHelpString, gLastIndex + 3, gReadHelpStringLength);
		AddString(gReadHelpString, name, gReadHelpStringLength);

		_sntprintf(gObjectListString, MAX_STRING_LENGTH, _T("%s %d"), gObjectListString, gLastIndex + 2);

		Registry.Open(HKEY_LOCAL_MACHINE, _T("SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Perflib\\012"));
		Registry.SetValueForMultiSz(_T("Counter"), gReadString, gReadStringLength);
		Registry.SetValueForMultiSz(_T("Help"), gReadHelpString, gReadHelpStringLength);
		Registry.Close();
		
		Registry.Open(HKEY_LOCAL_MACHINE, _T("SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Perflib"));
		Registry.SetValue(_T("Last Counter"), gLastIndex + 2);
		Registry.SetValue(_T("Last Help"), gLastIndex + 3);
		Registry.Close();
		
		Registry.Open(HKEY_LOCAL_MACHINE, _T("SYSTEM\\CurrentControlSet\\Services\\Performance Provider\\Performance"));
		Registry.SetValue(_T("Object List"), gObjectListString);
		Registry.Close();
		
		PERFORMANCE_COUNTER pc;

		pc.Index						= gLastIndex + 2;
		pc.Value						= 0;

		pc.Data.ByteLength				= sizeof(pc.Data);
		pc.Data.CounterNameTitleIndex	= gLastIndex + 2;
		pc.Data.CounterNameTitle		= NULL;
		pc.Data.CounterHelpTitleIndex	= gLastIndex + 3;
		pc.Data.CounterHelpTitle		= NULL;
		pc.Data.DefaultScale			= 0;
		pc.Data.DetailLevel				= PERF_DETAIL_NOVICE;
		pc.Data.CounterType				= PERF_COUNTER_RAWCOUNT;
		pc.Data.CounterSize				= sizeof(DWORD);
		pc.Data.CounterOffset			= sizeof(DWORD);

		GetName(gReadString, gLastIndex + 2, pc.Name);

		gCounterVector.push_back(pc);
		
		// 5. Header º¯°æ
		gHeader.TotalByteLength			= (DWORD) gCounterVector.size() * sizeof(PERF_COUNTER_DEFINITION) + sizeof(gHeader) + sizeof(DWORD) * gCounterVector.size() + sizeof(DWORD);
		gHeader.DefinitionLength		= (DWORD) gCounterVector.size() * sizeof(PERF_COUNTER_DEFINITION) + sizeof(gHeader);
		gHeader.NumCounters				= (DWORD) gCounterVector.size();

        gLastIndex = gLastIndex + 2;

		gSync.Leave();

		return TRUE;
	}

	BOOL __declspec(dllexport) UpdateCounter(LPTSTR name, DWORD value)
	{
		gSync.Enter();

		for (DWORD i=0;i<gCounterVector.size();i++)
		{
			if (!_tcscmp(gCounterVector[i].Name, name))
			{
				gCounterVector[i].Value = value;
				
				gSync.Leave();

				return TRUE;
			}
		}

		gSync.Leave();

		return FALSE;
	}

	DWORD __declspec(dllexport) WINAPI PP_Open(LPWSTR lpDevNames)
	{
		gSync.Enter();

		ZeroMemory(&gHeader, sizeof(gHeader));

		CRegistry Registry;

		Registry.Open(HKEY_LOCAL_MACHINE, _T("SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Perflib\\012"));
		Registry.GetValue(_T("Counter"), gReadString, &gReadStringLength);
		Registry.GetValue(_T("Help"), gReadHelpString, &gReadHelpStringLength);
		Registry.Close();

		Registry.Open(HKEY_LOCAL_MACHINE, _T("SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Perflib"));
		Registry.GetValue(_T("Last Counter"), &gLastIndex);
		Registry.Close();

		if (Registry.Open(HKEY_LOCAL_MACHINE, _T("SYSTEM\\CurrentControlSet\\Services\\Performance Provider\\Performance")))
		{
			DWORD MaxStringLength = MAX_STRING_LENGTH;

			Registry.GetValue(_T("Object List"), gObjectListString, &MaxStringLength);

			Registry.Close();

			TCHAR	Seperator[]				= _T(" ");
			LPTSTR	Token					= NULL;

			Token = _tcstok(gObjectListString, Seperator);

			DWORD FirstIndex				= _ttoi(Token);

			gHeader.TotalByteLength			= (DWORD) gCounterVector.size() * sizeof(PERF_COUNTER_DEFINITION) + sizeof(gHeader) + sizeof(DWORD) * gCounterVector.size() + sizeof(DWORD);
			gHeader.DefinitionLength		= (DWORD) gCounterVector.size() * sizeof(PERF_COUNTER_DEFINITION) + sizeof(gHeader);
			gHeader.HeaderLength			= sizeof(gHeader);
			gHeader.ObjectNameTitleIndex	= FirstIndex;
			gHeader.ObjectNameTitle			= NULL;
			gHeader.ObjectHelpTitleIndex	= FirstIndex + 1;
			gHeader.ObjectHelpTitle			= NULL;
			gHeader.DetailLevel				= PERF_DETAIL_NOVICE;
			gHeader.NumCounters				= (DWORD) gCounterVector.size();
			gHeader.DefaultCounter			= 0;
			gHeader.NumInstances			= PERF_NO_INSTANCES;
			gHeader.CodePage				= 0;

			ZeroMemory(&gHeader.PerfTime, sizeof(gHeader.PerfTime));
			ZeroMemory(&gHeader.PerfFreq, sizeof(gHeader.PerfFreq));

			Token = _tcstok(NULL, Seperator);

			while (Token)
			{
				DWORD Index = _ttoi(Token);

				PERFORMANCE_COUNTER pc;

				ZeroMemory(&pc, sizeof(pc));

				pc.Index						= Index;
				pc.Value						= 0;

				pc.Data.ByteLength				= sizeof(pc.Data);
				pc.Data.CounterNameTitleIndex	= Index;
				pc.Data.CounterNameTitle		= NULL;
				pc.Data.CounterHelpTitleIndex	= Index + 1;
				pc.Data.CounterHelpTitle		= NULL;
				pc.Data.DefaultScale			= 0;
				pc.Data.DetailLevel				= PERF_DETAIL_NOVICE;
				pc.Data.CounterType				= PERF_COUNTER_RAWCOUNT;
				pc.Data.CounterSize				= sizeof(DWORD);
				pc.Data.CounterOffset			= sizeof(DWORD);

				GetName(gReadString, Index, pc.Name);

				gCounterVector.push_back(pc);

				Token = _tcstok(NULL, Seperator);
			}

			gHeader.TotalByteLength			= (DWORD) gCounterVector.size() * sizeof(PERF_COUNTER_DEFINITION) + sizeof(gHeader) + sizeof(DWORD) * gCounterVector.size() + sizeof(DWORD);
			gHeader.DefinitionLength		= (DWORD) gCounterVector.size() * sizeof(PERF_COUNTER_DEFINITION) + sizeof(gHeader);
			gHeader.NumCounters				= (DWORD) gCounterVector.size();
		}

		gSync.Leave();

		return(ERROR_SUCCESS);
	}

	DWORD __declspec(dllexport) WINAPI PP_Close(VOID)
	{
		return(ERROR_SUCCESS);
	}

	DWORD __declspec(dllexport) WINAPI PP_Collect(LPWSTR lpValueName, 
		LPVOID* lppData, 
		LPDWORD lpcbTotalBytes, 
		LPDWORD lpNumObjectTypes)
	{
		gSync.Enter();
		*lpcbTotalBytes		= gHeader.TotalByteLength;
		*lpNumObjectTypes	= (DWORD) gCounterVector.size() + 1;

		PBYTE *Data			= (PBYTE*) lppData;

		CopyMemory(*Data, &gHeader, sizeof(gHeader));
		*Data += sizeof(gHeader);

		for (DWORD i=0;i<gCounterVector.size();i++)
		{
			CopyMemory(*Data, &gCounterVector[i].Data, sizeof(gCounterVector[i].Data));
			*Data += sizeof(gCounterVector[i].Data);
		}

		DWORD ValueLength = (DWORD) (sizeof(DWORD) * gCounterVector.size());
		CopyMemory(*Data, &ValueLength, sizeof(DWORD));
		*Data += sizeof(DWORD);
		
		for (DWORD i=0;i<gCounterVector.size();i++)
		{
			CopyMemory(*Data, &gCounterVector[i].Value, sizeof(DWORD));
			*Data += sizeof(DWORD);
		}

		gSync.Leave();

		return(ERROR_SUCCESS);
	}
}

#pragma comment(linker, "/export:PP_Open=_PP_Open@4")
#pragma comment(linker, "/export:PP_Close=_PP_Close@0")
#pragma comment(linker, "/export:PP_Collect=_PP_Collect@16")
