#include <Windows.h>
#include <windowsx.h>
#include <d3d11.h>
#include <d3dx11.h>
#include <d3dx10.h>

//include the Direct3D lirabry file

#pragma comment (lib, "d3d11.lib")
#pragma comment (lib, "d3dx11.lib")
#pragma comment (lib, "d3dx10.lib")


// global declarations

IDXGISwapChain *swapchain;			//the pointer to the swap chain interface
ID3D11Device *dev;					//the pointer to our Direct3D device interface
ID3D11DeviceContext *devcon;		//the pointer to our Direct3D device context

//prototypes

void InitD3D(HWND hWnd);			//sets up and initializes Direct3D
void CleanD3D();					//closes Direct3D and releases memory


//the windowProc func prototype
LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

//the entry poiint for any Windows program
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow){
	HWND hWnd;
	WNDCLASSEX wc;

	ZeroMemory(&wc, sizeof(WNDCLASSEX));

	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WindowProc;
	wc.hInstance = hInstance;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
	wc.lpszClassName = "WindowClass";

	RegisterClassEx(&wc);

	RECT wr = {0,0,800,600};
	AdjustWindowRect(&wr, WS_OVERLAPPEDWINDOW, FALSE);

	hWnd = CreateWindowEx(NULL,
						  "WindowClass",
						  "Our 1st Direct3D Program",
						  WS_OVERLAPPEDWINDOW,
						  300,
						  300,
						  wr.right-wr.left,
						  wr.bottom-wr.top,
						  NULL,
						  NULL,
						  hInstance,
						  NULL);

	ShowWindow(hWnd, nCmdShow);

	//init the 3D
	InitD3D(hWnd);

	//enter the main loop

	MSG msg;

	while(TRUE){
		if(PeekMessage(&msg, NULL, 0,0, PM_REMOVE)){
			TranslateMessage(&msg);
			DispatchMessage(&msg);

			if(msg.message == WM_QUIT)
				break;
		}
		else{
			//Run game code here
		}
	}


	//clean the directx and com
	CleanD3D();

	return msg.wParam;
}


LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam){

	switch (message)
	{
		case WM_DESTROY:{
			PostQuitMessage(0);
			return 0;
		}break;
	}

	return DefWindowProc(hWnd, message, wParam, lParam);
}







//init and prepares DIrect3D for use
void InitD3D(HWND hWnd){
	//create struct to hold info about the swap chain
	DXGI_SWAP_CHAIN_DESC scd;

	//clear out the struct for use
	ZeroMemory(&scd, sizeof(DXGI_SWAP_CHAIN_DESC));

	//fill the swap chain description struct
	scd.BufferCount = 1;										//one back buffer
	scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;			//use 32-bit color
	scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;			//hwo swap chains is to be used
	scd.OutputWindow = hWnd;									//the window to be used
	scd.SampleDesc.Count = 4;									//how many multisamples
	scd.Windowed = TRUE;										//windowed/fullscreen mode


	//create a device, device context and swap chain using the info in the scd struct
	D3D11CreateDeviceAndSwapChain(NULL,
								  D3D_DRIVER_TYPE_HARDWARE,
								  NULL,
								  NULL,
								  NULL,
								  NULL,
								  D3D11_SDK_VERSION,
								  &scd,
								  &swapchain,
								  &dev,
								  NULL,
								  &devcon);
}


void CleanD3D(){
	//close and release all existing COM objects
	swapchain->Release();
	dev->Release();
	devcon->Release();
}

