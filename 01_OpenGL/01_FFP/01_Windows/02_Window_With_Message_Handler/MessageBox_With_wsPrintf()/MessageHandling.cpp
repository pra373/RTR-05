//Windows header files

#include<windows.h>

// Global Function declarations

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

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

	hwnd = CreateWindow(szAppName,
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
	//local variable declarations
	int num1 = 10;
	int num2 = 20;
	int result = num1 + num2;
	TCHAR str[50];
	// code

	switch (iMsg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_CREATE:
		MessageBox(hwnd, TEXT("WM_CREATE ARRIVED"), TEXT("Message"), MB_OK);
		break;
	case WM_SIZE:
		MessageBox(hwnd, TEXT("WM_SIZE ARRIVED"), TEXT("Message"), MB_OK);
		break;
	case WM_KEYDOWN:
		switch (LOWORD(wParam))
		{
		case VK_ESCAPE:
			MessageBox(hwnd, TEXT("WM_KEYDOWN-VK_ESCAPE ARRIVED"), TEXT("Message"), MB_OK);
			break;


		}
		break;
	case WM_CHAR:
		switch (LOWORD(wParam))
		{
		case 'F':
		case 'f':
			MessageBox(hwnd, TEXT("WM_CHAR - F KEY IS PRESSED"), TEXT("Message"), MB_OK);
			break;

		case 'A':
		case 'a':
			wsprintf(str,TEXT("Addition of num1 and num2 is %d"),result);
			MessageBox(hwnd, str, TEXT("ADDITION"), MB_OKCANCEL);
			break;

		}

		break;


	default:
		break;



	}

	return(DefWindowProc(hwnd, iMsg, wParam, lParam));



}




