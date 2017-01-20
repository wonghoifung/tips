#include "stdafx.h"
#include "CriticalSection.h"
#include "MultiThreadSync.h"
#include "MemoryPool.h"
#include "Stream.h"
#include "CircularQueue.h"
using namespace std;

int main() {
	void* dummy=NULL;

	for (int i = 0; i < 10; ++i) {
		CLog::WriteLog(_T("%s, %d"), L"hello", i + 1);
	}

	CCircularQueue* cq = new CCircularQueue;
	cq->Begin();
	char* s1="hello";
	char* s2="world";
	cq->Push(cq, (BYTE*)s1, strlen(s1));
	cq->Push(cq, (BYTE*)s2, strlen(s2));
	char buf[32]={0};
	DWORD len=0;
	
	cq->Pop(&dummy, (BYTE*)buf, len);
	cout<<buf<<" "<<len<<endl;
	cq->Pop(&dummy, (BYTE*)buf, len);
	cout<<buf<<" "<<len<<endl;

	cin.get();
}