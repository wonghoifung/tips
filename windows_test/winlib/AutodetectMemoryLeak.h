#if !defined (__AutoDetectMemoryLeak_h__)
#define __AutoDetectMemoryLeak_h__

#if defined(_MSC_VER) && defined (_DEBUG)

#define _CRTDBG_MAP_ALLOC 
#include <crtdbg.h>
#if !defined (_CONSOLE)
#include <cstdlib> // for Consol Application
#endif

class __AutoDetectMemoryLeak
{
public:
	__AutoDetectMemoryLeak ()
	{
		_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

#if defined (_CONSOLE)
		// Send all reports to STDOUT
		_CrtSetReportMode( _CRT_WARN, _CRTDBG_MODE_FILE   );
		_CrtSetReportFile( _CRT_WARN, _CRTDBG_FILE_STDOUT );
		_CrtSetReportMode( _CRT_ERROR, _CRTDBG_MODE_FILE   );
		_CrtSetReportFile( _CRT_ERROR, _CRTDBG_FILE_STDOUT );
		_CrtSetReportMode( _CRT_ASSERT, _CRTDBG_MODE_FILE   );
		_CrtSetReportFile( _CRT_ASSERT, _CRTDBG_FILE_STDOUT );

#define DEBUG_NORMALBLOCK new(_NORMAL_BLOCK, __FILE__, __LINE__)
#ifdef new
#undef new
#endif
#define new DEBUG_NORMALBLOCK

#else
		// Send all reports to DEBUG window
		_CrtSetReportMode( _CRT_WARN, _CRTDBG_MODE_DEBUG  );
		_CrtSetReportMode( _CRT_ERROR, _CRTDBG_MODE_DEBUG  );
		_CrtSetReportMode( _CRT_ASSERT, _CRTDBG_MODE_DEBUG  );
#endif

#ifdef malloc
#undef malloc
#endif

#define malloc(s) (_malloc_dbg( s, _NORMAL_BLOCK, __FILE__, __LINE__ ))
	}
};

static __AutoDetectMemoryLeak __autoDetectMemoryLeak;

#endif // if defined(_MSC_VER) && defined (_DEBUG)

#endif // __AutoDetectMemoryLeak_h__
