//Windows header files

#include<windows.h>

// Global Function declarations

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

//global variable declarations



//Entry point function

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, int iCmdShow)
{
	// Local variable declaration



	WNDCLASSEX wndclass;
	HWND hwnd;
	MSG msg;
	TCHAR szAppName[] = TEXT("PLP Window");
	
	int ScreenWidth;
	int ScreenHeight;
	int x_screen_center;
	int y_screen_center;
	


	//code

	ScreenWidth = GetSystemMetrics(SM_CXSCREEN);
	ScreenHeight = GetSystemMetrics(SM_CYSCREEN);

	 x_screen_center = ScreenWidth / 2;
	 y_screen_center = ScreenHeight / 2;

	
	// WNDCLASSEX Initialization

	wndclass.cbSize = sizeof(WNDCLASSEX);
	wndclass.style = CS_HREDRAW | CS_VREDRAW;
	wndclass.lpfnWndProc = WndProc;
	wndclass.cbWndExtra = 0;
	wndclass.cbClsExtra = 0;
	wndclass.hInstance = hInstance;
	wndclass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndclass.lpszClassName = szAppName;
	wndclass.lpszMenuName = NULL;
	wndclass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

	RegisterClassEx(&wndclass);

	//create window

	hwnd = CreateWindow(szAppName,
		TEXT("Prathamesh Laxmikant Paropkari"),
		WS_OVERLAPPEDWINDOW,
		(x_screen_center-400),
		(y_screen_center-300),
		800,
		600,
		NULL,
		NULL,
		hInstance,
		NULL
	);

	// Show the window

	ShowWindow(hwnd, iCmdShow);

	// Paint or Redraw the window

	UpdateWindow(hwnd);

	// message loop

	while (GetMessage(&msg, NULL, 0, 0))   // heart of application
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return((int)msg.wParam);

}

// call back function

LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	//local variable declaration
	TCHAR str[250];
	
	// code

	

	

	//wsprintf(str, TEXT("ScreenWidth = %d \t ScreenHeight = %d"), ScreenWidth, ScreenHeight);

	switch (iMsg)
	{
	
	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	default:
		break;



	}

	return(DefWindowProc(hwnd, iMsg, wParam, lParam));



}




