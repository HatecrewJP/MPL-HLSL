#define ASSERT(x) if(!(x)) *(char*)0=0;

#define MAX_PIXEL_SHADER_COUNT 32
#define ArrayCount(x) (sizeof(x)/sizeof((x)[0]))

#define KB(x) (x)*1024
#define MB(x) KB(x)*1024
#define GB(x) MB(x)*1024

#include <d3d11.h>
#include <dxgi1_2.h>
#include <d3dcompiler.h>
#include <windows.h>
#include <math.h>
#define MAX_PIPELINE_STATES 64

#define internal static
#define global_variable static
#ifndef __cplusplus
	typedef int bool;
	#define true 1
	#define false 0
#endif

#include "Structs.h"
#include "Win32Platform.h"

global_variable int const Zero = 0;
global_variable GraphicsPipelineState NILGraphicsPipelineState = {0};
global_variable ComputeShaderState NILComputeShaderState = {0};


global_variable ID3D11Texture2D	*GlobalFrameBuffer = NULL;





//Windows functions
LRESULT Wndproc(HWND WindowHandle, UINT Message, WPARAM WParam, LPARAM LParam){
	switch(Message){
		case WM_CLOSE:{
			PostQuitMessage(0);
		}break;
		case WM_DESTROY:{
			PostQuitMessage(0);
		}break;
	}
	return DefWindowProcA(WindowHandle,Message,WParam,LParam);
}

internal void Win32ProcessError(DWORD Error){
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


internal void MessageLoop(ID3D11Device* Device, float *ConstantBuffer, MessageLoopStateInput *StateInput){
	ASSERT(Device!=NULL && ConstantBuffer != NULL && StateInput!=NULL);
	ASSERT(StateInput->Running
		&& StateInput->VsyncActive
		&& StateInput->AnimationIsActive 
		&& StateInput->ActiveShaderColor 
		&& StateInput->PipelineStateArray
		&& StateInput->ActivePipelineStateArray 
		&& StateInput->ActivePipelineStateCount 
		&& StateInput->ActiveCSState
		&& StateInput->ComputeShaderStateArray
		);
	
	MSG Message;
	while(PeekMessage(&Message, 0, 0, 0, PM_REMOVE)){
		switch(Message.message){
			case WM_QUIT:{
				*(StateInput->Running) = false;
			}break;
			case WM_KEYDOWN:{
				unsigned int VKCode = (unsigned int) Message.wParam;
				if(VKCode == '1'){
					//PassThrough
					ClearActivePipelineState(StateInput->ActivePipelineStateArray,StateInput->ActivePipelineStateCount);
					PushPipelineState(StateInput->ActivePipelineStateArray,StateInput->ActivePipelineStateCount,&(StateInput->PipelineStateArray[0]));
					if(!(*StateInput->AnimationIsActive)){
						ConstantBuffer[0] = 200.0f;
						*(StateInput->ActiveShaderColor)= RED;
					}
					*(StateInput->ActiveCSState) = &NILComputeShaderState;
				}
				else if(VKCode == '2'){
					ClearActivePipelineState(StateInput->ActivePipelineStateArray,StateInput->ActivePipelineStateCount);
					PushPipelineState(StateInput->ActivePipelineStateArray,StateInput->ActivePipelineStateCount,&(StateInput->PipelineStateArray[1]));
					if(!(*StateInput->AnimationIsActive)){
						ConstantBuffer[0] = 200.0f;
						*(StateInput->ActiveShaderColor) = RED;
					}
					*(StateInput->ActiveCSState) = &NILComputeShaderState;
				}
				else if(VKCode == '3'){
					ClearActivePipelineState(StateInput->ActivePipelineStateArray,StateInput->ActivePipelineStateCount);
					PushPipelineState(StateInput->ActivePipelineStateArray,StateInput->ActivePipelineStateCount,&(StateInput->PipelineStateArray[2]));
					if(!(*StateInput->AnimationIsActive)){
						ConstantBuffer[0] = 200.0f;
						*(StateInput->ActiveShaderColor) = RED;
					}
					*(StateInput->ActiveCSState) = &NILComputeShaderState;
				}
				else if(VKCode == '4'){
					ClearActivePipelineState(StateInput->ActivePipelineStateArray,StateInput->ActivePipelineStateCount);
					PushPipelineState(StateInput->ActivePipelineStateArray,StateInput->ActivePipelineStateCount,&(StateInput->PipelineStateArray[3]));
					if(!(*StateInput->AnimationIsActive)){
						ConstantBuffer[0] = 200.0f;
						*(StateInput->ActiveShaderColor) = RED;
					}
					*(StateInput->ActiveCSState) = &NILComputeShaderState;
				}
				else if(VKCode == '5'){
					ClearActivePipelineState(StateInput->ActivePipelineStateArray,StateInput->ActivePipelineStateCount);
					PushPipelineState(StateInput->ActivePipelineStateArray,StateInput->ActivePipelineStateCount,&(StateInput->PipelineStateArray[4]));
					if(!(*StateInput->AnimationIsActive)){
						ConstantBuffer[0] = 200.0f;
						*(StateInput->ActiveShaderColor) = RED;
					}
					*(StateInput->ActiveCSState) = &NILComputeShaderState;
				}
				else if(VKCode == '6'){
					ClearActivePipelineState(StateInput->ActivePipelineStateArray,StateInput->ActivePipelineStateCount);
					PushPipelineState(StateInput->ActivePipelineStateArray,StateInput->ActivePipelineStateCount,&(StateInput->PipelineStateArray[5]));
					if(!(*StateInput->AnimationIsActive)){
						ConstantBuffer[0] = 200.0f;
						*(StateInput->ActiveShaderColor) = RED;
					}
					*(StateInput->ActiveCSState) = &NILComputeShaderState;
				}
				else if(VKCode == '7'){
					ClearActivePipelineState(StateInput->ActivePipelineStateArray,StateInput->ActivePipelineStateCount);
					PushPipelineState(StateInput->ActivePipelineStateArray,StateInput->ActivePipelineStateCount,&(StateInput->PipelineStateArray[5]));
					if(!(*StateInput->AnimationIsActive)){
						ConstantBuffer[0] = 200.0f;
						*(StateInput->ActiveShaderColor) = RED;
					}
					*(StateInput->ActiveCSState) = &(StateInput->ComputeShaderStateArray[0]);
				}
				else if(VKCode == 'P'){
					*StateInput->VsyncActive ^=1;
				}
				else if(VKCode == VK_SPACE){
					*StateInput->AnimationIsActive ^= true;
				}
				
				bool AltKeyWasDown = ((Message.lParam & (1 << 29)) != 0);
				if((VKCode == VK_F4) && AltKeyWasDown){
					StateInput->Running = false;
				}
			}break;
			
			default:{
				TranslateMessage(&Message);
				DispatchMessage(&Message);
			}break;
		}
	}
}


//DXGI functions
internal int Win32GetIDXGIInterfacesFromD3DDevice(
	ID3D11Device *Device, 
	IDXGIDevice1 **IdxgiDevice,
	IDXGIAdapter **IdxgiAdapter,
	IDXGIFactory2 **IdxgiFactory)
{
	if(Device && IdxgiDevice){
		Device->lpVtbl->QueryInterface(Device,&IID_IDXGIDevice1,(void **)IdxgiDevice);
		if(*IdxgiDevice && IdxgiAdapter){
			(*IdxgiDevice)->lpVtbl->GetAdapter(*IdxgiDevice,IdxgiAdapter);
			if(*IdxgiAdapter && IdxgiFactory){
				(*IdxgiAdapter)->lpVtbl->GetParent(*IdxgiAdapter,&IID_IDXGIFactory2,(void **)IdxgiFactory);
				if(*IdxgiFactory){
					return 1;
				}
			}
		}
	}
	return 0;
}




internal IDXGISwapChain1* Win32GetSwapChain(
	ID3D11Device *Device, 
	HWND Window,
	IDXGIFactory2 *IdxgiFactory)
{
	ASSERT(Device && IdxgiFactory);
	
	DXGI_SAMPLE_DESC SampleDesc = {1,0};
	DXGI_SWAP_CHAIN_DESC1 SwapChainDesc1;
	SwapChainDesc1.Width = 0;
	SwapChainDesc1.Height = 0;
	SwapChainDesc1.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	SwapChainDesc1.Stereo = FALSE;
	SwapChainDesc1.SampleDesc = SampleDesc;
	SwapChainDesc1.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	SwapChainDesc1.BufferCount = 2;
	SwapChainDesc1.Scaling = DXGI_SCALING_NONE;
	SwapChainDesc1.SwapEffect = DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL;
	SwapChainDesc1.AlphaMode = DXGI_ALPHA_MODE_UNSPECIFIED;
	SwapChainDesc1.Flags = 0;
	
	IDXGISwapChain1 *SwapChain = NULL;
	if(IdxgiFactory->lpVtbl->CreateSwapChainForHwnd(IdxgiFactory,(IUnknown*)Device,Window,&SwapChainDesc1,NULL,NULL,&SwapChain)!=S_OK){
		return NULL;
	}
	ASSERT(SwapChain);
	return SwapChain;
	
}

//Shader compilation functions
internal ID3DBlob* Win32CompileShaderFromFile(LPCWSTR Filename, LPCSTR Entrypoint, LPCSTR Target){
	ASSERT(Filename);
	ASSERT(Entrypoint);
	ASSERT(Target);
	
	ID3DBlob *BlobCode = NULL;
	ID3DBlob *BlobError = NULL;

	if((D3DCompileFromFile(Filename, NULL, NULL, Entrypoint, Target, D3DCOMPILE_DEBUG, 0, &BlobCode, &BlobError)) != S_OK){
		if(BlobError){
			LPCSTR Buffer = (LPCSTR)BlobError->lpVtbl->GetBufferPointer(BlobError);
			OutputDebugStringA(Buffer);
			BlobError->lpVtbl->Release(BlobError);
		}
		return NULL;
	}
	
	ASSERT(BlobCode);
	return BlobCode;
}

internal ID3DBlob* Win32CompileShader(LPCSTR ShaderText, SIZE_T ShaderSize, LPCSTR Entrypoint, LPCSTR Target){
	ASSERT(ShaderText);
	ASSERT(Entrypoint);
	ASSERT(Target);
	
	ID3DBlob *BlobCode = NULL;
	ID3DBlob *BlobError = NULL;
	if((D3DCompile(ShaderText,ShaderSize, NULL, NULL, NULL, Entrypoint, Target, D3DCOMPILE_DEBUG, 0, &BlobCode, &BlobError)) != S_OK){
		if(BlobError){
			LPCSTR Buffer = (LPCSTR)BlobError->lpVtbl->GetBufferPointer(BlobError);
			OutputDebugStringA(Buffer);
			BlobError->lpVtbl->Release(BlobError);
		}
		return NULL;
	}
	ASSERT(BlobCode);
	return BlobCode;
}

//Buffer functions

internal ID3D11Buffer* Win32CreateVertexBuffer(
	ID3D11Device *Device,
	void* VertexBufferData, 
	UINT VertexBufferSize)
{
	ASSERT(Device);
	ASSERT(VertexBufferData);
	
	D3D11_BUFFER_DESC VertexBufferDesc;
	VertexBufferDesc.ByteWidth = VertexBufferSize;
	VertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	VertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	VertexBufferDesc.CPUAccessFlags = 0;
	VertexBufferDesc.MiscFlags = 0;
	VertexBufferDesc.StructureByteStride = 0;
	
	D3D11_SUBRESOURCE_DATA SubresourceData;
	SubresourceData.pSysMem = VertexBufferData;
	SubresourceData.SysMemPitch = 0;
	SubresourceData.SysMemSlicePitch = 0;
	
	ID3D11Buffer *VertexBuffer = NULL;
	ASSERT(Device->lpVtbl->CreateBuffer(Device,&VertexBufferDesc,&SubresourceData,&VertexBuffer)==S_OK);
	ASSERT(VertexBuffer);
	
	return VertexBuffer;
}

internal ID3D11InputLayout* Win32CreateVertexInputLayout(
	ID3D11Device *Device,
	void *CompiledVSShaderCode, 
	size_t ShaderSize)
{
	ASSERT(Device);
	ASSERT(CompiledVSShaderCode);
	
	D3D11_INPUT_ELEMENT_DESC VSInputElementDescArray[2];
	VSInputElementDescArray[0].SemanticName = "SV_POSITION";
	VSInputElementDescArray[0].SemanticIndex = 0;
	VSInputElementDescArray[0].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	VSInputElementDescArray[0].InputSlot = 0;
	VSInputElementDescArray[0].AlignedByteOffset = 0;
	VSInputElementDescArray[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	VSInputElementDescArray[0].InstanceDataStepRate = 0;
	
	VSInputElementDescArray[1].SemanticName = "COLOR";
	VSInputElementDescArray[1].SemanticIndex = 0;
	VSInputElementDescArray[1].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	VSInputElementDescArray[1].InputSlot = 0;
	VSInputElementDescArray[1].AlignedByteOffset = 3*sizeof(float);
	VSInputElementDescArray[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	VSInputElementDescArray[1].InstanceDataStepRate = 0;
	
	ID3D11InputLayout *VSInputLayout = NULL;
	ASSERT(Device->lpVtbl->CreateInputLayout(Device,VSInputElementDescArray,2,CompiledVSShaderCode,ShaderSize,&VSInputLayout)==S_OK);
	ASSERT(VSInputLayout);
	
	return VSInputLayout;	
}


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
	unsigned int IndexElementWidth)
{
	ASSERT(Device);
	ASSERT(VertexBufferArray);
	ASSERT(VertexBufferCount);
	ASSERT(IndexBufferArray);
	ASSERT(IndexedGeometryArray);
	ASSERT(IndexedGeometryCount);
	ASSERT(GeometryData);
	ASSERT(IndexData);
	
	IndexedGeometryObject NewObject;
	NewObject.VertexData = GeometryData;
	NewObject.VertexSize = VertexElementWidth;
	NewObject.VertexCount = VertexDataSize / VertexElementWidth;
	NewObject.VertexDataSize = NewObject.VertexCount * NewObject.VertexSize;
	
	NewObject.IndexData = IndexData;
	NewObject.IndexSize = IndexElementWidth;
	NewObject.IndexCount = IndexDataSize / IndexElementWidth;
	NewObject.IndexDataSize = NewObject.IndexSize * NewObject.IndexCount;
	

	IndexedGeometryArray[(*IndexedGeometryCount)++]=NewObject;

	VertexBufferArray[(*VertexBufferCount)] = Win32CreateVertexBuffer(Device, NewObject.VertexData, NewObject.VertexDataSize);
	
	D3D11_BUFFER_DESC VBDesc;
	VertexBufferArray[(*VertexBufferCount)]->lpVtbl->GetDesc(VertexBufferArray[(*VertexBufferCount)],&VBDesc);
	
	D3D11_BUFFER_DESC IndexBufferDesc;
	IndexBufferDesc.ByteWidth = NewObject.IndexDataSize;
	IndexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	IndexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	IndexBufferDesc.CPUAccessFlags = 0;
	IndexBufferDesc.MiscFlags = 0;
	IndexBufferDesc.StructureByteStride = 0;
	
	D3D11_SUBRESOURCE_DATA IndexSubresourceData;
	IndexSubresourceData.pSysMem = NewObject.IndexData;
	IndexSubresourceData.SysMemPitch = 0;
	IndexSubresourceData.SysMemSlicePitch = 0;
	

	HRESULT res = Device->lpVtbl->CreateBuffer(
		Device,
		&IndexBufferDesc,
		&IndexSubresourceData,
		&IndexBufferArray[(*VertexBufferCount)]);
		
	ASSERT(res == S_OK && IndexBufferArray[(*VertexBufferCount)]);
	(*VertexBufferCount)++;
}

//Shader Creation functions
internal ID3D11VertexShader* Win32CreateVertexShader(
	ID3D11Device *Device, 
	void *CompiledShaderCode, 
	size_t ShaderSize)
{
	ASSERT(Device);
	ASSERT(CompiledShaderCode);
	
	ID3D11VertexShader *VertexShader = NULL;
	ASSERT(Device->lpVtbl->CreateVertexShader(Device,CompiledShaderCode,ShaderSize,NULL,&VertexShader)==S_OK);
	ASSERT(VertexShader);
	return VertexShader;
	
}
internal ID3D11PixelShader* Win32CreatePixelShader(
	ID3D11Device *Device, 
	LPCWSTR Filename, 
	LPCSTR Entrypoint, 
	LPCSTR Target)
{
	ASSERT(Device);
	ASSERT(Filename);
	ASSERT(Entrypoint);
	ASSERT(Target);
	
	//Pixel Shader
	ID3DBlob* PSBlob = Win32CompileShaderFromFile(Filename,Entrypoint,Target);
	if(!PSBlob) return NULL;
	
	void *PSBinaryCode  = PSBlob->lpVtbl->GetBufferPointer(PSBlob);
	size_t PSBinarySize = PSBlob->lpVtbl->GetBufferSize(PSBlob);
	
	ID3D11PixelShader *PixelShader = NULL;
	ASSERT(Device->lpVtbl->CreatePixelShader(Device,PSBinaryCode,PSBinarySize,NULL,&PixelShader)==S_OK);
	ASSERT(PixelShader);
	return PixelShader;
	
}
//PipelineStates
internal UINT SetPipelineState(
	ID3D11DeviceContext *DeviceContext,
	GraphicsPipelineState *PipelineState,
	D3D11_VIEWPORT *ViewportArray, 
	UINT ViewportCount,
	D3D11_RECT *ScissorRectArray,
	UINT ScissorRectCount)
{
	ASSERT(DeviceContext);
	ASSERT(!(ViewportArray==NULL && ViewportCount > 0));
	ASSERT(!(ScissorRectArray == NULL && ScissorRectCount > 0));
	
	if(PipelineState == NULL){
		return 0;
	}
	
	//IA
	DeviceContext->lpVtbl->IASetVertexBuffers(DeviceContext,0, 
		PipelineState->VertexBufferCount, PipelineState->VertexBufferArray,
		PipelineState->StrideArray, PipelineState->OffsetArray);
	DeviceContext->lpVtbl->IASetIndexBuffer(DeviceContext,PipelineState->IndexBuffer, PipelineState->IndexBufferFormat, 0);
	DeviceContext->lpVtbl->IASetInputLayout(DeviceContext,PipelineState->InputLayout);
	DeviceContext->lpVtbl->IASetPrimitiveTopology(DeviceContext,PipelineState->PrimitiveTopology);
	//VS
	DeviceContext->lpVtbl->VSSetShader(DeviceContext,PipelineState->VertexShader, NULL, 0);
	DeviceContext->lpVtbl->VSSetConstantBuffers(DeviceContext,0,PipelineState->VertexShaderConstantBufferCount,PipelineState->VertexShaderConstantBufferArray);
	//HS
	DeviceContext->lpVtbl->HSSetShader(DeviceContext,PipelineState->HullShader,NULL,0);
	//DS
	DeviceContext->lpVtbl->DSSetShader(DeviceContext,PipelineState->DomainShader,NULL,0);
	//GS1
	DeviceContext->lpVtbl->GSSetShader(DeviceContext,PipelineState->GeometryShader,NULL,0);
	//RS
	DeviceContext->lpVtbl->RSSetScissorRects(DeviceContext,ScissorRectCount,ScissorRectArray);
	DeviceContext->lpVtbl->RSSetState(DeviceContext,PipelineState->RasterizerState);
	DeviceContext->lpVtbl->RSSetViewports(DeviceContext,ViewportCount,ViewportArray);
	//PS
	DeviceContext->lpVtbl->PSSetShader(DeviceContext,*(PipelineState->PixelShader), NULL, 0);
	DeviceContext->lpVtbl->PSSetConstantBuffers(DeviceContext,0,PipelineState->PixelShaderConstantBufferCount,PipelineState->PixelShaderConstantBufferArray);
	//OMS
	DeviceContext->lpVtbl->OMSetRenderTargets(DeviceContext,PipelineState->RenderTargetViewCount, PipelineState->RenderTargetViewArray, NULL);
	return PipelineState->IndexCount;
}

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
	char const *Description)
{
	GraphicsPipelineState NewPipelineState = {0};
	NewPipelineState.VertexBufferArray = VertexBufferArray;
	NewPipelineState.VertexBufferCount = VertexBufferCount;
	NewPipelineState.StrideArray = StrideArray;
	NewPipelineState.OffsetArray = OffsetArray;
	NewPipelineState.IndexBuffer = IndexBuffer;
	NewPipelineState.IndexBufferFormat = IndexBufferFormat;
	NewPipelineState.IndexCount = IndexCount;
	NewPipelineState.InputLayout = InputLayout;
	NewPipelineState.PrimitiveTopology = PrimitiveTopology;
	NewPipelineState.VertexShader = VertexShader;
	NewPipelineState.VertexShaderConstantBufferArray = VertexShaderConstantBufferArray;
	NewPipelineState.VertexShaderConstantBufferCount = VertexShaderConstantBufferCount;
	NewPipelineState.HullShader = HullShader;
	NewPipelineState.DomainShader = DomainShader;
	NewPipelineState.GeometryShader = GeometryShader;
	NewPipelineState.RasterizerState = RasterizerState;
	NewPipelineState.PixelShader = PixelShader;
	NewPipelineState.PixelShaderConstantBufferArray = PixelShaderConstantBufferArray;
	NewPipelineState.PixelShaderConstantBufferCount = PixelShaderConstantBufferCount;
	NewPipelineState.RenderTargetViewArray = RenderTargetViewArray;
	NewPipelineState.RenderTargetViewCount = 1;
	NewPipelineState.Description = Description;
	return NewPipelineState;
}
internal void AddPipelineStateToArray(
	GraphicsPipelineState *PipelineStateArray,
	unsigned int *PipelineStateCount, 
	GraphicsPipelineState PipelineState)
{
	ASSERT(PipelineStateArray);
	ASSERT(PipelineStateCount);
	
	
	PipelineStateArray[*PipelineStateCount] = PipelineState;
	(*PipelineStateCount)++;
}

internal void SetComputeShaderState(ID3D11DeviceContext *DeviceContext, ComputeShaderState *CSState){
	ASSERT(DeviceContext);
	ASSERT(CSState);
	
	DeviceContext->lpVtbl->CSSetUnorderedAccessViews(DeviceContext,0,CSState->UnorderedAccessViewCount,CSState->UnorderedAccessViewArray,NULL);
	DeviceContext->lpVtbl->CSSetShaderResources(DeviceContext,0,CSState->ShaderResourceViewCount,CSState->ShaderResourceViewArray);
	DeviceContext->lpVtbl->CSSetShader(DeviceContext,CSState->ComputeShader,NULL,0);
}
internal void ClearActivePipelineState(GraphicsPipelineState *ActivePipelineStateArray, unsigned int *ActivePipelineStateCount){
	ASSERT(ActivePipelineStateArray);
	ASSERT(ActivePipelineStateCount);
	for(unsigned int i = 0; i < (*ActivePipelineStateCount); i++){
		ActivePipelineStateArray[i] = NILGraphicsPipelineState;
	}
	*ActivePipelineStateCount = 0;
}
internal void PushPipelineState(GraphicsPipelineState *ActivePipelineStateArray,unsigned int *ActivePipelineStateCount, GraphicsPipelineState *State){
	ASSERT(ActivePipelineStateArray);
	ASSERT(ActivePipelineStateCount);
	ASSERT(State);
	
	ActivePipelineStateArray[(*ActivePipelineStateCount)++]=*State;
	
}



//Miscs
internal int Win32AddPixelShaderToArray(
	ID3D11PixelShader* *PixelShaderArray,
	unsigned int *PixelShaderArrayCount,
	ID3D11PixelShader* PixelShader)
{
	ASSERT(PixelShaderArray);
	ASSERT(PixelShaderArrayCount);
	ASSERT(PixelShader);
	
	if(PixelShaderArray){
		if((*PixelShaderArrayCount) < MAX_PIXEL_SHADER_COUNT){
			PixelShaderArray[(*PixelShaderArrayCount)] = PixelShader;
			(*PixelShaderArrayCount)++;
			return (*PixelShaderArrayCount)-1;
		}
	}
	return -1;
	
}


internal void ResizeSwapChainBuffers(
	ID3D11Device *Device,
	ID3D11DeviceContext *Context, 
	IDXGISwapChain1 *SwapChain, 
	UINT NewWidth, 
	UINT NewHeight, 
	ID3D11RenderTargetView* *RenderTargetView)
{
	ASSERT(Device);
	ASSERT(Context);
	ASSERT(SwapChain);
	ASSERT(RenderTargetView);
	
	Context->lpVtbl->OMSetRenderTargets(Context,0,0,0);
	GlobalFrameBuffer->lpVtbl->Release(GlobalFrameBuffer);
	GlobalFrameBuffer = NULL;
	
	
	ASSERT(*RenderTargetView)
	(*RenderTargetView)->lpVtbl->Release(*RenderTargetView);
	(*RenderTargetView) = NULL;
	
	
	ASSERT(SwapChain->lpVtbl->ResizeBuffers(SwapChain,0,NewWidth,NewHeight,DXGI_FORMAT_UNKNOWN,0)==S_OK);
	
	
	HRESULT res = SwapChain->lpVtbl->GetBuffer(SwapChain,0,&IID_ID3D11Texture2D,(void**)&GlobalFrameBuffer);
	if(res == S_OK){
		ASSERT(GlobalFrameBuffer);
		//RenderTargetView
		res = Device->lpVtbl->CreateRenderTargetView(Device,(ID3D11Resource*)GlobalFrameBuffer,NULL,RenderTargetView);
		if(res == S_OK){
			ASSERT(RenderTargetView);
			OutputDebugStringA("Resize Success\n");
		}
		else{
			OutputDebugStringA("Resize RTV failed\n");
		}
	}
	else{
		OutputDebugStringA("Resize Buffer failed\n");
	}
	
}

internal void UpdateCSTexture(
	ID3D11Device *Device, 
	UINT Width, UINT Height, 
	ID3D11Texture2D* *CSShaderResource, 
	ID3D11UnorderedAccessView* *UAVArray, 
	ID3D11RenderTargetView* *RenderTargetView)
{
	ASSERT(Device);
	ASSERT(CSShaderResource);
	ASSERT(UAVArray);
	ASSERT(RenderTargetView);
	
	if(*CSShaderResource){
		(*CSShaderResource)->lpVtbl->Release(*CSShaderResource);
		*CSShaderResource = NULL;
	}
	
	//Create new Texture2D
	D3D11_TEX2D_SRV CSSRV;
	CSSRV.MostDetailedMip = 0;
	CSSRV.MipLevels = 1;
	
	D3D11_SHADER_RESOURCE_VIEW_DESC SRVDesc;
	SRVDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	SRVDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	SRVDesc.Texture2D =  CSSRV;
	
	UINT TextureSize = Width * Height * 4;
	void *CSShaderResourceData =  VirtualAlloc(NULL,TextureSize,MEM_RESERVE|MEM_COMMIT,PAGE_READWRITE);
	ASSERT(CSShaderResourceData);
	
	D3D11_SUBRESOURCE_DATA CSSubresourceData;
	CSSubresourceData.pSysMem = CSShaderResourceData;
	CSSubresourceData.SysMemPitch = 4;
	CSSubresourceData.SysMemSlicePitch = 0;
	D3D11_TEXTURE2D_DESC Tex2DDesc;
	GlobalFrameBuffer->lpVtbl->GetDesc(GlobalFrameBuffer,&Tex2DDesc);
	Tex2DDesc.BindFlags |= (D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_UNORDERED_ACCESS);
	
	ASSERT((*CSShaderResource) == NULL);
	ASSERT(Device->lpVtbl->CreateTexture2D(Device,&Tex2DDesc,&CSSubresourceData,CSShaderResource)==S_OK);
	ASSERT((*CSShaderResource) != NULL);
	//Create new UAV
	D3D11_BUFFER_UAV UAVElementDesc;
	UAVElementDesc.FirstElement = 0;
	UAVElementDesc.NumElements = TextureSize / 4;
	UAVElementDesc.Flags = 0;
	
	D3D11_UNORDERED_ACCESS_VIEW_DESC UAVDesc;
	UAVDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	UAVDesc.ViewDimension = D3D11_UAV_DIMENSION_TEXTURE2D;
	UAVDesc.Buffer = UAVElementDesc;

	ASSERT(Device->lpVtbl->CreateUnorderedAccessView(Device,(ID3D11Resource*)*CSShaderResource,&UAVDesc,&(UAVArray[0]))==S_OK);
	ASSERT(UAVArray[0]!=NULL);	
	VirtualFree(CSShaderResourceData, 0, MEM_RELEASE);
}

internal void CycleShaderColors(ShaderColor *CurrentShaderColor){
	ASSERT(CurrentShaderColor);
	*CurrentShaderColor = (ShaderColor)((*CurrentShaderColor + 1) % SHADER_COLOR_COUNT);
}


int APIENTRY WinMain(HINSTANCE hInst, HINSTANCE hInstPrev, PSTR cmdline, int cmdshow){
	//Windows Window
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
	
	UINT Width = 1920;
	UINT Height = 1080;
	
	
	
	HRESULT res = 0;
	HWND Window = CreateWindowExA(0,WindowClass.lpszClassName,"Direct3D_",WS_OVERLAPPEDWINDOW|WS_VISIBLE, 0, 0, Width, Height,NULL,NULL,hInst,NULL);
	if(Window){
		OutputDebugStringA("Window created\n");
		D3D_FEATURE_LEVEL const FeatureLevels[]={
			D3D_FEATURE_LEVEL_11_1,
			D3D_FEATURE_LEVEL_11_0,
			};
		UINT FeatureLevelCount = sizeof(FeatureLevels)/sizeof(FeatureLevels[0]);
		
		RECT ClientRect;
		ASSERT(GetClientRect(Window,&ClientRect));
		
		Width = ClientRect.right - ClientRect.left;
		Height  = ClientRect.bottom - ClientRect.top;
        
		ID3D11Device 			*Device 	= NULL;
		IDXGISwapChain1 		*SwapChain 	= NULL;
		ID3D11DeviceContext 	*Context 	= NULL;
	

        D3D_FEATURE_LEVEL CurrentFeatureLevel;
		//D3D11 Device
		res = D3D11CreateDevice(NULL,D3D_DRIVER_TYPE_HARDWARE,NULL,D3D11_CREATE_DEVICE_DEBUG,
							FeatureLevels,FeatureLevelCount,D3D11_SDK_VERSION,
                            &Device,&CurrentFeatureLevel,&Context);
		if(res==S_OK){
			OutputDebugStringA("Device Created\n");
			
			UINT VertexBufferArrayCount = 0;
			ID3D11Buffer* 			VertexBufferArray[32] = {0};
			ID3D11Buffer* 			IndexBufferArray[64] 	= {0};
			ID3D11InputLayout*		VSInputLayoutArray[64] = {0};
			ID3D11VertexShader* 	VertexShaderArray[64];
			ID3D11HullShader* 		HullShaderArray[64];
			ID3D11DomainShader* 	DomainShaderArray[64];
			ID3D11GeometryShader* 	GeometryShaderArray[64];
			UINT PixelShaderInArrayCount = 0;
			ID3D11PixelShader* PixelShaderArray[MAX_PIXEL_SHADER_COUNT];
			
			
			//retrieve IDXGI Interfaces
			IDXGIDevice1 *IdxgiDevice = NULL;
			IDXGIAdapter *IdxgiAdapter = NULL;
			IDXGIFactory2 *IdxgiFactory = NULL;
			
			Win32GetIDXGIInterfacesFromD3DDevice(Device, &IdxgiDevice, &IdxgiAdapter, &IdxgiFactory);

			//Swap Chain
			SwapChain = Win32GetSwapChain(Device,Window,IdxgiFactory);
			
			IndexedGeometryObject IndexedGeometryArray[64];
			unsigned int IndexedGeometryCount = 0;
			
			
			
			
			
			float CubeVertices[7*8]={
				/*Pos*/ -0.50f,-0.50f, 0.00f, /*COLOR*/ 0.00f, 0.00f, 0.00f, 1.00f,
				/*Pos*/  0.50f,-0.50f, 0.00f, /*COLOR*/ 0.00f, 0.00f, 1.00f, 1.00f,
				/*Pos*/ -0.50f, 0.50f, 0.00f, /*COLOR*/ 0.00f, 1.00f, 0.00f, 1.00f,
				/*Pos*/  0.50f, 0.50f, 0.00f, /*COLOR*/ 0.00f, 1.00f, 1.00f, 1.00f,
				/*Pos*/ -0.50f,-0.50f, 1.00f, /*COLOR*/ 1.00f, 0.00f, 0.00f, 1.00f,
				/*Pos*/  0.50f,-0.50f, 1.00f, /*COLOR*/ 1.00f, 0.00f, 1.00f, 1.00f,
				/*Pos*/ -0.50f, 0.50f, 1.00f, /*COLOR*/ 1.00f, 1.00f, 0.00f, 1.00f,
				/*Pos*/  0.50f, 0.50f, 1.00f, /*COLOR*/ 1.00f, 1.00f, 1.00f, 1.00f,
			};
			UINT CubeIndices[12*3]={
				//Front Face
				0,2,3,  0,3,1,
				//Back Face
				7,6,4,  7,4,5,
				//Left Face
				4,6,2,  4,2,0,
				//Right Face
				1,3,7,  1,7,5,
				//Top Face
				2,6,7,  2,7,3,
				//Bottom Face
				0,1,4,  4,1,5,
			};
			
			
			CreateVBForIndexedGeometry(
				Device,
				VertexBufferArray,
				&VertexBufferArrayCount,
				IndexBufferArray,
				IndexedGeometryArray,
				&IndexedGeometryCount,
				CubeVertices,
				sizeof(CubeVertices),
				7 * sizeof(float),
				CubeIndices,
				sizeof(CubeIndices),
				1*sizeof(UINT));
				
				
			#define VSPassTrough VertexShaderArray[0]
			#define VSPassTroughInputLayout VSInputLayoutArray[0]
			ID3DBlob *VSBlob = Win32CompileShaderFromFile(L"VertexShaderPassThrough.hlsl","VSEntry","vs_5_0");
			ASSERT(VSBlob);
			void *VSBinaryCode = VSBlob->lpVtbl->GetBufferPointer(VSBlob);
			size_t VSBinarySize = VSBlob->lpVtbl->GetBufferSize(VSBlob);
			
			VSPassTrough = Win32CreateVertexShader(Device,VSBinaryCode,VSBinarySize);
				ASSERT(VSPassTrough);
				VSPassTroughInputLayout = Win32CreateVertexInputLayout(
				Device,
				VSBinaryCode,
				VSBinarySize);
			VSBlob->lpVtbl->Release(VSBlob);
			VSBlob = NULL;
			
			#define VSCube VertexShaderArray[1]
			#define VSCubeInputLayout VSInputLayoutArray[1]
			VSBlob = Win32CompileShaderFromFile(L"VertexShaderCube.hlsl","VSEntry","vs_5_0");
			ASSERT(VSBlob);
			VSBinaryCode = VSBlob->lpVtbl->GetBufferPointer(VSBlob);
			VSBinarySize = VSBlob->lpVtbl->GetBufferSize(VSBlob);
			VSCube = Win32CreateVertexShader(Device,VSBinaryCode,VSBinarySize);
				ASSERT(VSCube);
				VSCubeInputLayout = Win32CreateVertexInputLayout(
				Device,
				VSBinaryCode,
				VSBinarySize);
			VSBlob->lpVtbl->Release(VSBlob);
			VSBlob = NULL;
			
		
			
			
			//Adding PixelShaders
			Win32AddPixelShaderToArray(PixelShaderArray,&PixelShaderInArrayCount,Win32CreatePixelShader(Device,L"PixelShaderPassThrough.hlsl","PSEntry","ps_5_0"));
			Win32AddPixelShaderToArray(PixelShaderArray,&PixelShaderInArrayCount,Win32CreatePixelShader(Device,L"PixelShaderCube.hlsl","PSEntry","ps_5_0"));
			
			
			
			
			//HUllShader
			ID3DBlob *HSBlob = Win32CompileShaderFromFile(L"HullShaderCube.hlsl","HSEntry","hs_5_0");
			ASSERT(HSBlob);
			void *HSBinaryCode  = HSBlob->lpVtbl->GetBufferPointer(HSBlob);
			size_t HSBinarySize = HSBlob->lpVtbl->GetBufferSize(HSBlob);
			res = Device->lpVtbl->CreateHullShader(Device,HSBinaryCode,HSBinarySize,NULL,&HullShaderArray[0]);
			ASSERT(res==S_OK);
			HSBlob->lpVtbl->Release(HSBlob);
			HSBlob = NULL;
			
			//DomainShader
			ID3DBlob *DSBlob = Win32CompileShaderFromFile(L"DomainShaderCube.hlsl","DSEntry","ds_5_0");
			ASSERT(DSBlob);
			void *DSBinaryCode  = DSBlob->lpVtbl->GetBufferPointer(DSBlob);
			size_t DSBinarySize = DSBlob->lpVtbl->GetBufferSize(DSBlob);
			res = Device->lpVtbl->CreateDomainShader(Device,DSBinaryCode,DSBinarySize,NULL,&DomainShaderArray[0]);
			ASSERT(res==S_OK);
			DSBlob->lpVtbl->Release(DSBlob);
			DSBlob = NULL;
			
			//GeometryShader
			ID3DBlob *GSBlob = Win32CompileShaderFromFile(L"GeometryShaderCube.hlsl","GSEntry","gs_5_0");
			ASSERT(GSBlob);
			void *GSBinaryCode  = GSBlob->lpVtbl->GetBufferPointer(GSBlob);
			size_t GSBinarySize = GSBlob->lpVtbl->GetBufferSize(GSBlob);
			res = Device->lpVtbl->CreateGeometryShader(Device,GSBinaryCode,GSBinarySize,NULL,&GeometryShaderArray[0]);
			ASSERT(res==S_OK);
			GSBlob->lpVtbl->Release(GSBlob);
			GSBlob = NULL;
			
			
			//Rasterizer
			D3D11_RASTERIZER_DESC RasterizerDesc1;
			RasterizerDesc1.FillMode = D3D11_FILL_SOLID;
			RasterizerDesc1.CullMode = D3D11_CULL_BACK;
			RasterizerDesc1.FrontCounterClockwise = FALSE;
			RasterizerDesc1.DepthBias = 0;
			RasterizerDesc1.DepthBiasClamp = 1.0f;
			RasterizerDesc1.SlopeScaledDepthBias = 0.0f;
			RasterizerDesc1.DepthClipEnable = FALSE;
			RasterizerDesc1.ScissorEnable = FALSE;
			RasterizerDesc1.MultisampleEnable = FALSE;
			RasterizerDesc1.AntialiasedLineEnable = FALSE;
			
			ID3D11RasterizerState *RasterizerState1 = NULL;
			res = Device->lpVtbl->CreateRasterizerState(Device,&RasterizerDesc1,&RasterizerState1);
			ASSERT(RasterizerState1);

			D3D11_RASTERIZER_DESC RasterizerDesc2;
			RasterizerDesc2.FillMode = D3D11_FILL_WIREFRAME;
			RasterizerDesc2.CullMode = D3D11_CULL_BACK;
			RasterizerDesc2.FrontCounterClockwise = FALSE;
			RasterizerDesc2.DepthBias = 0;
			RasterizerDesc2.DepthBiasClamp = 1.0f;
			RasterizerDesc2.SlopeScaledDepthBias = 0.0f;
			RasterizerDesc2.DepthClipEnable = FALSE;
			RasterizerDesc2.ScissorEnable = FALSE;
			RasterizerDesc2.MultisampleEnable = FALSE;
			RasterizerDesc2.AntialiasedLineEnable = FALSE;
			
			ID3D11RasterizerState *RasterizerState2 = NULL;
			res = Device->lpVtbl->CreateRasterizerState(Device,&RasterizerDesc2,&RasterizerState2);
			ASSERT(RasterizerState2);
			
			ID3D11RenderTargetView 	*RenderTargetView = NULL;
			//FrameBuffer
			res = SwapChain->lpVtbl->GetBuffer(SwapChain,0,&IID_ID3D11Resource,(void**)&GlobalFrameBuffer);
			if(res == S_OK){
				ASSERT(GlobalFrameBuffer);
				//RenderTargetView
				res = Device->lpVtbl->CreateRenderTargetView(Device,(ID3D11Resource*)GlobalFrameBuffer,NULL,&RenderTargetView);
				if(res == S_OK){
					ASSERT(RenderTargetView);
					OutputDebugStringA("RTV Success\n");
				}
				else{
					OutputDebugStringA(" RTV failed\n");
				}
				}
			else{
				OutputDebugStringA("SwapChain no Buffer\n");
			}
			
			float ConstantBufferData[64] = {0};
			ConstantBufferData[4] = 1.0f;
			ConstantBufferData[5] = 1.0f;
			ConstantBufferData[8] = 0.0f;
			ConstantBufferData[9] = 1.0f;
			//Buffer for Angle
			D3D11_BUFFER_DESC AngleConstantBufferDesc;
			AngleConstantBufferDesc.ByteWidth = sizeof(ConstantBufferData);
			AngleConstantBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
			AngleConstantBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
			AngleConstantBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
			AngleConstantBufferDesc.MiscFlags = 0;
			AngleConstantBufferDesc.StructureByteStride = 0;
			
			D3D11_SUBRESOURCE_DATA ConstantBufferSubresourceData;
			ConstantBufferSubresourceData.pSysMem = ConstantBufferData;
			ConstantBufferSubresourceData.SysMemPitch = 0;
			ConstantBufferSubresourceData.SysMemSlicePitch = 0;
			ID3D11Buffer *ConstantBuffer = NULL;
			Device->lpVtbl->CreateBuffer(Device,&AngleConstantBufferDesc,&ConstantBufferSubresourceData,&ConstantBuffer);
			ASSERT(ConstantBuffer);
			
			
			GraphicsPipelineState PipelineStateArray[MAX_PIPELINE_STATES];
			unsigned int PipelineStateCount = 0;
			GraphicsPipelineState ActivePipelineStateArray[MAX_PIPELINE_STATES];
			unsigned int ActivePipelineStateCount = 0;
			
			
			
			
			AddPipelineStateToArray(
				PipelineStateArray,
				&PipelineStateCount,
				BuildPipelineState(
					&VertexBufferArray[0],1,
					(UINT*)&IndexedGeometryArray[0].VertexSize,
					(UINT*)&Zero,
					IndexBufferArray[0],DXGI_FORMAT_R32_UINT,ArrayCount(CubeIndices),
					VSPassTroughInputLayout,
					D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST,
					VSPassTrough,
					NULL,0,
					NULL,
					NULL,
					NULL,
					RasterizerState2,
					&PixelShaderArray[0],
					NULL,0,
					&RenderTargetView, 1,
					"0:PassThrough"));
			
			AddPipelineStateToArray(
				PipelineStateArray,
				&PipelineStateCount,
				BuildPipelineState(
					&VertexBufferArray[0],1,
					(UINT*)&IndexedGeometryArray[0].VertexSize,
					(UINT*)&Zero,
					IndexBufferArray[0],DXGI_FORMAT_R32_UINT,ArrayCount(CubeIndices),
					VSPassTroughInputLayout,
					D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST,
					VSCube,
					&ConstantBuffer,1,
					NULL,
					NULL,
					NULL,
					RasterizerState2,
					&PixelShaderArray[0],
					NULL,0,
					&RenderTargetView, 1,
					"1:VSActive"));
					
				
					
			AddPipelineStateToArray(
				PipelineStateArray,
				&PipelineStateCount,
				BuildPipelineState(
					&VertexBufferArray[0],1,
					(UINT*)&IndexedGeometryArray[0].VertexSize,
					(UINT*)&Zero,
					IndexBufferArray[0],DXGI_FORMAT_R32_UINT,ArrayCount(CubeIndices),
					VSPassTroughInputLayout,
					D3D11_PRIMITIVE_TOPOLOGY_3_CONTROL_POINT_PATCHLIST,
					VSCube,
					&ConstantBuffer,1,
					HullShaderArray[0],
					DomainShaderArray[0],
					NULL,
					RasterizerState2,
					&PixelShaderArray[0],
					NULL,0,
					&RenderTargetView, 1,
					"2:Tesselation"));
			
			AddPipelineStateToArray(
				PipelineStateArray,
				&PipelineStateCount,
				BuildPipelineState(
					&VertexBufferArray[0],1,
					(UINT*)&IndexedGeometryArray[0].VertexSize,
					(UINT*)&Zero,
					IndexBufferArray[0],DXGI_FORMAT_R32_UINT,ArrayCount(CubeIndices),
					VSPassTroughInputLayout,
					D3D11_PRIMITIVE_TOPOLOGY_3_CONTROL_POINT_PATCHLIST,
					VSCube,
					&ConstantBuffer,1,
					HullShaderArray[0],
					DomainShaderArray[0],
					GeometryShaderArray[0],
					RasterizerState2,
					&PixelShaderArray[0],
					NULL,0,
					&RenderTargetView, 1,
					"3:GeometryShader"));
			
			AddPipelineStateToArray(
				PipelineStateArray,
				&PipelineStateCount,
				BuildPipelineState(
					&VertexBufferArray[0],1,
					(UINT*)&IndexedGeometryArray[0].VertexSize,
					(UINT*)&Zero,
					IndexBufferArray[0],DXGI_FORMAT_R32_UINT,ArrayCount(CubeIndices),
					VSPassTroughInputLayout,
					D3D11_PRIMITIVE_TOPOLOGY_3_CONTROL_POINT_PATCHLIST,
					VSCube,
					&ConstantBuffer,1,
					HullShaderArray[0],
					DomainShaderArray[0],
					GeometryShaderArray[0],
					RasterizerState1,
					&PixelShaderArray[0],
					NULL,0,
					&RenderTargetView, 1,
					"4:RasterizerSet"));
					
			AddPipelineStateToArray(
				PipelineStateArray,
				&PipelineStateCount,
				BuildPipelineState(
					&VertexBufferArray[0],1,
					(UINT*)&IndexedGeometryArray[0].VertexSize,
					(UINT*)&Zero,
					IndexBufferArray[0],DXGI_FORMAT_R32_UINT,ArrayCount(CubeIndices),
					VSPassTroughInputLayout,
					D3D11_PRIMITIVE_TOPOLOGY_3_CONTROL_POINT_PATCHLIST,
					VSCube,
					&ConstantBuffer,1,
					HullShaderArray[0],
					DomainShaderArray[0],
					GeometryShaderArray[0],
					RasterizerState1,
					&PixelShaderArray[1],
					&ConstantBuffer,1,
					&RenderTargetView, 1,
					"5:PixelShader"));
			
			//ComputeShader
			ComputeShaderState* ActiveCSState = NULL;
			ID3D11ComputeShader* ComputeShaderArray[32];
			ComputeShaderState ComputeShaderStateArray[32];		
			ID3D11Texture2D *CSShaderResource = NULL;
			ID3D11UnorderedAccessView *UAVArray[32];

			
			
			ID3DBlob *CSBlob= Win32CompileShaderFromFile(L"ComputeShaderCube.hlsl","CSEntry","cs_5_0");
			ASSERT(CSBlob);
			void *CSBinaryCode  = CSBlob->lpVtbl->GetBufferPointer(CSBlob);
			size_t CSBinarySize = CSBlob->lpVtbl->GetBufferSize(CSBlob);
			res = Device->lpVtbl->CreateComputeShader(Device,CSBinaryCode,CSBinarySize,NULL,&ComputeShaderArray[0]);
			ASSERT(res==S_OK);
			CSBlob->lpVtbl->Release(CSBlob);
			CSBlob = NULL;
			
			UpdateCSTexture(Device,Width, Height, &CSShaderResource, UAVArray, &RenderTargetView);
			
		
			ComputeShaderStateArray[0].UnorderedAccessViewArray = UAVArray;
			ComputeShaderStateArray[0].UnorderedAccessViewCount = 1;
			ComputeShaderStateArray[0].ComputeShader = ComputeShaderArray[0];
			
			
			
			//Init Active PipelineState
			PushPipelineState(ActivePipelineStateArray,&ActivePipelineStateCount,&PipelineStateArray[0]);
			ActiveCSState = &NILComputeShaderState;


			unsigned int AnimationCount = 1;
			bool Running = true;
			bool VsyncActive = 1;
			bool AnimationIsActive = false;
			ShaderColor ActiveShaderColor = YELLOW;
			
			//Loop
			MessageLoopStateInput State;
			State.Running = &Running;
			State.VsyncActive = &VsyncActive;
			State.AnimationIsActive = &AnimationIsActive;
			State.ActiveShaderColor = &ActiveShaderColor;
			State.PipelineStateArray = PipelineStateArray;
			State.PipelineStateCount = &PipelineStateCount;
			State.ActivePipelineStateArray = ActivePipelineStateArray;
			State.ActivePipelineStateCount = &ActivePipelineStateCount;
			State.ActiveCSState = &ActiveCSState;
			State.ComputeShaderStateArray = ComputeShaderStateArray;
			
			
			while(Running){
				
				MessageLoop(Device,ConstantBufferData,&State);
				if(!Running) break;
				
				ASSERT(GetClientRect(Window,&ClientRect));
				UINT NewWidth  = ClientRect.right - ClientRect.left;
				UINT NewHeight = ClientRect.bottom - ClientRect.top;
				if(NewHeight * NewWidth == 0){
					continue;
				}
				if(Width != NewWidth || Height!= NewHeight){
					Width = NewWidth;
					Height = NewHeight;
					ResizeSwapChainBuffers(Device,Context,SwapChain,Width,Height,&RenderTargetView);
					UpdateCSTexture(Device,Width,Height,&CSShaderResource,UAVArray,&RenderTargetView);
				}
				
				D3D11_VIEWPORT ViewPort;
				ViewPort.TopLeftX = 0.0f;
				ViewPort.TopLeftY = 0.0f;
				ViewPort.Width = (float)Width;
				ViewPort.Height = (float)Height;
				ViewPort.MinDepth = 0.0f;
				ViewPort.MaxDepth = 1.0f;

				float const BackgroundClearColorRGBA[4] = {1,1,1,1};
				Context->lpVtbl->ClearRenderTargetView(Context,RenderTargetView, BackgroundClearColorRGBA);
				
				ConstantBufferData[1]=(float)Width;
				ConstantBufferData[2]=(float)Height;
				
				D3D11_MAPPED_SUBRESOURCE AngleSubresource = {0};
					Context->lpVtbl->Map(Context,(ID3D11Resource*)ConstantBuffer,0,D3D11_MAP_WRITE_DISCARD,0,&AngleSubresource);
					memcpy(AngleSubresource.pData,ConstantBufferData,sizeof(ConstantBufferData));
					
					Context->lpVtbl->Unmap(Context,(ID3D11Resource*)ConstantBuffer,0);
				
				
				for(unsigned int i = 0; i< ActivePipelineStateCount;i++){
					UINT DrawIndexCount = SetPipelineState(Context,&ActivePipelineStateArray[i],&ViewPort,1,NULL,0);
					ASSERT(DrawIndexCount);
					Context->lpVtbl->DrawIndexed(Context,DrawIndexCount,0,0);
				}
				if(ActiveCSState->ComputeShader){
					Context->lpVtbl->CopyResource(Context,(ID3D11Resource*)CSShaderResource, (ID3D11Resource*)GlobalFrameBuffer);
					SetComputeShaderState(Context,ActiveCSState);
					Context->lpVtbl->Dispatch(Context,Width,Height,1);
					Context->lpVtbl->CopyResource(Context,(ID3D11Resource*)GlobalFrameBuffer,(ID3D11Resource*)CSShaderResource);
				}
				
				SwapChain->lpVtbl->Present(SwapChain,VsyncActive, 0);
				
				if (AnimationIsActive) {
					AnimationCount = (AnimationCount + 1) % (60);
					if (AnimationCount == 0) {
						
						
						switch(ActiveShaderColor){
							case YELLOW:{
								ConstantBufferData[4] = 1.0f;
								ConstantBufferData[5] = 0.0f;
								ConstantBufferData[8] = 0.0f;
								ConstantBufferData[9] = 1.0f;
								
							}break;
							case RED:{
								ConstantBufferData[4] = 0.0f;
								ConstantBufferData[5] = 1.0f;
								ConstantBufferData[8] = 0.0f;
								ConstantBufferData[9] = 1.0f;
								
							}break;
							case GREEN:{
								ConstantBufferData[4] = 0.0f;
								ConstantBufferData[5] = 0.0f;
								ConstantBufferData[8] = 1.0f;
								ConstantBufferData[9] = 1.0f;
							}break;
							case BLUE:{
								ConstantBufferData[4] = 1.0f;
								ConstantBufferData[5] = 1.0f;
								ConstantBufferData[8] = 0.0f;
								ConstantBufferData[9] = 1.0f;
							}break;
						}
						CycleShaderColors(&ActiveShaderColor);
					}
					ConstantBufferData[0] = (float)fmod((ConstantBufferData[0]+ 0.5f),360.0f);
				}
			}
			
			#define ReleaseCOMArray(x)\
			for(int i = 0; i < ArrayCount(x); i++){\
				if((x)[i]){\
					(x)[i]->lpVtbl->Release((x)[i]);\
					(x)[i]=NULL;\
				}\
			}
			
			ReleaseCOMArray(VertexBufferArray);
			ReleaseCOMArray(IndexBufferArray);
			ReleaseCOMArray(VSInputLayoutArray);
			ReleaseCOMArray(VertexShaderArray);
			ReleaseCOMArray(HullShaderArray);
			ReleaseCOMArray(DomainShaderArray);
			ReleaseCOMArray(GeometryShaderArray);
			ReleaseCOMArray(PixelShaderArray);
			ReleaseCOMArray(ComputeShaderArray);
			ReleaseCOMArray(UAVArray);
			
			if(RasterizerState1){
				RasterizerState1->lpVtbl->Release(RasterizerState1);
				RasterizerState1 = NULL;
			}
			if(RasterizerState2){
				RasterizerState2->lpVtbl->Release(RasterizerState2);
				RasterizerState2 = NULL;
			}
			if(ConstantBuffer){
				ConstantBuffer->lpVtbl->Release(ConstantBuffer);
				ConstantBuffer = NULL;
			}
			
			if(IdxgiDevice){
				IdxgiDevice->lpVtbl->Release(IdxgiDevice);
				IdxgiDevice = NULL;
			}
			if(IdxgiAdapter){
				IdxgiAdapter->lpVtbl->Release(IdxgiAdapter);
				IdxgiAdapter = NULL;
			}
			if(IdxgiFactory){
				IdxgiFactory->lpVtbl->Release(IdxgiFactory);
				IdxgiFactory = NULL;
			}
			if(RenderTargetView){
				RenderTargetView->lpVtbl->Release(RenderTargetView);
				RenderTargetView = NULL;
			}
			if(CSShaderResource){
				CSShaderResource->lpVtbl->Release(CSShaderResource);
				CSShaderResource = NULL;
			}

			ASSERT(1);

		}
		else{
			Win32ProcessError(GetLastError());
		}
		if(Device){
			Device->lpVtbl->Release(Device);
			Device = NULL;
		}
		if(SwapChain){
			SwapChain->lpVtbl->Release(SwapChain);
			Device = NULL;
		}
		if(Context){
			Context->lpVtbl->Release(Context);
			Device = NULL;
		}
		if(GlobalFrameBuffer){
			GlobalFrameBuffer->lpVtbl->Release(GlobalFrameBuffer);
			GlobalFrameBuffer = NULL;
		}
		DestroyWindow(Window);
	}
	else{
		Win32ProcessError(GetLastError());	
	}
		
		
} 