#pragma once

#define DEFAULT_PORT 1820

#include "Global.h"

#include "CriticalSection.h"
#include "MultiThreadSync.h"
#include "MemoryPool.h"
#include "CircularQueue.h"
#include "NetworkSession.h"
#include "PacketSession.h"
#include "EventSelect.h"
#include "Stream.h"
#include "MiniDump.h"
#include "IniFile.h"
#include "ClientSession.h"

#include "../server/Protocol.h"
#include "../server/PT_Structure.h"
#include "../server/PT_ReadPacket.h"
#include "../server/PT_WritePacket.h"

#pragma comment(lib, "C:\\Users\\albert\\Desktop\\csharp_test\\winlib\\Debug\\winlib.lib")
