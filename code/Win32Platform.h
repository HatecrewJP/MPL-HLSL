#define internal static

//Windows functions
LRESULT Wndproc(HWND WindowHandle, UINT Message, WPARAM WParam, LPARAM LParam);
internal void Win32ProcessError(DWORD Error);
internal void MessageLoop(ID3D11Device* Device);

//DXGI Functions
internal int Win32GetIDXGIInterfacesFromD3DDevice(
	ID3D11Device *Device, 
	IDXGIDevice1 **IdxgiDevice,
	IDXGIAdapter **IdxgiAdapter,
	IDXGIFactory2 **IdxgiFactory
);
	
internal IDXGISwapChain1* Win32GetSwapChain(ID3D11Device *Device, HWND Window,IDXGIFactory2 *IdxgiFactory);



//Shader compilation functions
internal ShaderCode Win32CompileShaderFromFile(LPCWSTR Filename, LPCSTR Entrypoint, LPCSTR Target);

//Buffer functions
internal ID3D11Buffer* Win32CreateVertexBuffer(
	ID3D11Device *Device,
	void* VertexBufferData, 
	UINT VertexBufferSize);

internal ID3D11InputLayout* Win32CreateVertexInputLayout(
	ID3D11Device *Device, 
	ID3D11DeviceContext *DeviceContext,
	void *CompiledVSShaderCode, 
	size_t ShaderSize
);
internal void CreateVBForIndexedGeometry(
	float *GeometryData,
	unsigned int VertexCount, 
	unsigned int *IndexData,
	unsigned int IndexCount);

//Shader Creation functions
internal ID3D11VertexShader* Win32CreateVertexShader(
	ID3D11Device *Device, 
	void *CompiledShaderCode, 
	size_t ShaderSize);
internal ID3D11PixelShader* Win32CreatePixelShader(
	ID3D11Device *Device, 
	LPCWSTR Filename, 
	LPCSTR Entrypoint, 
	LPCSTR Target);
	
//PipelineStates

internal void SetComputeShaderState(ID3D11DeviceContext *DeviceContext, ComputeShaderState &CSState);
internal void ClearActivePipelineState();
internal void PushPipelineState(GraphicsPipelineState *State);
internal UINT SetPipelineState(
	ID3D11DeviceContext *Device,
	GraphicsPipelineState *PipelineState,
	D3D11_VIEWPORT *ViewportArray, 
	UINT ViewportCount,
	D3D11_RECT *ScissorRectArray,
	UINT ScissorRectCount);

//miscs
internal int Win32AddPixelShaderToArray(
	ID3D11PixelShader** PixelShaderArray, 
	ID3D11PixelShader* PixelShader);

internal void ResizeSwapChainBuffers(UINT NewWidth, UINT NewHeight);

internal void UpdateCSTexture(UINT Width, UINT Height);


