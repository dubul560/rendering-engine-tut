#include "systemclass.h"


int WINAPI main(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pScmdline, int iCmdshow)
{
	SystemClass* System;
	bool result;


	// Create the system object.
	// �ý��� ������Ʈ ����
	System = new SystemClass;

	// Initialize and run the system object.
	// �ý��� ������Ʈ�� �ʱ�ȭ �Լ� ȣ��
	result = System->Initialize();
	if (result)
	{
		System->Run();
	}

	// Shutdown and release the system object.
	// �ý��� ������Ʈ�� shutdown �Լ��� ȣ���Ͽ� �Ҵ�� �͵��� ��� �����ϰ� �ý��� ������Ʈ ����
	System->Shutdown();
	delete System;
	System = 0;

	return 0;
}