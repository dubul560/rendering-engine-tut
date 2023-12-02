#ifndef _SYSTEMCLASS_H_
#define _SYSTEMCLASS_H_

#include <windows.h>

#include "inputclass.h"
#include "applicationclass.h"

class SystemClass
{
public:
	//시스템 클래스 생성자
	SystemClass(); 
	SystemClass(const SystemClass&);
	~SystemClass();

	bool Initialize(); // 초기화
	void Shutdown(); // 종료
	void Run(); //시스템 실행

	LRESULT CALLBACK MessageHandler(HWND, UINT, WPARAM, LPARAM); //윈도우 메시지 처리 함수

private:
	bool Frame(); //프레임 처리 함수
	void InitializeWindows(int&, int&); //사이즈를 입력받아 윈도우를 초기화하는 함수
	void ShutdownWindows(); // 윈도우 종료 함수

private:

	// 윈도우 초기화를 위한 변수
	LPCWSTR m_applicationName;
	HINSTANCE m_hinstance;
	HWND m_hwnd;

	// 시스템 클래스 아래의 입력과 어플리케이션 객체 인스턴스
	InputClass* m_Input;
	ApplicationClass* m_Application;
};

// 전역 변수 인스턴스와 WndProc 함수를 통해 메세지 프로시저 함수에 리디렉션 가능
static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

static SystemClass* ApplicationHandle = 0;


#endif