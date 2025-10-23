
#define ASSERT(x) if(!(x)) *(char*)0=0;



#include <d3d11.h>
#include <dxgi1_2.h>
#include <d3dcompiler.h>
#include <windows.h>



typedef int bool32;

LRESULT Wndproc(HWND WindowHandle, UINT Message, WPARAM WParam, LPARAM LParam){
	return DefWindowProcA(WindowHandle,Message,WParam,LParam);
}

static void ProcessError(DWORD Error){
	LPVOID lpMsgBuf = 0;
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,
		Error,
		MAKELANGID(LANG_GERMAN, SUBLANG_DEFAULT),
		(LPTSTR) & lpMsgBuf,
		0, NULL);
	OutputDebugStringA((LPSTR)lpMsgBuf);
	exit(-1);
}

static int GetIDXGIInterfacesFromD3DDevice(ID3D11Device *Device, IDXGIDevice1 **IdxgiDevice,IDXGIAdapter **IdxgiAdapter,IDXGIFactory2 **IdxgiFactory ){
	*IdxgiDevice = NULL;
	*IdxgiAdapter = NULL;
	*IdxgiFactory = NULL;
	
	if(Device){
		Device->QueryInterface(__uuidof(IDXGIDevice1),(void **)IdxgiDevice);
		if(*IdxgiDevice){
			(*IdxgiDevice)->GetAdapter(IdxgiAdapter);
			if(*IdxgiAdapter){
				(*IdxgiAdapter)->GetParent(__uuidof(IDXGIFactory2),(void **)IdxgiFactory);
				if(*IdxgiFactory){
					return 1;
				}
			}
		}
	}
	return 0;
}

int APIENTRY WinMain(HINSTANCE hInst, HINSTANCE hInstPrev, PSTR cmdline, int cmdshow){
	WNDCLASSEXA WindowClass;
	WindowClass.cbSize = sizeof(WNDCLASSEXA);
	WindowClass.style = CS_HREDRAW|CS_OWNDC|CS_VREDRAW;
	WindowClass.lpfnWndProc = Wndproc;
	WindowClass.cbClsExtra = 0;
	WindowClass.cbWndExtra = 0;
	WindowClass.hInstance = hInst;
	WindowClass.hIcon = NULL;
	WindowClass.hCursor = NULL;
	WindowClass.hbrBackground = NULL;
	WindowClass.lpszMenuName = "Direct3D";
	WindowClass.lpszClassName="Direct3DWindow";
	WindowClass.hIconSm = NULL;
	
	ATOM WindowAtom = RegisterClassExA(&WindowClass);
	ASSERT(WindowAtom);
	
	int Width = 1920;
	int Height = 1080;
	
	const UINT VertexCount = 3;
	const UINT VertexSize = 3*sizeof(UINT);
	
	HWND Window = CreateWindowExA(NULL,WindowClass.lpszClassName,"Direct3D_",WS_OVERLAPPEDWINDOW|WS_VISIBLE, 0, 0, Width, Height,NULL,NULL,hInst,NULL);
	if(Window){
		OutputDebugStringA("Window created\n");
		const D3D_FEATURE_LEVEL FeatureLevels[]={
			D3D_FEATURE_LEVEL_11_1,
			D3D_FEATURE_LEVEL_11_0,
			D3D_FEATURE_LEVEL_10_1,
			D3D_FEATURE_LEVEL_10_0,
			D3D_FEATURE_LEVEL_9_3,
			D3D_FEATURE_LEVEL_9_2,
			D3D_FEATURE_LEVEL_9_1};
		UINT FeatureLevelCount = sizeof(FeatureLevels)/sizeof(FeatureLevels[0]);
		ID3D11Device *Device;
		ID3D11DeviceContext *DeviceContext;
		D3D_FEATURE_LEVEL CurrentFeatureLevel;
		
		if(D3D11CreateDevice(NULL,D3D_DRIVER_TYPE_HARDWARE,NULL,D3D11_CREATE_DEVICE_DEBUG ,FeatureLevels,FeatureLevelCount,D3D11_SDK_VERSION,
							&Device,&CurrentFeatureLevel,&DeviceContext)==S_OK){
			OutputDebugStringA("Device Created");
			
			
			//retrieve IDXGI Interfaces
			IDXGIDevice1 *IdxgiDevice = NULL;
			IDXGIAdapter *IdxgiAdapter = NULL;
			IDXGIFactory2 *IdxgiFactory = NULL;
			
			GetIDXGIInterfacesFromD3DDevice(Device, &IdxgiDevice, &IdxgiAdapter, &IdxgiFactory);
			ASSERT(IdxgiDevice);
			ASSERT(IdxgiAdapter);
			ASSERT(IdxgiFactory);
			
			DXGI_SAMPLE_DESC SampleDesc = {1,0};
			
			DXGI_SWAP_CHAIN_DESC1 SwapChainDesc1;
			SwapChainDesc1.Width = 0;
			SwapChainDesc1.Height = 0;
			SwapChainDesc1.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
			SwapChainDesc1.Stereo = FALSE;
			SwapChainDesc1.SampleDesc = SampleDesc;
			SwapChainDesc1.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
			SwapChainDesc1.BufferCount = 2;
			SwapChainDesc1.Scaling = DXGI_SCALING_STRETCH;
			SwapChainDesc1.SwapEffect = DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL;
			SwapChainDesc1.AlphaMode = DXGI_ALPHA_MODE_UNSPECIFIED;
			SwapChainDesc1.Flags = 0;
			HRESULT res;
			
			IDXGISwapChain1 *SwapChain;
			if((res=IdxgiFactory->CreateSwapChainForHwnd(Device,Window,&SwapChainDesc1,NULL,NULL,&SwapChain))==S_OK){
				//Vertex Shader Compilation
				ID3DBlob *BlobVSCode;
				ID3DBlob *BlobVSError;
				if((res = D3DCompileFromFile(L"VertexShader.hlsl",NULL,NULL,"VSEntry","vs_5_0",D3DCOMPILE_ENABLE_BACKWARDS_COMPATIBILITY,0,&BlobVSCode,&BlobVSError))==S_OK){
					void *CompiledVSShaderCode = BlobVSCode->GetBufferPointer();
					size_t ShaderSize = BlobVSCode->GetBufferSize();
					ASSERT(CompiledVSShaderCode);
					OutputDebugStringA("VertexShader compiled");
					
					//Vertex Shader Creation
					ID3D11VertexShader *VertexShader = NULL;
					if(Device->CreateVertexShader(CompiledVSShaderCode,ShaderSize,NULL,&VertexShader)==S_OK){
						ASSERT(VertexShader);
						OutputDebugStringA("Vertex Shader created");
						
						//VertexInput layout
						D3D11_INPUT_ELEMENT_DESC VSInputElementDesc;
						VSInputElementDesc.SemanticName = "POSITION";
						VSInputElementDesc.SemanticIndex = 0;
						VSInputElementDesc.Format = DXGI_FORMAT_R32G32B32_FLOAT;
						VSInputElementDesc.InputSlot = 0;
						VSInputElementDesc.AlignedByteOffset = 0;
						VSInputElementDesc.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
						VSInputElementDesc.InstanceDataStepRate = 0;
						
						ID3D11InputLayout *VSInputLayout = NULL;
						if((res = Device->CreateInputLayout(&VSInputElementDesc,1,CompiledVSShaderCode,ShaderSize,&VSInputLayout))==S_OK){
							ASSERT(VSInputLayout);
							ASSERT(DeviceContext);
							DeviceContext->IASetInputLayout(VSInputLayout);
							OutputDebugStringA("VS Input Layout created and set");
							
							//Vertex Buffer Creation
							UINT VertexBufferSize = VertexCount*VertexSize;
							float oVertexBufferData[] {
								0.95f,0.99f,0.0f,
								0.0f,0.9f,0.0f,
								0.0f,0.0f,0.0f
							};
						
							
							
							
							
							D3D11_BUFFER_DESC VertexBufferDesc;
							VertexBufferDesc.ByteWidth = VertexBufferSize;
							VertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
							VertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
							VertexBufferDesc.CPUAccessFlags = 0;
							VertexBufferDesc.MiscFlags = 0;
							VertexBufferDesc.StructureByteStride = VertexSize;
							
							D3D11_SUBRESOURCE_DATA SubresourceData;
							SubresourceData.pSysMem = oVertexBufferData;
							SubresourceData.SysMemPitch = 0;
							SubresourceData.SysMemSlicePitch = 0;
							
							
							ID3D11Buffer *VertexBuffer;
							UINT Strides[1] = {VertexSize};
							UINT Offsets[1] = {0};
							
							if((res=Device->CreateBuffer(&VertexBufferDesc,&SubresourceData,&VertexBuffer))==S_OK){
								DeviceContext->IASetVertexBuffers(0,1,&VertexBuffer,Strides,Offsets);
								OutputDebugStringA("VertexBuffer created and set\n");
								DeviceContext->VSSetShader(VertexShader,NULL,0);
								DeviceContext->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_POINTLIST);
								OutputDebugStringA("VS Initialized");
								
								//Pixel Shader
								ID3DBlob *BlobPSCode;
								ID3DBlob *BlobPSError;
								
								if((res = D3DCompileFromFile(L"PixelShader.hlsl",NULL,NULL,"PSEntry","ps_5_0",D3DCOMPILE_ENABLE_BACKWARDS_COMPATIBILITY,0,&BlobPSCode,&BlobPSError))==S_OK){
									
									void *CompiledPSShaderCode = BlobPSCode->GetBufferPointer();
									size_t PSShaderSize = BlobPSCode->GetBufferSize();
									ASSERT(CompiledPSShaderCode);
									OutputDebugStringA("PixelShader compiled");
									
									ID3D11PixelShader *PixelShader = NULL;
									if((res = Device->CreatePixelShader(CompiledPSShaderCode,PSShaderSize,NULL,&PixelShader))==S_OK){
										ASSERT(PixelShader);
										DeviceContext->PSSetShader(PixelShader,NULL,0);
										OutputDebugStringA("PixelShader creation successful");
										
										//Resource
										ID3D11Resource *RenderTargetResource = NULL;
										if((res=SwapChain->GetBuffer(0,__uuidof(ID3D11Resource),(void**)&RenderTargetResource))==S_OK){
											ASSERT(RenderTargetResource);
											//RenderTargetView
											ID3D11RenderTargetView *RenderTargetView = NULL;
											
											if((res=Device->CreateRenderTargetView(RenderTargetResource,NULL,&RenderTargetView))==S_OK){
												ASSERT(RenderTargetView);
												DeviceContext->OMSetRenderTargets(1,&RenderTargetView,NULL);
												OutputDebugStringA("RenderTargetView created");
												//ViewPort
												D3D11_VIEWPORT ViewPort;
												ViewPort.TopLeftX = 0.0f;
												ViewPort.TopLeftY = 0.0f;
												ViewPort.Width = (float)Width;
												ViewPort.Height = (float)Height;
												ViewPort.MinDepth = 0.0f;
												ViewPort.MaxDepth = 1.0f;
												DeviceContext->RSSetViewports(1,&ViewPort);


												DeviceContext->Draw(VertexCount, 0);
												SwapChain->Present(0, 0); 
												
											
												
											}
											else if(res==S_FALSE){
												OutputDebugStringA("RenderTargetView args correct");
											}
											else{
												OutputDebugStringA("RenderTargetView failed");
											}
										}
										
										
										
										
										
										
										
									}
									else{
										OutputDebugStringA("PixelShader creation failed");
									}
									
									
								}
								else{
									OutputDebugStringA("PS Compilation Failed");
									char* ErrorBuffer = (char*)BlobPSError->GetBufferPointer();
									SIZE_T ErrorBufferSize = BlobPSError->GetBufferSize();
									OutputDebugStringA(ErrorBuffer);
									ASSERT(0);
									
								}
				
							}
							else if(res==S_FALSE){
								OutputDebugStringA("VS Buffer Arguments okay");
							}
							else{
								ASSERT(res == E_OUTOFMEMORY);
								OutputDebugStringA("VS Buffer OutOfMemory\n");
								exit(-1);
							}
							
							VirtualFree(oVertexBufferData,0,MEM_RELEASE);
							
						}
						else if(res == S_FALSE){
							OutputDebugStringA("VS Input Layout Parameters correct");
						}
						else{
							OutputDebugStringA("VS Input Layout Error");
						}
					}
					else{
						OutputDebugStringA("VS Shader Creation Failed");
					}
				}
				else{
					OutputDebugStringA("VS Compilation Failed");
					char* ErrorBuffer = (char*)BlobVSError->GetBufferPointer();
					SIZE_T ErrorBufferSize = BlobVSError->GetBufferSize();
					OutputDebugStringA(ErrorBuffer);
					ASSERT(0);
					
				}
			}
			else{
				OutputDebugStringA("Swap Chain Failed");
			}
				
		}
		else{
			
		}
		
		
		
		
	
		
		
		

	}
	else{
		ProcessError(GetLastError());
	}
	
	for(;;)
	{ }
	
}