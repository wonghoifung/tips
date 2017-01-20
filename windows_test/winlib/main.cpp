#include "stdafx.h"
#include "CriticalSection.h"
#include "MultiThreadSync.h"
#include "MemoryPool.h"
#include "Stream.h"
#include "CircularQueue.h"
#include "Registry.h"
#include "IniFile.h"
using namespace std;

static void testIni();
static void testRegistry();

int main() {

	cin.get();
}

static void testIni() {
	CIniFile ini;
	ini.Open(L"C:\\Users\\albert\\Desktop\\csharp_test\\test_sqlserver2\\winlib.ini");
	ini.SetValue(L"SECTION", L"author", (FLOAT)123);
	FLOAT val = 0;
	ini.GetValue(L"SECTION", L"author", &val);
	cout << val << endl;
}

static void testRegistry() {
	CRegistry reg;
	if (!reg.CreateKey(HKEY_CURRENT_USER, L"Software\\WINLIB")) {
		cout << "cannot createkey" << endl;
	}

	cout << "to set value" << endl;
	cin.get();
	if (!reg.SetValue(L"author", 123)) {
		cout << "cannot setvalue" << endl;
	}

	cout << "to get value" << endl;
	cin.get();
	DWORD val = 0;
	if (!reg.GetValue(L"author", &val)) {
		cout << "cannot getvalue" << endl;
	}
	cout << val << endl;

	cout << "to del value" << endl;
	cin.get();
	if (!reg.DeleteValue(L"author")) {
		cout << "cannot deletevalue" << endl;
	}

	cout << "to del key" << endl;
	cin.get();
	if (!reg.DeleteKey(HKEY_CURRENT_USER, L"SOFTWARE\\WINLIB")) {
		cout << "cannot deletekey" << endl;
	}
}
