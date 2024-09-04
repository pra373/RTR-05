//Windows header files

#include<windows.h>
#include<stdio.h>              // for file I/O
#include<stdlib.h>             // for exit()

#include<d3d11.h>   // direct 3d related header file
#include<d3dcompiler.h>          //shader compiler hearder file

#pragma warning(disable:4838)


#include"WICTextureLoader.h"

using namespace DirectX;

#include"XNAMath_204/xnamath.h"

#include "D3D.h"            // Header file in same directory



// link libraries related to D3D

#pragma comment (lib,"d3d11.lib")


#pragma comment (lib,"d3dcompiler.lib")
#pragma comment (lib, "DirectXTK.lib")

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

// Pyramid

ID3D11Buffer* gpID3D11Buffer_PositionBuffer_Pyramid = NULL;

ID3D11Buffer* gpID3D11Buffer_TexCoordBuffer_Pyramid = NULL;

// buffers for cube

ID3D11Buffer* gpID3D11Buffer_PositionBuffer_Cube = NULL;

ID3D11Buffer* gpID3D11Buffer_TexCoordBuffer_Cube = NULL;



ID3D11Buffer* gpID3D11Buffer_ConstantBuffer = NULL;

ID3D11RasterizerState* gpID3D11RasterizerState = NULL;

ID3D11DepthStencilView* gpID3D11DepthStencilView = NULL;

// Interfaces for texture

ID3D11ShaderResourceView* gpID3D11ShaderResourceView = NULL;

ID3D11ShaderResourceView* gpID3D11ShaderResourceView2 = NULL;

ID3D11SamplerState* gpID3D11SamplerState = NULL;

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
		TEXT("Prathamesh Laxmikant Paropkari            Direct3D   -  Textured Two 3D Shapes"),
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

	//case WM_CLOSE:
	//	//DestroyWindow(hwnd);
	//	PostQuitMessage()
	//	break;




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
	HRESULT LoadD3DTexture(const wchar_t*, ID3D11ShaderResourceView**);

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
		"float2 texCoord:TEXCOORD;\n"
		"};\n"
		"vertexOutput main(float4 pos:POSITION, float2 tex:TEXCOORD)\n"
		"{\n"
		"vertexOutput output;\n"
		"output.position = mul(worldViewProjectionMatrix,pos);\n"
		"output.texCoord = tex;\n"
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
		"float2 texCoord:TEXCOORD;\n"
		"};\n"
		"Texture2D myTexture2D;\n"
		"SamplerState mySamplerState;\n"
		"float4 main(vertexOutput input):SV_TARGET\n"
		"{\n"
			"float4 Color = myTexture2D.Sample(mySamplerState,input.texCoord);\n"
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

	//Texcoord

	d3dInputElementDesc[1].SemanticName = "TEXCOORD";
	d3dInputElementDesc[1].SemanticIndex = 0;
	d3dInputElementDesc[1].Format = DXGI_FORMAT_R32G32_FLOAT;
	d3dInputElementDesc[1].InputSlot = 1;
	d3dInputElementDesc[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;   // similar to glPixelStorei() unpack allignment
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

	// position
	const float pyramidPosition[] =
	{
		// front
	   +0.0f, +1.0f, +0.0f, // front-top
	   +1.0f, -1.0f, -1.0f, // front-right
	   -1.0f, -1.0f, -1.0f, // front-left

	   // right
	   +0.0f, +1.0f, +0.0f, // right-top
	   +1.0f, -1.0f, +1.0f, // right-right
	   +1.0f, -1.0f, -1.0f, // right-left

	   // back
	   +0.0f, +1.0f, +0.0f, // back-top
	   -1.0f, -1.0f, +1.0f, // back-right
	   +1.0f, -1.0f, +1.0f, // back-left

	   // left
	   +0.0f, +1.0f, +0.0f, // left-top
	   -1.0f, -1.0f, -1.0f, // left-right
	   -1.0f, -1.0f, +1.0f, // left-left
	};

	// texcoords
	const float pyramidTexcoords[] =
	{
		// front
		0.5, 1.0, // front-top
		1.0, 0.0, // front-right
		0.0, 0.0, // front-left

		// right
		0.5, 1.0, // right-top
		0.0, 0.0, // right-right
		1.0, 0.0, // right-left

		// back
		0.5, 1.0, // back-top
		1.0, 0.0, // back-right
		0.0, 0.0, // back-left

		// left
		0.5, 1.0, // left-top
		0.0, 0.0, // left-right
		1.0, 0.0, // left-left
	};

	// position
	const float cubePositions[] =
	{
		// front
		-1.0f, +1.0f, -1.0f, // top-left of front
		+1.0f, +1.0f, -1.0f, // top-right of front
		-1.0f, -1.0f, -1.0f, // bottom-left of front

		-1.0f, -1.0f, -1.0f, // bottom-left of front
		+1.0f, +1.0f, -1.0f, // top-right of front
		+1.0f, -1.0f, -1.0f, // bottom-right of front

		// right
		+1.0f, +1.0f, -1.0f, // top-left of right
		+1.0f, +1.0f, +1.0f, // top-right of right
		+1.0f, -1.0f, -1.0f, // bottom-left of right

		+1.0f, -1.0f, -1.0f, // bottom-left of right
		+1.0f, +1.0f, +1.0f, // top-right of right
		+1.0f, -1.0f, +1.0f, // bottom-right of right

		// back
		+1.0f, +1.0f, +1.0f, // top-left of back
		-1.0f, +1.0f, +1.0f, // top-right of back
		+1.0f, -1.0f, +1.0f, // bottom-left of back

		+1.0f, -1.0f, +1.0f, // bottom-left of back
		-1.0f, +1.0f, +1.0f, // top-right of back
		-1.0f, -1.0f, +1.0f, // bottom-right of back

		// left
		-1.0f, +1.0f, +1.0f, // top-left of left
		-1.0f, +1.0f, -1.0f, // top-right of left
		-1.0f, -1.0f, +1.0f, // bottom-left of left

		-1.0f, -1.0f, +1.0f, // bottom-left of left
		-1.0f, +1.0f, -1.0f, // top-right of left
		-1.0f, -1.0f, -1.0f, // bottom-right of left

		// top
		-1.0f, +1.0f, +1.0f, // top-left of top
		+1.0f, +1.0f, +1.0f, // top-right of top
		-1.0f, +1.0f, -1.0f, // bottom-left of top

		-1.0f, +1.0f, -1.0f, // bottom-left of top
		+1.0f, +1.0f, +1.0f, // top-right of top
		+1.0f, +1.0f, -1.0f, // bottom-right of top

		// bottom
		-1.0f, -1.0f, -1.0f, // top-left of bottom
		+1.0f, -1.0f, -1.0f, // top-right of bottom
		-1.0f, -1.0f, +1.0f, // bottom-left of bottom

		-1.0f, -1.0f, +1.0f, // bottom-left of bottom
		+1.0f, -1.0f, -1.0f, // top-right of bottom
		+1.0f, -1.0f, +1.0f, // bottom-right of bottom
	};

	// texcoords
	const float cubeTexCoords[] =
	{
		// front
		0.0f, 1.0f, // top-left of front
		1.0f, 1.0f, // top-right of front
		0.0f, 0.0f, // bottom-left of front

		0.0f, 0.0f, // bottom-left of front
		1.0f, 1.0f, // top-right of front
		1.0f, 0.0f, // bottom-right of front

		// right
		0.0f, 1.0f, // top-left of right
		1.0f, 1.0f, // top-right of right
		0.0f, 0.0f, // bottom-left of right

		0.0f, 0.0f, // bottom-left of right
		1.0f, 1.0f, // top-right of right
		1.0f, 0.0f, // bottom-right of right

		// back
		0.0f, 1.0f, // top-left of back
		1.0f, 1.0f, // top-right of back
		0.0f, 0.0f, // bottom-left of back

		0.0f, 0.0f, // bottom-left of back
		1.0f, 1.0f, // top-right of back
		1.0f, 0.0f, // bottom-right of back

		// left
		0.0f, 1.0f, // top-left of left
		1.0f, 1.0f, // top-right of left
		0.0f, 0.0f, // bottom-left of left

		0.0f, 0.0f, // bottom-left of left
		1.0f, 1.0f, // top-right of left
		1.0f, 0.0f, // bottom-right of left

		// top
		0.0f, 1.0f, // top-left of top
		1.0f, 1.0f, // top-right of top
		0.0f, 0.0f, // bottom-left of top

		0.0f, 0.0f, // bottom-left of top
		1.0f, 1.0f, // top-right of top
		1.0f, 0.0f, // bottom-right of top

		// bottom
		0.0f, 1.0f, // top-left of bottom
		1.0f, 1.0f, // top-right of bottom
		0.0f, 0.0f, // bottom-left of bottom

		0.0f, 0.0f, // bottom-left of bottom
		1.0f, 1.0f, // top-right of bottom
		1.0f, 0.0f, // bottom-right of bottom
	};



	//create vertex buffer for position

	//position

	D3D11_BUFFER_DESC d3dBufferDesc;

	ZeroMemory((void*)&d3dBufferDesc, sizeof(D3D11_BUFFER_DESC));

	d3dBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	d3dBufferDesc.ByteWidth = sizeof(pyramidPosition);
	d3dBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	d3dBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	
	//create vertex buffer using above structure

	hr = gpID3D11Device->CreateBuffer(&d3dBufferDesc, NULL, &gpID3D11Buffer_PositionBuffer_Pyramid);

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

	gpID3D11DeviceContext->Map(gpID3D11Buffer_PositionBuffer_Pyramid, 0, D3D11_MAP_WRITE_DISCARD, 0, &d3dMappedSubResource);

	//now copy actual data

	memcpy(d3dMappedSubResource.pData, pyramidPosition, sizeof(pyramidPosition));

	gpID3D11DeviceContext->Unmap(gpID3D11Buffer_PositionBuffer_Pyramid, 0);

	//texture

	ZeroMemory((void*)&d3dBufferDesc, sizeof(D3D11_BUFFER_DESC));

	d3dBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	d3dBufferDesc.ByteWidth = sizeof(pyramidTexcoords);
	d3dBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	d3dBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	//create vertex buffer using above structure

	hr = gpID3D11Device->CreateBuffer(&d3dBufferDesc, NULL, &gpID3D11Buffer_TexCoordBuffer_Pyramid);

	if (FAILED(hr))
	{

		gpFile = fopen(gszLogFileName, "a+");

		fprintf(gpFile, "gpID3D11Device -> CreateBuffer() for Texture buffer failed\n\n");

		fclose(gpFile);


	}

	else
	{
		gpFile = fopen(gszLogFileName, "a+");

		fprintf(gpFile, "gpID3D11Device -> CreateBuffer() for Texture buffer success !!\n\n");

		fclose(gpFile);
	}

	//we will set above buffer into pipeline not here but in display because our buffer is dynamic

	//copy manualy data from CPU buffer to GPU Buffer

	ZeroMemory((void*)&d3dMappedSubResource, sizeof(D3D11_MAPPED_SUBRESOURCE));

	// now map our position buffer with above resource

	gpID3D11DeviceContext->Map(gpID3D11Buffer_TexCoordBuffer_Pyramid, 0, D3D11_MAP_WRITE_DISCARD, 0, &d3dMappedSubResource);

	//now copy actual data

	memcpy(d3dMappedSubResource.pData, pyramidTexcoords, sizeof(pyramidTexcoords));

	gpID3D11DeviceContext->Unmap(gpID3D11Buffer_TexCoordBuffer_Pyramid, 0);

	// ************************************************* Position For Cube *************************************************

	ZeroMemory((void*)&d3dBufferDesc, sizeof(D3D11_BUFFER_DESC));

	d3dBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	d3dBufferDesc.ByteWidth = sizeof(cubePositions);
	d3dBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	d3dBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	//create vertex buffer using above structure

	hr = gpID3D11Device->CreateBuffer(&d3dBufferDesc, NULL, &gpID3D11Buffer_PositionBuffer_Cube);

	if (FAILED(hr))
	{

		gpFile = fopen(gszLogFileName, "a+");

		fprintf(gpFile, "gpID3D11Device -> CreateBuffer() for position buffer cube failed\n\n");

		fclose(gpFile);


	}

	else
	{
		gpFile = fopen(gszLogFileName, "a+");

		fprintf(gpFile, "gpID3D11Device -> CreateBuffer() for position buffer cube success !!\n\n");

		fclose(gpFile);
	}

	//we will set above buffer into pipeline not here but in display because our buffer is dynamic

	//copy manualy data from CPU buffer to GPU Buffer

	ZeroMemory((void*)&d3dMappedSubResource, sizeof(D3D11_MAPPED_SUBRESOURCE));

	// now map our position buffer with above resource

	gpID3D11DeviceContext->Map(gpID3D11Buffer_PositionBuffer_Cube, 0, D3D11_MAP_WRITE_DISCARD, 0, &d3dMappedSubResource);

	//now copy actual data

	memcpy(d3dMappedSubResource.pData, cubePositions, sizeof(cubePositions));

	gpID3D11DeviceContext->Unmap(gpID3D11Buffer_PositionBuffer_Cube, 0);

	// **************************************** Texcoord buffer for Cube ******************************************

	//texture

	ZeroMemory((void*)&d3dBufferDesc, sizeof(D3D11_BUFFER_DESC));

	d3dBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	d3dBufferDesc.ByteWidth = sizeof(cubeTexCoords);
	d3dBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	d3dBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	//create vertex buffer using above structure

	hr = gpID3D11Device->CreateBuffer(&d3dBufferDesc, NULL, &gpID3D11Buffer_TexCoordBuffer_Cube);

	if (FAILED(hr))
	{

		gpFile = fopen(gszLogFileName, "a+");

		fprintf(gpFile, "gpID3D11Device -> CreateBuffer() for Texture buffer texture failed\n\n");

		fclose(gpFile);


	}

	else
	{
		gpFile = fopen(gszLogFileName, "a+");

		fprintf(gpFile, "gpID3D11Device -> CreateBuffer() for Texture buffer texture success !!\n\n");

		fclose(gpFile);
	}

	//we will set above buffer into pipeline not here but in display because our buffer is dynamic

	//copy manualy data from CPU buffer to GPU Buffer

	ZeroMemory((void*)&d3dMappedSubResource, sizeof(D3D11_MAPPED_SUBRESOURCE));

	// now map our position buffer with above resource

	gpID3D11DeviceContext->Map(gpID3D11Buffer_TexCoordBuffer_Cube, 0, D3D11_MAP_WRITE_DISCARD, 0, &d3dMappedSubResource);

	//now copy actual data

	memcpy(d3dMappedSubResource.pData, cubeTexCoords, sizeof(cubeTexCoords));

	gpID3D11DeviceContext->Unmap(gpID3D11Buffer_TexCoordBuffer_Cube, 0);


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

	//load the texture

	hr = LoadD3DTexture(L"stone.bmp", &gpID3D11ShaderResourceView);



	if (FAILED(hr))
	{

		gpFile = fopen(gszLogFileName, "a+");

		fprintf(gpFile, "gpID3D11Device -> LoadD3DTexture() for loading stone texture failed\n\n");

		fclose(gpFile);


	}

	else
	{
		gpFile = fopen(gszLogFileName, "a+");

		fprintf(gpFile, "gpID3D11Device -> LoadD3DTexture() for loading stone texture success !!\n\n");

		fclose(gpFile);
	}

	//load the texture

	hr = LoadD3DTexture(L"kundali.bmp", &gpID3D11ShaderResourceView2);

	if (FAILED(hr))
	{

		gpFile = fopen(gszLogFileName, "a+");

		fprintf(gpFile, "gpID3D11Device -> LoadD3DTexture() for loading kundali texture failed\n\n");

		fclose(gpFile);


	}

	else
	{
		gpFile = fopen(gszLogFileName, "a+");

		fprintf(gpFile, "gpID3D11Device -> LoadD3DTexture() for loading Kundali texture success !!\n\n");

		fclose(gpFile);
	}

	//create texture sampler state

	D3D11_SAMPLER_DESC d3dSamplerDesc;

	ZeroMemory((void*)&d3dSamplerDesc, sizeof(D3D11_SAMPLER_DESC));

	d3dSamplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	d3dSamplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	d3dSamplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	d3dSamplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;

	hr = gpID3D11Device->CreateSamplerState(&d3dSamplerDesc, &gpID3D11SamplerState);

	if (FAILED(hr))
	{

		gpFile = fopen(gszLogFileName, "a+");

		fprintf(gpFile, "gpID3D11Device -> CreateSamplerState() failed\n\n");

		fclose(gpFile);


	}

	else
	{
		gpFile = fopen(gszLogFileName, "a+");

		fprintf(gpFile, "gpID3D11Device -> CreateSamplerState() success !!\n\n");

		fclose(gpFile);
	}


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

HRESULT LoadD3DTexture(const wchar_t* textureFileName, ID3D11ShaderResourceView** ppID3D11ShaderResourceView)
{
	//code

	
	
	HRESULT hr = S_OK;

	hr = CreateWICTextureFromFile(gpID3D11Device, gpID3D11DeviceContext, textureFileName, NULL, ppID3D11ShaderResourceView);

	if (FAILED(hr))
	{
		gpFile = fopen(gszLogFileName, "a+");

		fprintf(gpFile, "CreateWICTextureFromFile() failed !!\n\n");

		fclose(gpFile);

		return(hr);
	}
	else
	{
		gpFile = fopen(gszLogFileName, "a+");

		fprintf(gpFile, "CreateWICTextureFromFile() success \n\n");

		fclose(gpFile);
	}

	return(hr);
}

HRESULT resize(int width, int height)
{
	//variable declarations

	HRESULT hr = S_OK;

	//release DSV if it is already created

	if (gpID3D11DepthStencilView)
	{
		gpID3D11DepthStencilView->Release();
		gpID3D11DepthStencilView = NULL;
	}

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

		pID3D11Texture2D->Release();

		pID3D11Texture2D = NULL;

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

	//create an empty texture according to the new changed size we will call it as depth texture

	D3D11_TEXTURE2D_DESC d3dTexture2DDesk;

	ZeroMemory((void*)&d3dTexture2DDesk, sizeof(D3D11_TEXTURE2D_DESC));

	d3dTexture2DDesk.Width = (UINT)width;
	d3dTexture2DDesk.Height = (UINT)height;
	d3dTexture2DDesk.MipLevels = 1;
	d3dTexture2DDesk.ArraySize = 1;
	d3dTexture2DDesk.SampleDesc.Count = 1;
	d3dTexture2DDesk.SampleDesc.Quality = 0;
	d3dTexture2DDesk.Usage = D3D11_USAGE_DEFAULT;
	d3dTexture2DDesk.Format = DXGI_FORMAT_D32_FLOAT;
	d3dTexture2DDesk.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	d3dTexture2DDesk.CPUAccessFlags = 0;
	d3dTexture2DDesk.MiscFlags = 0;

	ID3D11Texture2D* piID3D11Texture2D_depthBuffer = NULL;

	hr = gpID3D11Device->CreateTexture2D(&d3dTexture2DDesk, NULL, &piID3D11Texture2D_depthBuffer);

	if (FAILED(hr))
	{
		gpFile = fopen(gszLogFileName, "a+");

		fprintf(gpFile, "CreateTexture2D() for depth failed !!\n\n");

		fclose(gpFile);

		piID3D11Texture2D_depthBuffer->Release();

		piID3D11Texture2D_depthBuffer = NULL;

		return(hr);
	}
	else
	{
		gpFile = fopen(gszLogFileName, "a+");

		fprintf(gpFile, "CreateTexture2D() for depth success !!\n\n");

		fclose(gpFile);
	}

	

	//create dsv according to above depth buffer texture

	D3D11_DEPTH_STENCIL_VIEW_DESC d3dDepthStenclViewDesc;

	ZeroMemory((void*)&d3dDepthStenclViewDesc, sizeof(D3D11_DEPTH_STENCIL_VIEW_DESC));

	d3dDepthStenclViewDesc.Format = DXGI_FORMAT_D32_FLOAT;
	d3dDepthStenclViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DMS;

	hr = gpID3D11Device->CreateDepthStencilView(piID3D11Texture2D_depthBuffer, &d3dDepthStenclViewDesc, &gpID3D11DepthStencilView);

	if (FAILED(hr))
	{
		gpFile = fopen(gszLogFileName, "a+");

		fprintf(gpFile, "CreateDepthStencilView() for depth failed !!\n\n");

		fclose(gpFile);

		piID3D11Texture2D_depthBuffer->Release();

		piID3D11Texture2D_depthBuffer = NULL;

		return(hr);
	}
	else
	{
		gpFile = fopen(gszLogFileName, "a+");

		fprintf(gpFile, "CreateDepthStencilView() for depth success !!\n\n");

		fclose(gpFile);
	}

	piID3D11Texture2D_depthBuffer->Release();

	piID3D11Texture2D_depthBuffer = NULL;

	//c - set this new render target view into the pipeline

	gpID3D11DeviceContext->OMSetRenderTargets(1, &gpID3D11RenderTargetView, gpID3D11DepthStencilView);

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

	gpID3D11DeviceContext->ClearDepthStencilView(gpID3D11DepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);

	
	//transformations

	XMMATRIX worldMatrix = XMMatrixIdentity();

	XMMATRIX viewMatrix = XMMatrixIdentity();

	XMMATRIX wvpMatrix = XMMatrixIdentity();

	XMMATRIX translationMatrix = XMMatrixIdentity();

	XMMATRIX rotationMatrix = XMMatrixIdentity();

	translationMatrix = XMMatrixTranslation(-1.8f, 0.0f, 6.0f);

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

	gpID3D11DeviceContext->IASetVertexBuffers(0, 1, &gpID3D11Buffer_PositionBuffer_Pyramid, &stride, &offset);

	//set texcoord buffer into the pipeline here 

	stride = (sizeof(float) * 2);

	offset = 0;

	gpID3D11DeviceContext->IASetVertexBuffers(1, 1, &gpID3D11Buffer_TexCoordBuffer_Pyramid, &stride, &offset);

	//set the Texture shader resource view in pixel shader

	gpID3D11DeviceContext->PSSetShaderResources(0, 1, &gpID3D11ShaderResourceView);

	//set texture sampler state in pixel shader

	gpID3D11DeviceContext->PSSetSamplers(0, 1, &gpID3D11SamplerState);

	//set primitive geometry

	gpID3D11DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	
	//draw the geometry

	gpID3D11DeviceContext->Draw(12, 0);

	//transformations for textured cube

	worldMatrix = XMMatrixIdentity();

	viewMatrix = XMMatrixIdentity();

	wvpMatrix = XMMatrixIdentity();

	translationMatrix = XMMatrixIdentity();

	XMMATRIX finalRotationMatrix = XMMatrixIdentity();

	XMMATRIX scaleMaterix = XMMatrixIdentity();

	translationMatrix = XMMatrixTranslation(1.8f, 0.0f, 6.0f);

	scaleMaterix = XMMatrixScaling(0.73, 0.73, 0.73);

	XMMATRIX rotationMatrixY = XMMatrixIdentity();
	XMMATRIX rotationMatrixX = XMMatrixIdentity();
	XMMATRIX rotationMatrixZ = XMMatrixIdentity();


	rotationMatrixY = XMMatrixRotationY(-(XMConvertToRadians(angle)));

	rotationMatrixX = XMMatrixRotationX(-(XMConvertToRadians(angle)));

	rotationMatrixZ = XMMatrixRotationZ(-(XMConvertToRadians(angle)));

	finalRotationMatrix = rotationMatrixX * rotationMatrixY * rotationMatrixZ;

	worldMatrix = finalRotationMatrix * scaleMaterix * translationMatrix;

	wvpMatrix = worldMatrix * viewMatrix * perspectiveProjectionMatrix;

	ZeroMemory((void*)&constantBuffer, sizeof(CBUFFER));

	constantBuffer.WorldViewProjectionMatrix = wvpMatrix;

	gpID3D11DeviceContext->UpdateSubresource(gpID3D11Buffer_ConstantBuffer, 0, NULL, &constantBuffer, 0, 0);                                    //glUniformmatrix3fv in OpenGL


	//set position buffer into the pipeline here 

	stride = (sizeof(float) * 3);

	offset = 0;

	gpID3D11DeviceContext->IASetVertexBuffers(0, 1, &gpID3D11Buffer_PositionBuffer_Cube, &stride, &offset);

	//set texcoord buffer into the pipeline here 

	stride = (sizeof(float) * 2);

	offset = 0;

	gpID3D11DeviceContext->IASetVertexBuffers(1, 1, &gpID3D11Buffer_TexCoordBuffer_Cube, &stride, &offset);

	//set the Texture shader resource view in pixel shader

	gpID3D11DeviceContext->PSSetShaderResources(0, 1, &gpID3D11ShaderResourceView2);

	//set texture sampler state in pixel shader

	gpID3D11DeviceContext->PSSetSamplers(0, 1, &gpID3D11SamplerState);

	//set primitive geometry

	gpID3D11DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);


	//draw the geometry

	gpID3D11DeviceContext->Draw(36, 0);



	// do double buffering by presenting the swapchain

	gpIDXGISwapChain->Present(0, 0);

}

void update(void)
{
	//code

	if (angle < 360.0f)
	{
		angle += 0.5f;
	}
	if (angle >= 360.0f)
	{
		angle = 0.0f;
	}

}

void uninitialize(void)
{
	//Function declarations

	void ToggleFullScreen(void);

	//code



	if (gpID3D11RasterizerState)
	{
		gpID3D11RasterizerState->Release();
		gpID3D11RasterizerState = NULL;
	}

	if (gpID3D11Buffer_ConstantBuffer)
	{
		gpID3D11Buffer_ConstantBuffer->Release();
		gpID3D11Buffer_ConstantBuffer = NULL;
	}

	

	if (gpID3D11Buffer_TexCoordBuffer_Pyramid)
	{
		gpID3D11Buffer_TexCoordBuffer_Pyramid->Release();
		gpID3D11Buffer_TexCoordBuffer_Pyramid = NULL;
	}

	if (gpID3D11Buffer_PositionBuffer_Pyramid)
	{
		gpID3D11Buffer_PositionBuffer_Pyramid->Release();
		gpID3D11Buffer_PositionBuffer_Pyramid = NULL;
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


	if (gpID3D11SamplerState)
	{
		gpID3D11SamplerState->Release();
		gpID3D11SamplerState = NULL;
	}


	if (gpID3D11ShaderResourceView)
	{
		gpID3D11ShaderResourceView->Release();
		gpID3D11ShaderResourceView = NULL;
	}

	if (gpID3D11DepthStencilView)
	{
		gpID3D11DepthStencilView->Release();
		gpID3D11DepthStencilView = NULL;
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







