#ifndef _SYSTEMCLASS_H_
#define _SYSTEMCLASS_H_

#include <windows.h>

#include "inputclass.h"
#include "applicationclass.h"

class SystemClass
{
public:
	//�ý��� Ŭ���� ������
	SystemClass(); 
	SystemClass(const SystemClass&);
	~SystemClass();

	bool Initialize(); // �ʱ�ȭ
	void Shutdown(); // ����
	void Run(); //�ý��� ����

	LRESULT CALLBACK MessageHandler(HWND, UINT, WPARAM, LPARAM); //������ �޽��� ó�� �Լ�

private:
	bool Frame(); //������ ó�� �Լ�
	void InitializeWindows(int&, int&); //����� �Է¹޾� �����츦 �ʱ�ȭ�ϴ� �Լ�
	void ShutdownWindows(); // ������ ���� �Լ�

private:

	// ������ �ʱ�ȭ�� ���� ����
	LPCWSTR m_applicationName;
	HINSTANCE m_hinstance;
	HWND m_hwnd;

	// �ý��� Ŭ���� �Ʒ��� �Է°� ���ø����̼� ��ü �ν��Ͻ�
	InputClass* m_Input;
	ApplicationClass* m_Application;
};

// ���� ���� �ν��Ͻ��� WndProc �Լ��� ���� �޼��� ���ν��� �Լ��� ���𷺼� ����
static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

static SystemClass* ApplicationHandle = 0;


#endif