//Windows header files

#include<windows.h>
#include<stdio.h>              // for file I/O
#include<stdlib.h>             // for exit()
#include<math.h>

#include<d3d11.h>   // direct 3d related header file
#include<d3dcompiler.h>          //shader compiler hearder file

#include"Sphere.h"

#pragma warning(disable:4838)

#include"XNAMath_204/xnamath.h"

#include "D3D.h"            // Header file in same directory

// link libraries related to D3D

#pragma comment (lib,"d3d11.lib")


#pragma comment (lib,"d3dcompiler.lib")
#pragma comment (lib,"Sphere.lib")

//Macros

#define WIN_WIDTH 800
#define WIN_HEIGHT 600

#define M_PI 3.14159265358979

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

ID3D11Buffer* gpID3D11Buffer_PositionBuffer = NULL;

ID3D11Buffer* gpID3D11Buffer_NormalBuffer = NULL;

ID3D11RasterizerState* gpID3D11RasterizerState = NULL;

ID3D11DepthStencilView* gpID3D11DepthStencilView = NULL;



ID3D11Buffer* gpID3D11Buffer_ConstantBuffer = NULL;

struct CBUFFER
{
	XMMATRIX WorldMatrix;
	XMMATRIX ViewMatrix;
	XMMATRIX ProjectionMatrix;

	//light Uniforms

	XMVECTOR LightAmbient;
	XMVECTOR LightDiffuse;
	XMVECTOR LightSpecular;
	XMVECTOR LightPosition;

	//material Uniforms

	XMVECTOR MaterialAmbient;
	XMVECTOR MaterialDiffuse;
	XMVECTOR MaterialSpecular;
	float MaterialShininess;

	unsigned int KeyPress;

};

XMMATRIX perspectiveProjectionMatrix;


float ClearColor[4];

//Sphere related variables

ID3D11Buffer* gpID3D11Buffer_IndexBuffer = NULL;

float sphere_vertices[1146];
float sphere_normals[1146];
float sphere_textures[764];
unsigned short sphere_elements[2280];
unsigned int gNumElements;
unsigned int gNumVertices;

BOOL bLightingEnabled = FALSE;

//light source properties

float lightAmbient[] = { 0.0f, 0.0f, 0.0f, 1.0f };
float lightDiffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
float lightSpecular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
float lightPosition[] = { 0.0f, 0.0f, 0.0f, 1.0f };

//material Properties

//float materialAmbient[] = { 0.0f, 0.0f, 0.0f, 1.0f };
//float materialDiffuse[] = { 0.5f, 0.2f, 0.7f, 1.0f };
//float materialSpecular[] = { 0.7f, 0.7f, 0.7f, 1.0f };
//float materialShininess = 128.0f;

//light rotation variables

float angleForXRotation = 0.0f;
float angleForYRotation = 0.0f;
float angleForZRotation = 0.0f;

float WindowWidth;
float WindowHeight;

int keyPressed = 0;



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
		TEXT("Prathamesh Laxmikant Paropkari            Direct3D   -  24 Spheres"),
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

		case 'L':
		case 'l':
			if (bLightingEnabled == FALSE)
			{
				bLightingEnabled = TRUE;
			}

			else
			{
				bLightingEnabled = FALSE;
			}

			break;

		case 'X':
		case 'x':
			keyPressed = 1;
			angleForXRotation = 0.0f;  //reset rotation
			break;

		case 'Y':
		case 'y':
			keyPressed = 2;
			angleForYRotation = 0.0f; //reset rotation
			break;

		case 'Z':
		case 'z':
			keyPressed = 3;
			angleForZRotation = 0.0f; //reset rotation
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
			"float4x4 worldMatrix;\n"
			"float4x4 viewMatrix;\n"
			"float4x4 projectionMatrix;\n"
			"float4 lightAmbient;\n"
			"float4 lightDiffuse;\n"
			"float4 lightSpecular;\n"
			"float4 lightPosition;\n"
			"float4 materialAmbient;\n"
			"float4 materialDiffuse;\n"
			"float4 materialSpecular;\n"
			"float materialShininess;\n"
			"uint keyPress;\n"
			"}\n"
			"struct vertexOutput\n"
			"{\n"
			"float4 position:SV_POSITION;\n"
			"float3 transformedNormals:NORMAL0;\n"
			"float3 lightDirection:NORMAL1;\n"
			"float3 viewerVector:NORMAL2;\n"			
			//"float3 phongADSLight:COLOR;\n"
			"};\n"
			"vertexOutput main(float4 pos:POSITION, float3 norm:NORMAL)\n"
			"{\n"
			"vertexOutput output;\n"
			"if(keyPress == 1)\n"
			"{\n"
			"float4 eyeCoordinate = mul(viewMatrix, mul(worldMatrix, pos));\n"
			"output.transformedNormals = mul((float3x3)worldMatrix, norm);\n"
			"output.lightDirection = (float3)(lightPosition - eyeCoordinate);\n"		
			"output.viewerVector = (-eyeCoordinate.xyz);\n"
			"}\n"
			"else\n"
			"{\n"
			"output.transformedNormals = float3(0.0,0.0,0.0);\n"
			"output.lightDirection = float3(0.0,0.0,0.0);\n"
			"output.viewerVector = float3(0.0,0.0,0.0);\n"
			"}\n"
			"output.position = mul(projectionMatrix, mul(viewMatrix, mul(worldMatrix, pos)));\n"
			"return output;\n"
			"}\n";		


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
		"cbuffer constantBuffer\n"
		"{\n"
		"float4x4 worldMatrix;\n"
		"float4x4 viewMatrix;\n"
		"float4x4 projectionMatrix;\n"
		"float4 lightAmbient;\n"
		"float4 lightDiffuse;\n"
		"float4 lightSpecular;\n"
		"float4 lightPosition;\n"
		"float4 materialAmbient;\n"
		"float4 materialDiffuse;\n"
		"float4 materialSpecular;\n"
		"float materialShininess;\n"
		"uint keyPress;\n"
		"}\n"
		"struct vertexOutput\n"
		"{\n"
		"float4 position:SV_POSITION;\n"
		"float3 transformedNormals:NORMAL0;\n"
		"float3 lightDirection:NORMAL1;\n"
		"float3 viewerVector:NORMAL2;\n"
		"};\n"
		"float4 main(vertexOutput input):SV_TARGET\n"
		"{\n"
		"float3 phongADSLight;"
		"if(keyPress == 1)\n"
		"{\n"
		"float3 normalizedTransformedNormals = normalize(input.transformedNormals);\n"
		"float3 normalizedLightDirection = normalize(input.lightDirection);\n"
		"float3 normalizedViewerVector = normalize(input.viewerVector);\n"
		"float3 reflectionVector = reflect(-normalizedLightDirection, normalizedTransformedNormals);\n"
		"float3 ambientLight = lightAmbient * materialAmbient;\n"
		"float3 diffuseLight = lightDiffuse * materialDiffuse * max(dot(normalizedLightDirection, normalizedTransformedNormals), 0.0);\n"
		"float3 specularLight = lightSpecular * materialSpecular * pow(max(dot(reflectionVector, normalizedViewerVector), 0.0), materialShininess);\n"
		"phongADSLight = ambientLight + diffuseLight + specularLight;\n"
		"}\n"
		"else\n"
		"{\n"
		"phongADSLight = float3(1.0,1.0,1.0);\n"
		"}\n"
		"float4 Color = float4(phongADSLight,1.0);\n"
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

	ZeroMemory((void*)d3dInputElementDesc, sizeof(D3D11_INPUT_ELEMENT_DESC) * 2);

	d3dInputElementDesc[0].SemanticName = "POSITION";
	d3dInputElementDesc[0].SemanticIndex = 0;
	d3dInputElementDesc[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	d3dInputElementDesc[0].InputSlot = 0;
	d3dInputElementDesc[0].AlignedByteOffset = 0;
	d3dInputElementDesc[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	d3dInputElementDesc[0].InstanceDataStepRate = 0;

	//Normal

	d3dInputElementDesc[1].SemanticName = "NORMAL";
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

	getSphereVertexData(sphere_vertices, sphere_normals, sphere_textures, sphere_elements);
	gNumVertices = getNumberOfSphereVertices();
	gNumElements = getNumberOfSphereElements();



	//create vertex buffer for position

	D3D11_BUFFER_DESC d3dBufferDesc;

	ZeroMemory((void*)&d3dBufferDesc, sizeof(D3D11_BUFFER_DESC));

	d3dBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	d3dBufferDesc.ByteWidth = gNumVertices * 3 * sizeof(float);
	d3dBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	D3D11_SUBRESOURCE_DATA d3d11SubresourceData;

	ZeroMemory((void*)&d3d11SubresourceData, sizeof(D3D11_SUBRESOURCE_DATA));

	d3d11SubresourceData.pSysMem = sphere_vertices;
	
	
	
	//create vertex buffer using above structure

	hr = gpID3D11Device->CreateBuffer(&d3dBufferDesc, &d3d11SubresourceData, &gpID3D11Buffer_PositionBuffer);

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

	//create normal buffer

	ZeroMemory((void*)&d3dBufferDesc, sizeof(D3D11_BUFFER_DESC));

	d3dBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	d3dBufferDesc.ByteWidth = gNumVertices * 3 * sizeof(float);
	d3dBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	//D3D11_SUBRESOURCE_DATA d3d11SubresourceData;

	ZeroMemory((void*)&d3d11SubresourceData, sizeof(D3D11_SUBRESOURCE_DATA));

	d3d11SubresourceData.pSysMem = sphere_normals;



	//create vertex buffer using above structure

	hr = gpID3D11Device->CreateBuffer(&d3dBufferDesc, &d3d11SubresourceData, &gpID3D11Buffer_NormalBuffer);

	if (FAILED(hr))
	{

		gpFile = fopen(gszLogFileName, "a+");

		fprintf(gpFile, "gpID3D11Device -> CreateBuffer() for normal buffer failed\n\n");

		fclose(gpFile);


	}

	else
	{
		gpFile = fopen(gszLogFileName, "a+");

		fprintf(gpFile, "gpID3D11Device -> CreateBuffer() for normal buffer success !!\n\n");

		fclose(gpFile);
	}

	// create index buffer

	ZeroMemory((void*)&d3dBufferDesc, sizeof(D3D11_BUFFER_DESC));

	d3dBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	d3dBufferDesc.ByteWidth = gNumElements * sizeof(unsigned short);
	d3dBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;

	ZeroMemory((void*)&d3d11SubresourceData, sizeof(D3D11_SUBRESOURCE_DATA));

	d3d11SubresourceData.pSysMem = sphere_elements;

	hr = gpID3D11Device->CreateBuffer(&d3dBufferDesc, &d3d11SubresourceData, &gpID3D11Buffer_IndexBuffer);

	if (FAILED(hr))
	{

		gpFile = fopen(gszLogFileName, "a+");

		fprintf(gpFile, "gpID3D11Device -> CreateBuffer() for index buffer failed\n\n");

		fclose(gpFile);


	}

	else
	{
		gpFile = fopen(gszLogFileName, "a+");

		fprintf(gpFile, "gpID3D11Device -> CreateBuffer() for index buffer success !!\n\n");

		fclose(gpFile);
	}


	

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

	gpID3D11DeviceContext->PSSetConstantBuffers(0, 1, &gpID3D11Buffer_ConstantBuffer);

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

	ClearColor[0] = 0.7f;
	ClearColor[1] = 0.7f;
	ClearColor[2] = 0.7f;
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

	WindowHeight = height;
	WindowWidth = width;

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

	float materialAmbient[4];
	float materialDiffuse[4];
	float materialSpecular[4];

	float Shininess;

	float horizontalDistance = WindowWidth / 6;
	float verticalDistance = WindowHeight / 6;

	gpID3D11DeviceContext->ClearRenderTargetView(gpID3D11RenderTargetView, ClearColor);

	gpID3D11DeviceContext->ClearDepthStencilView(gpID3D11DepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);

	

	//First Column precious stones

	//(emerald)

	materialAmbient[0] = 0.0215;
	materialAmbient[1] = 0.1745;
	materialAmbient[2] = 0.0215;
	materialAmbient[3] = 1.0f;



	materialDiffuse[0] = 0.07568;
	materialDiffuse[1] = 0.61424;
	materialDiffuse[2] = 0.07568;
	materialDiffuse[3] = 1.0f;



	materialSpecular[0] = 0.633;
	materialSpecular[1] = 0.727811;
	materialSpecular[2] = 0.633;
	materialSpecular[3] = 1.0f;



	float materialShininess = 0.6f * 128.0f;


	//transformations

	XMMATRIX worldMatrix = XMMatrixIdentity();

	XMMATRIX viewMatrix = XMMatrixIdentity();

	XMMATRIX wvpMatrix = XMMatrixIdentity();

	worldMatrix = XMMatrixTranslation(0.0f, 0.0f, 1.5f);

	//wvpMatrix = worldMatrix * viewMatrix * perspectiveProjectionMatrix;

	CBUFFER constantBuffer;

	ZeroMemory((void*)&constantBuffer, sizeof(CBUFFER));

	constantBuffer.WorldMatrix = worldMatrix;
	constantBuffer.ViewMatrix = viewMatrix;
	constantBuffer.ProjectionMatrix = perspectiveProjectionMatrix;

	if (bLightingEnabled == TRUE)
	{
		constantBuffer.KeyPress = 1;

		constantBuffer.LightAmbient = XMVectorSet(lightAmbient[0], lightAmbient[1], lightAmbient[2], 0.0f);

		constantBuffer.LightDiffuse = XMVectorSet(lightDiffuse[0], lightDiffuse[1], lightDiffuse[2], 0.0f);

		constantBuffer.LightSpecular = XMVectorSet(lightSpecular[0], lightSpecular[1], lightSpecular[2], 0.0f);

		constantBuffer.LightPosition = XMVectorSet(lightPosition[0], lightPosition[1], lightPosition[2], lightPosition[3]);

		//material 
		constantBuffer.MaterialAmbient = XMVectorSet(materialAmbient[0], materialAmbient[1], materialAmbient[2], 0.0f);
		constantBuffer.MaterialDiffuse = XMVectorSet(materialDiffuse[0], materialDiffuse[1], materialDiffuse[2], 0.0f);
		constantBuffer.MaterialSpecular = XMVectorSet(materialSpecular[0], materialSpecular[1], materialSpecular[2], 0.0f);

		constantBuffer.MaterialShininess = materialShininess;

	}
	else
	{
		constantBuffer.KeyPress = 0;
	}

	//constantBuffer.WorldViewProjectionMatrix = wvpMatrix;

	gpID3D11DeviceContext->UpdateSubresource(gpID3D11Buffer_ConstantBuffer, 0, NULL, &constantBuffer, 0, 0);                                    //glUniformmatrix3fv in OpenGL


	//set position buffer into the pipeline here  

	UINT stride = (sizeof(float) * 3);

	UINT offset = 0;

	gpID3D11DeviceContext->IASetVertexBuffers(0, 1, &gpID3D11Buffer_PositionBuffer, &stride, &offset);

	//set Normal buffer into the pipeline here 

	stride = (sizeof(float) * 3);

	offset = 0;

	gpID3D11DeviceContext->IASetVertexBuffers(1, 1, &gpID3D11Buffer_NormalBuffer, &stride, &offset);

	// Set the index buffer
	gpID3D11DeviceContext->IASetIndexBuffer(gpID3D11Buffer_IndexBuffer, DXGI_FORMAT_R16_UINT, 0); // Using R16_UINT as you have short indices

	//set primitive geometry

	gpID3D11DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	
	//draw the geometry

	// fill and set viewport structure here
	D3D11_VIEWPORT d3dViewPort;

	ZeroMemory((void*)&d3dViewPort, sizeof(D3D11_VIEWPORT));

	d3dViewPort.TopLeftX = 0.0f;

	d3dViewPort.TopLeftY = 0.0f;

	d3dViewPort.Width = horizontalDistance;

	d3dViewPort.Height = verticalDistance;

	d3dViewPort.MinDepth = 0.0f;

	d3dViewPort.MaxDepth = 1.0f;

	// set above details into pipeline

	gpID3D11DeviceContext->RSSetViewports(1, &d3dViewPort);
	 // draw
	gpID3D11DeviceContext->DrawIndexed(gNumElements, 0, 0);

	//first column second sphere

	materialAmbient[0] = 0.135;
	materialAmbient[1] = 0.2225;
	materialAmbient[2] = 0.1575;
	materialAmbient[3] = 1.0f;



	materialDiffuse[0] = 0.54;
	materialDiffuse[1] = 0.89;
	materialDiffuse[2] = 0.63;
	materialDiffuse[3] = 1.0f;



	materialSpecular[0] = 0.316228;
	materialSpecular[1] = 0.316228;
	materialSpecular[2] = 0.316228;
	materialSpecular[3] = 1.0f;



	materialShininess = 0.1 * 128;


	//transformations

	 worldMatrix = XMMatrixIdentity();

	 viewMatrix = XMMatrixIdentity();

	 wvpMatrix = XMMatrixIdentity();

	worldMatrix = XMMatrixTranslation(0.0f, 0.0f, 1.5f);

	//wvpMatrix = worldMatrix * viewMatrix * perspectiveProjectionMatrix;

	

	ZeroMemory((void*)&constantBuffer, sizeof(CBUFFER));

	constantBuffer.WorldMatrix = worldMatrix;
	constantBuffer.ViewMatrix = viewMatrix;
	constantBuffer.ProjectionMatrix = perspectiveProjectionMatrix;

	if (bLightingEnabled == TRUE)
	{
		constantBuffer.KeyPress = 1;

		constantBuffer.LightAmbient = XMVectorSet(lightAmbient[0], lightAmbient[1], lightAmbient[2], 0.0f);

		constantBuffer.LightDiffuse = XMVectorSet(lightDiffuse[0], lightDiffuse[1], lightDiffuse[2], 0.0f);

		constantBuffer.LightSpecular = XMVectorSet(lightSpecular[0], lightSpecular[1], lightSpecular[2], 0.0f);

		constantBuffer.LightPosition = XMVectorSet(lightPosition[0], lightPosition[1], lightPosition[2], lightPosition[3]);

		//material 
		constantBuffer.MaterialAmbient = XMVectorSet(materialAmbient[0], materialAmbient[1], materialAmbient[2], 0.0f);
		constantBuffer.MaterialDiffuse = XMVectorSet(materialDiffuse[0], materialDiffuse[1], materialDiffuse[2], 0.0f);
		constantBuffer.MaterialSpecular = XMVectorSet(materialSpecular[0], materialSpecular[1], materialSpecular[2], 0.0f);

		constantBuffer.MaterialShininess = materialShininess;

	}
	else
	{
		constantBuffer.KeyPress = 0;
	}

	//constantBuffer.WorldViewProjectionMatrix = wvpMatrix;

	gpID3D11DeviceContext->UpdateSubresource(gpID3D11Buffer_ConstantBuffer, 0, NULL, &constantBuffer, 0, 0);                                    //glUniformmatrix3fv in OpenGL


	//set position buffer into the pipeline here  

	 stride = (sizeof(float) * 3);

	 offset = 0;

	gpID3D11DeviceContext->IASetVertexBuffers(0, 1, &gpID3D11Buffer_PositionBuffer, &stride, &offset);

	//set Normal buffer into the pipeline here 

	stride = (sizeof(float) * 3);

	offset = 0;

	gpID3D11DeviceContext->IASetVertexBuffers(1, 1, &gpID3D11Buffer_NormalBuffer, &stride, &offset);

	// Set the index buffer
	gpID3D11DeviceContext->IASetIndexBuffer(gpID3D11Buffer_IndexBuffer, DXGI_FORMAT_R16_UINT, 0); // Using R16_UINT as you have short indices

	//set primitive geometry

	gpID3D11DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);


	//draw the geometry

	// fill and set viewport structure here

	

	ZeroMemory((void*)&d3dViewPort, sizeof(D3D11_VIEWPORT));

	d3dViewPort.TopLeftX = 0.0f;

	d3dViewPort.TopLeftY = verticalDistance;

	d3dViewPort.Width = horizontalDistance;

	d3dViewPort.Height = verticalDistance;

	d3dViewPort.MinDepth = 0.0f;

	d3dViewPort.MaxDepth = 1.0f;

	// set above details into pipeline

	gpID3D11DeviceContext->RSSetViewports(1, &d3dViewPort);
	// draw
	gpID3D11DeviceContext->DrawIndexed(gNumElements, 0, 0);

	//first column Third sphere

	materialAmbient[0] = 0.05375;
	materialAmbient[1] = 0.05;
	materialAmbient[2] = 0.06625;
	materialAmbient[3] = 1.0f;



	materialDiffuse[0] = 0.18275;
	materialDiffuse[1] = 0.17;
	materialDiffuse[2] = 0.22525;
	materialDiffuse[3] = 1.0f;



	materialSpecular[0] = 0.332741;
	materialSpecular[1] = 0.328634;
	materialSpecular[2] = 0.346435;
	materialSpecular[3] = 1.0f;



	materialShininess = 0.3 * 128;

	//transformations

	worldMatrix = XMMatrixIdentity();

	viewMatrix = XMMatrixIdentity();

	wvpMatrix = XMMatrixIdentity();

	worldMatrix = XMMatrixTranslation(0.0f, 0.0f, 1.5f);

	//wvpMatrix = worldMatrix * viewMatrix * perspectiveProjectionMatrix;



	ZeroMemory((void*)&constantBuffer, sizeof(CBUFFER));

	constantBuffer.WorldMatrix = worldMatrix;
	constantBuffer.ViewMatrix = viewMatrix;
	constantBuffer.ProjectionMatrix = perspectiveProjectionMatrix;

	if (bLightingEnabled == TRUE)
	{
		constantBuffer.KeyPress = 1;

		constantBuffer.LightAmbient = XMVectorSet(lightAmbient[0], lightAmbient[1], lightAmbient[2], 0.0f);

		constantBuffer.LightDiffuse = XMVectorSet(lightDiffuse[0], lightDiffuse[1], lightDiffuse[2], 0.0f);

		constantBuffer.LightSpecular = XMVectorSet(lightSpecular[0], lightSpecular[1], lightSpecular[2], 0.0f);

		constantBuffer.LightPosition = XMVectorSet(lightPosition[0], lightPosition[1], lightPosition[2], lightPosition[3]);

		//material 
		constantBuffer.MaterialAmbient = XMVectorSet(materialAmbient[0], materialAmbient[1], materialAmbient[2], 0.0f);
		constantBuffer.MaterialDiffuse = XMVectorSet(materialDiffuse[0], materialDiffuse[1], materialDiffuse[2], 0.0f);
		constantBuffer.MaterialSpecular = XMVectorSet(materialSpecular[0], materialSpecular[1], materialSpecular[2], 0.0f);

		constantBuffer.MaterialShininess = materialShininess;

	}
	else
	{
		constantBuffer.KeyPress = 0;
	}

	//constantBuffer.WorldViewProjectionMatrix = wvpMatrix;

	gpID3D11DeviceContext->UpdateSubresource(gpID3D11Buffer_ConstantBuffer, 0, NULL, &constantBuffer, 0, 0);                                    //glUniformmatrix3fv in OpenGL


	//set position buffer into the pipeline here  

	stride = (sizeof(float) * 3);

	offset = 0;

	gpID3D11DeviceContext->IASetVertexBuffers(0, 1, &gpID3D11Buffer_PositionBuffer, &stride, &offset);

	//set Normal buffer into the pipeline here 

	stride = (sizeof(float) * 3);

	offset = 0;

	gpID3D11DeviceContext->IASetVertexBuffers(1, 1, &gpID3D11Buffer_NormalBuffer, &stride, &offset);

	// Set the index buffer
	gpID3D11DeviceContext->IASetIndexBuffer(gpID3D11Buffer_IndexBuffer, DXGI_FORMAT_R16_UINT, 0); // Using R16_UINT as you have short indices

	//set primitive geometry

	gpID3D11DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);


	//draw the geometry

	// fill and set viewport structure here



	ZeroMemory((void*)&d3dViewPort, sizeof(D3D11_VIEWPORT));

	d3dViewPort.TopLeftX = 0.0f;

	d3dViewPort.TopLeftY = verticalDistance * 2;

	d3dViewPort.Width = horizontalDistance;

	d3dViewPort.Height = verticalDistance;

	d3dViewPort.MinDepth = 0.0f;

	d3dViewPort.MaxDepth = 1.0f;

	// set above details into pipeline

	gpID3D11DeviceContext->RSSetViewports(1, &d3dViewPort);
	// draw
	gpID3D11DeviceContext->DrawIndexed(gNumElements, 0, 0);

	//first column Fourth sphere


	materialAmbient[0] = 0.25;
	materialAmbient[1] = 0.20725;
	materialAmbient[2] = 0.20725;
	materialAmbient[3] = 1.0f;



	materialDiffuse[0] = 1.0;
	materialDiffuse[1] = 0.829;
	materialDiffuse[2] = 0.829;
	materialDiffuse[3] = 1.0f;



	materialSpecular[0] = 0.296648;
	materialSpecular[1] = 0.296648;
	materialSpecular[2] = 0.296648;
	materialSpecular[3] = 1.0f;



	materialShininess = 0.088 * 128;

	//transformations

	worldMatrix = XMMatrixIdentity();

	viewMatrix = XMMatrixIdentity();

	wvpMatrix = XMMatrixIdentity();

	worldMatrix = XMMatrixTranslation(0.0f, 0.0f, 1.5f);

	//wvpMatrix = worldMatrix * viewMatrix * perspectiveProjectionMatrix;



	ZeroMemory((void*)&constantBuffer, sizeof(CBUFFER));

	constantBuffer.WorldMatrix = worldMatrix;
	constantBuffer.ViewMatrix = viewMatrix;
	constantBuffer.ProjectionMatrix = perspectiveProjectionMatrix;

	if (bLightingEnabled == TRUE)
	{
		constantBuffer.KeyPress = 1;

		constantBuffer.LightAmbient = XMVectorSet(lightAmbient[0], lightAmbient[1], lightAmbient[2], 0.0f);

		constantBuffer.LightDiffuse = XMVectorSet(lightDiffuse[0], lightDiffuse[1], lightDiffuse[2], 0.0f);

		constantBuffer.LightSpecular = XMVectorSet(lightSpecular[0], lightSpecular[1], lightSpecular[2], 0.0f);

		constantBuffer.LightPosition = XMVectorSet(lightPosition[0], lightPosition[1], lightPosition[2], lightPosition[3]);

		//material 
		constantBuffer.MaterialAmbient = XMVectorSet(materialAmbient[0], materialAmbient[1], materialAmbient[2], 0.0f);
		constantBuffer.MaterialDiffuse = XMVectorSet(materialDiffuse[0], materialDiffuse[1], materialDiffuse[2], 0.0f);
		constantBuffer.MaterialSpecular = XMVectorSet(materialSpecular[0], materialSpecular[1], materialSpecular[2], 0.0f);

		constantBuffer.MaterialShininess = materialShininess;

	}
	else
	{
		constantBuffer.KeyPress = 0;
	}

	//constantBuffer.WorldViewProjectionMatrix = wvpMatrix;

	gpID3D11DeviceContext->UpdateSubresource(gpID3D11Buffer_ConstantBuffer, 0, NULL, &constantBuffer, 0, 0);                                    //glUniformmatrix3fv in OpenGL


	//set position buffer into the pipeline here  

	stride = (sizeof(float) * 3);

	offset = 0;

	gpID3D11DeviceContext->IASetVertexBuffers(0, 1, &gpID3D11Buffer_PositionBuffer, &stride, &offset);

	//set Normal buffer into the pipeline here 

	stride = (sizeof(float) * 3);

	offset = 0;

	gpID3D11DeviceContext->IASetVertexBuffers(1, 1, &gpID3D11Buffer_NormalBuffer, &stride, &offset);

	// Set the index buffer
	gpID3D11DeviceContext->IASetIndexBuffer(gpID3D11Buffer_IndexBuffer, DXGI_FORMAT_R16_UINT, 0); // Using R16_UINT as you have short indices

	//set primitive geometry

	gpID3D11DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);


	//draw the geometry

	// fill and set viewport structure here



	ZeroMemory((void*)&d3dViewPort, sizeof(D3D11_VIEWPORT));

	d3dViewPort.TopLeftX = 0.0f;

	d3dViewPort.TopLeftY = verticalDistance * 3;

	d3dViewPort.Width = horizontalDistance;

	d3dViewPort.Height = verticalDistance;

	d3dViewPort.MinDepth = 0.0f;

	d3dViewPort.MaxDepth = 1.0f;

	// set above details into pipeline

	gpID3D11DeviceContext->RSSetViewports(1, &d3dViewPort);
	// draw
	gpID3D11DeviceContext->DrawIndexed(gNumElements, 0, 0);

	//first column Fifth sphere

	materialAmbient[0] = 0.1745;
	materialAmbient[1] = 0.01175;
	materialAmbient[2] = 0.01175;
	materialAmbient[3] = 1.0f;


	materialDiffuse[0] = 0.61424;
	materialDiffuse[1] = 0.04136;
	materialDiffuse[2] = 0.04136;
	materialDiffuse[3] = 1.0f;


	materialSpecular[0] = 0.727811;
	materialSpecular[1] = 0.626959;
	materialSpecular[2] = 0.626959;
	materialSpecular[3] = 1.0f;


	materialShininess = 0.6 * 128;

	//transformations

	worldMatrix = XMMatrixIdentity();

	viewMatrix = XMMatrixIdentity();

	wvpMatrix = XMMatrixIdentity();

	worldMatrix = XMMatrixTranslation(0.0f, 0.0f, 1.5f);




	ZeroMemory((void*)&constantBuffer, sizeof(CBUFFER));

	constantBuffer.WorldMatrix = worldMatrix;
	constantBuffer.ViewMatrix = viewMatrix;
	constantBuffer.ProjectionMatrix = perspectiveProjectionMatrix;

	if (bLightingEnabled == TRUE)
	{
		constantBuffer.KeyPress = 1;

		constantBuffer.LightAmbient = XMVectorSet(lightAmbient[0], lightAmbient[1], lightAmbient[2], 0.0f);

		constantBuffer.LightDiffuse = XMVectorSet(lightDiffuse[0], lightDiffuse[1], lightDiffuse[2], 0.0f);

		constantBuffer.LightSpecular = XMVectorSet(lightSpecular[0], lightSpecular[1], lightSpecular[2], 0.0f);

		constantBuffer.LightPosition = XMVectorSet(lightPosition[0], lightPosition[1], lightPosition[2], lightPosition[3]);

		//material 
		constantBuffer.MaterialAmbient = XMVectorSet(materialAmbient[0], materialAmbient[1], materialAmbient[2], 0.0f);
		constantBuffer.MaterialDiffuse = XMVectorSet(materialDiffuse[0], materialDiffuse[1], materialDiffuse[2], 0.0f);
		constantBuffer.MaterialSpecular = XMVectorSet(materialSpecular[0], materialSpecular[1], materialSpecular[2], 0.0f);

		constantBuffer.MaterialShininess = materialShininess;

	}
	else
	{
		constantBuffer.KeyPress = 0;
	}

	//constantBuffer.WorldViewProjectionMatrix = wvpMatrix;

	gpID3D11DeviceContext->UpdateSubresource(gpID3D11Buffer_ConstantBuffer, 0, NULL, &constantBuffer, 0, 0);                                    //glUniformmatrix3fv in OpenGL


	//set position buffer into the pipeline here  

	stride = (sizeof(float) * 3);

	offset = 0;

	gpID3D11DeviceContext->IASetVertexBuffers(0, 1, &gpID3D11Buffer_PositionBuffer, &stride, &offset);

	//set Normal buffer into the pipeline here 

	stride = (sizeof(float) * 3);

	offset = 0;

	gpID3D11DeviceContext->IASetVertexBuffers(1, 1, &gpID3D11Buffer_NormalBuffer, &stride, &offset);

	// Set the index buffer
	gpID3D11DeviceContext->IASetIndexBuffer(gpID3D11Buffer_IndexBuffer, DXGI_FORMAT_R16_UINT, 0); // Using R16_UINT as you have short indices

	//set primitive geometry

	gpID3D11DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);


	//draw the geometry

	// fill and set viewport structure here



	ZeroMemory((void*)&d3dViewPort, sizeof(D3D11_VIEWPORT));

	d3dViewPort.TopLeftX = 0.0f;

	d3dViewPort.TopLeftY = verticalDistance * 4;

	d3dViewPort.Width = horizontalDistance;

	d3dViewPort.Height = verticalDistance;

	d3dViewPort.MinDepth = 0.0f;

	d3dViewPort.MaxDepth = 1.0f;

	// set above details into pipeline

	gpID3D11DeviceContext->RSSetViewports(1, &d3dViewPort);
	// draw
	gpID3D11DeviceContext->DrawIndexed(gNumElements, 0, 0);

	//first column Sixth sphere

	materialAmbient[0] = 0.1;
	materialAmbient[1] = 0.18725;
	materialAmbient[2] = 0.1745;
	materialAmbient[3] = 1.0f;


	materialDiffuse[0] = 0.396;
	materialDiffuse[1] = 0.74151;
	materialDiffuse[2] = 0.69102;
	materialDiffuse[3] = 1.0f;


	materialSpecular[0] = 0.297254;
	materialSpecular[1] = 0.30829;
	materialSpecular[2] = 0.306678;
	materialSpecular[3] = 1.0f;

	materialShininess = 0.1 * 128;


	//transformations

	worldMatrix = XMMatrixIdentity();

	viewMatrix = XMMatrixIdentity();

	wvpMatrix = XMMatrixIdentity();

	worldMatrix = XMMatrixTranslation(0.0f, 0.0f, 1.5f);

	ZeroMemory((void*)&constantBuffer, sizeof(CBUFFER));

	constantBuffer.WorldMatrix = worldMatrix;
	constantBuffer.ViewMatrix = viewMatrix;
	constantBuffer.ProjectionMatrix = perspectiveProjectionMatrix;

	if (bLightingEnabled == TRUE)
	{
		constantBuffer.KeyPress = 1;

		constantBuffer.LightAmbient = XMVectorSet(lightAmbient[0], lightAmbient[1], lightAmbient[2], 0.0f);

		constantBuffer.LightDiffuse = XMVectorSet(lightDiffuse[0], lightDiffuse[1], lightDiffuse[2], 0.0f);

		constantBuffer.LightSpecular = XMVectorSet(lightSpecular[0], lightSpecular[1], lightSpecular[2], 0.0f);

		constantBuffer.LightPosition = XMVectorSet(lightPosition[0], lightPosition[1], lightPosition[2], lightPosition[3]);

		//material 
		constantBuffer.MaterialAmbient = XMVectorSet(materialAmbient[0], materialAmbient[1], materialAmbient[2], 0.0f);
		constantBuffer.MaterialDiffuse = XMVectorSet(materialDiffuse[0], materialDiffuse[1], materialDiffuse[2], 0.0f);
		constantBuffer.MaterialSpecular = XMVectorSet(materialSpecular[0], materialSpecular[1], materialSpecular[2], 0.0f);

		constantBuffer.MaterialShininess = materialShininess;

	}
	else
	{
		constantBuffer.KeyPress = 0;
	}

	
	gpID3D11DeviceContext->UpdateSubresource(gpID3D11Buffer_ConstantBuffer, 0, NULL, &constantBuffer, 0, 0);   //glUniformmatrix3fv in OpenGL


	//set position buffer into the pipeline here  

	stride = (sizeof(float) * 3);

	offset = 0;

	gpID3D11DeviceContext->IASetVertexBuffers(0, 1, &gpID3D11Buffer_PositionBuffer, &stride, &offset);

	//set Normal buffer into the pipeline here 

	stride = (sizeof(float) * 3);

	offset = 0;

	gpID3D11DeviceContext->IASetVertexBuffers(1, 1, &gpID3D11Buffer_NormalBuffer, &stride, &offset);

	// Set the index buffer
	gpID3D11DeviceContext->IASetIndexBuffer(gpID3D11Buffer_IndexBuffer, DXGI_FORMAT_R16_UINT, 0); // Using R16_UINT as you have short indices

	//set primitive geometry

	gpID3D11DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);


	//draw the geometry

	// fill and set viewport structure here

	ZeroMemory((void*)&d3dViewPort, sizeof(D3D11_VIEWPORT));

	d3dViewPort.TopLeftX = 0.0f;

	d3dViewPort.TopLeftY = verticalDistance * 5;

	d3dViewPort.Width = horizontalDistance;

	d3dViewPort.Height = verticalDistance;

	d3dViewPort.MinDepth = 0.0f;

	d3dViewPort.MaxDepth = 1.0f;

	// set above details into pipeline

	gpID3D11DeviceContext->RSSetViewports(1, &d3dViewPort);
	// draw
	gpID3D11DeviceContext->DrawIndexed(gNumElements, 0, 0);

	//second column first sphere

	materialAmbient[0] = 0.329412;
	materialAmbient[1] = 0.223529;
	materialAmbient[2] = 0.027451;
	materialAmbient[3] = 1.0f;

	materialDiffuse[0] = 0.780392;
	materialDiffuse[1] = 0.568627;
	materialDiffuse[2] = 0.113725;
	materialDiffuse[3] = 1.0f;


	materialSpecular[0] = 0.992157;
	materialSpecular[1] = 0.941176;
	materialSpecular[2] = 0.807843;
	materialSpecular[3] = 1.0f;

	materialShininess = 0.21794872 * 128;

	//transformations

	worldMatrix = XMMatrixIdentity();

	viewMatrix = XMMatrixIdentity();

	wvpMatrix = XMMatrixIdentity();

	worldMatrix = XMMatrixTranslation(0.0f, 0.0f, 1.5f);

	ZeroMemory((void*)&constantBuffer, sizeof(CBUFFER));

	constantBuffer.WorldMatrix = worldMatrix;
	constantBuffer.ViewMatrix = viewMatrix;
	constantBuffer.ProjectionMatrix = perspectiveProjectionMatrix;

	if (bLightingEnabled == TRUE)
	{
		constantBuffer.KeyPress = 1;

		constantBuffer.LightAmbient = XMVectorSet(lightAmbient[0], lightAmbient[1], lightAmbient[2], 0.0f);

		constantBuffer.LightDiffuse = XMVectorSet(lightDiffuse[0], lightDiffuse[1], lightDiffuse[2], 0.0f);

		constantBuffer.LightSpecular = XMVectorSet(lightSpecular[0], lightSpecular[1], lightSpecular[2], 0.0f);

		constantBuffer.LightPosition = XMVectorSet(lightPosition[0], lightPosition[1], lightPosition[2], lightPosition[3]);

		//material 
		constantBuffer.MaterialAmbient = XMVectorSet(materialAmbient[0], materialAmbient[1], materialAmbient[2], 0.0f);
		constantBuffer.MaterialDiffuse = XMVectorSet(materialDiffuse[0], materialDiffuse[1], materialDiffuse[2], 0.0f);
		constantBuffer.MaterialSpecular = XMVectorSet(materialSpecular[0], materialSpecular[1], materialSpecular[2], 0.0f);

		constantBuffer.MaterialShininess = materialShininess;

	}
	else
	{
		constantBuffer.KeyPress = 0;
	}


	gpID3D11DeviceContext->UpdateSubresource(gpID3D11Buffer_ConstantBuffer, 0, NULL, &constantBuffer, 0, 0);   //glUniformmatrix3fv in OpenGL


	//set position buffer into the pipeline here  

	stride = (sizeof(float) * 3);

	offset = 0;

	gpID3D11DeviceContext->IASetVertexBuffers(0, 1, &gpID3D11Buffer_PositionBuffer, &stride, &offset);

	//set Normal buffer into the pipeline here 

	stride = (sizeof(float) * 3);

	offset = 0;

	gpID3D11DeviceContext->IASetVertexBuffers(1, 1, &gpID3D11Buffer_NormalBuffer, &stride, &offset);

	// Set the index buffer
	gpID3D11DeviceContext->IASetIndexBuffer(gpID3D11Buffer_IndexBuffer, DXGI_FORMAT_R16_UINT, 0); // Using R16_UINT as you have short indices

	//set primitive geometry

	gpID3D11DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);


	//draw the geometry

	// fill and set viewport structure here

	ZeroMemory((void*)&d3dViewPort, sizeof(D3D11_VIEWPORT));

	d3dViewPort.TopLeftX = horizontalDistance;

	d3dViewPort.TopLeftY = 0.0f;

	d3dViewPort.Width = horizontalDistance;

	d3dViewPort.Height = verticalDistance;

	d3dViewPort.MinDepth = 0.0f;

	d3dViewPort.MaxDepth = 1.0f;

	// set above details into pipeline

	gpID3D11DeviceContext->RSSetViewports(1, &d3dViewPort);
	// draw
	gpID3D11DeviceContext->DrawIndexed(gNumElements, 0, 0);

	//second column second sphere

	materialAmbient[0] = 0.2125;
	materialAmbient[1] = 0.1275;
	materialAmbient[2] = 0.054;
	materialAmbient[3] = 1.0f;

	materialDiffuse[0] = 0.714;
	materialDiffuse[1] = 0.568627;
	materialDiffuse[2] = 0.113725;
	materialDiffuse[3] = 1.0f;

	materialSpecular[0] = 0.992157;
	materialSpecular[1] = 0.941176;
	materialSpecular[2] = 0.807843;
	materialSpecular[3] = 1.0f;


	materialShininess = 0.21794872 * 128;

	//transformations

	worldMatrix = XMMatrixIdentity();

	viewMatrix = XMMatrixIdentity();

	wvpMatrix = XMMatrixIdentity();

	worldMatrix = XMMatrixTranslation(0.0f, 0.0f, 1.5f);

	ZeroMemory((void*)&constantBuffer, sizeof(CBUFFER));

	constantBuffer.WorldMatrix = worldMatrix;
	constantBuffer.ViewMatrix = viewMatrix;
	constantBuffer.ProjectionMatrix = perspectiveProjectionMatrix;

	if (bLightingEnabled == TRUE)
	{
		constantBuffer.KeyPress = 1;

		constantBuffer.LightAmbient = XMVectorSet(lightAmbient[0], lightAmbient[1], lightAmbient[2], 0.0f);

		constantBuffer.LightDiffuse = XMVectorSet(lightDiffuse[0], lightDiffuse[1], lightDiffuse[2], 0.0f);

		constantBuffer.LightSpecular = XMVectorSet(lightSpecular[0], lightSpecular[1], lightSpecular[2], 0.0f);

		constantBuffer.LightPosition = XMVectorSet(lightPosition[0], lightPosition[1], lightPosition[2], lightPosition[3]);

		//material 
		constantBuffer.MaterialAmbient = XMVectorSet(materialAmbient[0], materialAmbient[1], materialAmbient[2], 0.0f);
		constantBuffer.MaterialDiffuse = XMVectorSet(materialDiffuse[0], materialDiffuse[1], materialDiffuse[2], 0.0f);
		constantBuffer.MaterialSpecular = XMVectorSet(materialSpecular[0], materialSpecular[1], materialSpecular[2], 0.0f);

		constantBuffer.MaterialShininess = materialShininess;

	}
	else
	{
		constantBuffer.KeyPress = 0;
	}


	gpID3D11DeviceContext->UpdateSubresource(gpID3D11Buffer_ConstantBuffer, 0, NULL, &constantBuffer, 0, 0);   //glUniformmatrix3fv in OpenGL


	//set position buffer into the pipeline here  

	stride = (sizeof(float) * 3);

	offset = 0;

	gpID3D11DeviceContext->IASetVertexBuffers(0, 1, &gpID3D11Buffer_PositionBuffer, &stride, &offset);

	//set Normal buffer into the pipeline here 

	stride = (sizeof(float) * 3);

	offset = 0;

	gpID3D11DeviceContext->IASetVertexBuffers(1, 1, &gpID3D11Buffer_NormalBuffer, &stride, &offset);

	// Set the index buffer
	gpID3D11DeviceContext->IASetIndexBuffer(gpID3D11Buffer_IndexBuffer, DXGI_FORMAT_R16_UINT, 0); // Using R16_UINT as you have short indices

	//set primitive geometry

	gpID3D11DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);


	//draw the geometry

	// fill and set viewport structure here

	ZeroMemory((void*)&d3dViewPort, sizeof(D3D11_VIEWPORT));

	d3dViewPort.TopLeftX = horizontalDistance;

	d3dViewPort.TopLeftY = verticalDistance;

	d3dViewPort.Width = horizontalDistance;

	d3dViewPort.Height = verticalDistance;

	d3dViewPort.MinDepth = 0.0f;

	d3dViewPort.MaxDepth = 1.0f;

	// set above details into pipeline

	gpID3D11DeviceContext->RSSetViewports(1, &d3dViewPort);
	// draw
	gpID3D11DeviceContext->DrawIndexed(gNumElements, 0, 0);

	//second column third sphere

	materialAmbient[0] = 0.25;
	materialAmbient[1] = 0.25;
	materialAmbient[2] = 0.25;
	materialAmbient[3] = 1.0f;

	materialDiffuse[0] = 0.4;
	materialDiffuse[1] = 0.4;
	materialDiffuse[2] = 0.4;
	materialDiffuse[3] = 1.0f;


	materialSpecular[0] = 0.774597;
	materialSpecular[1] = 0.774597;
	materialSpecular[2] = 0.774597;
	materialSpecular[3] = 1.0f;


	materialShininess = 0.6 * 128;

	//transformations

	worldMatrix = XMMatrixIdentity();

	viewMatrix = XMMatrixIdentity();

	wvpMatrix = XMMatrixIdentity();

	worldMatrix = XMMatrixTranslation(0.0f, 0.0f, 1.5f);

	ZeroMemory((void*)&constantBuffer, sizeof(CBUFFER));

	constantBuffer.WorldMatrix = worldMatrix;
	constantBuffer.ViewMatrix = viewMatrix;
	constantBuffer.ProjectionMatrix = perspectiveProjectionMatrix;

	if (bLightingEnabled == TRUE)
	{
		constantBuffer.KeyPress = 1;

		constantBuffer.LightAmbient = XMVectorSet(lightAmbient[0], lightAmbient[1], lightAmbient[2], 0.0f);

		constantBuffer.LightDiffuse = XMVectorSet(lightDiffuse[0], lightDiffuse[1], lightDiffuse[2], 0.0f);

		constantBuffer.LightSpecular = XMVectorSet(lightSpecular[0], lightSpecular[1], lightSpecular[2], 0.0f);

		constantBuffer.LightPosition = XMVectorSet(lightPosition[0], lightPosition[1], lightPosition[2], lightPosition[3]);

		//material 
		constantBuffer.MaterialAmbient = XMVectorSet(materialAmbient[0], materialAmbient[1], materialAmbient[2], 0.0f);
		constantBuffer.MaterialDiffuse = XMVectorSet(materialDiffuse[0], materialDiffuse[1], materialDiffuse[2], 0.0f);
		constantBuffer.MaterialSpecular = XMVectorSet(materialSpecular[0], materialSpecular[1], materialSpecular[2], 0.0f);

		constantBuffer.MaterialShininess = materialShininess;

	}
	else
	{
		constantBuffer.KeyPress = 0;
	}


	gpID3D11DeviceContext->UpdateSubresource(gpID3D11Buffer_ConstantBuffer, 0, NULL, &constantBuffer, 0, 0);   //glUniformmatrix3fv in OpenGL


	//set position buffer into the pipeline here  

	stride = (sizeof(float) * 3);

	offset = 0;

	gpID3D11DeviceContext->IASetVertexBuffers(0, 1, &gpID3D11Buffer_PositionBuffer, &stride, &offset);

	//set Normal buffer into the pipeline here 

	stride = (sizeof(float) * 3);

	offset = 0;

	gpID3D11DeviceContext->IASetVertexBuffers(1, 1, &gpID3D11Buffer_NormalBuffer, &stride, &offset);

	// Set the index buffer
	gpID3D11DeviceContext->IASetIndexBuffer(gpID3D11Buffer_IndexBuffer, DXGI_FORMAT_R16_UINT, 0); // Using R16_UINT as you have short indices

	//set primitive geometry

	gpID3D11DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);


	//draw the geometry

	// fill and set viewport structure here

	ZeroMemory((void*)&d3dViewPort, sizeof(D3D11_VIEWPORT));

	d3dViewPort.TopLeftX = horizontalDistance;

	d3dViewPort.TopLeftY = verticalDistance * 2.0;

	d3dViewPort.Width = horizontalDistance;

	d3dViewPort.Height = verticalDistance;

	d3dViewPort.MinDepth = 0.0f;

	d3dViewPort.MaxDepth = 1.0f;

	// set above details into pipeline

	gpID3D11DeviceContext->RSSetViewports(1, &d3dViewPort);
	// draw
	gpID3D11DeviceContext->DrawIndexed(gNumElements, 0, 0);

	//second column fourth sphere

	materialAmbient[0] = 0.19125;
	materialAmbient[1] = 0.0735;
	materialAmbient[2] = 0.0225;
	materialAmbient[3] = 1.0f;

	materialDiffuse[0] = 0.7038;
	materialDiffuse[1] = 0.27048;
	materialDiffuse[2] = 0.0828;
	materialDiffuse[3] = 1.0f;


	materialSpecular[0] = 0.256777;
	materialSpecular[1] = 0.137622;
	materialSpecular[2] = 0.086014;
	materialSpecular[3] = 1.0f;


	materialShininess = 0.1 * 128;

	//transformations

	worldMatrix = XMMatrixIdentity();

	viewMatrix = XMMatrixIdentity();

	wvpMatrix = XMMatrixIdentity();

	worldMatrix = XMMatrixTranslation(0.0f, 0.0f, 1.5f);

	ZeroMemory((void*)&constantBuffer, sizeof(CBUFFER));

	constantBuffer.WorldMatrix = worldMatrix;
	constantBuffer.ViewMatrix = viewMatrix;
	constantBuffer.ProjectionMatrix = perspectiveProjectionMatrix;

	if (bLightingEnabled == TRUE)
	{
		constantBuffer.KeyPress = 1;

		constantBuffer.LightAmbient = XMVectorSet(lightAmbient[0], lightAmbient[1], lightAmbient[2], 0.0f);

		constantBuffer.LightDiffuse = XMVectorSet(lightDiffuse[0], lightDiffuse[1], lightDiffuse[2], 0.0f);

		constantBuffer.LightSpecular = XMVectorSet(lightSpecular[0], lightSpecular[1], lightSpecular[2], 0.0f);

		constantBuffer.LightPosition = XMVectorSet(lightPosition[0], lightPosition[1], lightPosition[2], lightPosition[3]);

		//material 
		constantBuffer.MaterialAmbient = XMVectorSet(materialAmbient[0], materialAmbient[1], materialAmbient[2], 0.0f);
		constantBuffer.MaterialDiffuse = XMVectorSet(materialDiffuse[0], materialDiffuse[1], materialDiffuse[2], 0.0f);
		constantBuffer.MaterialSpecular = XMVectorSet(materialSpecular[0], materialSpecular[1], materialSpecular[2], 0.0f);

		constantBuffer.MaterialShininess = materialShininess;

	}
	else
	{
		constantBuffer.KeyPress = 0;
	}


	gpID3D11DeviceContext->UpdateSubresource(gpID3D11Buffer_ConstantBuffer, 0, NULL, &constantBuffer, 0, 0);   //glUniformmatrix3fv in OpenGL


	//set position buffer into the pipeline here  

	stride = (sizeof(float) * 3);

	offset = 0;

	gpID3D11DeviceContext->IASetVertexBuffers(0, 1, &gpID3D11Buffer_PositionBuffer, &stride, &offset);

	//set Normal buffer into the pipeline here 

	stride = (sizeof(float) * 3);

	offset = 0;

	gpID3D11DeviceContext->IASetVertexBuffers(1, 1, &gpID3D11Buffer_NormalBuffer, &stride, &offset);

	// Set the index buffer
	gpID3D11DeviceContext->IASetIndexBuffer(gpID3D11Buffer_IndexBuffer, DXGI_FORMAT_R16_UINT, 0); // Using R16_UINT as you have short indices

	//set primitive geometry

	gpID3D11DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);


	//draw the geometry

	// fill and set viewport structure here

	ZeroMemory((void*)&d3dViewPort, sizeof(D3D11_VIEWPORT));

	d3dViewPort.TopLeftX = horizontalDistance;

	d3dViewPort.TopLeftY = verticalDistance * 3.0;

	d3dViewPort.Width = horizontalDistance;

	d3dViewPort.Height = verticalDistance;

	d3dViewPort.MinDepth = 0.0f;

	d3dViewPort.MaxDepth = 1.0f;

	// set above details into pipeline

	gpID3D11DeviceContext->RSSetViewports(1, &d3dViewPort);
	// draw
	gpID3D11DeviceContext->DrawIndexed(gNumElements, 0, 0);

	//second column fifth sphere

	materialAmbient[0] = 0.24725;
	materialAmbient[1] = 0.1995;
	materialAmbient[2] = 0.0745;
	materialAmbient[3] = 1.0f;

	materialDiffuse[0] = 0.75164;
	materialDiffuse[1] = 0.60648;
	materialDiffuse[2] = 0.22648;
	materialDiffuse[3] = 1.0f;


	materialSpecular[0] = 0.628281;
	materialSpecular[1] = 0.555802;
	materialSpecular[2] = 0.366065;
	materialSpecular[3] = 1.0f;


	materialShininess = 0.4 * 128;

	//transformations

	worldMatrix = XMMatrixIdentity();

	viewMatrix = XMMatrixIdentity();

	wvpMatrix = XMMatrixIdentity();

	worldMatrix = XMMatrixTranslation(0.0f, 0.0f, 1.5f);

	ZeroMemory((void*)&constantBuffer, sizeof(CBUFFER));

	constantBuffer.WorldMatrix = worldMatrix;
	constantBuffer.ViewMatrix = viewMatrix;
	constantBuffer.ProjectionMatrix = perspectiveProjectionMatrix;

	if (bLightingEnabled == TRUE)
	{
		constantBuffer.KeyPress = 1;

		constantBuffer.LightAmbient = XMVectorSet(lightAmbient[0], lightAmbient[1], lightAmbient[2], 0.0f);

		constantBuffer.LightDiffuse = XMVectorSet(lightDiffuse[0], lightDiffuse[1], lightDiffuse[2], 0.0f);

		constantBuffer.LightSpecular = XMVectorSet(lightSpecular[0], lightSpecular[1], lightSpecular[2], 0.0f);

		constantBuffer.LightPosition = XMVectorSet(lightPosition[0], lightPosition[1], lightPosition[2], lightPosition[3]);

		//material 
		constantBuffer.MaterialAmbient = XMVectorSet(materialAmbient[0], materialAmbient[1], materialAmbient[2], 0.0f);
		constantBuffer.MaterialDiffuse = XMVectorSet(materialDiffuse[0], materialDiffuse[1], materialDiffuse[2], 0.0f);
		constantBuffer.MaterialSpecular = XMVectorSet(materialSpecular[0], materialSpecular[1], materialSpecular[2], 0.0f);

		constantBuffer.MaterialShininess = materialShininess;

	}
	else
	{
		constantBuffer.KeyPress = 0;
	}


	gpID3D11DeviceContext->UpdateSubresource(gpID3D11Buffer_ConstantBuffer, 0, NULL, &constantBuffer, 0, 0);   //glUniformmatrix3fv in OpenGL


	//set position buffer into the pipeline here  

	stride = (sizeof(float) * 3);

	offset = 0;

	gpID3D11DeviceContext->IASetVertexBuffers(0, 1, &gpID3D11Buffer_PositionBuffer, &stride, &offset);

	//set Normal buffer into the pipeline here 

	stride = (sizeof(float) * 3);

	offset = 0;

	gpID3D11DeviceContext->IASetVertexBuffers(1, 1, &gpID3D11Buffer_NormalBuffer, &stride, &offset);

	// Set the index buffer
	gpID3D11DeviceContext->IASetIndexBuffer(gpID3D11Buffer_IndexBuffer, DXGI_FORMAT_R16_UINT, 0); // Using R16_UINT as you have short indices

	//set primitive geometry

	gpID3D11DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);


	//draw the geometry

	// fill and set viewport structure here

	ZeroMemory((void*)&d3dViewPort, sizeof(D3D11_VIEWPORT));

	d3dViewPort.TopLeftX = horizontalDistance;

	d3dViewPort.TopLeftY = verticalDistance * 4.0;

	d3dViewPort.Width = horizontalDistance;

	d3dViewPort.Height = verticalDistance;

	d3dViewPort.MinDepth = 0.0f;

	d3dViewPort.MaxDepth = 1.0f;

	// set above details into pipeline

	gpID3D11DeviceContext->RSSetViewports(1, &d3dViewPort);
	// draw
	gpID3D11DeviceContext->DrawIndexed(gNumElements, 0, 0);

	//second column Sixth sphere

	materialAmbient[0] = 0.19225;
	materialAmbient[1] = 0.19225;
	materialAmbient[2] = 0.19225;
	materialAmbient[3] = 1.0f;

	materialDiffuse[0] = 0.50754;
	materialDiffuse[1] = 0.50754;
	materialDiffuse[2] = 0.50754;
	materialDiffuse[3] = 1.0f;


	materialSpecular[0] = 0.508273;
	materialSpecular[1] = 0.508273;
	materialSpecular[2] = 0.508273;
	materialSpecular[3] = 1.0f;


	materialShininess = 0.4 * 128;



	//transformations

	worldMatrix = XMMatrixIdentity();

	viewMatrix = XMMatrixIdentity();

	wvpMatrix = XMMatrixIdentity();

	worldMatrix = XMMatrixTranslation(0.0f, 0.0f, 1.5f);

	ZeroMemory((void*)&constantBuffer, sizeof(CBUFFER));

	constantBuffer.WorldMatrix = worldMatrix;
	constantBuffer.ViewMatrix = viewMatrix;
	constantBuffer.ProjectionMatrix = perspectiveProjectionMatrix;

	if (bLightingEnabled == TRUE)
	{
		constantBuffer.KeyPress = 1;

		constantBuffer.LightAmbient = XMVectorSet(lightAmbient[0], lightAmbient[1], lightAmbient[2], 0.0f);

		constantBuffer.LightDiffuse = XMVectorSet(lightDiffuse[0], lightDiffuse[1], lightDiffuse[2], 0.0f);

		constantBuffer.LightSpecular = XMVectorSet(lightSpecular[0], lightSpecular[1], lightSpecular[2], 0.0f);

		constantBuffer.LightPosition = XMVectorSet(lightPosition[0], lightPosition[1], lightPosition[2], lightPosition[3]);

		//material 
		constantBuffer.MaterialAmbient = XMVectorSet(materialAmbient[0], materialAmbient[1], materialAmbient[2], 0.0f);
		constantBuffer.MaterialDiffuse = XMVectorSet(materialDiffuse[0], materialDiffuse[1], materialDiffuse[2], 0.0f);
		constantBuffer.MaterialSpecular = XMVectorSet(materialSpecular[0], materialSpecular[1], materialSpecular[2], 0.0f);

		constantBuffer.MaterialShininess = materialShininess;

	}
	else
	{
		constantBuffer.KeyPress = 0;
	}


	gpID3D11DeviceContext->UpdateSubresource(gpID3D11Buffer_ConstantBuffer, 0, NULL, &constantBuffer, 0, 0);   //glUniformmatrix3fv in OpenGL


	//set position buffer into the pipeline here  

	stride = (sizeof(float) * 3);

	offset = 0;

	gpID3D11DeviceContext->IASetVertexBuffers(0, 1, &gpID3D11Buffer_PositionBuffer, &stride, &offset);

	//set Normal buffer into the pipeline here 

	stride = (sizeof(float) * 3);

	offset = 0;

	gpID3D11DeviceContext->IASetVertexBuffers(1, 1, &gpID3D11Buffer_NormalBuffer, &stride, &offset);

	// Set the index buffer
	gpID3D11DeviceContext->IASetIndexBuffer(gpID3D11Buffer_IndexBuffer, DXGI_FORMAT_R16_UINT, 0); // Using R16_UINT as you have short indices

	//set primitive geometry

	gpID3D11DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);


	//draw the geometry

	// fill and set viewport structure here

	ZeroMemory((void*)&d3dViewPort, sizeof(D3D11_VIEWPORT));

	d3dViewPort.TopLeftX = horizontalDistance;

	d3dViewPort.TopLeftY = verticalDistance * 5.0;

	d3dViewPort.Width = horizontalDistance;

	d3dViewPort.Height = verticalDistance;

	d3dViewPort.MinDepth = 0.0f;

	d3dViewPort.MaxDepth = 1.0f;

	// set above details into pipeline

	gpID3D11DeviceContext->RSSetViewports(1, &d3dViewPort);
	// draw
	gpID3D11DeviceContext->DrawIndexed(gNumElements, 0, 0);

	//Third column First sphere

	materialAmbient[0] = 0.0f;
	materialAmbient[1] = 0.0f;
	materialAmbient[2] = 0.0f;
	materialAmbient[3] = 1.0f;

	materialDiffuse[0] = 0.01;
	materialDiffuse[1] = 0.01;
	materialDiffuse[2] = 0.01;
	materialDiffuse[3] = 1.0f;


	materialSpecular[0] = 0.5;
	materialSpecular[1] = 0.5;
	materialSpecular[2] = 0.5;
	materialSpecular[3] = 1.0f;


	materialShininess = 0.25 * 128;

	//transformations

	worldMatrix = XMMatrixIdentity();

	viewMatrix = XMMatrixIdentity();

	wvpMatrix = XMMatrixIdentity();

	worldMatrix = XMMatrixTranslation(0.0f, 0.0f, 1.5f);

	ZeroMemory((void*)&constantBuffer, sizeof(CBUFFER));

	constantBuffer.WorldMatrix = worldMatrix;
	constantBuffer.ViewMatrix = viewMatrix;
	constantBuffer.ProjectionMatrix = perspectiveProjectionMatrix;

	if (bLightingEnabled == TRUE)
	{
		constantBuffer.KeyPress = 1;

		constantBuffer.LightAmbient = XMVectorSet(lightAmbient[0], lightAmbient[1], lightAmbient[2], 0.0f);

		constantBuffer.LightDiffuse = XMVectorSet(lightDiffuse[0], lightDiffuse[1], lightDiffuse[2], 0.0f);

		constantBuffer.LightSpecular = XMVectorSet(lightSpecular[0], lightSpecular[1], lightSpecular[2], 0.0f);

		constantBuffer.LightPosition = XMVectorSet(lightPosition[0], lightPosition[1], lightPosition[2], lightPosition[3]);

		//material 
		constantBuffer.MaterialAmbient = XMVectorSet(materialAmbient[0], materialAmbient[1], materialAmbient[2], 0.0f);
		constantBuffer.MaterialDiffuse = XMVectorSet(materialDiffuse[0], materialDiffuse[1], materialDiffuse[2], 0.0f);
		constantBuffer.MaterialSpecular = XMVectorSet(materialSpecular[0], materialSpecular[1], materialSpecular[2], 0.0f);

		constantBuffer.MaterialShininess = materialShininess;

	}
	else
	{
		constantBuffer.KeyPress = 0;
	}


	gpID3D11DeviceContext->UpdateSubresource(gpID3D11Buffer_ConstantBuffer, 0, NULL, &constantBuffer, 0, 0);   //glUniformmatrix3fv in OpenGL


	//set position buffer into the pipeline here  

	stride = (sizeof(float) * 3);

	offset = 0;

	gpID3D11DeviceContext->IASetVertexBuffers(0, 1, &gpID3D11Buffer_PositionBuffer, &stride, &offset);

	//set Normal buffer into the pipeline here 

	stride = (sizeof(float) * 3);

	offset = 0;

	gpID3D11DeviceContext->IASetVertexBuffers(1, 1, &gpID3D11Buffer_NormalBuffer, &stride, &offset);

	// Set the index buffer
	gpID3D11DeviceContext->IASetIndexBuffer(gpID3D11Buffer_IndexBuffer, DXGI_FORMAT_R16_UINT, 0); // Using R16_UINT as you have short indices

	//set primitive geometry

	gpID3D11DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);


	//draw the geometry

	// fill and set viewport structure here

	ZeroMemory((void*)&d3dViewPort, sizeof(D3D11_VIEWPORT));

	d3dViewPort.TopLeftX = horizontalDistance * 2.0;

	d3dViewPort.TopLeftY = 0.0f;

	d3dViewPort.Width = horizontalDistance;

	d3dViewPort.Height = verticalDistance;

	d3dViewPort.MinDepth = 0.0f;

	d3dViewPort.MaxDepth = 1.0f;

	// set above details into pipeline

	gpID3D11DeviceContext->RSSetViewports(1, &d3dViewPort);
	// draw
	gpID3D11DeviceContext->DrawIndexed(gNumElements, 0, 0);

	//Third column Second sphere

	materialAmbient[0] = 0.0;
	materialAmbient[1] = 0.1;
	materialAmbient[2] = 0.06;
	materialAmbient[3] = 1.0f;

	materialDiffuse[0] = 0.0;
	materialDiffuse[1] = 0.50980392;
	materialDiffuse[2] = 0.50980392;
	materialDiffuse[3] = 1.0f;


	materialSpecular[0] = 0.50196078;
	materialSpecular[1] = 0.50196078;
	materialSpecular[2] = 0.50196078;
	materialSpecular[3] = 1.0f;


	materialShininess = 0.25 * 128;

	//transformations

	worldMatrix = XMMatrixIdentity();

	viewMatrix = XMMatrixIdentity();

	wvpMatrix = XMMatrixIdentity();

	worldMatrix = XMMatrixTranslation(0.0f, 0.0f, 1.5f);

	ZeroMemory((void*)&constantBuffer, sizeof(CBUFFER));

	constantBuffer.WorldMatrix = worldMatrix;
	constantBuffer.ViewMatrix = viewMatrix;
	constantBuffer.ProjectionMatrix = perspectiveProjectionMatrix;

	if (bLightingEnabled == TRUE)
	{
		constantBuffer.KeyPress = 1;

		constantBuffer.LightAmbient = XMVectorSet(lightAmbient[0], lightAmbient[1], lightAmbient[2], 0.0f);

		constantBuffer.LightDiffuse = XMVectorSet(lightDiffuse[0], lightDiffuse[1], lightDiffuse[2], 0.0f);

		constantBuffer.LightSpecular = XMVectorSet(lightSpecular[0], lightSpecular[1], lightSpecular[2], 0.0f);

		constantBuffer.LightPosition = XMVectorSet(lightPosition[0], lightPosition[1], lightPosition[2], lightPosition[3]);

		//material 
		constantBuffer.MaterialAmbient = XMVectorSet(materialAmbient[0], materialAmbient[1], materialAmbient[2], 0.0f);
		constantBuffer.MaterialDiffuse = XMVectorSet(materialDiffuse[0], materialDiffuse[1], materialDiffuse[2], 0.0f);
		constantBuffer.MaterialSpecular = XMVectorSet(materialSpecular[0], materialSpecular[1], materialSpecular[2], 0.0f);

		constantBuffer.MaterialShininess = materialShininess;

	}
	else
	{
		constantBuffer.KeyPress = 0;
	}


	gpID3D11DeviceContext->UpdateSubresource(gpID3D11Buffer_ConstantBuffer, 0, NULL, &constantBuffer, 0, 0);   //glUniformmatrix3fv in OpenGL


	//set position buffer into the pipeline here  

	stride = (sizeof(float) * 3);

	offset = 0;

	gpID3D11DeviceContext->IASetVertexBuffers(0, 1, &gpID3D11Buffer_PositionBuffer, &stride, &offset);

	//set Normal buffer into the pipeline here 

	stride = (sizeof(float) * 3);

	offset = 0;

	gpID3D11DeviceContext->IASetVertexBuffers(1, 1, &gpID3D11Buffer_NormalBuffer, &stride, &offset);

	// Set the index buffer
	gpID3D11DeviceContext->IASetIndexBuffer(gpID3D11Buffer_IndexBuffer, DXGI_FORMAT_R16_UINT, 0); // Using R16_UINT as you have short indices

	//set primitive geometry

	gpID3D11DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);


	//draw the geometry

	// fill and set viewport structure here

	ZeroMemory((void*)&d3dViewPort, sizeof(D3D11_VIEWPORT));

	d3dViewPort.TopLeftX = horizontalDistance * 2.0;

	d3dViewPort.TopLeftY = verticalDistance;

	d3dViewPort.Width = horizontalDistance;

	d3dViewPort.Height = verticalDistance;

	d3dViewPort.MinDepth = 0.0f;

	d3dViewPort.MaxDepth = 1.0f;

	// set above details into pipeline

	gpID3D11DeviceContext->RSSetViewports(1, &d3dViewPort);
	// draw
	gpID3D11DeviceContext->DrawIndexed(gNumElements, 0, 0);

	//Third column Third sphere

	materialAmbient[0] = 0.0;
	materialAmbient[1] = 0.0;
	materialAmbient[2] = 0.0;
	materialAmbient[3] = 1.0f;

	materialDiffuse[0] = 0.1;
	materialDiffuse[1] = 0.35;
	materialDiffuse[2] = 0.1;
	materialDiffuse[3] = 1.0f;


	materialSpecular[0] = 0.45;
	materialSpecular[1] = 0.55;
	materialSpecular[2] = 0.45;
	materialSpecular[3] = 1.0f;

	materialShininess = 0.25 * 128;

	//transformations

	worldMatrix = XMMatrixIdentity();

	viewMatrix = XMMatrixIdentity();

	wvpMatrix = XMMatrixIdentity();

	worldMatrix = XMMatrixTranslation(0.0f, 0.0f, 1.5f);

	ZeroMemory((void*)&constantBuffer, sizeof(CBUFFER));

	constantBuffer.WorldMatrix = worldMatrix;
	constantBuffer.ViewMatrix = viewMatrix;
	constantBuffer.ProjectionMatrix = perspectiveProjectionMatrix;

	if (bLightingEnabled == TRUE)
	{
		constantBuffer.KeyPress = 1;

		constantBuffer.LightAmbient = XMVectorSet(lightAmbient[0], lightAmbient[1], lightAmbient[2], 0.0f);

		constantBuffer.LightDiffuse = XMVectorSet(lightDiffuse[0], lightDiffuse[1], lightDiffuse[2], 0.0f);

		constantBuffer.LightSpecular = XMVectorSet(lightSpecular[0], lightSpecular[1], lightSpecular[2], 0.0f);

		constantBuffer.LightPosition = XMVectorSet(lightPosition[0], lightPosition[1], lightPosition[2], lightPosition[3]);

		//material 
		constantBuffer.MaterialAmbient = XMVectorSet(materialAmbient[0], materialAmbient[1], materialAmbient[2], 0.0f);
		constantBuffer.MaterialDiffuse = XMVectorSet(materialDiffuse[0], materialDiffuse[1], materialDiffuse[2], 0.0f);
		constantBuffer.MaterialSpecular = XMVectorSet(materialSpecular[0], materialSpecular[1], materialSpecular[2], 0.0f);

		constantBuffer.MaterialShininess = materialShininess;

	}
	else
	{
		constantBuffer.KeyPress = 0;
	}


	gpID3D11DeviceContext->UpdateSubresource(gpID3D11Buffer_ConstantBuffer, 0, NULL, &constantBuffer, 0, 0);   //glUniformmatrix3fv in OpenGL


	//set position buffer into the pipeline here  

	stride = (sizeof(float) * 3);

	offset = 0;

	gpID3D11DeviceContext->IASetVertexBuffers(0, 1, &gpID3D11Buffer_PositionBuffer, &stride, &offset);

	//set Normal buffer into the pipeline here 

	stride = (sizeof(float) * 3);

	offset = 0;

	gpID3D11DeviceContext->IASetVertexBuffers(1, 1, &gpID3D11Buffer_NormalBuffer, &stride, &offset);

	// Set the index buffer
	gpID3D11DeviceContext->IASetIndexBuffer(gpID3D11Buffer_IndexBuffer, DXGI_FORMAT_R16_UINT, 0); // Using R16_UINT as you have short indices

	//set primitive geometry

	gpID3D11DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);


	//draw the geometry

	// fill and set viewport structure here

	ZeroMemory((void*)&d3dViewPort, sizeof(D3D11_VIEWPORT));

	d3dViewPort.TopLeftX = horizontalDistance * 2.0;

	d3dViewPort.TopLeftY = verticalDistance * 2.0;

	d3dViewPort.Width = horizontalDistance;

	d3dViewPort.Height = verticalDistance;

	d3dViewPort.MinDepth = 0.0f;

	d3dViewPort.MaxDepth = 1.0f;

	// set above details into pipeline

	gpID3D11DeviceContext->RSSetViewports(1, &d3dViewPort);
	// draw
	gpID3D11DeviceContext->DrawIndexed(gNumElements, 0, 0);

	//Third column fourth sphere

	materialAmbient[0] = 0.0;
	materialAmbient[1] = 0.0;
	materialAmbient[2] = 0.0;
	materialAmbient[3] = 1.0f;

	materialDiffuse[0] = 0.5;
	materialDiffuse[1] = 0.0;
	materialDiffuse[2] = 0.0;
	materialDiffuse[3] = 1.0f;


	materialSpecular[0] = 0.7;
	materialSpecular[1] = 0.6;
	materialSpecular[2] = 0.6;
	materialSpecular[3] = 1.0f;

	materialShininess = 0.25 * 128;


	//transformations

	worldMatrix = XMMatrixIdentity();

	viewMatrix = XMMatrixIdentity();

	wvpMatrix = XMMatrixIdentity();

	worldMatrix = XMMatrixTranslation(0.0f, 0.0f, 1.5f);

	ZeroMemory((void*)&constantBuffer, sizeof(CBUFFER));

	constantBuffer.WorldMatrix = worldMatrix;
	constantBuffer.ViewMatrix = viewMatrix;
	constantBuffer.ProjectionMatrix = perspectiveProjectionMatrix;

	if (bLightingEnabled == TRUE)
	{
		constantBuffer.KeyPress = 1;

		constantBuffer.LightAmbient = XMVectorSet(lightAmbient[0], lightAmbient[1], lightAmbient[2], 0.0f);

		constantBuffer.LightDiffuse = XMVectorSet(lightDiffuse[0], lightDiffuse[1], lightDiffuse[2], 0.0f);

		constantBuffer.LightSpecular = XMVectorSet(lightSpecular[0], lightSpecular[1], lightSpecular[2], 0.0f);

		constantBuffer.LightPosition = XMVectorSet(lightPosition[0], lightPosition[1], lightPosition[2], lightPosition[3]);

		//material 
		constantBuffer.MaterialAmbient = XMVectorSet(materialAmbient[0], materialAmbient[1], materialAmbient[2], 0.0f);
		constantBuffer.MaterialDiffuse = XMVectorSet(materialDiffuse[0], materialDiffuse[1], materialDiffuse[2], 0.0f);
		constantBuffer.MaterialSpecular = XMVectorSet(materialSpecular[0], materialSpecular[1], materialSpecular[2], 0.0f);

		constantBuffer.MaterialShininess = materialShininess;

	}
	else
	{
		constantBuffer.KeyPress = 0;
	}


	gpID3D11DeviceContext->UpdateSubresource(gpID3D11Buffer_ConstantBuffer, 0, NULL, &constantBuffer, 0, 0);   //glUniformmatrix3fv in OpenGL


	//set position buffer into the pipeline here  

	stride = (sizeof(float) * 3);

	offset = 0;

	gpID3D11DeviceContext->IASetVertexBuffers(0, 1, &gpID3D11Buffer_PositionBuffer, &stride, &offset);

	//set Normal buffer into the pipeline here 

	stride = (sizeof(float) * 3);

	offset = 0;

	gpID3D11DeviceContext->IASetVertexBuffers(1, 1, &gpID3D11Buffer_NormalBuffer, &stride, &offset);

	// Set the index buffer
	gpID3D11DeviceContext->IASetIndexBuffer(gpID3D11Buffer_IndexBuffer, DXGI_FORMAT_R16_UINT, 0); // Using R16_UINT as you have short indices

	//set primitive geometry

	gpID3D11DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);


	//draw the geometry

	// fill and set viewport structure here

	ZeroMemory((void*)&d3dViewPort, sizeof(D3D11_VIEWPORT));

	d3dViewPort.TopLeftX = horizontalDistance * 2.0;

	d3dViewPort.TopLeftY = verticalDistance * 3.0;

	d3dViewPort.Width = horizontalDistance;

	d3dViewPort.Height = verticalDistance;

	d3dViewPort.MinDepth = 0.0f;

	d3dViewPort.MaxDepth = 1.0f;

	// set above details into pipeline

	gpID3D11DeviceContext->RSSetViewports(1, &d3dViewPort);
	// draw
	gpID3D11DeviceContext->DrawIndexed(gNumElements, 0, 0);

	//Third column fifth sphere

	materialAmbient[0] = 0.0;
	materialAmbient[1] = 0.0;
	materialAmbient[2] = 0.0;
	materialAmbient[3] = 1.0f;

	materialDiffuse[0] = 0.55;
	materialDiffuse[1] = 0.55;
	materialDiffuse[2] = 0.55;
	materialDiffuse[3] = 1.0f;


	materialSpecular[0] = 0.70;
	materialSpecular[1] = 0.70;
	materialSpecular[2] = 0.70;
	materialSpecular[3] = 1.0f;

	materialShininess = 0.25 * 128;


	//transformations

	worldMatrix = XMMatrixIdentity();

	viewMatrix = XMMatrixIdentity();

	wvpMatrix = XMMatrixIdentity();

	worldMatrix = XMMatrixTranslation(0.0f, 0.0f, 1.5f);

	ZeroMemory((void*)&constantBuffer, sizeof(CBUFFER));

	constantBuffer.WorldMatrix = worldMatrix;
	constantBuffer.ViewMatrix = viewMatrix;
	constantBuffer.ProjectionMatrix = perspectiveProjectionMatrix;

	if (bLightingEnabled == TRUE)
	{
		constantBuffer.KeyPress = 1;

		constantBuffer.LightAmbient = XMVectorSet(lightAmbient[0], lightAmbient[1], lightAmbient[2], 0.0f);

		constantBuffer.LightDiffuse = XMVectorSet(lightDiffuse[0], lightDiffuse[1], lightDiffuse[2], 0.0f);

		constantBuffer.LightSpecular = XMVectorSet(lightSpecular[0], lightSpecular[1], lightSpecular[2], 0.0f);

		constantBuffer.LightPosition = XMVectorSet(lightPosition[0], lightPosition[1], lightPosition[2], lightPosition[3]);

		//material 
		constantBuffer.MaterialAmbient = XMVectorSet(materialAmbient[0], materialAmbient[1], materialAmbient[2], 0.0f);
		constantBuffer.MaterialDiffuse = XMVectorSet(materialDiffuse[0], materialDiffuse[1], materialDiffuse[2], 0.0f);
		constantBuffer.MaterialSpecular = XMVectorSet(materialSpecular[0], materialSpecular[1], materialSpecular[2], 0.0f);

		constantBuffer.MaterialShininess = materialShininess;

	}
	else
	{
		constantBuffer.KeyPress = 0;
	}


	gpID3D11DeviceContext->UpdateSubresource(gpID3D11Buffer_ConstantBuffer, 0, NULL, &constantBuffer, 0, 0);   //glUniformmatrix3fv in OpenGL


	//set position buffer into the pipeline here  

	stride = (sizeof(float) * 3);

	offset = 0;

	gpID3D11DeviceContext->IASetVertexBuffers(0, 1, &gpID3D11Buffer_PositionBuffer, &stride, &offset);

	//set Normal buffer into the pipeline here 

	stride = (sizeof(float) * 3);

	offset = 0;

	gpID3D11DeviceContext->IASetVertexBuffers(1, 1, &gpID3D11Buffer_NormalBuffer, &stride, &offset);

	// Set the index buffer
	gpID3D11DeviceContext->IASetIndexBuffer(gpID3D11Buffer_IndexBuffer, DXGI_FORMAT_R16_UINT, 0); // Using R16_UINT as you have short indices

	//set primitive geometry

	gpID3D11DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);


	//draw the geometry

	// fill and set viewport structure here

	ZeroMemory((void*)&d3dViewPort, sizeof(D3D11_VIEWPORT));

	d3dViewPort.TopLeftX = horizontalDistance * 2.0;

	d3dViewPort.TopLeftY = verticalDistance * 4.0;

	d3dViewPort.Width = horizontalDistance;

	d3dViewPort.Height = verticalDistance;

	d3dViewPort.MinDepth = 0.0f;

	d3dViewPort.MaxDepth = 1.0f;

	// set above details into pipeline

	gpID3D11DeviceContext->RSSetViewports(1, &d3dViewPort);
	// draw
	gpID3D11DeviceContext->DrawIndexed(gNumElements, 0, 0);

	//Third column sixth sphere

	materialAmbient[0] = 0.0;
	materialAmbient[1] = 0.0;
	materialAmbient[2] = 0.0;
	materialAmbient[3] = 1.0f;

	materialDiffuse[0] = 0.5;
	materialDiffuse[1] = 0.5;
	materialDiffuse[2] = 0.0;
	materialDiffuse[3] = 1.0f;


	materialSpecular[0] = 0.60;
	materialSpecular[1] = 0.60;
	materialSpecular[2] = 0.50;
	materialSpecular[3] = 1.0f;

	materialShininess = 0.25 * 128;


	//transformations

	worldMatrix = XMMatrixIdentity();

	viewMatrix = XMMatrixIdentity();

	wvpMatrix = XMMatrixIdentity();

	worldMatrix = XMMatrixTranslation(0.0f, 0.0f, 1.5f);

	ZeroMemory((void*)&constantBuffer, sizeof(CBUFFER));

	constantBuffer.WorldMatrix = worldMatrix;
	constantBuffer.ViewMatrix = viewMatrix;
	constantBuffer.ProjectionMatrix = perspectiveProjectionMatrix;

	if (bLightingEnabled == TRUE)
	{
		constantBuffer.KeyPress = 1;

		constantBuffer.LightAmbient = XMVectorSet(lightAmbient[0], lightAmbient[1], lightAmbient[2], 0.0f);

		constantBuffer.LightDiffuse = XMVectorSet(lightDiffuse[0], lightDiffuse[1], lightDiffuse[2], 0.0f);

		constantBuffer.LightSpecular = XMVectorSet(lightSpecular[0], lightSpecular[1], lightSpecular[2], 0.0f);

		constantBuffer.LightPosition = XMVectorSet(lightPosition[0], lightPosition[1], lightPosition[2], lightPosition[3]);

		//material 
		constantBuffer.MaterialAmbient = XMVectorSet(materialAmbient[0], materialAmbient[1], materialAmbient[2], 0.0f);
		constantBuffer.MaterialDiffuse = XMVectorSet(materialDiffuse[0], materialDiffuse[1], materialDiffuse[2], 0.0f);
		constantBuffer.MaterialSpecular = XMVectorSet(materialSpecular[0], materialSpecular[1], materialSpecular[2], 0.0f);

		constantBuffer.MaterialShininess = materialShininess;

	}
	else
	{
		constantBuffer.KeyPress = 0;
	}


	gpID3D11DeviceContext->UpdateSubresource(gpID3D11Buffer_ConstantBuffer, 0, NULL, &constantBuffer, 0, 0);   //glUniformmatrix3fv in OpenGL


	//set position buffer into the pipeline here  

	stride = (sizeof(float) * 3);

	offset = 0;

	gpID3D11DeviceContext->IASetVertexBuffers(0, 1, &gpID3D11Buffer_PositionBuffer, &stride, &offset);

	//set Normal buffer into the pipeline here 

	stride = (sizeof(float) * 3);

	offset = 0;

	gpID3D11DeviceContext->IASetVertexBuffers(1, 1, &gpID3D11Buffer_NormalBuffer, &stride, &offset);

	// Set the index buffer
	gpID3D11DeviceContext->IASetIndexBuffer(gpID3D11Buffer_IndexBuffer, DXGI_FORMAT_R16_UINT, 0); // Using R16_UINT as you have short indices

	//set primitive geometry

	gpID3D11DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);


	//draw the geometry

	// fill and set viewport structure here

	ZeroMemory((void*)&d3dViewPort, sizeof(D3D11_VIEWPORT));

	d3dViewPort.TopLeftX = horizontalDistance * 2.0;

	d3dViewPort.TopLeftY = verticalDistance * 5.0;

	d3dViewPort.Width = horizontalDistance;

	d3dViewPort.Height = verticalDistance;

	d3dViewPort.MinDepth = 0.0f;

	d3dViewPort.MaxDepth = 1.0f;

	// set above details into pipeline

	gpID3D11DeviceContext->RSSetViewports(1, &d3dViewPort);
	// draw
	gpID3D11DeviceContext->DrawIndexed(gNumElements, 0, 0);

	//Fourth Column First sphere

	materialAmbient[0] = 0.02;
	materialAmbient[1] = 0.02;
	materialAmbient[2] = 0.02;
	materialAmbient[3] = 1.0f;

	materialDiffuse[0] = 0.01;
	materialDiffuse[1] = 0.01;
	materialDiffuse[2] = 0.01;
	materialDiffuse[3] = 1.0f;


	materialSpecular[0] = 0.4;
	materialSpecular[1] = 0.4;
	materialSpecular[2] = 0.4;
	materialSpecular[3] = 1.0f;

	materialShininess = 0.078125 * 128;

	//transformations

	worldMatrix = XMMatrixIdentity();

	viewMatrix = XMMatrixIdentity();

	wvpMatrix = XMMatrixIdentity();

	worldMatrix = XMMatrixTranslation(0.0f, 0.0f, 1.5f);

	ZeroMemory((void*)&constantBuffer, sizeof(CBUFFER));

	constantBuffer.WorldMatrix = worldMatrix;
	constantBuffer.ViewMatrix = viewMatrix;
	constantBuffer.ProjectionMatrix = perspectiveProjectionMatrix;

	if (bLightingEnabled == TRUE)
	{
		constantBuffer.KeyPress = 1;

		constantBuffer.LightAmbient = XMVectorSet(lightAmbient[0], lightAmbient[1], lightAmbient[2], 0.0f);

		constantBuffer.LightDiffuse = XMVectorSet(lightDiffuse[0], lightDiffuse[1], lightDiffuse[2], 0.0f);

		constantBuffer.LightSpecular = XMVectorSet(lightSpecular[0], lightSpecular[1], lightSpecular[2], 0.0f);

		constantBuffer.LightPosition = XMVectorSet(lightPosition[0], lightPosition[1], lightPosition[2], lightPosition[3]);

		//material 
		constantBuffer.MaterialAmbient = XMVectorSet(materialAmbient[0], materialAmbient[1], materialAmbient[2], 0.0f);
		constantBuffer.MaterialDiffuse = XMVectorSet(materialDiffuse[0], materialDiffuse[1], materialDiffuse[2], 0.0f);
		constantBuffer.MaterialSpecular = XMVectorSet(materialSpecular[0], materialSpecular[1], materialSpecular[2], 0.0f);

		constantBuffer.MaterialShininess = materialShininess;

	}
	else
	{
		constantBuffer.KeyPress = 0;
	}


	gpID3D11DeviceContext->UpdateSubresource(gpID3D11Buffer_ConstantBuffer, 0, NULL, &constantBuffer, 0, 0);   //glUniformmatrix3fv in OpenGL


	//set position buffer into the pipeline here  

	stride = (sizeof(float) * 3);

	offset = 0;

	gpID3D11DeviceContext->IASetVertexBuffers(0, 1, &gpID3D11Buffer_PositionBuffer, &stride, &offset);

	//set Normal buffer into the pipeline here 

	stride = (sizeof(float) * 3);

	offset = 0;

	gpID3D11DeviceContext->IASetVertexBuffers(1, 1, &gpID3D11Buffer_NormalBuffer, &stride, &offset);

	// Set the index buffer
	gpID3D11DeviceContext->IASetIndexBuffer(gpID3D11Buffer_IndexBuffer, DXGI_FORMAT_R16_UINT, 0); // Using R16_UINT as you have short indices

	//set primitive geometry

	gpID3D11DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);


	//draw the geometry

	// fill and set viewport structure here

	ZeroMemory((void*)&d3dViewPort, sizeof(D3D11_VIEWPORT));

	d3dViewPort.TopLeftX = horizontalDistance * 3.0;

	d3dViewPort.TopLeftY = verticalDistance * 0.0;

	d3dViewPort.Width = horizontalDistance;

	d3dViewPort.Height = verticalDistance;

	d3dViewPort.MinDepth = 0.0f;

	d3dViewPort.MaxDepth = 1.0f;

	// set above details into pipeline

	gpID3D11DeviceContext->RSSetViewports(1, &d3dViewPort);
	// draw
	gpID3D11DeviceContext->DrawIndexed(gNumElements, 0, 0);

	//Fourth Column Second sphere

	materialAmbient[0] = 0.0;
	materialAmbient[1] = 0.05;
	materialAmbient[2] = 0.05;
	materialAmbient[3] = 1.0f;

	materialDiffuse[0] = 0.4;
	materialDiffuse[1] = 0.5;
	materialDiffuse[2] = 0.5;
	materialDiffuse[3] = 1.0f;


	materialSpecular[0] = 0.04;
	materialSpecular[1] = 0.7;
	materialSpecular[2] = 0.7;
	materialSpecular[3] = 1.0f;

	materialShininess = 0.078125 * 128;

	//transformations

	worldMatrix = XMMatrixIdentity();

	viewMatrix = XMMatrixIdentity();

	wvpMatrix = XMMatrixIdentity();

	worldMatrix = XMMatrixTranslation(0.0f, 0.0f, 1.5f);

	ZeroMemory((void*)&constantBuffer, sizeof(CBUFFER));

	constantBuffer.WorldMatrix = worldMatrix;
	constantBuffer.ViewMatrix = viewMatrix;
	constantBuffer.ProjectionMatrix = perspectiveProjectionMatrix;

	if (bLightingEnabled == TRUE)
	{
		constantBuffer.KeyPress = 1;

		constantBuffer.LightAmbient = XMVectorSet(lightAmbient[0], lightAmbient[1], lightAmbient[2], 0.0f);

		constantBuffer.LightDiffuse = XMVectorSet(lightDiffuse[0], lightDiffuse[1], lightDiffuse[2], 0.0f);

		constantBuffer.LightSpecular = XMVectorSet(lightSpecular[0], lightSpecular[1], lightSpecular[2], 0.0f);

		constantBuffer.LightPosition = XMVectorSet(lightPosition[0], lightPosition[1], lightPosition[2], lightPosition[3]);

		//material 
		constantBuffer.MaterialAmbient = XMVectorSet(materialAmbient[0], materialAmbient[1], materialAmbient[2], 0.0f);
		constantBuffer.MaterialDiffuse = XMVectorSet(materialDiffuse[0], materialDiffuse[1], materialDiffuse[2], 0.0f);
		constantBuffer.MaterialSpecular = XMVectorSet(materialSpecular[0], materialSpecular[1], materialSpecular[2], 0.0f);

		constantBuffer.MaterialShininess = materialShininess;

	}
	else
	{
		constantBuffer.KeyPress = 0;
	}


	gpID3D11DeviceContext->UpdateSubresource(gpID3D11Buffer_ConstantBuffer, 0, NULL, &constantBuffer, 0, 0);   //glUniformmatrix3fv in OpenGL


	//set position buffer into the pipeline here  

	stride = (sizeof(float) * 3);

	offset = 0;

	gpID3D11DeviceContext->IASetVertexBuffers(0, 1, &gpID3D11Buffer_PositionBuffer, &stride, &offset);

	//set Normal buffer into the pipeline here 

	stride = (sizeof(float) * 3);

	offset = 0;

	gpID3D11DeviceContext->IASetVertexBuffers(1, 1, &gpID3D11Buffer_NormalBuffer, &stride, &offset);

	// Set the index buffer
	gpID3D11DeviceContext->IASetIndexBuffer(gpID3D11Buffer_IndexBuffer, DXGI_FORMAT_R16_UINT, 0); // Using R16_UINT as you have short indices

	//set primitive geometry

	gpID3D11DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);


	//draw the geometry

	// fill and set viewport structure here

	ZeroMemory((void*)&d3dViewPort, sizeof(D3D11_VIEWPORT));

	d3dViewPort.TopLeftX = horizontalDistance * 3.0;

	d3dViewPort.TopLeftY = verticalDistance * 1.0;

	d3dViewPort.Width = horizontalDistance;

	d3dViewPort.Height = verticalDistance;

	d3dViewPort.MinDepth = 0.0f;

	d3dViewPort.MaxDepth = 1.0f;

	// set above details into pipeline

	gpID3D11DeviceContext->RSSetViewports(1, &d3dViewPort);
	// draw
	gpID3D11DeviceContext->DrawIndexed(gNumElements, 0, 0);

	//Fourth Column Third sphere

	materialAmbient[0] = 0.0;
	materialAmbient[1] = 0.05;
	materialAmbient[2] = 0.0;
	materialAmbient[3] = 1.0f;

	materialDiffuse[0] = 0.4;
	materialDiffuse[1] = 0.5;
	materialDiffuse[2] = 0.4;
	materialDiffuse[3] = 1.0f;

	materialSpecular[0] = 0.04;
	materialSpecular[1] = 0.7;
	materialSpecular[2] = 0.04;
	materialSpecular[3] = 1.0f;

	materialShininess = 0.078125 * 128;

	//transformations

	worldMatrix = XMMatrixIdentity();

	viewMatrix = XMMatrixIdentity();

	wvpMatrix = XMMatrixIdentity();

	worldMatrix = XMMatrixTranslation(0.0f, 0.0f, 1.5f);

	ZeroMemory((void*)&constantBuffer, sizeof(CBUFFER));

	constantBuffer.WorldMatrix = worldMatrix;
	constantBuffer.ViewMatrix = viewMatrix;
	constantBuffer.ProjectionMatrix = perspectiveProjectionMatrix;

	if (bLightingEnabled == TRUE)
	{
		constantBuffer.KeyPress = 1;

		constantBuffer.LightAmbient = XMVectorSet(lightAmbient[0], lightAmbient[1], lightAmbient[2], 0.0f);

		constantBuffer.LightDiffuse = XMVectorSet(lightDiffuse[0], lightDiffuse[1], lightDiffuse[2], 0.0f);

		constantBuffer.LightSpecular = XMVectorSet(lightSpecular[0], lightSpecular[1], lightSpecular[2], 0.0f);

		constantBuffer.LightPosition = XMVectorSet(lightPosition[0], lightPosition[1], lightPosition[2], lightPosition[3]);

		//material 
		constantBuffer.MaterialAmbient = XMVectorSet(materialAmbient[0], materialAmbient[1], materialAmbient[2], 0.0f);
		constantBuffer.MaterialDiffuse = XMVectorSet(materialDiffuse[0], materialDiffuse[1], materialDiffuse[2], 0.0f);
		constantBuffer.MaterialSpecular = XMVectorSet(materialSpecular[0], materialSpecular[1], materialSpecular[2], 0.0f);

		constantBuffer.MaterialShininess = materialShininess;

	}
	else
	{
		constantBuffer.KeyPress = 0;
	}


	gpID3D11DeviceContext->UpdateSubresource(gpID3D11Buffer_ConstantBuffer, 0, NULL, &constantBuffer, 0, 0);   //glUniformmatrix3fv in OpenGL


	//set position buffer into the pipeline here  

	stride = (sizeof(float) * 3);

	offset = 0;

	gpID3D11DeviceContext->IASetVertexBuffers(0, 1, &gpID3D11Buffer_PositionBuffer, &stride, &offset);

	//set Normal buffer into the pipeline here 

	stride = (sizeof(float) * 3);

	offset = 0;

	gpID3D11DeviceContext->IASetVertexBuffers(1, 1, &gpID3D11Buffer_NormalBuffer, &stride, &offset);

	// Set the index buffer
	gpID3D11DeviceContext->IASetIndexBuffer(gpID3D11Buffer_IndexBuffer, DXGI_FORMAT_R16_UINT, 0); // Using R16_UINT as you have short indices

	//set primitive geometry

	gpID3D11DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);


	//draw the geometry

	// fill and set viewport structure here

	ZeroMemory((void*)&d3dViewPort, sizeof(D3D11_VIEWPORT));

	d3dViewPort.TopLeftX = horizontalDistance * 3.0;

	d3dViewPort.TopLeftY = verticalDistance * 2.0;

	d3dViewPort.Width = horizontalDistance;

	d3dViewPort.Height = verticalDistance;

	d3dViewPort.MinDepth = 0.0f;

	d3dViewPort.MaxDepth = 1.0f;

	// set above details into pipeline

	gpID3D11DeviceContext->RSSetViewports(1, &d3dViewPort);
	// draw
	gpID3D11DeviceContext->DrawIndexed(gNumElements, 0, 0);

	//Fourth Column fourth sphere

	materialAmbient[0] = 0.05;
	materialAmbient[1] = 0.0;
	materialAmbient[2] = 0.0;
	materialAmbient[3] = 1.0f;

	materialDiffuse[0] = 0.5;
	materialDiffuse[1] = 0.4;
	materialDiffuse[2] = 0.4;
	materialDiffuse[3] = 1.0f;

	materialSpecular[0] = 0.7;
	materialSpecular[1] = 0.04;
	materialSpecular[2] = 0.04;
	materialSpecular[3] = 1.0f;

	materialShininess = 0.078125 * 128;

	//transformations

	worldMatrix = XMMatrixIdentity();

	viewMatrix = XMMatrixIdentity();

	wvpMatrix = XMMatrixIdentity();

	worldMatrix = XMMatrixTranslation(0.0f, 0.0f, 1.5f);

	ZeroMemory((void*)&constantBuffer, sizeof(CBUFFER));

	constantBuffer.WorldMatrix = worldMatrix;
	constantBuffer.ViewMatrix = viewMatrix;
	constantBuffer.ProjectionMatrix = perspectiveProjectionMatrix;

	if (bLightingEnabled == TRUE)
	{
		constantBuffer.KeyPress = 1;

		constantBuffer.LightAmbient = XMVectorSet(lightAmbient[0], lightAmbient[1], lightAmbient[2], 0.0f);

		constantBuffer.LightDiffuse = XMVectorSet(lightDiffuse[0], lightDiffuse[1], lightDiffuse[2], 0.0f);

		constantBuffer.LightSpecular = XMVectorSet(lightSpecular[0], lightSpecular[1], lightSpecular[2], 0.0f);

		constantBuffer.LightPosition = XMVectorSet(lightPosition[0], lightPosition[1], lightPosition[2], lightPosition[3]);

		//material 
		constantBuffer.MaterialAmbient = XMVectorSet(materialAmbient[0], materialAmbient[1], materialAmbient[2], 0.0f);
		constantBuffer.MaterialDiffuse = XMVectorSet(materialDiffuse[0], materialDiffuse[1], materialDiffuse[2], 0.0f);
		constantBuffer.MaterialSpecular = XMVectorSet(materialSpecular[0], materialSpecular[1], materialSpecular[2], 0.0f);

		constantBuffer.MaterialShininess = materialShininess;

	}
	else
	{
		constantBuffer.KeyPress = 0;
	}


	gpID3D11DeviceContext->UpdateSubresource(gpID3D11Buffer_ConstantBuffer, 0, NULL, &constantBuffer, 0, 0);   //glUniformmatrix3fv in OpenGL


	//set position buffer into the pipeline here  

	stride = (sizeof(float) * 3);

	offset = 0;

	gpID3D11DeviceContext->IASetVertexBuffers(0, 1, &gpID3D11Buffer_PositionBuffer, &stride, &offset);

	//set Normal buffer into the pipeline here 

	stride = (sizeof(float) * 3);

	offset = 0;

	gpID3D11DeviceContext->IASetVertexBuffers(1, 1, &gpID3D11Buffer_NormalBuffer, &stride, &offset);

	// Set the index buffer
	gpID3D11DeviceContext->IASetIndexBuffer(gpID3D11Buffer_IndexBuffer, DXGI_FORMAT_R16_UINT, 0); // Using R16_UINT as you have short indices

	//set primitive geometry

	gpID3D11DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);


	//draw the geometry

	// fill and set viewport structure here

	ZeroMemory((void*)&d3dViewPort, sizeof(D3D11_VIEWPORT));

	d3dViewPort.TopLeftX = horizontalDistance * 3.0;

	d3dViewPort.TopLeftY = verticalDistance * 3.0;

	d3dViewPort.Width = horizontalDistance;

	d3dViewPort.Height = verticalDistance;

	d3dViewPort.MinDepth = 0.0f;

	d3dViewPort.MaxDepth = 1.0f;

	// set above details into pipeline

	gpID3D11DeviceContext->RSSetViewports(1, &d3dViewPort);
	// draw
	gpID3D11DeviceContext->DrawIndexed(gNumElements, 0, 0);

	//Fourth Column fifth sphere

	materialAmbient[0] = 0.05;
	materialAmbient[1] = 0.05;
	materialAmbient[2] = 0.05;
	materialAmbient[3] = 1.0f;

	materialDiffuse[0] = 0.5;
	materialDiffuse[1] = 0.5;
	materialDiffuse[2] = 0.5;
	materialDiffuse[3] = 1.0f;

	materialSpecular[0] = 0.7;
	materialSpecular[1] = 0.7;
	materialSpecular[2] = 0.7;
	materialSpecular[3] = 1.0f;

	materialShininess = 0.078125 * 128;

	//transformations

	worldMatrix = XMMatrixIdentity();

	viewMatrix = XMMatrixIdentity();

	wvpMatrix = XMMatrixIdentity();

	worldMatrix = XMMatrixTranslation(0.0f, 0.0f, 1.5f);

	ZeroMemory((void*)&constantBuffer, sizeof(CBUFFER));

	constantBuffer.WorldMatrix = worldMatrix;
	constantBuffer.ViewMatrix = viewMatrix;
	constantBuffer.ProjectionMatrix = perspectiveProjectionMatrix;

	if (bLightingEnabled == TRUE)
	{
		constantBuffer.KeyPress = 1;

		constantBuffer.LightAmbient = XMVectorSet(lightAmbient[0], lightAmbient[1], lightAmbient[2], 0.0f);

		constantBuffer.LightDiffuse = XMVectorSet(lightDiffuse[0], lightDiffuse[1], lightDiffuse[2], 0.0f);

		constantBuffer.LightSpecular = XMVectorSet(lightSpecular[0], lightSpecular[1], lightSpecular[2], 0.0f);

		constantBuffer.LightPosition = XMVectorSet(lightPosition[0], lightPosition[1], lightPosition[2], lightPosition[3]);

		//material 
		constantBuffer.MaterialAmbient = XMVectorSet(materialAmbient[0], materialAmbient[1], materialAmbient[2], 0.0f);
		constantBuffer.MaterialDiffuse = XMVectorSet(materialDiffuse[0], materialDiffuse[1], materialDiffuse[2], 0.0f);
		constantBuffer.MaterialSpecular = XMVectorSet(materialSpecular[0], materialSpecular[1], materialSpecular[2], 0.0f);

		constantBuffer.MaterialShininess = materialShininess;

	}
	else
	{
		constantBuffer.KeyPress = 0;
	}


	gpID3D11DeviceContext->UpdateSubresource(gpID3D11Buffer_ConstantBuffer, 0, NULL, &constantBuffer, 0, 0);   //glUniformmatrix3fv in OpenGL


	//set position buffer into the pipeline here  

	stride = (sizeof(float) * 3);

	offset = 0;

	gpID3D11DeviceContext->IASetVertexBuffers(0, 1, &gpID3D11Buffer_PositionBuffer, &stride, &offset);

	//set Normal buffer into the pipeline here 

	stride = (sizeof(float) * 3);

	offset = 0;

	gpID3D11DeviceContext->IASetVertexBuffers(1, 1, &gpID3D11Buffer_NormalBuffer, &stride, &offset);

	// Set the index buffer
	gpID3D11DeviceContext->IASetIndexBuffer(gpID3D11Buffer_IndexBuffer, DXGI_FORMAT_R16_UINT, 0); // Using R16_UINT as you have short indices

	//set primitive geometry

	gpID3D11DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);


	//draw the geometry

	// fill and set viewport structure here

	ZeroMemory((void*)&d3dViewPort, sizeof(D3D11_VIEWPORT));

	d3dViewPort.TopLeftX = horizontalDistance * 3.0;

	d3dViewPort.TopLeftY = verticalDistance * 4.0;

	d3dViewPort.Width = horizontalDistance;

	d3dViewPort.Height = verticalDistance;

	d3dViewPort.MinDepth = 0.0f;

	d3dViewPort.MaxDepth = 1.0f;

	// set above details into pipeline

	gpID3D11DeviceContext->RSSetViewports(1, &d3dViewPort);
	// draw
	gpID3D11DeviceContext->DrawIndexed(gNumElements, 0, 0);

	//Fourth Column sixth sphere

	materialAmbient[0] = 0.05;
	materialAmbient[1] = 0.05;
	materialAmbient[2] = 0.0;
	materialAmbient[3] = 1.0f;

	materialDiffuse[0] = 0.5;
	materialDiffuse[1] = 0.5;
	materialDiffuse[2] = 0.4;
	materialDiffuse[3] = 1.0f;

	materialSpecular[0] = 0.7;
	materialSpecular[1] = 0.7;
	materialSpecular[2] = 0.04;
	materialSpecular[3] = 1.0f;


	materialShininess = 0.078125 * 128;

	//transformations

	worldMatrix = XMMatrixIdentity();

	viewMatrix = XMMatrixIdentity();

	wvpMatrix = XMMatrixIdentity();

	worldMatrix = XMMatrixTranslation(0.0f, 0.0f, 1.5f);

	ZeroMemory((void*)&constantBuffer, sizeof(CBUFFER));

	constantBuffer.WorldMatrix = worldMatrix;
	constantBuffer.ViewMatrix = viewMatrix;
	constantBuffer.ProjectionMatrix = perspectiveProjectionMatrix;

	if (bLightingEnabled == TRUE)
	{
		constantBuffer.KeyPress = 1;

		constantBuffer.LightAmbient = XMVectorSet(lightAmbient[0], lightAmbient[1], lightAmbient[2], 0.0f);

		constantBuffer.LightDiffuse = XMVectorSet(lightDiffuse[0], lightDiffuse[1], lightDiffuse[2], 0.0f);

		constantBuffer.LightSpecular = XMVectorSet(lightSpecular[0], lightSpecular[1], lightSpecular[2], 0.0f);

		constantBuffer.LightPosition = XMVectorSet(lightPosition[0], lightPosition[1], lightPosition[2], lightPosition[3]);

		//material 
		constantBuffer.MaterialAmbient = XMVectorSet(materialAmbient[0], materialAmbient[1], materialAmbient[2], 0.0f);
		constantBuffer.MaterialDiffuse = XMVectorSet(materialDiffuse[0], materialDiffuse[1], materialDiffuse[2], 0.0f);
		constantBuffer.MaterialSpecular = XMVectorSet(materialSpecular[0], materialSpecular[1], materialSpecular[2], 0.0f);

		constantBuffer.MaterialShininess = materialShininess;

	}
	else
	{
		constantBuffer.KeyPress = 0;
	}


	gpID3D11DeviceContext->UpdateSubresource(gpID3D11Buffer_ConstantBuffer, 0, NULL, &constantBuffer, 0, 0);   //glUniformmatrix3fv in OpenGL


	//set position buffer into the pipeline here  

	stride = (sizeof(float) * 3);

	offset = 0;

	gpID3D11DeviceContext->IASetVertexBuffers(0, 1, &gpID3D11Buffer_PositionBuffer, &stride, &offset);

	//set Normal buffer into the pipeline here 

	stride = (sizeof(float) * 3);

	offset = 0;

	gpID3D11DeviceContext->IASetVertexBuffers(1, 1, &gpID3D11Buffer_NormalBuffer, &stride, &offset);

	// Set the index buffer
	gpID3D11DeviceContext->IASetIndexBuffer(gpID3D11Buffer_IndexBuffer, DXGI_FORMAT_R16_UINT, 0); // Using R16_UINT as you have short indices

	//set primitive geometry

	gpID3D11DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);


	//draw the geometry

	// fill and set viewport structure here

	ZeroMemory((void*)&d3dViewPort, sizeof(D3D11_VIEWPORT));

	d3dViewPort.TopLeftX = horizontalDistance * 3.0;

	d3dViewPort.TopLeftY = verticalDistance * 5.0;

	d3dViewPort.Width = horizontalDistance;

	d3dViewPort.Height = verticalDistance;

	d3dViewPort.MinDepth = 0.0f;

	d3dViewPort.MaxDepth = 1.0f;

	// set above details into pipeline

	gpID3D11DeviceContext->RSSetViewports(1, &d3dViewPort);
	// draw
	gpID3D11DeviceContext->DrawIndexed(gNumElements, 0, 0);

	// do double buffering by presenting the swapchain

	gpIDXGISwapChain->Present(0, 0);

}

void update(void)
{
	//code

	if (keyPressed == 1)
	{
		if (angleForXRotation < (2 * M_PI))   // rotation around X axis
		{
			lightPosition[0] = 0.0f;
			lightPosition[1] = 45.0f * (float)sin(angleForXRotation);
			lightPosition[2] = 45.0f * (float)cos(angleForXRotation);
			lightPosition[3] = 1.0f;

			angleForXRotation = angleForXRotation + 0.005;
		}

		else
		{
			angleForXRotation = angleForXRotation - (2 * M_PI);
		}

	}

	if (keyPressed == 2)   // rotation around Y axis
	{
		if (angleForYRotation < (2 * M_PI))
		{
			lightPosition[0] = 45.0f * (float)sin(angleForYRotation);
			lightPosition[1] = 0.0f;
			lightPosition[2] = 45.0f * (float)cos(angleForYRotation);
			lightPosition[3] = 1.0f;

			angleForYRotation = angleForYRotation + 0.005;
		}

		else
		{
			angleForYRotation = angleForYRotation - (2 * M_PI);
		}

	}

	if (keyPressed == 3)   // rotation around Z axis
	{
		if (angleForZRotation < (2 * M_PI))
		{
			lightPosition[0] = 45.0f * (float)sin(angleForZRotation);
			lightPosition[1] = 45.0f * (float)cos(angleForZRotation);
			lightPosition[2] = 0.0f;
			lightPosition[3] = 1.0f;

			angleForZRotation = angleForZRotation + 0.005;
		}

		else
		{
			angleForZRotation = angleForZRotation - (2 * M_PI);
		}

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

	if (gpID3D11Buffer_PositionBuffer)
	{
		gpID3D11Buffer_PositionBuffer->Release();
		gpID3D11Buffer_PositionBuffer = NULL;
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

	//// Distroy Window

	//if (ghwnd)
	//{
	//	DestroyWindow(ghwnd);
	//	ghwnd = NULL;

	//}

	//close the log file

	gpFile = fopen(gszLogFileName, "a+");

	fprintf(gpFile, "Log File is Successfully Closed !!\n\n");

	fclose(gpFile);

	


}







