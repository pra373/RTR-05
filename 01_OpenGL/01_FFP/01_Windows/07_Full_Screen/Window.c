//Windows header files

#include<windows.h>

// Global Function declarations

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

//global variable declarations 

HWND ghwnd = NULL;
DWORD dwStyle = 0;
WINDOWPLACEMENT wpPrev = { sizeof(WINDOWPLACEMENT) };
BOOL gbFullScreen = FALSE;


//Entry point function

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, int iCmdShow)
{
	// Local variable declaration

	WNDCLASSEX wndclass;
	HWND hwnd;
	MSG msg;
	TCHAR szAppName[] = TEXT("PLP Window");

	//code
	
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

	hwnd = CreateWindowEx(WS_EX_APPWINDOW,
		szAppName,
		TEXT("Prathamesh Laxmikant Paropkari"),
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		NULL,
		NULL,
		hInstance,
		NULL
	);

	ghwnd = hwnd;

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
	// function declaration
	void ToggleFullScreen(void);
	
	// code

	switch (iMsg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_KEYDOWN:
		switch (LOWORD(wParam))
		{
		case VK_ESCAPE:
			DestroyWindow(hwnd);
			break;

		}
		break;
	case WM_CHAR:
		switch (LOWORD(wParam))
		{
		case 'F':
		case 'f':
			if (gbFullScreen == FALSE)
			{
				ToggleFullScreen();
				gbFullScreen = TRUE;
			}
			else
			{
				ToggleFullScreen();
				gbFullScreen = FALSE;

			}

			break;

			


		}

		break;

	default:
		break;

		
		
	}

	return(DefWindowProc(hwnd, iMsg, wParam,lParam));
	

	
}

void ToggleFullScreen(void)
{
	// Local variable declarations 
	MONITORINFO mi = { sizeof(MONITORINFO) };


	//code

	if (gbFullScreen == FALSE)
	{
		dwStyle = GetWindowLong(ghwnd, GWL_STYLE);

		if (dwStyle & WS_OVERLAPPEDWINDOW)
		{
			if (GetWindowPlacement(ghwnd, &wpPrev) && GetMonitorInfo(MonitorFromWindow(ghwnd, MONITORINFOF_PRIMARY), &mi))
			{
				SetWindowLong(ghwnd, GWL_STYLE, dwStyle & ~WS_OVERLAPPEDWINDOW);
				SetWindowPos(ghwnd, HWND_TOP, mi.rcMonitor.left, mi.rcMonitor.top, mi.rcMonitor.right - mi.rcMonitor.left, mi.rcMonitor.bottom - mi.rcMonitor.top, SWP_NOZORDER | SWP_FRAMECHANGED);
			}
		}

		ShowCursor(FALSE);
	}

	else
	{
		SetWindowPlacement(ghwnd, &wpPrev);
		SetWindowLong(ghwnd, GWL_STYLE, dwStyle | WS_OVERLAPPEDWINDOW);
		SetWindowPos(ghwnd, HWND_TOP, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOOWNERZORDER | SWP_NOZORDER | SWP_FRAMECHANGED);
		ShowCursor(TRUE);
		
	}


	

	


}




