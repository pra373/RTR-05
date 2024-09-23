//Windows header files

#include<windows.h>
#include<stdio.h>              // for file I/O
#include<stdlib.h>             // for exit()


#include<d3d11.h>   // direct 3d related header file
#include<d3dcompiler.h>

#pragma warning(disable:4838) 

#include "XNAMath_204/xnamath.h"
#include "D3D.h"            // Header file in same directory
#include "Sphere.h"


// link libraries related to D3D

#pragma comment (lib,"d3d11.lib")

#pragma comment (lib,"d3dcompiler.lib")
#pragma comment(lib,"Sphere.lib")


//Macros

#define WIN_WIDTH 800
#define WIN_HEIGHT 600

#define MODEL_VIEW_MATRIX_STACK 32

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

// Interfaces
ID3D11VertexShader* gpID3D11VertexShader = NULL;
ID3D11PixelShader* gpID3D11PixelShader = NULL;
ID3D11InputLayout* gpID3D11InputLayout = NULL;
ID3D11Buffer* gpID3D11Buffer_PositionBuffer = NULL;
ID3D11Buffer* gpID3D11Buffer_ConstantBuffer = NULL; 

ID3D11RasterizerState* gpID3D11RasterizerState;

ID3D11DepthStencilView* gpID3D11DepthStencilView = NULL; // for depth



struct CBUFFER
{
	XMMATRIX worldViewProjectionMatrix; // OpenGL madhla modelViewProjectionMatrix word madla "model" means "world" in DirectX

	XMVECTOR sphereColor;
};

XMMATRIX perspectiveProjectionMatrix;

float ClearColor[4];

// Sphere related global variables
ID3D11Buffer* gpID3D11Buffer_IndexBuffer = NULL;

ID3D11Buffer* gpID3D11Buffer_NormalBuffer = NULL;


float sphere_vertices[1146];
float sphere_normals[1146];
float sphere_textures[764];
unsigned short sphere_elements[2280];
unsigned int gNumElements;
unsigned int gNumVertices;

// Pushpop related function declaration
void initializeMatrixStack(void);
void pushMatrix(XMMATRIX);
XMMATRIX popMatrix(void);

//PushPop Related variables

XMMATRIX matrixStack[MODEL_VIEW_MATRIX_STACK];
int matrixStackTop = -1;

//rotation related variables

int day = 0;
int year = 0;

int moon_day;
int moon_year;



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
		TEXT("Prathamesh Laxmikant Paropkari :  DirectX - PushPop : Solar System"),
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
		case'D':
			day = (day + 6) % 360;
			break;
		case'd':
			day = (day - 6) % 360;
			break;
		case'Y':
			year = (year + 3) % 360;
			break;
		case'y':
			year = (year - 3) % 360;
			break;
		case'M':
			moon_day = (moon_day + 6) % 360;
			break;

		case'm':
			moon_day = (moon_day - 6) % 360;
			break;

		case'N':
			moon_year = (moon_year + 3) % 360;
			break;

		case'n':
			moon_year = (moon_year - 3) % 360;
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


	// Vertex Shader
	const char* vertexShaderSourceCode =
		"cbuffer ConstantBuffer" \
		"{" \
		"float4x4 worldViewProjectionMatrix;" \
		"float4 sphereColor;" \
		"}" \
		"float4 main(float4 pos:POSITION):SV_POSITION" \
		"{" \
		"float4 position =mul(worldViewProjectionMatrix,pos);" \
		"return(position);" \
		"}";

	// declare local interface
	ID3DBlob* pID3DBlob_VertexShaderSoureCode = NULL;
	ID3DBlob* pID3DBlob_Error = NULL;

	// Compile above shader
	hr = D3DCompile(vertexShaderSourceCode,
		lstrlenA(vertexShaderSourceCode)+1, 
		"VS",
		NULL,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, 
		"main",
		"vs_5_0", 
		0,
		0, 
		&pID3DBlob_VertexShaderSoureCode,
		&pID3DBlob_Error);


	if (FAILED(hr))
	{
		gpFile = fopen(gszLogFileName, "a+");

		fprintf(gpFile, "Vertex Shader Compilation Error : %s\n\n",(char*)pID3DBlob_Error->GetBufferPointer());

		fclose(gpFile);

		return(hr);
	}
	else
	{
		gpFile = fopen(gszLogFileName, "a+");

		fprintf(gpFile, "Vertex Shader Compilation Successful !!\n\n");

		fclose(gpFile);
	}

	// Create the vertex shader from above code

	hr = gpID3D11Device->CreateVertexShader(pID3DBlob_VertexShaderSoureCode->GetBufferPointer(),
		pID3DBlob_VertexShaderSoureCode->GetBufferSize(), 
		NULL, 
		&gpID3D11VertexShader);


	// Error checking
	if (FAILED(hr))
	{
		gpFile = fopen(gszLogFileName, "a+");

		fprintf(gpFile, "CreateVertexShader Failed!!\n\n");

		fclose(gpFile);

		return(hr);
	}
	else
	{
		gpFile = fopen(gszLogFileName, "a+");

		fprintf(gpFile, "CreateVertexShader Succeded!!\n\n");

		fclose(gpFile);
	}

	// Set Above Created Vertex Shader Into Pipeline

	gpID3D11DeviceContext->VSSetShader(gpID3D11VertexShader, NULL, 0);


	// Pixel Shader
	const char* pixelShaderSourceCode =
		"cbuffer ConstantBuffer" \
		"{" \
		"float4x4 worldViewProjectionMatrix;" \
		"float4 sphereColor;" \
		"}" \
		"float4 main(void):SV_TARGET" \
		"{" \
		"float4 color = sphereColor;" \
		"return(color);" \
		"}";

	// declare local interface
	ID3DBlob* pID3DBlob_PixelShaderSoureCode = NULL;
	pID3DBlob_Error = NULL;

	// Compile above shader
	hr = D3DCompile(pixelShaderSourceCode,
		lstrlenA(pixelShaderSourceCode) + 1,
		"PS",
		NULL,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,
		"main",
		"ps_5_0",
		0,
		0,
		&pID3DBlob_PixelShaderSoureCode,
		&pID3DBlob_Error);


	if (FAILED(hr))
	{
		gpFile = fopen(gszLogFileName, "a+");

		fprintf(gpFile, "Pixel Shader Compilation Error : %s\n\n", (char*)pID3DBlob_Error->GetBufferPointer());

		fclose(gpFile);

		return(hr);
	}
	else
	{
		gpFile = fopen(gszLogFileName, "a+");

		fprintf(gpFile, "Pixel Shader Compilation Successful !!\n\n");

		fclose(gpFile);
	}

	// Create the pixel shader from above code

	hr = gpID3D11Device->CreatePixelShader(pID3DBlob_PixelShaderSoureCode->GetBufferPointer(),
		pID3DBlob_PixelShaderSoureCode->GetBufferSize(),
		NULL,
		&gpID3D11PixelShader);


	// Error checking
	if (FAILED(hr))
	{
		gpFile = fopen(gszLogFileName, "a+");

		fprintf(gpFile, "CreatePixelShader() Failed!!\n\n");

		fclose(gpFile);

		return(hr);
	}
	else
	{
		gpFile = fopen(gszLogFileName, "a+");

		fprintf(gpFile, "CreatePixelShader() Succeded!!\n\n");

		fclose(gpFile);
	}

	// Set Above Created Pixel Shader Into Pipeline

	gpID3D11DeviceContext->PSSetShader(gpID3D11PixelShader, NULL, 0);


	// Release Error Blob Interface

	if (pID3DBlob_Error)
	{
		pID3DBlob_Error->Release();
		pID3DBlob_Error = NULL;

	}


	// Release Pixel Shader blob here

	if (pID3DBlob_PixelShaderSoureCode)
	{
		pID3DBlob_PixelShaderSoureCode->Release();
		pID3DBlob_PixelShaderSoureCode = NULL;

	}


	// Initialize input element structure
	D3D11_INPUT_ELEMENT_DESC d3dInputElementDesc;

	ZeroMemory((void*)&d3dInputElementDesc, sizeof(D3D11_INPUT_ELEMENT_DESC));

	d3dInputElementDesc.SemanticName = "POSITION";
	
	d3dInputElementDesc.SemanticIndex = 0;

	d3dInputElementDesc.Format = DXGI_FORMAT_R32G32B32_FLOAT;

	d3dInputElementDesc.InputSlot = 0; // when color will come it will be 1, texcoord = 3 etc..

	d3dInputElementDesc.AlignedByteOffset = 0;

	d3dInputElementDesc.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;

	d3dInputElementDesc.InstanceDataStepRate = 0;

	// Using above structure create input layout

	hr = gpID3D11Device->CreateInputLayout(&d3dInputElementDesc, 1 , pID3DBlob_VertexShaderSoureCode->GetBufferPointer(),
		pID3DBlob_VertexShaderSoureCode->GetBufferSize(), &gpID3D11InputLayout);

	if (FAILED(hr))
	{
		gpFile = fopen(gszLogFileName, "a+");

		fprintf(gpFile, "CreateInputLayout() Failed!!\n\n");

		fclose(gpFile);

		return(hr);
	}
	else
	{
		gpFile = fopen(gszLogFileName, "a+");

		fprintf(gpFile, "CreateInputLayout() Succeded!!\n\n");

		fclose(gpFile);
	}
	
	// Set above input layout in pipeline

	gpID3D11DeviceContext->IASetInputLayout(gpID3D11InputLayout);

	// Now we can release the vertex shader source code blob

	if (pID3DBlob_VertexShaderSoureCode)
	{
		pID3DBlob_VertexShaderSoureCode->Release();
		pID3DBlob_VertexShaderSoureCode = NULL;
	}


	// Sphere related code
	getSphereVertexData(sphere_vertices, sphere_normals, sphere_textures, sphere_elements);
	gNumVertices = getNumberOfSphereVertices();
	gNumElements = getNumberOfSphereElements();

	gpFile = fopen(gszLogFileName, "a+");

	fprintf(gpFile, "No of vertices %d No of elements %d!!\n\n", gNumVertices, gNumElements);

	fclose(gpFile);


	// Position VB
	D3D11_BUFFER_DESC d3dBufferDesc;
	ZeroMemory(&d3dBufferDesc, sizeof(D3D11_BUFFER_DESC));
	d3dBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	d3dBufferDesc.ByteWidth = gNumVertices * 3 * sizeof(float);
	d3dBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	D3D11_SUBRESOURCE_DATA d3d11SubresourceData;
	ZeroMemory((void*)&d3d11SubresourceData, sizeof(D3D11_SUBRESOURCE_DATA));
	d3d11SubresourceData.pSysMem = sphere_vertices;
	hr = gpID3D11Device->CreateBuffer(&d3dBufferDesc, &d3d11SubresourceData, &gpID3D11Buffer_PositionBuffer);

	if (FAILED(hr))
	{
		gpFile = fopen(gszLogFileName, "a+");

		fprintf(gpFile, "CreateBuffer() failed for creating vertex buffer!!\n\n");

		fclose(gpFile);

		return(hr);
	}
	else
	{
		gpFile = fopen(gszLogFileName, "a+");

		fprintf(gpFile, "CreateBuffer() succeded for creating vertex buffer!!\n\n");

		fclose(gpFile);
	}

	// Normals VB
	// Follow Same Above Method For Normals And Textures
	ZeroMemory(&d3dBufferDesc, sizeof(D3D11_BUFFER_DESC));
	d3dBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	d3dBufferDesc.ByteWidth = gNumVertices * 3 * sizeof(float);
	d3dBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	ZeroMemory((void*)&d3d11SubresourceData, sizeof(D3D11_SUBRESOURCE_DATA));
	d3d11SubresourceData.pSysMem = sphere_normals;

	hr = gpID3D11Device->CreateBuffer(&d3dBufferDesc, &d3d11SubresourceData, &gpID3D11Buffer_NormalBuffer);
	if (FAILED(hr))
	{
		gpFile = fopen(gszLogFileName, "a+");

		fprintf(gpFile, "CreateBuffer() failed for creating Normals buffer!!\n\n");

		fclose(gpFile);

		return(hr);
	}
	else
	{
		gpFile = fopen(gszLogFileName, "a+");

		fprintf(gpFile, "CreateBuffer() succeded for creating Normals buffer!!\n\n");

		fclose(gpFile);
	}

	// Index Buffer
	ZeroMemory(&d3dBufferDesc, sizeof(D3D11_BUFFER_DESC));
	d3dBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	d3dBufferDesc.ByteWidth = gNumElements * sizeof(short);
	d3dBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;

	ZeroMemory((void*)&d3d11SubresourceData, sizeof(D3D11_SUBRESOURCE_DATA));
	d3d11SubresourceData.pSysMem = sphere_elements;

	hr = gpID3D11Device->CreateBuffer(&d3dBufferDesc, &d3d11SubresourceData, &gpID3D11Buffer_IndexBuffer);

	if (FAILED(hr))
	{
		gpFile = fopen(gszLogFileName, "a+");

		fprintf(gpFile, "CreateBuffer() failed for creating index buffer!!\n\n");

		fclose(gpFile);

		return(hr);
	}
	else
	{
		gpFile = fopen(gszLogFileName, "a+");

		fprintf(gpFile, "CreateBuffer() succeded for creating index buffer!!\n\n");

		fclose(gpFile);
	}
	

	// Create constant buffer to send transformation like uniform data

	ZeroMemory((void*)&d3dBufferDesc, sizeof(D3D11_BUFFER_DESC));

	d3dBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	d3dBufferDesc.ByteWidth = sizeof(CBUFFER);
	d3dBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;


	// Create vertex buffer using above buffer/ structure

	hr = gpID3D11Device->CreateBuffer(&d3dBufferDesc, NULL, &gpID3D11Buffer_ConstantBuffer);

	if (FAILED(hr))
	{
		gpFile = fopen(gszLogFileName, "a+");

		fprintf(gpFile, "CreateBuffer() failed for creating constant buffer!!\n\n");

		fclose(gpFile);

		return(hr);
	}
	else
	{
		gpFile = fopen(gszLogFileName, "a+");

		fprintf(gpFile, "CreateBuffer() succeded for creating constant buffer!!\n\n");

		fclose(gpFile);
	}


	// Set Above Buffer Into Pipeline
	gpID3D11DeviceContext->VSSetConstantBuffers(0, 1, &gpID3D11Buffer_ConstantBuffer);

	gpID3D11DeviceContext->PSSetConstantBuffers(0, 1, &gpID3D11Buffer_ConstantBuffer);


	// Create and Set Rasterizer State to Off the backface culling

	D3D11_RASTERIZER_DESC d3dRasterizerDesc;

	ZeroMemory((void*)&d3dRasterizerDesc, sizeof(D3D11_RASTERIZER_DESC));

	d3dRasterizerDesc.CullMode = D3D11_CULL_NONE;

	d3dRasterizerDesc.FillMode = D3D11_FILL_WIREFRAME; //D3D11_FILL_WIREFRAME

	d3dRasterizerDesc.MultisampleEnable = FALSE;

	d3dRasterizerDesc.DepthBias = 0.0f;

	d3dRasterizerDesc.DepthBiasClamp = 0.0f;

	d3dRasterizerDesc.SlopeScaledDepthBias = 0.0f;

	d3dRasterizerDesc.DepthClipEnable = TRUE;

	d3dRasterizerDesc.AntialiasedLineEnable = FALSE;

	d3dRasterizerDesc.FrontCounterClockwise = FALSE;

	d3dRasterizerDesc.ScissorEnable = FALSE;



	hr = gpID3D11Device->CreateRasterizerState(&d3dRasterizerDesc, &gpID3D11RasterizerState);

	if (FAILED(hr))
	{
		gpFile = fopen(gszLogFileName, "a+");

		fprintf(gpFile, "CreateRasterizerState() failed!!\n\n");

		fclose(gpFile);

		return(hr);
	}
	else
	{
		gpFile = fopen(gszLogFileName, "a+");

		fprintf(gpFile, "CreateRasterizerState() succeded!!\n\n");

		fclose(gpFile);
	}

	gpID3D11DeviceContext->RSSetState(gpID3D11RasterizerState);

	// define clear color

	ClearColor[0] = 0.0f;
	ClearColor[1] = 0.0f;
	ClearColor[2] = 0.0f;
	ClearColor[3] = 1.0f;


	initializeMatrixStack();

	// set perspective projection matrix
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

	// release DTV  
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

	ID3D11Texture2D* pID3D11Texture2D = NULL;

	hr = gpIDXGISwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&pID3D11Texture2D);

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


	// Create an empty texture according to the new changed size we will call it as depth buffer/texture

	D3D11_TEXTURE2D_DESC d3dTexture2DDesc;

	ZeroMemory((void*)&d3dTexture2DDesc, sizeof(D3D11_TEXTURE2D_DESC));


	d3dTexture2DDesc.Width = (UINT)width;

	d3dTexture2DDesc.Height = (UINT)height;

	d3dTexture2DDesc.MipLevels = 1;

	d3dTexture2DDesc.ArraySize = 1;

	d3dTexture2DDesc.SampleDesc.Count = 1; // highest quality 4

	d3dTexture2DDesc.SampleDesc.Quality = 0;

	d3dTexture2DDesc.Usage = D3D11_USAGE_DEFAULT;

	d3dTexture2DDesc.Format = DXGI_FORMAT_D32_FLOAT;

	d3dTexture2DDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;

	d3dTexture2DDesc.CPUAccessFlags = 0.0f;

	d3dTexture2DDesc.MiscFlags = 0;

	ID3D11Texture2D* pId3D11Texture2D_DepthBuffer = NULL;

	hr = gpID3D11Device->CreateTexture2D(&d3dTexture2DDesc, NULL, &pId3D11Texture2D_DepthBuffer);

	if (FAILED(hr))
	{
		gpFile = fopen(gszLogFileName, "a+");

		fprintf(gpFile, "CreateTexture2D() failed !!\n\n");

		fclose(gpFile);

		return(hr);
	}
	else
	{
		gpFile = fopen(gszLogFileName, "a+");

		fprintf(gpFile, "CreateTexture2D() success !!\n\n");

		fclose(gpFile);
	}

	// Create DSV according to above depth buffer texture

	D3D11_DEPTH_STENCIL_VIEW_DESC d3dDepthStencilViewDesc;

	ZeroMemory((void*)&d3dDepthStencilViewDesc, sizeof(D3D11_DEPTH_STENCIL_VIEW_DESC));

	d3dDepthStencilViewDesc.Format = DXGI_FORMAT_D32_FLOAT;

	d3dDepthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DMS; // MS = Multisampling

	hr = gpID3D11Device->CreateDepthStencilView(pId3D11Texture2D_DepthBuffer, &d3dDepthStencilViewDesc, &gpID3D11DepthStencilView);



	if (FAILED(hr))
	{
		gpFile = fopen(gszLogFileName, "a+");

		fprintf(gpFile, "CreateDepthStencilView() failed !!\n\n");

		fclose(gpFile);

		pId3D11Texture2D_DepthBuffer->Release();

		pId3D11Texture2D_DepthBuffer = NULL;

		return(hr);
	}
	else
	{
		gpFile = fopen(gszLogFileName, "a+");

		fprintf(gpFile, "CreateDepthStencilView() success !!\n\n");

		fclose(gpFile);
	}

	pId3D11Texture2D_DepthBuffer->Release();

	pId3D11Texture2D_DepthBuffer = NULL;

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

	// Set above viewport in pipeline
	gpID3D11DeviceContext->RSSetViewports(1, &d3dViewPort);

	// Initialize perspectiveProjectionMatrix 
	// set perspective projection matrix

	perspectiveProjectionMatrix = XMMatrixPerspectiveFovLH(XMConvertToRadians(45.0f), ((float)width / (float)height),
		0.1f,
		100.0f);


	return(hr);
}


void display(void)
{
	//code

	gpID3D11DeviceContext->ClearRenderTargetView(gpID3D11RenderTargetView, ClearColor);

	gpID3D11DeviceContext->ClearDepthStencilView(gpID3D11DepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);

	// Transformations
	XMMATRIX worldMatrix = XMMatrixIdentity();

	XMMATRIX translationMatrix = XMMatrixIdentity();

	translationMatrix = XMMatrixTranslation(0.0f, 0.0f, 3.3f);

	worldMatrix = translationMatrix;

	XMMATRIX viewMatrix = XMMatrixIdentity();

	pushMatrix(worldMatrix);
	{
		XMMATRIX wvpMatrix = worldMatrix * viewMatrix * perspectiveProjectionMatrix;

		CBUFFER constantBuffer;

		ZeroMemory((void*)&constantBuffer, sizeof(CBUFFER));

		constantBuffer.worldViewProjectionMatrix = wvpMatrix;

		constantBuffer.sphereColor = XMVectorSet(1.0f, 0.5f, 0.0f, 1.0f);

		gpID3D11DeviceContext->UpdateSubresource(gpID3D11Buffer_ConstantBuffer, 0, NULL, &constantBuffer, 0, 0);

		// Set Position Buffer Into The Pipeline Here (that means bind with vao)

		UINT stride = sizeof(float) * 3;

		UINT offset = NULL;

		gpID3D11DeviceContext->IASetVertexBuffers(0, 1, &gpID3D11Buffer_PositionBuffer, &stride, &offset);


		// set index buffer
		gpID3D11DeviceContext->IASetIndexBuffer(gpID3D11Buffer_IndexBuffer, DXGI_FORMAT_R16_UINT, 0); // R16 maps with 'short'


		// Set primitive geometry

		gpID3D11DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);


		// draw
		gpID3D11DeviceContext->DrawIndexed(gNumElements, 0, 0);


		// ****** Earth Related Transformations ***************


		translationMatrix = XMMatrixTranslation(1.0f, 0.0f, 0.0f);

		XMMATRIX eyRotationMatrix = XMMatrixIdentity();

		eyRotationMatrix = XMMatrixRotationY(XMConvertToRadians(year));

		XMMATRIX edRotationMatrix = XMMatrixIdentity();

		edRotationMatrix = XMMatrixRotationY(XMConvertToRadians(day));

		worldMatrix = edRotationMatrix * translationMatrix * eyRotationMatrix * worldMatrix;

		pushMatrix(worldMatrix);
		{

	
			XMMATRIX scaleMatrix = XMMatrixIdentity();

			scaleMatrix = XMMatrixScaling(0.52f, 0.52f, 0.52f);

			worldMatrix = scaleMatrix * worldMatrix;

			wvpMatrix = worldMatrix * viewMatrix * perspectiveProjectionMatrix;

			constantBuffer;

			ZeroMemory((void*)&constantBuffer, sizeof(CBUFFER));

			constantBuffer.worldViewProjectionMatrix = wvpMatrix;

			constantBuffer.sphereColor = XMVectorSet(0.4f, 0.9f, 1.0f, 1.0f);

			gpID3D11DeviceContext->UpdateSubresource(gpID3D11Buffer_ConstantBuffer, 0, NULL, &constantBuffer, 0, 0);

			// Set Position Buffer Into The Pipeline Here (that means bind with vao)

			stride = sizeof(float) * 3;

			offset = NULL;

			gpID3D11DeviceContext->IASetVertexBuffers(0, 1, &gpID3D11Buffer_PositionBuffer, &stride, &offset);


			// set index buffer
			gpID3D11DeviceContext->IASetIndexBuffer(gpID3D11Buffer_IndexBuffer, DXGI_FORMAT_R16_UINT, 0); // R16 maps with 'short'


			// Set primitive geometry

			gpID3D11DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);


			// draw
			gpID3D11DeviceContext->DrawIndexed(gNumElements, 0, 0);

		}
		worldMatrix = popMatrix();


		// ****** Moon Related Transformations ***************


		translationMatrix = XMMatrixTranslation(0.6f, 0.0f, 0.0f);

		XMMATRIX myRotationMatrix = XMMatrixIdentity();

		myRotationMatrix = XMMatrixRotationY(XMConvertToRadians(moon_year));

		XMMATRIX mdRotationMatrix = XMMatrixIdentity();

		mdRotationMatrix = XMMatrixRotationY(XMConvertToRadians(moon_day));

		worldMatrix = mdRotationMatrix * translationMatrix * myRotationMatrix * worldMatrix;

		pushMatrix(worldMatrix);
		{


			XMMATRIX scaleMatrix = XMMatrixIdentity();

			scaleMatrix = XMMatrixScaling(0.26f, 0.26f, 0.26f);

			worldMatrix = scaleMatrix * worldMatrix;

			wvpMatrix = worldMatrix * viewMatrix * perspectiveProjectionMatrix;

			constantBuffer;

			ZeroMemory((void*)&constantBuffer, sizeof(CBUFFER));

			constantBuffer.worldViewProjectionMatrix = wvpMatrix;

			constantBuffer.sphereColor = XMVectorSet(1.0f, 1.0f, 1.0f, 1.0f);

			gpID3D11DeviceContext->UpdateSubresource(gpID3D11Buffer_ConstantBuffer, 0, NULL, &constantBuffer, 0, 0);

			// Set Position Buffer Into The Pipeline Here (that means bind with vao)

			stride = sizeof(float) * 3;

			offset = NULL;

			gpID3D11DeviceContext->IASetVertexBuffers(0, 1, &gpID3D11Buffer_PositionBuffer, &stride, &offset);


			// set index buffer
			gpID3D11DeviceContext->IASetIndexBuffer(gpID3D11Buffer_IndexBuffer, DXGI_FORMAT_R16_UINT, 0); // R16 maps with 'short'


			// Set primitive geometry

			gpID3D11DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);


			// draw
			gpID3D11DeviceContext->DrawIndexed(gNumElements, 0, 0);

		}
		worldMatrix = popMatrix();


	}
	worldMatrix = popMatrix();

	



	


	// do double buffering by presenting the swapchain

	gpIDXGISwapChain->Present(0, 0);

}

void update(void)
{
	//code

}

void uninitialize(void)
{
	//Function declarations

	void ToggleFullScreen(void);

	//code

	// Index Buffer
	if (gpID3D11Buffer_IndexBuffer)
	{
		gpID3D11Buffer_IndexBuffer->Release();
		gpID3D11Buffer_IndexBuffer = NULL;
	}


	// Constant Buffer
	if (gpID3D11Buffer_ConstantBuffer)
	{
		gpID3D11Buffer_ConstantBuffer->Release();

		gpID3D11Buffer_ConstantBuffer = NULL;
	}

	// Position Buffer 
	if (gpID3D11Buffer_PositionBuffer)
	{
		gpID3D11Buffer_ConstantBuffer->Release();

		gpID3D11Buffer_ConstantBuffer = NULL;
	}

	// Input layout

	if (gpID3D11InputLayout)
	{
		gpID3D11InputLayout->Release();

		gpID3D11InputLayout = NULL;
	}

	// Pixel Shader

	if (gpID3D11PixelShader)
	{
		gpID3D11PixelShader->Release();
		gpID3D11PixelShader = NULL;
	}
	

	// vertex Shader

	if (gpID3D11VertexShader)
	{
		gpID3D11VertexShader->Release();
		gpID3D11VertexShader = NULL;
	}

	// Render Target View
	if (gpID3D11RenderTargetView)
	{
		gpID3D11RenderTargetView->Release();

		gpID3D11RenderTargetView = NULL;
	}

	// Device Context
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


void initializeMatrixStack(void)
{
	matrixStackTop = 0;

	for (int i = 0; i < MODEL_VIEW_MATRIX_STACK; i++)
	{
		matrixStack[i] = XMMatrixIdentity();
	}
}

void pushMatrix(XMMATRIX matrix)
{
	void uninitialize(void);

	// Check if stack exceeds its limit
	if (matrixStackTop >= MODEL_VIEW_MATRIX_STACK)
	{
		fprintf(gpFile, "ERROR : Exceeded matrix stack limit\n");
		uninitialize();
	}

	// Store matrix in stack
	matrixStack[matrixStackTop] = matrix;

	// Increment stack top
	matrixStackTop++;
}

XMMATRIX popMatrix(void)
{
	void uninitialize(void);

	// Check if stack is empty
	if (matrixStackTop <= 0)
	{
		fprintf(gpFile, "ERROR : Matrix Stack is empty\n");
		uninitialize();
	}

	// Reset the current top matrix to identity before popping
	matrixStack[matrixStackTop] = XMMatrixIdentity();
	matrixStackTop--;

	// Return the matrix at the new top of the stack
	XMMATRIX matrix = matrixStack[matrixStackTop];

	return matrix;
}






