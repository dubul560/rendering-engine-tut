#include "systemclass.h"


int WINAPI main(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pScmdline, int iCmdshow)
{
	SystemClass* System;
	bool result;


	// Create the system object.
	// 시스템 오브젝트 생성
	System = new SystemClass;

	// Initialize and run the system object.
	// 시스템 오브젝트의 초기화 함수 호출
	result = System->Initialize();
	if (result)
	{
		System->Run();
	}

	// Shutdown and release the system object.
	// 시스템 오브젝트의 shutdown 함수를 호출하여 할당된 것들을 모두 해제하고 시스템 오브젝트 삭제
	System->Shutdown();
	delete System;
	System = 0;

	return 0;
}