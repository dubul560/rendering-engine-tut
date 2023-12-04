#include "systemclass.h"


SystemClass::SystemClass()
{
	// 시스템 클래스 생성자
	// 인풋 인스턴스와 어플리케이션 인스턴스 초기화

	m_Input = 0;
	m_Application = 0;
}

SystemClass::SystemClass(const SystemClass& other)
{
}


SystemClass::~SystemClass()
{
	// 버전에 따라 소멸자 호출을 안 할수도 있기 때문에 생성된 객체들은
	// shutdown 함수를 통해 해제 예정 
}

bool SystemClass::Initialize()
{
	int screenWidth, screenHeight;
	bool result;


	// Initialize the width and height of the screen to zero before sending the variables into the function.
	// 화면의 너비와 높이 변수를 함수에 보내기 전에 0으로 초기화
	screenWidth = 0;
	screenHeight = 0;

	// Initialize the windows api.
	// 윈도우 api 호출하여 초기화
	InitializeWindows(screenWidth, screenHeight);

	// Create and initialize the input object.  This object will be used to handle reading the keyboard input from the user.
	// 키보드 입력을 활용하기 위해 인풋 객체 생성과 초기화
	m_Input = new InputClass;

	result = m_Input->Initialize(m_hinstance, m_hwnd, screenWidth, screenHeight);
	if (!result)
	{
		return false;
	}

	// Create and initialize the application class object.  This object will handle rendering all the graphics for this application.
	// 프로그램의 모든 그래픽 렌더링을 관리하기 위한 객체 생성과 초기화
	m_Application = new ApplicationClass;

	// 렌더링 화면을 생성하기 위한 정보와 객체를 어플리케이션 초기화함수에 전달
	result = m_Application->Initialize(screenWidth, screenHeight, m_hwnd);
	if (!result)
	{
		return false;
	}

	return true;
}

void SystemClass::Shutdown()
{
	// 프로그램에 관련된 모든 객체들을 해제하기 위한 함수

	// Release the application class object.
	// 어플리케이션 객체 해제
	if (m_Application)
	{
		m_Application->Shutdown();
		delete m_Application;
		m_Application = 0;
	}

	// Release the input object.
	// 입력 객체 해제
	if (m_Input)
	{
		delete m_Input;
		m_Input = 0;
	}

	// Shutdown the window.
	// 윈도우 종료 함수 호출
	ShutdownWindows();

	return;
}

void SystemClass::Run()
{
	// 프로그램을 종료하기로 결정할 때까지 계속 실행되는 함수
	// 윈도우의 시스템 메시지 확인
	// 시스템 메시지 처리
	// 어플리케이션 루프 처리
	// 프레임 처리중 종료 신호 확인


	MSG msg;
	bool done, result;


	// Initialize the message structure.
	ZeroMemory(&msg, sizeof(MSG));

	// Loop until there is a quit message from the window or the user.
	// 종료 메시지가 호출되기 전 까지 계속해서 실행
	done = false;
	while (!done)
	{
		// Handle the windows messages.
		// 윈도우 메시지 처리
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		// If windows signals to end the application then exit out.
		// 윈도우 메시지가 종료라면 종료를 위한 변수 설정
		if (msg.message == WM_QUIT)
		{
			done = true;
		}
		else
		{
			// Otherwise do the frame processing.
			result = Frame();
			if (!result)
			{
				done = true;
			}
		}

	}

	return;
}


bool SystemClass::Frame()
{
	// 입력 객체가 종료(esc) 버튼을 눌렀을 경우 프르그램 종료 
	// 그렇지 않을 경우 어플리케이션 객체의 렌더링을 위한 함수 호출

	bool result;


	// Do the input frame processing.
	result = m_Input->Frame();
	if (!result)
	{
		return false;
	}


	// Do the frame processing for the application class object.
	result = m_Application->Frame(m_Input);
	if (!result)
	{
		return false;
	}

	return true;
}

LRESULT CALLBACK SystemClass::MessageHandler(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam)
{
	return DefWindowProc(hwnd, umsg, wparam, lparam);
}

void SystemClass::InitializeWindows(int& screenWidth, int& screenHeight)
{
	// 렌더링할 윈도우를 생성하기위한 초기화를 하는 함수

	WNDCLASSEX wc;
	DEVMODE dmScreenSettings;
	int posX, posY;


	// Get an external pointer to this object.	
	// 현재 객체에 대한 포인터를 할당
	ApplicationHandle = this;

	// Get the instance of this application.
	m_hinstance = GetModuleHandle(NULL);

	// Give the application a name.
	m_applicationName = L"Engine";

	// Setup the windows class with default settings.
	// 윈도우 생성을 위한 변수 세팅
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = m_hinstance;
	wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);
	wc.hIconSm = wc.hIcon;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = m_applicationName;
	wc.cbSize = sizeof(WNDCLASSEX);

	// Register the window class.
	RegisterClassEx(&wc);

	// Determine the resolution of the clients desktop screen.
	// 현재 사용하는 모니터의 크기만큼 너비와 높이 설정
	screenWidth = GetSystemMetrics(SM_CXSCREEN);
	screenHeight = GetSystemMetrics(SM_CYSCREEN);

	// Setup the screen settings depending on whether it is running in full screen or in windowed mode.
	if (FULL_SCREEN)
	{
		// If full screen set the screen to maximum size of the users desktop and 32bit.
		memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));
		dmScreenSettings.dmSize = sizeof(dmScreenSettings);
		dmScreenSettings.dmPelsWidth = (unsigned long)screenWidth;
		dmScreenSettings.dmPelsHeight = (unsigned long)screenHeight;
		dmScreenSettings.dmBitsPerPel = 32;
		dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

		// Change the display settings to full screen.
		ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN);

		// Set the position of the window to the top left corner.
		posX = posY = 0;
	}
	else
	{
		// If windowed then set it to 800x600 resolution.
		screenWidth = 800;
		screenHeight = 600;

		// Place the window in the middle of the screen.
		// 윈도우의 위치를 화면 중앙에 설정
		posX = (GetSystemMetrics(SM_CXSCREEN) - screenWidth) / 2;
		posY = (GetSystemMetrics(SM_CYSCREEN) - screenHeight) / 2;
	}

	// Create the window with the screen settings and get the handle to it.
	// 윈도우 생성함수를 생성하고 관리를 위해 반환하여 저장
	m_hwnd = CreateWindowEx(WS_EX_APPWINDOW, m_applicationName, m_applicationName,
		WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_POPUP,
		posX, posY, screenWidth, screenHeight, NULL, NULL, m_hinstance, NULL);

	// Bring the window up on the screen and set it as main focus.
	ShowWindow(m_hwnd, SW_SHOW);
	SetForegroundWindow(m_hwnd);
	SetFocus(m_hwnd);

	// Hide the mouse cursor.
	ShowCursor(false);

	return;
}

void SystemClass::ShutdownWindows()
{
	// 윈도우와 관련된 모든 객체를 해제하기 위한 함수
	
	// Show the mouse cursor.
	ShowCursor(true);

	// Fix the display settings if leaving full screen mode.
	if (FULL_SCREEN)
	{
		ChangeDisplaySettings(NULL, 0);
	}

	// Remove the window.
	DestroyWindow(m_hwnd);
	m_hwnd = NULL;

	// Remove the application instance.
	UnregisterClass(m_applicationName, m_hinstance);
	m_hinstance = NULL;

	// Release the pointer to this class.
	// 현재 객체의 포인터 해제
	ApplicationHandle = NULL;

	return;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam)
{
	// 윈도우 프로시저의 메시지 중 종료와 관련된 메시지를 제외한 메시지 핸들러 함수로 보냄

	switch (umessage)
	{
		// Check if the window is being destroyed.
	case WM_DESTROY:
	{
		PostQuitMessage(0);
		return 0;
	}

	// Check if the window is being closed.
	case WM_CLOSE:
	{
		PostQuitMessage(0);
		return 0;
	}

	// All other messages pass to the message handler in the system class.
	default:
	{
		return ApplicationHandle->MessageHandler(hwnd, umessage, wparam, lparam);
	}
	}
}