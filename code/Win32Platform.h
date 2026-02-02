#define internal static

//Windows functions
LRESULT Wndproc(HWND WindowHandle, UINT Message, WPARAM WParam, LPARAM LParam);
internal void Win32ProcessError(DWORD Error);
internal void MessageLoop(ID3D11Device* Device, float *ConstantBuffer, MessageLoopStateInput *StateInput);

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
	ID3D11Device* Device,
	ID3D11Buffer* *VertexBufferArray,
	unsigned int *VertexBufferCount,
	ID3D11Buffer* *IndexBufferArray,
	IndexedGeometryObject *IndexedGeometryArray,
	unsigned int *IndexedGeometryCount,
	float *GeometryData,
	unsigned int VertexDataSize,
	unsigned int VertexElementWidth,
	unsigned int *IndexData,
	unsigned int IndexDataSize,
	unsigned int IndexElementWidth);

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
internal UINT SetPipelineState(
	ID3D11DeviceContext *Device,
	GraphicsPipelineState *PipelineState,
	D3D11_VIEWPORT *ViewportArray, 
	UINT ViewportCount,
	D3D11_RECT *ScissorRectArray,
	UINT ScissorRectCount);
	
internal GraphicsPipelineState BuildPipelineState(
	ID3D11Buffer* *VertexBufferArray,
	UINT VertexBufferCount,
	UINT *StrideArray,
	UINT *OffsetArray,
	ID3D11Buffer *IndexBuffer,
	DXGI_FORMAT IndexBufferFormat,
	UINT IndexCount,
	ID3D11InputLayout *InputLayout,
	D3D11_PRIMITIVE_TOPOLOGY PrimitiveTopology,
	ID3D11VertexShader *VertexShader,
	ID3D11Buffer* *VertexShaderConstantBufferArray,
	UINT VertexShaderConstantBufferCount,
	ID3D11HullShader *HullShader,
	ID3D11DomainShader *DomainShader,
	ID3D11GeometryShader *GeometryShader,
	ID3D11RasterizerState *RasterizerState,
	ID3D11PixelShader* *PixelShader,
	ID3D11Buffer* *PixelShaderConstantBufferArray,
	UINT PixelShaderConstantBufferCount,
	ID3D11RenderTargetView* *RenderTargetViewArray,
	UINT RenderTargetViewCount,
	char *Description
	);

internal void AddPipelineStateToArray(GraphicsPipelineState PipelineState);
internal void SetComputeShaderState(ID3D11DeviceContext *DeviceContext, ComputeShaderState &CSState);
internal void ClearActivePipelineState(GraphicsPipelineState*ActivePipelineStateArray,unsigned int *ActivePipelineStateCount);
internal void PushPipelineState(GraphicsPipelineState*ActivePipelineStateArray,unsigned int *ActivePipelineStateCount, GraphicsPipelineState *State);


//miscs
internal int Win32AddPixelShaderToArray(
	ID3D11PixelShader* *PixelShaderArray,
	unsigned int *PixelShaderArrayCount,
	ID3D11PixelShader* PixelShader);

internal void ResizeSwapChainBuffers(ID3D11Device *Device,ID3D11DeviceContext *Context, IDXGISwapChain1 *SwapChain, UINT NewWidth, UINT NewHeight);

internal void UpdateCSTexture(ID3D11Device *Device, UINT Width, UINT Height, ID3D11Texture2D* *CSShaderResource, ID3D11UnorderedAccessView* *UAVArray);

internal void CycleShaderColors(ShaderColor *CurrentShaderColor);
