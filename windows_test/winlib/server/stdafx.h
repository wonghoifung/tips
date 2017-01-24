#pragma once

#define MAX_USER 100
#define DEFAULT_PORT 1820

#include "Global.h"

#pragma comment(lib, "C:\\Users\\albert\\Desktop\\csharp_test\\winlib\\Debug\\winlib.lib")

typedef struct _USER
{
	WCHAR szUserID[32];
	WCHAR szUserName[32];
	DWORD dwAge;
	BYTE cSex;
	WCHAR szAddress[32];
} USER;

typedef struct _COMPUTER
{
	WCHAR szComputerName[32];
	WCHAR szIPAddress[32];
	BYTE cCPUType;
	DWORD dwRam;
	DWORD dwHDD;
} COMPUTER;

typedef struct _PROGRAM
{
	WCHAR szProgramName[32];
	DWORD dwCost;
	WCHAR szComment[32];
} PROGRAM;

#include "CriticalSection.h"
#include "MultiThreadSync.h"
#include "MemoryPool.h"
#include "CircularQueue.h"
#include "NetworkSession.h"
#include "PacketSession.h"
#include "Iocp.h"
#include "Stream.h"
#include "MiniDump.h"
#include "IniFile.h"

#include "Protocol.h"
#include "PT_Structure.h"
#include "PT_ReadPacket.h"
#include "PT_WritePacket.h"

#include "ConnectedSession.h"
#include "ConnectedSessionManager.h"
#include "ServerIocp.h"
