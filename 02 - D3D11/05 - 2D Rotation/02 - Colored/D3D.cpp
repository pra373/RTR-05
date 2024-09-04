//Windows header files

#include<windows.h>
#include<stdio.h>              // for file I/O
#include<stdlib.h>             // for exit()

#include<d3d11.h>   // direct 3d related header file
#include<d3dcompiler.h>          //shader compiler hearder file

#pragma warning(disable:4838)

#include"XNAMath_204/xnamath.h"

#include "D3D.h"            // Header file in same directory

// link libraries related to D3D

#pragma comment (lib,"d3d11.lib")


#pragma comment (lib,"d3dcompiler.lib")

//Macros

#define WIN_WIDTH 800
#define WIN_HEIGHT 600

// Global Function declarations

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

//global variable declarations

FILE *gpFile = NULL;           // (gpFile) global pointer to a file

HWND ghwnd = NULL;             // handle of window
BOOL gbActive = FALSE;
DWORD dwStyle = 0;
WINDOWPLACEMENT wpPrev = { sizeof(WINDOWPLACEMENT) }; 
BOOL gbFullScreen = FALSE;

char gszLogFileName[] = "Log.txt";

//d3d related global variables

IDXGISwapChain* gpIDXGISwapChain = NULL;
ID3D11Device* gpID3D11Device = NULL;
ID3D11DeviceContext* gpID3D11DeviceContext = NULL;
ID3D11RenderTargetView* gpID3D11RenderTargetView = NULL;

ID3D11VertexShader* gpID3D11VertexShader = NULL;

ID3D11PixelShader* gpID3D11PixelShader = NULL;

ID3D11InputLayout* gpID3D11InputLayout = NULL;

// for triangle 

ID3D11Buffer* gpID3D11Buffer_PositionBuffer_Triangle = NULL;

ID3D11Buffer* gpID3D11Buffer_ColorBuffer_Triangle = NULL;

//for Rectangle

ID3D11Buffer* gpID3D11Buffer_PositionBuffer_Rectangle = NULL;

ID3D11Buffer* gpID3D11Buffer_ColorBuffer_Rectangle = NULL;

ID3D11Buffer* gpID3D11Buffer_ConstantBuffer = NULL;

ID3D11RasterizerState* gpID3D11RasterizerState = NULL;

struct CBUFFER
{
	XMMATRIX WorldViewProjectionMatrix;  // model view projection matrix

};

XMMATRIX perspectiveProjectionMatrix;


float ClearColor[4];

// angle variable

float angle = 0.0f;


//Entry point function

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, int iCmdShow)
{

	// Local Function Declarations

	HRESULT initialize(void);
	void uninitialize(void);
	void display(void);
	void update(void);

	
	// Local variable declaration

	WNDCLASSEX wndclass;
	HWND hwnd;
	MSG msg;
	TCHAR szAppName[] = TEXT("PLP Window");
	int iResult = 0;      
	BOOL bDone = FALSE;   // To check game loop while condition

	HRESULT hr = S_OK;


	int ScreenWidth;
	int ScreenHeight;
	int x_screen_center;
	int y_screen_center;

	//code

	ScreenWidth = GetSystemMetrics(SM_CXSCREEN);
	ScreenHeight = GetSystemMetrics(SM_CYSCREEN);

	x_screen_center = ScreenWidth / 2;
	y_screen_center = ScreenHeight / 2;

	gpFile = fopen(gszLogFileName, "w");
	if (gpFile == NULL)
	{
		MessageBox(NULL, TEXT("Log file cannot be opened"), TEXT("ERROR"), MB_OK);
		exit(0);
	}
	else
	{
		fprintf(gpFile, "Program startrd successfully.\n \nLog file created successfully\n\n");
		fclose(gpFile);
	}
	
	
	// WNDCLASSEX Initialization

	wndclass.cbSize = sizeof(WNDCLASSEX);
	wndclass.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;  // use CS_OWNDC to stop purging from OS
	wndclass.lpfnWndProc = WndProc;
	wndclass.cbWndExtra = 0;
	wndclass.cbClsExtra = 0;
	wndclass.hInstance = hInstance;
	wndclass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wndclass.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(MYICON));
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndclass.lpszClassName = szAppName;
	wndclass.lpszMenuName = NULL;
	wndclass.hIconSm = LoadIcon(hInstance, MAKEINTRESOURCE(MYICON));

	RegisterClassEx(&wndclass);

	//create window

	hwnd = CreateWindowEx(
		WS_EX_APPWINDOW,
		szAppName,
		TEXT("Prathamesh Laxmikant Paropkari            Direct3D   -  2D Rotation (Colored)"),
		WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN | WS_CLIPSIBLINGS | WS_VISIBLE,
		(x_screen_center - 400),
		(y_screen_center - 300),
			WIN_WIDTH,
			WIN_HEIGHT,
		NULL,
		NULL,
		hInstance,
		NULL
	);

	ghwnd = hwnd;

	//initialization

	hr = initialize();

	if (FAILED(hr))
	{

		gpFile = fopen(gszLogFileName, "a+");

		fprintf(gpFile, "initialize() failed !!\n\n");

		fclose(gpFile);

		DestroyWindow(hwnd);

		hwnd = NULL;
	}

	else
	{
		gpFile = fopen(gszLogFileName, "a+");

		fprintf(gpFile, "initialize() success !!\n\n");

		fclose(gpFile);
	}


	// Show the window

	ShowWindow(hwnd, iCmdShow);
	SetForegroundWindow(hwnd);
	SetFocus(hwnd);

	

	

	// Game loop

	while (bDone == FALSE)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.wParam == WM_QUIT)
			{
				bDone = TRUE;
			}

			else
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}

		else
		{
			if (gbActive == TRUE)
			{
				//Render

				display();

				//update

				update();

			}
		}
	}

	//Uninitialization

	uninitialize();

	

	return((int)msg.wParam);

}

// call back function

LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	//local variables

	HRESULT hr = S_OK;
	
	//Function declaration

	void ToggleFullScreen(void);
	HRESULT resize(int, int);


	// code

	switch (iMsg)
	{
	case WM_DESTROY:

		
		PostQuitMessage(0);
		break;

	case WM_SETFOCUS:
		gbActive = TRUE;
		break;

	case WM_KILLFOCUS:
		gbActive = FALSE;
		break;

	case WM_SIZE:

		if (gpID3D11DeviceContext)
		{
			hr = resize(LOWORD(lParam), HIWORD(lParam));  //lParam of WM_SIZE will give me WIDTH and HEIGHT

			if (FAILED(hr))
			{
				gpFile = fopen(gszLogFileName, "a+");

				fprintf(gpFile, "resize() failed !!\n\n");

				fclose(gpFile);

				return(hr);
			}
			else
			{
				gpFile = fopen(gszLogFileName, "a+");

				fprintf(gpFile, "resize() success !!\n\n");

				fclose(gpFile);
			}
		}
		
		  
		break;

	case WM_ERASEBKGND:
		//return(0);
		break;
		

	case WM_KEYDOWN:                 // use for non character keys
		switch (LOWORD(wParam))
		{
		case VK_ESCAPE:
			DestroyWindow(hwnd);
			break;

		}
		break;

	case WM_CHAR:                    // use for character keys
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

	case WM_CLOSE:
		DestroyWindow(hwnd);
		break;




	default:
		break;

		
		
	}

	return(DefWindowProc(hwnd, iMsg, wParam,lParam));  // Default Window procedure for all messages.
	

	
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

HRESULT initialize(void)
{
	//variable declarations
	
	HRESULT hr = S_OK;

	//function declarations

	HRESULT resize(int, int);

	// step 1 - initialize swapchain descriptor structure   

	DXGI_SWAP_CHAIN_DESC dxgiSwapChainDesc;

	//step 2 - fill above structure

	ZeroMemory((void*)&dxgiSwapChainDesc, sizeof(DXGI_SWAP_CHAIN_DESC));

	dxgiSwapChainDesc.BufferDesc.Width = WIN_WIDTH;

	dxgiSwapChainDesc.BufferDesc.Height = WIN_HEIGHT;

	dxgiSwapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

	dxgiSwapChainDesc.BufferDesc.RefreshRate.Numerator = 60;

	dxgiSwapChainDesc.BufferDesc.RefreshRate.Denominator = 1;

	dxgiSwapChainDesc.SampleDesc.Count = 1;

	dxgiSwapChainDesc.SampleDesc.Quality = 0;

	dxgiSwapChainDesc.BufferCount = 1;

	dxgiSwapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;

	dxgiSwapChainDesc.OutputWindow = ghwnd;

	dxgiSwapChainDesc.Windowed = TRUE;


	//create d3d swapchain, device, context , RTV 

	D3D_DRIVER_TYPE d3dDriverTypes[] =
	{
		D3D_DRIVER_TYPE_HARDWARE,
		D3D_DRIVER_TYPE_WARP,
		D3D_DRIVER_TYPE_SOFTWARE,
		D3D_DRIVER_TYPE_REFERENCE,
		D3D_DRIVER_TYPE_NULL,
		D3D_DRIVER_TYPE_UNKNOWN

	};

	D3D_FEATURE_LEVEL d3dFeatureLevel_Required = D3D_FEATURE_LEVEL_11_0;
	D3D_FEATURE_LEVEL d3dFeatureLevel_Acquired = D3D_FEATURE_LEVEL_10_0;

	UINT numDriverTypes;
	UINT numfeatureLevels = 1;
	D3D_DRIVER_TYPE d3dDriverType;

	numDriverTypes = sizeof(d3dDriverTypes) / sizeof(d3dDriverTypes[0]);


	for (UINT i = 0; i < numDriverTypes; i++)
	{
		d3dDriverType = d3dDriverTypes[i];

		hr = D3D11CreateDeviceAndSwapChain(NULL, d3dDriverType, NULL, 0, &d3dFeatureLevel_Required, numfeatureLevels, D3D11_SDK_VERSION, &dxgiSwapChainDesc, &gpIDXGISwapChain, &gpID3D11Device, &d3dFeatureLevel_Acquired, &gpID3D11DeviceContext);

		if (SUCCEEDED(hr))
		{
			break;
		}


	}

	if (FAILED(hr))
	{
		gpFile = fopen(gszLogFileName, "a+");

		fprintf(gpFile, "D3D11CreateDeviceAndSwapChain() failed !!");

		fclose(gpFile);

		return(hr);
	}
	else
	{
		gpFile = fopen(gszLogFileName, "a+");

		// which driver we found

		if (d3dDriverType == D3D_DRIVER_TYPE_HARDWARE)
		{
			fprintf(gpFile, " hardware driver found !!\n\n");
		}
		else if (d3dDriverType == D3D_DRIVER_TYPE_WARP)
		{
			fprintf(gpFile, " WARP driver found !!\n\n");
		}
		else if (d3dDriverType == D3D_DRIVER_TYPE_SOFTWARE)
		{
			fprintf(gpFile, " software driver found !!\n\n");
		}
		else if (d3dDriverType == D3D_DRIVER_TYPE_REFERENCE)
		{
			fprintf(gpFile, " reference driver found !!\n\n");
		}
		else if (d3dDriverType == D3D_DRIVER_TYPE_NULL)
		{
			fprintf(gpFile, " NULL driver found !!\n\n");
		}
		else if (d3dDriverType == D3D_DRIVER_TYPE_UNKNOWN)
		{
			fprintf(gpFile, " UNKNOWN driver found !!\n\n");
		}
		else
		{
			fprintf(gpFile, " Undefiend driver found !!\n\n");
		}

		// which feature level we found

		if (d3dFeatureLevel_Acquired == D3D_FEATURE_LEVEL_11_0)
		{
			fprintf(gpFile, " 11.0 feature level found !!\n\n");
		}
		else if (d3dFeatureLevel_Acquired == D3D_FEATURE_LEVEL_10_1)
		{
			fprintf(gpFile, " 10.1 feature level found !!\n\n");
		}
		else if (d3dFeatureLevel_Acquired == D3D_FEATURE_LEVEL_10_0)
		{
			fprintf(gpFile, " 10.0 feature level found !!\n\n");
		}
		else
		{
			fprintf(gpFile, " unknown feature level found !!\n\n");
		}

		fclose(gpFile);
	}

	//vertex shader

	const char* vertexShaderSourceCode =
		"cbuffer constantBuffer\n"
		"{\n"
		"float4x4 worldViewProjectionMatrix;\n"
		"}\n"
		"struct vertexOutput\n"
		"{\n"
		"float4 position:SV_POSITION;\n"
		"float4 color:COLOR;\n"
		"};\n"
		"vertexOutput main(float4 pos:POSITION, float4 col:COLOR)\n"
		"{\n"
		"vertexOutput output;\n"
		"output.position = mul(worldViewProjectionMatrix,pos);\n"
		"output.color = col;\n"
		"return(output);\n"
		"}";

	ID3DBlob* pID3DBlob_VertexShaderSourceCode = NULL;

	ID3DBlob* pID3DBlob_Error = NULL;

	//compile above shader

	hr = D3DCompile(vertexShaderSourceCode, lstrlenA(vertexShaderSourceCode) + 1, "VS", NULL, D3D_COMPILE_STANDARD_FILE_INCLUDE, "main", "vs_5_0", 0, 0, &pID3DBlob_VertexShaderSourceCode, &pID3DBlob_Error);

	if (FAILED(hr))
	{

		gpFile = fopen(gszLogFileName, "a+");

		fprintf(gpFile, "Vertex Shader Compiler Error = %s\n\n", (char*)pID3DBlob_Error->GetBufferPointer());

		fclose(gpFile);

		
	}

	else
	{
		gpFile = fopen(gszLogFileName, "a+");

		fprintf(gpFile, "Vertex Shader Compilation success !!\n\n");

		fclose(gpFile);
	}

	//create vertex shader from above code

	hr = gpID3D11Device->CreateVertexShader(pID3DBlob_VertexShaderSourceCode->GetBufferPointer(), pID3DBlob_VertexShaderSourceCode->GetBufferSize(), NULL, &gpID3D11VertexShader);

	if (FAILED(hr))
	{

		gpFile = fopen(gszLogFileName, "a+");

		fprintf(gpFile, "Vertex Shader Creation failed\n\n");

		fclose(gpFile);

		
	}

	else
	{
		gpFile = fopen(gszLogFileName, "a+");

		fprintf(gpFile, "Vertex Shader Creation success !!\n\n");

		fclose(gpFile);
	}

	//set above created vertex shader into pipeline

	gpID3D11DeviceContext->VSSetShader(gpID3D11VertexShader, NULL, 0);

	//pixel shader

	const char* pixelShaderSourceCode =
		"struct vertexOutput\n"
		"{\n"
		"float4 position:SV_POSITION;\n"
		"float4 color:COLOR;\n"
		"};\n"
		"float4 main(vertexOutput input):SV_TARGET\n"
		"{\n"
			"float4 Color = input.color;\n"
			"return(Color);\n"
		"}\n";

	ID3DBlob* pID3DBlob_PixelShaderSourceCode = NULL;

	pID3DBlob_Error = NULL;

	//compile above shader

	hr = D3DCompile(pixelShaderSourceCode, lstrlenA(pixelShaderSourceCode) + 1, "PS", NULL, D3D_COMPILE_STANDARD_FILE_INCLUDE, "main", "ps_5_0", 0, 0, &pID3DBlob_PixelShaderSourceCode, &pID3DBlob_Error);

	if (FAILED(hr))
	{

		gpFile = fopen(gszLogFileName, "a+");

		fprintf(gpFile, "Pixel Shader Compiler Error = %s\n\n", (char*)pID3DBlob_Error->GetBufferPointer());

		fclose(gpFile);

		
	}

	else
	{
		gpFile = fopen(gszLogFileName, "a+");

		fprintf(gpFile, "Pixel Shader Compilation success !!\n\n");

		fclose(gpFile);
	}

	//create pixel shader from above code

	hr = gpID3D11Device->CreatePixelShader(pID3DBlob_PixelShaderSourceCode->GetBufferPointer(), pID3DBlob_PixelShaderSourceCode->GetBufferSize(), NULL, &gpID3D11PixelShader);

	if (FAILED(hr))
	{

		gpFile = fopen(gszLogFileName, "a+");

		fprintf(gpFile, "Pixel Shader Creation failed\n\n");

		fclose(gpFile);

		
	}

	else
	{
		gpFile = fopen(gszLogFileName, "a+");

		fprintf(gpFile, "Pixel Shader Creation success !!\n\n");

		fclose(gpFile);
	}

	//set above created pixel shader into pipeline

	gpID3D11DeviceContext->PSSetShader(gpID3D11PixelShader, NULL, 0);

	//release error blob interface

	if (pID3DBlob_Error)
	{
		pID3DBlob_Error->Release();
		pID3DBlob_Error = NULL;
	}

	//release pixel shader blob here

	if (pID3DBlob_PixelShaderSourceCode)
	{
		pID3DBlob_PixelShaderSourceCode->Release();
		pID3DBlob_PixelShaderSourceCode = NULL;
	}

	//initialize input element structure

	D3D11_INPUT_ELEMENT_DESC d3dInputElementDesc[2];

	ZeroMemory((void*)d3dInputElementDesc, sizeof(D3D11_INPUT_ELEMENT_DESC)* _ARRAYSIZE(d3dInputElementDesc));

	//Position

	d3dInputElementDesc[0].SemanticName = "POSITION";
	d3dInputElementDesc[0].SemanticIndex = 0;
	d3dInputElementDesc[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	d3dInputElementDesc[0].InputSlot = 0;
	d3dInputElementDesc[0].AlignedByteOffset = 0;
	d3dInputElementDesc[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	d3dInputElementDesc[0].InstanceDataStepRate = 0;

	//Color

	d3dInputElementDesc[1].SemanticName = "COLOR";
	d3dInputElementDesc[1].SemanticIndex = 0;
	d3dInputElementDesc[1].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	d3dInputElementDesc[1].InputSlot = 1;
	d3dInputElementDesc[1].AlignedByteOffset = 0;
	d3dInputElementDesc[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	d3dInputElementDesc[1].InstanceDataStepRate = 0;

	//using above structure create input layout

	hr = gpID3D11Device->CreateInputLayout(d3dInputElementDesc, _ARRAYSIZE(d3dInputElementDesc), pID3DBlob_VertexShaderSourceCode->GetBufferPointer(), pID3DBlob_VertexShaderSourceCode->GetBufferSize(), &gpID3D11InputLayout);

	if (FAILED(hr))
	{

		gpFile = fopen(gszLogFileName, "a+");

		fprintf(gpFile, "CreateInputLayout() failed\n\n");

		fclose(gpFile);

	}

	else
	{
		gpFile = fopen(gszLogFileName, "a+");

		fprintf(gpFile, "CreateInputLayout() success !!\n\n");

		fclose(gpFile);
	}

	//set above input layout into pipeline

	gpID3D11DeviceContext->IASetInputLayout(gpID3D11InputLayout);


	//now we can release the vertex shader source code blob

	if (pID3DBlob_VertexShaderSourceCode)
	{
		pID3DBlob_VertexShaderSourceCode->Release();
		pID3DBlob_VertexShaderSourceCode = NULL;

	}

	//declare triangle grometry

	const float trianglePositions[] =       // follow clock wise vertex order in Direct3D unlike OpenGL
	{ 
		0.0f, 1.0f, 0.0f,
		1.0f,-1.0f,0.0f,
		-1.0f,-1.0f,0.0f
		
	};

	const float triangleColors[] =       // follow clock wise vertex order in Direct3D unlike OpenGL
	{
		1.0f, 0.0f, 0.0f,
		0.0f,0.0f,1.0f,
		0.0f,1.0f,0.0f

	};

	// declare Rectangle Geometry



	const float rectanglePosition[] =       // follow clock wise vertex order in Direct3D unlike OpenGL
	{
		-1.0f,1.0f,0.0f,
		1.0f,1.0f,0.0f,
		-1.0f,-1.0f,0.0f,

		-1.0f,-1.0f,0.0f,
		1.0f,1.0f,0.0f,
		1.0f,-1.0f,0.0f

	};

	const float rectangleColor[] =       // follow clock wise vertex order in Direct3D unlike OpenGL
	{
		1.0f,1.0f,0.0f,
		1.0f,1.0f,0.0f,
		1.0f,1.0f,0.0f,

		1.0f,1.0f,0.0f,
		1.0f,1.0f,0.0f,
		1.0f,1.0f,0.0f

	};


	//create vertex buffer for position

	//position

	D3D11_BUFFER_DESC d3dBufferDesc;

	ZeroMemory((void*)&d3dBufferDesc, sizeof(D3D11_BUFFER_DESC));

	d3dBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	d3dBufferDesc.ByteWidth = sizeof(trianglePositions);
	d3dBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	d3dBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	
	//create vertex buffer using above structure

	hr = gpID3D11Device->CreateBuffer(&d3dBufferDesc, NULL, &gpID3D11Buffer_PositionBuffer_Triangle);

	if (FAILED(hr))
	{

		gpFile = fopen(gszLogFileName, "a+");

		fprintf(gpFile, "gpID3D11Device -> CreateBuffer() for position buffer failed\n\n");

		fclose(gpFile);

		
	}

	else
	{
		gpFile = fopen(gszLogFileName, "a+");

		fprintf(gpFile, "gpID3D11Device -> CreateBuffer() for position buffer success !!\n\n");

		fclose(gpFile);
	}

	//we will set above buffer into pipeline not here but in display because our buffer is dynamic

	//copy manualy data from CPU buffer to GPU Buffer

	D3D11_MAPPED_SUBRESOURCE d3dMappedSubResource;

	ZeroMemory((void*)&d3dMappedSubResource, sizeof(D3D11_MAPPED_SUBRESOURCE));

	// now map our position buffer with above resource

	gpID3D11DeviceContext->Map(gpID3D11Buffer_PositionBuffer_Triangle, 0, D3D11_MAP_WRITE_DISCARD, 0, &d3dMappedSubResource);

	//now copy actual data

	memcpy(d3dMappedSubResource.pData, trianglePositions, sizeof(trianglePositions));

	gpID3D11DeviceContext->Unmap(gpID3D11Buffer_PositionBuffer_Triangle, 0);

	//Color

	ZeroMemory((void*)&d3dBufferDesc, sizeof(D3D11_BUFFER_DESC));

	d3dBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	d3dBufferDesc.ByteWidth = sizeof(triangleColors);
	d3dBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	d3dBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	//create vertex buffer using above structure

	hr = gpID3D11Device->CreateBuffer(&d3dBufferDesc, NULL, &gpID3D11Buffer_ColorBuffer_Triangle);

	if (FAILED(hr))
	{

		gpFile = fopen(gszLogFileName, "a+");

		fprintf(gpFile, "gpID3D11Device -> CreateBuffer() for Color buffer failed\n\n");

		fclose(gpFile);


	}

	else
	{
		gpFile = fopen(gszLogFileName, "a+");

		fprintf(gpFile, "gpID3D11Device -> CreateBuffer() for Color buffer success !!\n\n");

		fclose(gpFile);
	}

	//we will set above buffer into pipeline not here but in display because our buffer is dynamic

	//copy manualy data from CPU buffer to GPU Buffer

	ZeroMemory((void*)&d3dMappedSubResource, sizeof(D3D11_MAPPED_SUBRESOURCE));

	// now map our position buffer with above resource

	gpID3D11DeviceContext->Map(gpID3D11Buffer_ColorBuffer_Triangle, 0, D3D11_MAP_WRITE_DISCARD, 0, &d3dMappedSubResource);

	//now copy actual data

	memcpy(d3dMappedSubResource.pData, triangleColors, sizeof(triangleColors));

	gpID3D11DeviceContext->Unmap(gpID3D11Buffer_ColorBuffer_Triangle, 0);

	// **********************************************  position buffer for rectangle  **********************************************

	ZeroMemory((void*)&d3dBufferDesc, sizeof(D3D11_BUFFER_DESC));

	d3dBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	d3dBufferDesc.ByteWidth = sizeof(rectanglePosition);
	d3dBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	d3dBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	//create vertex buffer using above structure

	hr = gpID3D11Device->CreateBuffer(&d3dBufferDesc, NULL, &gpID3D11Buffer_PositionBuffer_Rectangle);

	if (FAILED(hr))
	{

		gpFile = fopen(gszLogFileName, "a+");

		fprintf(gpFile, "gpID3D11Device -> CreateBuffer() for position buffer Rectangle failed\n\n");

		fclose(gpFile);


	}

	else
	{
		gpFile = fopen(gszLogFileName, "a+");

		fprintf(gpFile, "gpID3D11Device -> CreateBuffer() for position buffer Rectangle success !!\n\n");

		fclose(gpFile);
	}

	//we will set above buffer into pipeline not here but in display because our buffer is dynamic

	//copy manualy data from CPU buffer to GPU Buffer


	ZeroMemory((void*)&d3dMappedSubResource, sizeof(D3D11_MAPPED_SUBRESOURCE));

	// now map our position buffer with above resource

	gpID3D11DeviceContext->Map(gpID3D11Buffer_PositionBuffer_Rectangle, 0, D3D11_MAP_WRITE_DISCARD, 0, &d3dMappedSubResource);

	//now copy actual data

	memcpy(d3dMappedSubResource.pData, rectanglePosition, sizeof(rectanglePosition));

	gpID3D11DeviceContext->Unmap(gpID3D11Buffer_PositionBuffer_Rectangle, 0);

	// ********************************** Color Buffer for Rectangle **************************************************************

	ZeroMemory((void*)&d3dBufferDesc, sizeof(D3D11_BUFFER_DESC));

	d3dBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	d3dBufferDesc.ByteWidth = sizeof(rectangleColor);
	d3dBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	d3dBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	//create vertex buffer using above structure

	hr = gpID3D11Device->CreateBuffer(&d3dBufferDesc, NULL, &gpID3D11Buffer_ColorBuffer_Rectangle);

	if (FAILED(hr))
	{

		gpFile = fopen(gszLogFileName, "a+");

		fprintf(gpFile, "gpID3D11Device -> CreateBuffer() for Color buffer Rectangle failed\n\n");

		fclose(gpFile);


	}

	else
	{
		gpFile = fopen(gszLogFileName, "a+");

		fprintf(gpFile, "gpID3D11Device -> CreateBuffer() for Color buffer Rectangle success !!\n\n");

		fclose(gpFile);
	}

	//we will set above buffer into pipeline not here but in display because our buffer is dynamic

	//copy manualy data from CPU buffer to GPU Buffer

	ZeroMemory((void*)&d3dMappedSubResource, sizeof(D3D11_MAPPED_SUBRESOURCE));

	// now map our position buffer with above resource

	gpID3D11DeviceContext->Map(gpID3D11Buffer_ColorBuffer_Rectangle, 0, D3D11_MAP_WRITE_DISCARD, 0, &d3dMappedSubResource);

	//now copy actual data

	memcpy(d3dMappedSubResource.pData, rectangleColor, sizeof(rectangleColor));

	gpID3D11DeviceContext->Unmap(gpID3D11Buffer_ColorBuffer_Rectangle, 0);





	//create constant buffer to send transformation like unifrom data

	ZeroMemory((void*)&d3dBufferDesc, sizeof(D3D11_BUFFER_DESC));

	d3dBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	d3dBufferDesc.ByteWidth = sizeof(CBUFFER);
	d3dBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	

	//create vertex buffer using above structure

	hr = gpID3D11Device->CreateBuffer(&d3dBufferDesc, NULL, &gpID3D11Buffer_ConstantBuffer);

	if (FAILED(hr))
	{

		gpFile = fopen(gszLogFileName, "a+");

		fprintf(gpFile, "gpID3D11Device -> CreateBuffer() for constant buffer failed\n\n");

		fclose(gpFile);

		
	}

	else
	{
		gpFile = fopen(gszLogFileName, "a+");

		fprintf(gpFile, "gpID3D11Device -> CreateBuffer() for constant buffer success !!\n\n");

		fclose(gpFile);
	}

	//set above buffer into pipline

	gpID3D11DeviceContext->VSSetConstantBuffers(0, 1, &gpID3D11Buffer_ConstantBuffer);

	//create and set rasteriser state to off the back face culling

	D3D11_RASTERIZER_DESC d3dRasterizerDesc;

	ZeroMemory((void*)&d3dRasterizerDesc, sizeof(D3D11_RASTERIZER_DESC));

	d3dRasterizerDesc.CullMode = D3D11_CULL_NONE;
	d3dRasterizerDesc.FillMode = D3D11_FILL_SOLID;
	d3dRasterizerDesc.MultisampleEnable = FALSE;
	d3dRasterizerDesc.DepthBias = 0.0f;
	d3dRasterizerDesc.DepthBiasClamp = 0.0f;
	d3dRasterizerDesc.SlopeScaledDepthBias = 0.0f;
	d3dRasterizerDesc.DepthClipEnable = TRUE;
	d3dRasterizerDesc.AntialiasedLineEnable = FALSE;
	d3dRasterizerDesc.FrontCounterClockwise = FALSE;
	d3dRasterizerDesc.ScissorEnable = FALSE;

	// create state 

	hr = gpID3D11Device->CreateRasterizerState(&d3dRasterizerDesc, &gpID3D11RasterizerState);

	if (FAILED(hr))
	{

		gpFile = fopen(gszLogFileName, "a+");

		fprintf(gpFile, "gpID3D11Device -> CreateRasterizerState() for Rasterizer State failed\n\n");

		fclose(gpFile);


	}

	else
	{
		gpFile = fopen(gszLogFileName, "a+");

		fprintf(gpFile, "gpID3D11Device -> CreateRasterizerState() for Rasterizer State success !!\n\n");

		fclose(gpFile);
	}

	gpID3D11DeviceContext->RSSetState(gpID3D11RasterizerState);



	// define clear color

	ClearColor[0] = 0.0f;
	ClearColor[1] = 0.0f;
	ClearColor[2] = 0.0f;
	ClearColor[3] = 1.0f;

	//set orthographics projection matrix to identity

	perspectiveProjectionMatrix = XMMatrixIdentity();


	// call warmup resize

	hr = resize(WIN_WIDTH, WIN_HEIGHT);  

	if (FAILED(hr))
	{
		gpFile = fopen(gszLogFileName, "a+");

		fprintf(gpFile, "resize() failed at the end of initialize()!!\n\n");

		fclose(gpFile);

		return(hr);
	}
	else
	{
		gpFile = fopen(gszLogFileName, "a+");

		fprintf(gpFile, "resize() success at the end of initialize()!!\n\n");

		fclose(gpFile);
	}


	return(S_OK);
	
}

HRESULT resize(int width, int height)
{
	//variable declarations

	HRESULT hr = S_OK;

	//release RTV if it is already created

	if (gpID3D11RenderTargetView)
	{
		gpID3D11RenderTargetView->Release();

		gpID3D11RenderTargetView = NULL;
	}

	// recreate swapchain buffer according to the changed size

	gpIDXGISwapChain->ResizeBuffers(1, width, height, DXGI_FORMAT_R8G8B8A8_UNORM, 0);

	// create new render target view

	//A - the buffer from rtv from swapchain into the texture(just like FBO)

	ID3D11Texture2D *pID3D11Texture2D = NULL;

	hr = gpIDXGISwapChain -> GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&pID3D11Texture2D);

	if (FAILED(hr))
	{
		gpFile = fopen(gszLogFileName, "a+");

		fprintf(gpFile, "GetBuffer() failed !!\n\n");

		fclose(gpFile);

		return(hr);
	}
	else
	{
		gpFile = fopen(gszLogFileName, "a+");

		fprintf(gpFile, "GetBuffer() success !!\n\n");

		fclose(gpFile);
	}

	// B - create RTV using above buffer

	hr = gpID3D11Device->CreateRenderTargetView(pID3D11Texture2D, NULL, &gpID3D11RenderTargetView);

	if (FAILED(hr))
	{
		gpFile = fopen(gszLogFileName, "a+");

		fprintf(gpFile, "CreateRenderTargetView() failed !!\n\n");

		fclose(gpFile);

		return(hr);
	}
	else
	{
		gpFile = fopen(gszLogFileName, "a+");

		fprintf(gpFile, "CreateRenderTargetView() success !!\n\n");

		fclose(gpFile);
	}

	pID3D11Texture2D->Release();

	pID3D11Texture2D = NULL;

	//c - set this new render target view into the pipeline

	gpID3D11DeviceContext->OMSetRenderTargets(1, &gpID3D11RenderTargetView, NULL);

	//set the view port

	D3D11_VIEWPORT d3dViewPort;

	ZeroMemory((void*)&d3dViewPort, sizeof(D3D11_VIEWPORT));

	d3dViewPort.TopLeftX = 0.0f;

	d3dViewPort.TopLeftY = 0.0f;

	d3dViewPort.Width = (float)width;

	d3dViewPort.Height = (float)height;

	d3dViewPort.MinDepth = 0.0f;

	d3dViewPort.MaxDepth = 1.0f;

	// set above details into pipeline

	gpID3D11DeviceContext->RSSetViewports(1, &d3dViewPort);

	//initialize perspective projection matrix

	perspectiveProjectionMatrix = XMMatrixPerspectiveFovLH(XMConvertToRadians(45.0f), (float)width / (float)height, 0.1f, 100.0f);

	

	

	return(hr);
}

void display(void)
{
	//code

	gpID3D11DeviceContext->ClearRenderTargetView(gpID3D11RenderTargetView, ClearColor);

	
	//transformations for Triangle

	XMMATRIX worldMatrix = XMMatrixIdentity();

	XMMATRIX viewMatrix = XMMatrixIdentity();

	XMMATRIX wvpMatrix = XMMatrixIdentity();

	XMMATRIX rotationMatrix = XMMatrixIdentity();

	XMMATRIX translationMatrix = XMMatrixIdentity();

	translationMatrix = XMMatrixTranslation(-1.5f, 0.0f, 5.0f);

	rotationMatrix = XMMatrixRotationY(-(XMConvertToRadians(angle)));

	worldMatrix = rotationMatrix * translationMatrix;

	wvpMatrix = worldMatrix * viewMatrix * perspectiveProjectionMatrix;

	CBUFFER constantBuffer;

	ZeroMemory((void*)&constantBuffer, sizeof(CBUFFER));

	constantBuffer.WorldViewProjectionMatrix = wvpMatrix;

	gpID3D11DeviceContext->UpdateSubresource(gpID3D11Buffer_ConstantBuffer, 0, NULL, &constantBuffer, 0, 0);                                    //glUniformmatrix3fv in OpenGL


	//set position buffer into the pipeline here 

	UINT stride = (sizeof(float) * 3);

	UINT offset = 0;

	gpID3D11DeviceContext->IASetVertexBuffers(0, 1, &gpID3D11Buffer_PositionBuffer_Triangle, &stride, &offset);

	//set Color buffer into the pipeline here 

	stride = (sizeof(float) * 3);

	offset = 0;

	gpID3D11DeviceContext->IASetVertexBuffers(1, 1, &gpID3D11Buffer_ColorBuffer_Triangle, &stride, &offset);

	//set primitive geometry

	gpID3D11DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	
	//draw the geometry

	gpID3D11DeviceContext->Draw(3, 0);

	// Transformation for Rectangle

	worldMatrix = XMMatrixIdentity();

	viewMatrix = XMMatrixIdentity();

	wvpMatrix = XMMatrixIdentity();

	rotationMatrix = XMMatrixIdentity();

	translationMatrix = XMMatrixIdentity();

	translationMatrix = XMMatrixTranslation(1.5f, 0.0f, 5.0f);

	rotationMatrix = XMMatrixRotationX(-(XMConvertToRadians(angle)));

	worldMatrix = rotationMatrix * translationMatrix;

	wvpMatrix = worldMatrix * viewMatrix * perspectiveProjectionMatrix;

	ZeroMemory((void*)&constantBuffer, sizeof(CBUFFER));

	constantBuffer.WorldViewProjectionMatrix = wvpMatrix;

	gpID3D11DeviceContext->UpdateSubresource(gpID3D11Buffer_ConstantBuffer, 0, NULL, &constantBuffer, 0, 0);                                    //glUniformmatrix3fv in OpenGL


	//set position buffer into the pipeline here 

	 stride = (sizeof(float) * 3);

	 offset = 0;

	gpID3D11DeviceContext->IASetVertexBuffers(0, 1, &gpID3D11Buffer_PositionBuffer_Rectangle, &stride, &offset);

	//set Color buffer into the pipeline here 

	stride = (sizeof(float) * 3);

	offset = 0;

	gpID3D11DeviceContext->IASetVertexBuffers(1, 1, &gpID3D11Buffer_ColorBuffer_Rectangle, &stride, &offset);

	//set primitive geometry

	gpID3D11DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);


	//draw the geometry

	gpID3D11DeviceContext->Draw(3, 0);

	gpID3D11DeviceContext->Draw(3, 3);



	// do double buffering by presenting the swapchain

	gpIDXGISwapChain->Present(0, 0);

}

void update(void)
{
	//code

	if (angle < 360)
	{
		angle += 0.1f;
	}
	else
	{
		angle = 0.0f;
	}

}

void uninitialize(void)
{
	//Function declarations

	void ToggleFullScreen(void);

	//code

	if (gpID3D11Buffer_ConstantBuffer)
	{
		gpID3D11Buffer_ConstantBuffer->Release();
		gpID3D11Buffer_ConstantBuffer = NULL;
	}

	if (gpID3D11Buffer_ColorBuffer_Rectangle)
	{
		gpID3D11Buffer_ColorBuffer_Rectangle->Release();
		gpID3D11Buffer_ColorBuffer_Rectangle = NULL;
	}

	if (gpID3D11Buffer_PositionBuffer_Rectangle)
	{
		gpID3D11Buffer_PositionBuffer_Rectangle->Release();
		gpID3D11Buffer_PositionBuffer_Rectangle = NULL;
	}

	if (gpID3D11Buffer_ColorBuffer_Triangle)
	{
		gpID3D11Buffer_ColorBuffer_Triangle->Release();
		gpID3D11Buffer_ColorBuffer_Triangle = NULL;
	}

	if (gpID3D11Buffer_PositionBuffer_Triangle)
	{
		gpID3D11Buffer_PositionBuffer_Triangle->Release();
		gpID3D11Buffer_PositionBuffer_Triangle = NULL;
	}

	if (gpID3D11InputLayout)
	{
		gpID3D11InputLayout->Release();
		gpID3D11InputLayout = NULL;
	}

	if (gpID3D11PixelShader)
	{
		gpID3D11PixelShader->Release();
		gpID3D11PixelShader = NULL;
	}

	if (gpID3D11VertexShader)
	{
		gpID3D11VertexShader->Release();
		gpID3D11VertexShader = NULL;
	}


	if (gpID3D11RenderTargetView)
	{
		gpID3D11RenderTargetView->Release();

		gpID3D11RenderTargetView = NULL;
	}

	if (gpID3D11DeviceContext)
	{
		gpID3D11DeviceContext->Release();

		gpID3D11DeviceContext = NULL;
	}

	if (gpIDXGISwapChain)
	{
		gpIDXGISwapChain->Release();

		gpIDXGISwapChain = NULL;
	}

	if (gpID3D11Device)
	{
		gpID3D11Device->Release();

		gpID3D11Device = NULL;
	}

	

	
	// if application is exiting in full screen

	if (gbFullScreen == TRUE)
	{
		ToggleFullScreen();
		gbFullScreen = FALSE;

	}

	// Distroy Window

	if (ghwnd)
	{
		DestroyWindow(ghwnd);
		ghwnd = NULL;

	}

	//close the log file

	gpFile = fopen(gszLogFileName, "a+");

	fprintf(gpFile, "Log File is Successfully Closed !!\n\n");

	fclose(gpFile);

	


}







