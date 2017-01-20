#include "stdafx.h"
#include "CriticalSection.h"
#include "MultiThreadSync.h"
#include "MemoryPool.h"
#include "Stream.h"
#include "CircularQueue.h"
#include "Registry.h"
using namespace std;

int main() {
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

	cin.get();
}
