LRESULT Wndproc(HWND WindowHandle, UINT Message, WPARAM WParam, LPARAM LParam);
static void Win32ProcessError(DWORD Error);

//DXGI Functions
static int Win32GetIDXGIInterfacesFromD3DDevice(
	ID3D11Device *Device, 
	IDXGIDevice1 **IdxgiDevice,
	IDXGIAdapter **IdxgiAdapter,
	IDXGIFactory2 **IdxgiFactory
);
	
static IDXGISwapChain1* Win32GetSwapChain(ID3D11Device *Device, HWND Window,IDXGIFactory2 *IdxgiFactory);


//D3D11 Setup Functions
static ID3D11InputLayout* Win32CreateVertexInputLayout(
	ID3D11Device *Device, 
	ID3D11DeviceContext *DeviceContext,
	void *CompiledVSShaderCode, 
	size_t ShaderSize
);
static ID3D11Buffer * Win32CreateVertexBuffer(ID3D11Device *Device,void* VertexBufferData, UINT VertexBufferSize, UINT VertexSize);


//Shader functions
static ID3DBlob *Win32CompileShaderFromFile(LPCWSTR Filename, LPCSTR Entrypoint, LPCSTR Target);
static ID3D11VertexShader* Win32CreateVertexShader(ID3D11Device *Device, void *CompiledShaderCode, size_t ShaderSize);
static ID3D11PixelShader* Win32CreatePixelShader(ID3D11Device *Device, LPCWSTR Filename, LPCSTR Entrypoint, LPCSTR Target);
static int Win32AddPixelShaderToArray(ID3D11PixelShader** PixelShaderArray, ID3D11PixelShader* PixelShader);
