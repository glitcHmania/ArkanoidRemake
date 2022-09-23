/******************************************************************************************
*	Chili DirectX Framework Version 16.07.20											  *
*	Graphics.cpp																		  *
*	Copyright 2016 PlanetChili.net <http://www.planetchili.net>							  *
*																						  *
*	This file is part of The Chili DirectX Framework.									  *
*																						  *
*	The Chili DirectX Framework is free software: you can redistribute it and/or modify	  *
*	it under the terms of the GNU General Public License as published by				  *
*	the Free Software Foundation, either version 3 of the License, or					  *
*	(at your option) any later version.													  *
*																						  *
*	The Chili DirectX Framework is distributed in the hope that it will be useful,		  *
*	but WITHOUT ANY WARRANTY; without even the implied warranty of						  *
*	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the						  *
*	GNU General Public License for more details.										  *
*																						  *
*	You should have received a copy of the GNU General Public License					  *
*	along with The Chili DirectX Framework.  If not, see <http://www.gnu.org/licenses/>.  *
******************************************************************************************/
#include "MainWindow.h"
#include "Graphics.h"
#include "DXErr.h"
#include "ChiliException.h"
#include <assert.h>
#include <string>
#include <array>

// Ignore the intellisense error "cannot open source file" for .shh files.
// They will be created during the build sequence before the preprocessor runs.
namespace FramebufferShaders
{
#include "FramebufferPS.shh"
#include "FramebufferVS.shh"
}

#pragma comment( lib,"d3d11.lib" )

#define CHILI_GFX_EXCEPTION( hr,note ) Graphics::Exception( hr,note,_CRT_WIDE(__FILE__),__LINE__ )

using Microsoft::WRL::ComPtr;

Graphics::Graphics( HWNDKey& key )
{
	assert( key.hWnd != nullptr );

	//////////////////////////////////////////////////////
	// create device and swap chain/get render target view
	DXGI_SWAP_CHAIN_DESC sd = {};
	sd.BufferCount = 1;
	sd.BufferDesc.Width = Graphics::ScreenWidth;
	sd.BufferDesc.Height = Graphics::ScreenHeight;
	sd.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	sd.BufferDesc.RefreshRate.Numerator = 1;
	sd.BufferDesc.RefreshRate.Denominator = 60;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.OutputWindow = key.hWnd;
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;
	sd.Windowed = TRUE;

	HRESULT				hr;
	UINT				createFlags = 0u;
#ifdef CHILI_USE_D3D_DEBUG_LAYER
#ifdef _DEBUG
	createFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif
#endif
	
	// create device and front/back buffers
	if( FAILED( hr = D3D11CreateDeviceAndSwapChain( 
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE,
		nullptr,
		createFlags,
		nullptr,
		0,
		D3D11_SDK_VERSION,
		&sd,
		&pSwapChain,
		&pDevice,
		nullptr,
		&pImmediateContext ) ) )
	{
		throw CHILI_GFX_EXCEPTION( hr,L"Creating device and swap chain" );
	}

	// get handle to backbuffer
	ComPtr<ID3D11Resource> pBackBuffer;
	if( FAILED( hr = pSwapChain->GetBuffer(
		0,
		__uuidof( ID3D11Texture2D ),
		(LPVOID*)&pBackBuffer ) ) )
	{
		throw CHILI_GFX_EXCEPTION( hr,L"Getting back buffer" );
	}

	// create a view on backbuffer that we can render to
	if( FAILED( hr = pDevice->CreateRenderTargetView( 
		pBackBuffer.Get(),
		nullptr,
		&pRenderTargetView ) ) )
	{
		throw CHILI_GFX_EXCEPTION( hr,L"Creating render target view on backbuffer" );
	}


	// set backbuffer as the render target using created view
	pImmediateContext->OMSetRenderTargets( 1,pRenderTargetView.GetAddressOf(),nullptr );


	// set viewport dimensions
	D3D11_VIEWPORT vp;
	vp.Width = float( Graphics::ScreenWidth );
	vp.Height = float( Graphics::ScreenHeight );
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0.0f;
	vp.TopLeftY = 0.0f;
	pImmediateContext->RSSetViewports( 1,&vp );


	///////////////////////////////////////
	// create texture for cpu render target
	D3D11_TEXTURE2D_DESC sysTexDesc;
	sysTexDesc.Width = Graphics::ScreenWidth;
	sysTexDesc.Height = Graphics::ScreenHeight;
	sysTexDesc.MipLevels = 1;
	sysTexDesc.ArraySize = 1;
	sysTexDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	sysTexDesc.SampleDesc.Count = 1;
	sysTexDesc.SampleDesc.Quality = 0;
	sysTexDesc.Usage = D3D11_USAGE_DYNAMIC;
	sysTexDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	sysTexDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	sysTexDesc.MiscFlags = 0;
	// create the texture
	if( FAILED( hr = pDevice->CreateTexture2D( &sysTexDesc,nullptr,&pSysBufferTexture ) ) )
	{
		throw CHILI_GFX_EXCEPTION( hr,L"Creating sysbuffer texture" );
	}

	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.Format = sysTexDesc.Format;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = 1;
	// create the resource view on the texture
	if( FAILED( hr = pDevice->CreateShaderResourceView( pSysBufferTexture.Get(),
		&srvDesc,&pSysBufferTextureView ) ) )
	{
		throw CHILI_GFX_EXCEPTION( hr,L"Creating view on sysBuffer texture" );
	}


	////////////////////////////////////////////////
	// create pixel shader for framebuffer
	// Ignore the intellisense error "namespace has no member"
	if( FAILED( hr = pDevice->CreatePixelShader(
		FramebufferShaders::FramebufferPSBytecode,
		sizeof( FramebufferShaders::FramebufferPSBytecode ),
		nullptr,
		&pPixelShader ) ) )
	{
		throw CHILI_GFX_EXCEPTION( hr,L"Creating pixel shader" );
	}
	

	/////////////////////////////////////////////////
	// create vertex shader for framebuffer
	// Ignore the intellisense error "namespace has no member"
	if( FAILED( hr = pDevice->CreateVertexShader(
		FramebufferShaders::FramebufferVSBytecode,
		sizeof( FramebufferShaders::FramebufferVSBytecode ),
		nullptr,
		&pVertexShader ) ) )
	{
		throw CHILI_GFX_EXCEPTION( hr,L"Creating vertex shader" );
	}
	

	//////////////////////////////////////////////////////////////
	// create and fill vertex buffer with quad for rendering frame
	const FSQVertex vertices[] =
	{
		{ -1.0f,1.0f,0.5f,0.0f,0.0f },
		{ 1.0f,1.0f,0.5f,1.0f,0.0f },
		{ 1.0f,-1.0f,0.5f,1.0f,1.0f },
		{ -1.0f,1.0f,0.5f,0.0f,0.0f },
		{ 1.0f,-1.0f,0.5f,1.0f,1.0f },
		{ -1.0f,-1.0f,0.5f,0.0f,1.0f },
	};
	D3D11_BUFFER_DESC bd = {};
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof( FSQVertex ) * 6;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0u;
	D3D11_SUBRESOURCE_DATA initData = {};
	initData.pSysMem = vertices;
	if( FAILED( hr = pDevice->CreateBuffer( &bd,&initData,&pVertexBuffer ) ) )
	{
		throw CHILI_GFX_EXCEPTION( hr,L"Creating vertex buffer" );
	}

	
	//////////////////////////////////////////
	// create input layout for fullscreen quad
	const D3D11_INPUT_ELEMENT_DESC ied[] =
	{
		{ "POSITION",0,DXGI_FORMAT_R32G32B32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0 },
		{ "TEXCOORD",0,DXGI_FORMAT_R32G32_FLOAT,0,12,D3D11_INPUT_PER_VERTEX_DATA,0 }
	};

	// Ignore the intellisense error "namespace has no member"
	if( FAILED( hr = pDevice->CreateInputLayout( ied,2,
		FramebufferShaders::FramebufferVSBytecode,
		sizeof( FramebufferShaders::FramebufferVSBytecode ),
		&pInputLayout ) ) )
	{
		throw CHILI_GFX_EXCEPTION( hr,L"Creating input layout" );
	}


	////////////////////////////////////////////////////
	// Create sampler state for fullscreen textured quad
	D3D11_SAMPLER_DESC sampDesc = {};
	sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
	sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
	sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	sampDesc.MinLOD = 0;
	sampDesc.MaxLOD = D3D11_FLOAT32_MAX;
	if( FAILED( hr = pDevice->CreateSamplerState( &sampDesc,&pSamplerState ) ) )
	{
		throw CHILI_GFX_EXCEPTION( hr,L"Creating sampler state" );
	}

	// allocate memory for sysbuffer (16-byte aligned for faster access)
	pSysBuffer = reinterpret_cast<Color*>( 
		_aligned_malloc( sizeof( Color ) * Graphics::ScreenWidth * Graphics::ScreenHeight,16u ) );
}

Graphics::~Graphics()
{
	// free sysbuffer memory (aligned free)
	if( pSysBuffer )
	{
		_aligned_free( pSysBuffer );
		pSysBuffer = nullptr;
	}
	// clear the state of the device context before destruction
	if( pImmediateContext ) pImmediateContext->ClearState();
}

void Graphics::EndFrame()
{
	HRESULT hr;

	// lock and map the adapter memory for copying over the sysbuffer
	if( FAILED( hr = pImmediateContext->Map( pSysBufferTexture.Get(),0u,
		D3D11_MAP_WRITE_DISCARD,0u,&mappedSysBufferTexture ) ) )
	{
		throw CHILI_GFX_EXCEPTION( hr,L"Mapping sysbuffer" );
	}
	// setup parameters for copy operation
	Color* pDst = reinterpret_cast<Color*>(mappedSysBufferTexture.pData );
	const size_t dstPitch = mappedSysBufferTexture.RowPitch / sizeof( Color );
	const size_t srcPitch = Graphics::ScreenWidth;
	const size_t rowBytes = srcPitch * sizeof( Color );
	// perform the copy line-by-line
	for( size_t y = 0u; y < Graphics::ScreenHeight; y++ )
	{
		memcpy( &pDst[ y * dstPitch ],&pSysBuffer[y * srcPitch],rowBytes );
	}
	// release the adapter memory
	pImmediateContext->Unmap( pSysBufferTexture.Get(),0u );

	// render offscreen scene texture to back buffer
	pImmediateContext->IASetInputLayout( pInputLayout.Get() );
	pImmediateContext->VSSetShader( pVertexShader.Get(),nullptr,0u );
	pImmediateContext->PSSetShader( pPixelShader.Get(),nullptr,0u );
	pImmediateContext->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST );
	const UINT stride = sizeof( FSQVertex );
	const UINT offset = 0u;
	pImmediateContext->IASetVertexBuffers( 0u,1u,pVertexBuffer.GetAddressOf(),&stride,&offset );
	pImmediateContext->PSSetShaderResources( 0u,1u,pSysBufferTextureView.GetAddressOf() );
	pImmediateContext->PSSetSamplers( 0u,1u,pSamplerState.GetAddressOf() );
	pImmediateContext->Draw( 6u,0u );

	// flip back/front buffers
	if( FAILED( hr = pSwapChain->Present( 1u,0u ) ) )
	{
		if( hr == DXGI_ERROR_DEVICE_REMOVED )
		{
			throw CHILI_GFX_EXCEPTION( pDevice->GetDeviceRemovedReason(),L"Presenting back buffer [device removed]" );
		}
		else
		{
			throw CHILI_GFX_EXCEPTION( hr,L"Presenting back buffer" );
		}
	}
}

void Graphics::BeginFrame()
{
	// clear the sysbuffer
	memset( pSysBuffer,0u,sizeof( Color ) * Graphics::ScreenHeight * Graphics::ScreenWidth );
}

void Graphics::PutPixel( int x,int y,Color c )
{
	assert( x >= 0 );
	assert( x < int( Graphics::ScreenWidth ) );
	assert( y >= 0 );
	assert( y < int( Graphics::ScreenHeight ) );
	pSysBuffer[Graphics::ScreenWidth * y + x] = c;
}

void Graphics::DrawRect(int x0, int y0, int x1, int y1, Color c)
{
	if (x0 > x1)
	{
		std::swap(x0, x1);
	}
	if (y0 > y1)
	{
		std::swap(y0, y1);
	}

	for (int y = y0; y < y1; ++y)
	{
		for (int x = x0; x < x1; ++x)
		{
			if (x > 0 && y > 0 && x < ScreenWidth && y < ScreenHeight)
			{
				PutPixel(x, y, c);
			}
		}
	}
}

void Graphics::DrawCircle(int x, int y, int radius, Color c)
{
	const int rad_sq = radius * radius;
	for (int y_loop = y - radius + 1; y_loop < y + radius; y_loop++)
	{
		for (int x_loop = x - radius + 1; x_loop < x + radius; x_loop++)
		{
			const int x_diff = x - x_loop;
			const int y_diff = y - y_loop;
			if (x_diff * x_diff + y_diff * y_diff <= rad_sq)
			{
				PutPixel(x_loop, y_loop, c);
			}
		}
	}
}

void Graphics::DrawStartGame(RectF& walls)
{
	int x = int(walls.right + walls.left) / 2 - 25;
	int y = int(walls.top + walls.bottom) / 2 - 25;
	PutPixel(11 + x, 10 + y, 255, 255, 255);
	PutPixel(12 + x, 10 + y, 255, 255, 255);
	PutPixel(13 + x, 10 + y, 255, 255, 255);
	PutPixel(14 + x, 10 + y, 218, 218, 218);
	PutPixel(15 + x, 10 + y, 51, 51, 51);
	PutPixel(17 + x, 10 + y, 255, 255, 255);
	PutPixel(18 + x, 10 + y, 255, 255, 255);
	PutPixel(19 + x, 10 + y, 255, 255, 255);
	PutPixel(20 + x, 10 + y, 255, 255, 255);
	PutPixel(21 + x, 10 + y, 92, 92, 92);
	PutPixel(23 + x, 10 + y, 51, 51, 51);
	PutPixel(24 + x, 10 + y, 255, 255, 255);
	PutPixel(25 + x, 10 + y, 255, 255, 255);
	PutPixel(26 + x, 10 + y, 255, 255, 255);
	PutPixel(27 + x, 10 + y, 255, 255, 255);
	PutPixel(28 + x, 10 + y, 51, 51, 51);
	PutPixel(29 + x, 10 + y, 38, 38, 38);
	PutPixel(30 + x, 10 + y, 174, 174, 174);
	PutPixel(31 + x, 10 + y, 255, 255, 255);
	PutPixel(32 + x, 10 + y, 255, 255, 255);
	PutPixel(33 + x, 10 + y, 156, 156, 156);
	PutPixel(35 + x, 10 + y, 38, 38, 38);
	PutPixel(36 + x, 10 + y, 174, 174, 174);
	PutPixel(37 + x, 10 + y, 255, 255, 255);
	PutPixel(38 + x, 10 + y, 255, 255, 255);
	PutPixel(39 + x, 10 + y, 156, 156, 156);
	PutPixel(11 + x, 11 + y, 255, 255, 255);
	PutPixel(12 + x, 11 + y, 174, 174, 174);
	PutPixel(13 + x, 11 + y, 12, 12, 12);
	PutPixel(14 + x, 11 + y, 218, 218, 218);
	PutPixel(15 + x, 11 + y, 218, 218, 218);
	PutPixel(17 + x, 11 + y, 255, 255, 255);
	PutPixel(18 + x, 11 + y, 107, 107, 107);
	PutPixel(20 + x, 11 + y, 195, 195, 195);
	PutPixel(21 + x, 11 + y, 255, 255, 255);
	PutPixel(23 + x, 11 + y, 51, 51, 51);
	PutPixel(24 + x, 11 + y, 255, 255, 255);
	PutPixel(25 + x, 11 + y, 51, 51, 51);
	PutPixel(29 + x, 11 + y, 156, 156, 156);
	PutPixel(30 + x, 11 + y, 218, 218, 218);
	PutPixel(31 + x, 11 + y, 12, 12, 12);
	PutPixel(32 + x, 11 + y, 12, 12, 12);
	PutPixel(33 + x, 11 + y, 139, 139, 139);
	PutPixel(35 + x, 11 + y, 156, 156, 156);
	PutPixel(36 + x, 11 + y, 218, 218, 218);
	PutPixel(37 + x, 11 + y, 12, 12, 12);
	PutPixel(38 + x, 11 + y, 12, 12, 12);
	PutPixel(39 + x, 11 + y, 139, 139, 139);
	PutPixel(11 + x, 12 + y, 255, 255, 255);
	PutPixel(12 + x, 12 + y, 174, 174, 174);
	PutPixel(14 + x, 12 + y, 174, 174, 174);
	PutPixel(15 + x, 12 + y, 255, 255, 255);
	PutPixel(17 + x, 12 + y, 255, 255, 255);
	PutPixel(18 + x, 12 + y, 107, 107, 107);
	PutPixel(20 + x, 12 + y, 195, 195, 195);
	PutPixel(21 + x, 12 + y, 255, 255, 255);
	PutPixel(23 + x, 12 + y, 51, 51, 51);
	PutPixel(24 + x, 12 + y, 255, 255, 255);
	PutPixel(25 + x, 12 + y, 51, 51, 51);
	PutPixel(29 + x, 12 + y, 195, 195, 195);
	PutPixel(30 + x, 12 + y, 255, 255, 255);
	PutPixel(31 + x, 12 + y, 92, 92, 92);
	PutPixel(32 + x, 12 + y, 38, 38, 38);
	PutPixel(35 + x, 12 + y, 195, 195, 195);
	PutPixel(36 + x, 12 + y, 255, 255, 255);
	PutPixel(37 + x, 12 + y, 92, 92, 92);
	PutPixel(38 + x, 12 + y, 38, 38, 38);
	PutPixel(11 + x, 13 + y, 255, 255, 255);
	PutPixel(12 + x, 13 + y, 174, 174, 174);
	PutPixel(13 + x, 13 + y, 12, 12, 12);
	PutPixel(14 + x, 13 + y, 218, 218, 218);
	PutPixel(15 + x, 13 + y, 174, 174, 174);
	PutPixel(17 + x, 13 + y, 255, 255, 255);
	PutPixel(18 + x, 13 + y, 255, 255, 255);
	PutPixel(19 + x, 13 + y, 255, 255, 255);
	PutPixel(20 + x, 13 + y, 255, 255, 255);
	PutPixel(21 + x, 13 + y, 38, 38, 38);
	PutPixel(23 + x, 13 + y, 51, 51, 51);
	PutPixel(24 + x, 13 + y, 255, 255, 255);
	PutPixel(25 + x, 13 + y, 255, 255, 255);
	PutPixel(26 + x, 13 + y, 255, 255, 255);
	PutPixel(27 + x, 13 + y, 255, 255, 255);
	PutPixel(29 + x, 13 + y, 78, 78, 78);
	PutPixel(30 + x, 13 + y, 255, 255, 255);
	PutPixel(31 + x, 13 + y, 255, 255, 255);
	PutPixel(32 + x, 13 + y, 255, 255, 255);
	PutPixel(33 + x, 13 + y, 174, 174, 174);
	PutPixel(35 + x, 13 + y, 78, 78, 78);
	PutPixel(36 + x, 13 + y, 255, 255, 255);
	PutPixel(37 + x, 13 + y, 255, 255, 255);
	PutPixel(38 + x, 13 + y, 255, 255, 255);
	PutPixel(39 + x, 13 + y, 174, 174, 174);
	PutPixel(6 + x, 14 + y, 1, 0, 0);
	PutPixel(11 + x, 14 + y, 255, 255, 255);
	PutPixel(12 + x, 14 + y, 255, 255, 255);
	PutPixel(13 + x, 14 + y, 255, 255, 255);
	PutPixel(14 + x, 14 + y, 174, 174, 174);
	PutPixel(15 + x, 14 + y, 38, 38, 38);
	PutPixel(17 + x, 14 + y, 255, 255, 255);
	PutPixel(18 + x, 14 + y, 107, 107, 107);
	PutPixel(19 + x, 14 + y, 92, 92, 92);
	PutPixel(20 + x, 14 + y, 255, 255, 255);
	PutPixel(21 + x, 14 + y, 64, 64, 64);
	PutPixel(23 + x, 14 + y, 51, 51, 51);
	PutPixel(24 + x, 14 + y, 255, 255, 255);
	PutPixel(25 + x, 14 + y, 51, 51, 51);
	PutPixel(30 + x, 14 + y, 12, 12, 12);
	PutPixel(31 + x, 14 + y, 64, 64, 64);
	PutPixel(32 + x, 14 + y, 195, 195, 195);
	PutPixel(33 + x, 14 + y, 255, 255, 255);
	PutPixel(34 + x, 14 + y, 51, 51, 51);
	PutPixel(36 + x, 14 + y, 12, 12, 12);
	PutPixel(37 + x, 14 + y, 64, 64, 64);
	PutPixel(38 + x, 14 + y, 195, 195, 195);
	PutPixel(39 + x, 14 + y, 255, 255, 255);
	PutPixel(40 + x, 14 + y, 51, 51, 51);
	PutPixel(11 + x, 15 + y, 255, 255, 255);
	PutPixel(12 + x, 15 + y, 174, 174, 174);
	PutPixel(17 + x, 15 + y, 255, 255, 255);
	PutPixel(18 + x, 15 + y, 107, 107, 107);
	PutPixel(20 + x, 15 + y, 195, 195, 195);
	PutPixel(21 + x, 15 + y, 255, 255, 255);
	PutPixel(22 + x, 15 + y, 12, 12, 12);
	PutPixel(23 + x, 15 + y, 51, 51, 51);
	PutPixel(24 + x, 15 + y, 255, 255, 255);
	PutPixel(25 + x, 15 + y, 51, 51, 51);
	PutPixel(29 + x, 15 + y, 195, 195, 195);
	PutPixel(30 + x, 15 + y, 38, 38, 38);
	PutPixel(32 + x, 15 + y, 139, 139, 139);
	PutPixel(33 + x, 15 + y, 255, 255, 255);
	PutPixel(34 + x, 15 + y, 25, 25, 25);
	PutPixel(35 + x, 15 + y, 195, 195, 195);
	PutPixel(36 + x, 15 + y, 38, 38, 38);
	PutPixel(38 + x, 15 + y, 139, 139, 139);
	PutPixel(39 + x, 15 + y, 255, 255, 255);
	PutPixel(40 + x, 15 + y, 25, 25, 25);
	PutPixel(11 + x, 16 + y, 255, 255, 255);
	PutPixel(12 + x, 16 + y, 174, 174, 174);
	PutPixel(17 + x, 16 + y, 255, 255, 255);
	PutPixel(18 + x, 16 + y, 107, 107, 107);
	PutPixel(20 + x, 16 + y, 38, 38, 38);
	PutPixel(21 + x, 16 + y, 255, 255, 255);
	PutPixel(22 + x, 16 + y, 156, 156, 156);
	PutPixel(23 + x, 16 + y, 51, 51, 51);
	PutPixel(24 + x, 16 + y, 255, 255, 255);
	PutPixel(25 + x, 16 + y, 255, 255, 255);
	PutPixel(26 + x, 16 + y, 255, 255, 255);
	PutPixel(27 + x, 16 + y, 255, 255, 255);
	PutPixel(28 + x, 16 + y, 51, 51, 51);
	PutPixel(29 + x, 16 + y, 139, 139, 139);
	PutPixel(30 + x, 16 + y, 255, 255, 255);
	PutPixel(31 + x, 16 + y, 255, 255, 255);
	PutPixel(32 + x, 16 + y, 218, 218, 218);
	PutPixel(33 + x, 16 + y, 78, 78, 78);
	PutPixel(35 + x, 16 + y, 139, 139, 139);
	PutPixel(36 + x, 16 + y, 255, 255, 255);
	PutPixel(37 + x, 16 + y, 255, 255, 255);
	PutPixel(38 + x, 16 + y, 218, 218, 218);
	PutPixel(39 + x, 16 + y, 78, 78, 78);
	PutPixel(6 + x, 19 + y, 51, 51, 51);
	PutPixel(7 + x, 19 + y, 25, 25, 25);
	PutPixel(8 + x, 19 + y, 51, 51, 51);
	PutPixel(9 + x, 19 + y, 12, 12, 12);
	PutPixel(41 + x, 19 + y, 25, 25, 25);
	PutPixel(42 + x, 19 + y, 51, 51, 51);
	PutPixel(43 + x, 19 + y, 25, 25, 25);
	PutPixel(44 + x, 19 + y, 38, 38, 38);
	PutPixel(6 + x, 20 + y, 218, 218, 218);
	PutPixel(7 + x, 20 + y, 107, 107, 107);
	PutPixel(8 + x, 20 + y, 255, 255, 255);
	PutPixel(9 + x, 20 + y, 51, 51, 51);
	PutPixel(10 + x, 20 + y, 12, 12, 12);
	PutPixel(11 + x, 20 + y, 51, 51, 51);
	PutPixel(12 + x, 20 + y, 51, 51, 51);
	PutPixel(13 + x, 20 + y, 51, 51, 51);
	PutPixel(14 + x, 20 + y, 51, 51, 51);
	PutPixel(15 + x, 20 + y, 12, 12, 12);
	PutPixel(16 + x, 20 + y, 25, 25, 25);
	PutPixel(17 + x, 20 + y, 51, 51, 51);
	PutPixel(18 + x, 20 + y, 25, 25, 25);
	PutPixel(20 + x, 20 + y, 25, 25, 25);
	PutPixel(21 + x, 20 + y, 51, 51, 51);
	PutPixel(22 + x, 20 + y, 12, 12, 12);
	PutPixel(23 + x, 20 + y, 51, 51, 51);
	PutPixel(24 + x, 20 + y, 51, 51, 51);
	PutPixel(25 + x, 20 + y, 51, 51, 51);
	PutPixel(26 + x, 20 + y, 51, 51, 51);
	PutPixel(27 + x, 20 + y, 51, 51, 51);
	PutPixel(28 + x, 20 + y, 12, 12, 12);
	PutPixel(29 + x, 20 + y, 38, 38, 38);
	PutPixel(30 + x, 20 + y, 51, 51, 51);
	PutPixel(31 + x, 20 + y, 51, 51, 51);
	PutPixel(32 + x, 20 + y, 51, 51, 51);
	PutPixel(33 + x, 20 + y, 38, 38, 38);
	PutPixel(35 + x, 20 + y, 51, 51, 51);
	PutPixel(36 + x, 20 + y, 51, 51, 51);
	PutPixel(37 + x, 20 + y, 51, 51, 51);
	PutPixel(38 + x, 20 + y, 38, 38, 38);
	PutPixel(41 + x, 20 + y, 78, 78, 78);
	PutPixel(42 + x, 20 + y, 255, 255, 255);
	PutPixel(43 + x, 20 + y, 107, 107, 107);
	PutPixel(44 + x, 20 + y, 174, 174, 174);
	PutPixel(6 + x, 21 + y, 174, 174, 174);
	PutPixel(7 + x, 21 + y, 64, 64, 64);
	PutPixel(8 + x, 21 + y, 255, 255, 255);
	PutPixel(9 + x, 21 + y, 51, 51, 51);
	PutPixel(10 + x, 21 + y, 51, 51, 51);
	PutPixel(11 + x, 21 + y, 255, 255, 255);
	PutPixel(12 + x, 21 + y, 195, 195, 195);
	PutPixel(13 + x, 21 + y, 174, 174, 174);
	PutPixel(14 + x, 21 + y, 174, 174, 174);
	PutPixel(15 + x, 21 + y, 38, 38, 38);
	PutPixel(16 + x, 21 + y, 107, 107, 107);
	PutPixel(17 + x, 21 + y, 255, 255, 255);
	PutPixel(18 + x, 21 + y, 174, 174, 174);
	PutPixel(20 + x, 21 + y, 107, 107, 107);
	PutPixel(21 + x, 21 + y, 255, 255, 255);
	PutPixel(22 + x, 21 + y, 38, 38, 38);
	PutPixel(23 + x, 21 + y, 174, 174, 174);
	PutPixel(24 + x, 21 + y, 195, 195, 195);
	PutPixel(25 + x, 21 + y, 255, 255, 255);
	PutPixel(26 + x, 21 + y, 195, 195, 195);
	PutPixel(27 + x, 21 + y, 174, 174, 174);
	PutPixel(28 + x, 21 + y, 38, 38, 38);
	PutPixel(29 + x, 21 + y, 174, 174, 174);
	PutPixel(30 + x, 21 + y, 255, 255, 255);
	PutPixel(31 + x, 21 + y, 174, 174, 174);
	PutPixel(32 + x, 21 + y, 174, 174, 174);
	PutPixel(33 + x, 21 + y, 122, 122, 122);
	PutPixel(35 + x, 21 + y, 255, 255, 255);
	PutPixel(36 + x, 21 + y, 218, 218, 218);
	PutPixel(37 + x, 21 + y, 174, 174, 174);
	PutPixel(38 + x, 21 + y, 255, 255, 255);
	PutPixel(39 + x, 21 + y, 156, 156, 156);
	PutPixel(41 + x, 21 + y, 51, 51, 51);
	PutPixel(42 + x, 21 + y, 195, 195, 195);
	PutPixel(43 + x, 21 + y, 92, 92, 92);
	PutPixel(44 + x, 21 + y, 174, 174, 174);
	PutPixel(6 + x, 22 + y, 122, 122, 122);
	PutPixel(7 + x, 22 + y, 38, 38, 38);
	PutPixel(8 + x, 22 + y, 122, 122, 122);
	PutPixel(9 + x, 22 + y, 25, 25, 25);
	PutPixel(10 + x, 22 + y, 51, 51, 51);
	PutPixel(11 + x, 22 + y, 255, 255, 255);
	PutPixel(12 + x, 22 + y, 51, 51, 51);
	PutPixel(16 + x, 22 + y, 107, 107, 107);
	PutPixel(17 + x, 22 + y, 255, 255, 255);
	PutPixel(18 + x, 22 + y, 195, 195, 195);
	PutPixel(19 + x, 22 + y, 25, 25, 25);
	PutPixel(20 + x, 22 + y, 107, 107, 107);
	PutPixel(21 + x, 22 + y, 255, 255, 255);
	PutPixel(24 + x, 22 + y, 51, 51, 51);
	PutPixel(25 + x, 22 + y, 255, 255, 255);
	PutPixel(26 + x, 22 + y, 51, 51, 51);
	PutPixel(29 + x, 22 + y, 174, 174, 174);
	PutPixel(30 + x, 22 + y, 174, 174, 174);
	PutPixel(35 + x, 22 + y, 255, 255, 255);
	PutPixel(36 + x, 22 + y, 107, 107, 107);
	PutPixel(38 + x, 22 + y, 174, 174, 174);
	PutPixel(39 + x, 22 + y, 255, 255, 255);
	PutPixel(41 + x, 22 + y, 38, 38, 38);
	PutPixel(42 + x, 22 + y, 122, 122, 122);
	PutPixel(43 + x, 22 + y, 38, 38, 38);
	PutPixel(44 + x, 22 + y, 107, 107, 107);
	PutPixel(10 + x, 23 + y, 51, 51, 51);
	PutPixel(11 + x, 23 + y, 255, 255, 255);
	PutPixel(12 + x, 23 + y, 92, 92, 92);
	PutPixel(13 + x, 23 + y, 51, 51, 51);
	PutPixel(14 + x, 23 + y, 51, 51, 51);
	PutPixel(16 + x, 23 + y, 107, 107, 107);
	PutPixel(17 + x, 23 + y, 255, 255, 255);
	PutPixel(18 + x, 23 + y, 78, 78, 78);
	PutPixel(19 + x, 23 + y, 122, 122, 122);
	PutPixel(20 + x, 23 + y, 107, 107, 107);
	PutPixel(21 + x, 23 + y, 255, 255, 255);
	PutPixel(24 + x, 23 + y, 51, 51, 51);
	PutPixel(25 + x, 23 + y, 255, 255, 255);
	PutPixel(26 + x, 23 + y, 51, 51, 51);
	PutPixel(29 + x, 23 + y, 174, 174, 174);
	PutPixel(30 + x, 23 + y, 195, 195, 195);
	PutPixel(31 + x, 23 + y, 51, 51, 51);
	PutPixel(32 + x, 23 + y, 51, 51, 51);
	PutPixel(33 + x, 23 + y, 25, 25, 25);
	PutPixel(35 + x, 23 + y, 255, 255, 255);
	PutPixel(36 + x, 23 + y, 139, 139, 139);
	PutPixel(37 + x, 23 + y, 51, 51, 51);
	PutPixel(38 + x, 23 + y, 218, 218, 218);
	PutPixel(39 + x, 23 + y, 195, 195, 195);
	PutPixel(10 + x, 24 + y, 51, 51, 51);
	PutPixel(11 + x, 24 + y, 255, 255, 255);
	PutPixel(12 + x, 24 + y, 195, 195, 195);
	PutPixel(13 + x, 24 + y, 174, 174, 174);
	PutPixel(14 + x, 24 + y, 174, 174, 174);
	PutPixel(16 + x, 24 + y, 107, 107, 107);
	PutPixel(17 + x, 24 + y, 255, 255, 255);
	PutPixel(19 + x, 24 + y, 218, 218, 218);
	PutPixel(20 + x, 24 + y, 122, 122, 122);
	PutPixel(21 + x, 24 + y, 255, 255, 255);
	PutPixel(24 + x, 24 + y, 51, 51, 51);
	PutPixel(25 + x, 24 + y, 255, 255, 255);
	PutPixel(26 + x, 24 + y, 51, 51, 51);
	PutPixel(29 + x, 24 + y, 174, 174, 174);
	PutPixel(30 + x, 24 + y, 255, 255, 255);
	PutPixel(31 + x, 24 + y, 174, 174, 174);
	PutPixel(32 + x, 24 + y, 174, 174, 174);
	PutPixel(33 + x, 24 + y, 78, 78, 78);
	PutPixel(35 + x, 24 + y, 255, 255, 255);
	PutPixel(36 + x, 24 + y, 218, 218, 218);
	PutPixel(37 + x, 24 + y, 255, 255, 255);
	PutPixel(38 + x, 24 + y, 255, 255, 255);
	PutPixel(39 + x, 24 + y, 12, 12, 12);
	PutPixel(10 + x, 25 + y, 51, 51, 51);
	PutPixel(11 + x, 25 + y, 255, 255, 255);
	PutPixel(12 + x, 25 + y, 51, 51, 51);
	PutPixel(16 + x, 25 + y, 107, 107, 107);
	PutPixel(17 + x, 25 + y, 255, 255, 255);
	PutPixel(19 + x, 25 + y, 107, 107, 107);
	PutPixel(20 + x, 25 + y, 218, 218, 218);
	PutPixel(21 + x, 25 + y, 255, 255, 255);
	PutPixel(24 + x, 25 + y, 51, 51, 51);
	PutPixel(25 + x, 25 + y, 255, 255, 255);
	PutPixel(26 + x, 25 + y, 51, 51, 51);
	PutPixel(29 + x, 25 + y, 174, 174, 174);
	PutPixel(30 + x, 25 + y, 174, 174, 174);
	PutPixel(35 + x, 25 + y, 255, 255, 255);
	PutPixel(36 + x, 25 + y, 107, 107, 107);
	PutPixel(37 + x, 25 + y, 51, 51, 51);
	PutPixel(38 + x, 25 + y, 255, 255, 255);
	PutPixel(39 + x, 25 + y, 107, 107, 107);
	PutPixel(10 + x, 26 + y, 51, 51, 51);
	PutPixel(11 + x, 26 + y, 255, 255, 255);
	PutPixel(12 + x, 26 + y, 92, 92, 92);
	PutPixel(13 + x, 26 + y, 51, 51, 51);
	PutPixel(14 + x, 26 + y, 51, 51, 51);
	PutPixel(15 + x, 26 + y, 12, 12, 12);
	PutPixel(16 + x, 26 + y, 107, 107, 107);
	PutPixel(17 + x, 26 + y, 255, 255, 255);
	PutPixel(19 + x, 26 + y, 25, 25, 25);
	PutPixel(20 + x, 26 + y, 255, 255, 255);
	PutPixel(21 + x, 26 + y, 255, 255, 255);
	PutPixel(24 + x, 26 + y, 51, 51, 51);
	PutPixel(25 + x, 26 + y, 255, 255, 255);
	PutPixel(26 + x, 26 + y, 51, 51, 51);
	PutPixel(29 + x, 26 + y, 174, 174, 174);
	PutPixel(30 + x, 26 + y, 195, 195, 195);
	PutPixel(31 + x, 26 + y, 51, 51, 51);
	PutPixel(32 + x, 26 + y, 51, 51, 51);
	PutPixel(33 + x, 26 + y, 38, 38, 38);
	PutPixel(35 + x, 26 + y, 255, 255, 255);
	PutPixel(36 + x, 26 + y, 107, 107, 107);
	PutPixel(38 + x, 26 + y, 156, 156, 156);
	PutPixel(39 + x, 26 + y, 255, 255, 255);
	PutPixel(40 + x, 26 + y, 38, 38, 38);
	PutPixel(10 + x, 27 + y, 38, 38, 38);
	PutPixel(11 + x, 27 + y, 174, 174, 174);
	PutPixel(12 + x, 27 + y, 174, 174, 174);
	PutPixel(13 + x, 27 + y, 174, 174, 174);
	PutPixel(14 + x, 27 + y, 174, 174, 174);
	PutPixel(15 + x, 27 + y, 38, 38, 38);
	PutPixel(16 + x, 27 + y, 78, 78, 78);
	PutPixel(17 + x, 27 + y, 174, 174, 174);
	PutPixel(20 + x, 27 + y, 122, 122, 122);
	PutPixel(21 + x, 27 + y, 174, 174, 174);
	PutPixel(24 + x, 27 + y, 38, 38, 38);
	PutPixel(25 + x, 27 + y, 174, 174, 174);
	PutPixel(26 + x, 27 + y, 38, 38, 38);
	PutPixel(29 + x, 27 + y, 122, 122, 122);
	PutPixel(30 + x, 27 + y, 174, 174, 174);
	PutPixel(31 + x, 27 + y, 174, 174, 174);
	PutPixel(32 + x, 27 + y, 174, 174, 174);
	PutPixel(33 + x, 27 + y, 122, 122, 122);
	PutPixel(35 + x, 27 + y, 174, 174, 174);
	PutPixel(36 + x, 27 + y, 78, 78, 78);
	PutPixel(38 + x, 27 + y, 12, 12, 12);
	PutPixel(39 + x, 27 + y, 174, 174, 174);
	PutPixel(40 + x, 27 + y, 122, 122, 122);
	PutPixel(49 + x, 27 + y, 1, 1, 1);
	PutPixel(48 + x, 28 + y, 1, 1, 1);
	PutPixel(49 + x, 28 + y, 3, 3, 3);
	PutPixel(49 + x, 29 + y, 1, 1, 1);
	PutPixel(2 + x, 31 + y, 25, 25, 25);
	PutPixel(3 + x, 31 + y, 51, 51, 51);
	PutPixel(4 + x, 31 + y, 51, 51, 51);
	PutPixel(5 + x, 31 + y, 51, 51, 51);
	PutPixel(6 + x, 31 + y, 51, 51, 51);
	PutPixel(7 + x, 31 + y, 51, 51, 51);
	PutPixel(10 + x, 31 + y, 12, 12, 12);
	PutPixel(11 + x, 31 + y, 51, 51, 51);
	PutPixel(12 + x, 31 + y, 38, 38, 38);
	PutPixel(20 + x, 31 + y, 51, 51, 51);
	PutPixel(21 + x, 31 + y, 51, 51, 51);
	PutPixel(22 + x, 31 + y, 12, 12, 12);
	PutPixel(24 + x, 31 + y, 51, 51, 51);
	PutPixel(25 + x, 31 + y, 51, 51, 51);
	PutPixel(26 + x, 31 + y, 51, 51, 51);
	PutPixel(27 + x, 31 + y, 51, 51, 51);
	PutPixel(28 + x, 31 + y, 51, 51, 51);
	PutPixel(29 + x, 31 + y, 25, 25, 25);
	PutPixel(31 + x, 31 + y, 12, 12, 12);
	PutPixel(32 + x, 31 + y, 51, 51, 51);
	PutPixel(33 + x, 31 + y, 25, 25, 25);
	PutPixel(36 + x, 31 + y, 12, 12, 12);
	PutPixel(37 + x, 31 + y, 51, 51, 51);
	PutPixel(38 + x, 31 + y, 51, 51, 51);
	PutPixel(39 + x, 31 + y, 51, 51, 51);
	PutPixel(40 + x, 31 + y, 25, 25, 25);
	PutPixel(42 + x, 31 + y, 12, 12, 12);
	PutPixel(43 + x, 31 + y, 51, 51, 51);
	PutPixel(44 + x, 31 + y, 51, 51, 51);
	PutPixel(45 + x, 31 + y, 51, 51, 51);
	PutPixel(46 + x, 31 + y, 51, 51, 51);
	PutPixel(47 + x, 31 + y, 51, 51, 51);
	PutPixel(48 + x, 31 + y, 12, 12, 12);
	PutPixel(2 + x, 32 + y, 78, 78, 78);
	PutPixel(3 + x, 32 + y, 174, 174, 174);
	PutPixel(4 + x, 32 + y, 218, 218, 218);
	PutPixel(5 + x, 32 + y, 255, 255, 255);
	PutPixel(6 + x, 32 + y, 174, 174, 174);
	PutPixel(7 + x, 32 + y, 174, 174, 174);
	PutPixel(9 + x, 32 + y, 78, 78, 78);
	PutPixel(10 + x, 32 + y, 255, 255, 255);
	PutPixel(11 + x, 32 + y, 195, 195, 195);
	PutPixel(12 + x, 32 + y, 255, 255, 255);
	PutPixel(13 + x, 32 + y, 174, 174, 174);
	PutPixel(14 + x, 32 + y, 12, 12, 12);
	PutPixel(18 + x, 32 + y, 25, 25, 25);
	PutPixel(19 + x, 32 + y, 218, 218, 218);
	PutPixel(20 + x, 32 + y, 218, 218, 218);
	PutPixel(21 + x, 32 + y, 174, 174, 174);
	PutPixel(22 + x, 32 + y, 255, 255, 255);
	PutPixel(23 + x, 32 + y, 38, 38, 38);
	PutPixel(24 + x, 32 + y, 174, 174, 174);
	PutPixel(25 + x, 32 + y, 174, 174, 174);
	PutPixel(26 + x, 32 + y, 255, 255, 255);
	PutPixel(27 + x, 32 + y, 218, 218, 218);
	PutPixel(28 + x, 32 + y, 174, 174, 174);
	PutPixel(29 + x, 32 + y, 78, 78, 78);
	PutPixel(31 + x, 32 + y, 122, 122, 122);
	PutPixel(32 + x, 32 + y, 255, 255, 255);
	PutPixel(33 + x, 32 + y, 156, 156, 156);
	PutPixel(36 + x, 32 + y, 51, 51, 51);
	PutPixel(37 + x, 32 + y, 255, 255, 255);
	PutPixel(38 + x, 32 + y, 195, 195, 195);
	PutPixel(39 + x, 32 + y, 195, 195, 195);
	PutPixel(40 + x, 32 + y, 255, 255, 255);
	PutPixel(41 + x, 32 + y, 92, 92, 92);
	PutPixel(42 + x, 32 + y, 38, 38, 38);
	PutPixel(43 + x, 32 + y, 174, 174, 174);
	PutPixel(44 + x, 32 + y, 195, 195, 195);
	PutPixel(45 + x, 32 + y, 255, 255, 255);
	PutPixel(46 + x, 32 + y, 195, 195, 195);
	PutPixel(47 + x, 32 + y, 174, 174, 174);
	PutPixel(48 + x, 32 + y, 38, 38, 38);
	PutPixel(4 + x, 33 + y, 107, 107, 107);
	PutPixel(5 + x, 33 + y, 255, 255, 255);
	PutPixel(8 + x, 33 + y, 25, 25, 25);
	PutPixel(9 + x, 33 + y, 255, 255, 255);
	PutPixel(10 + x, 33 + y, 139, 139, 139);
	PutPixel(12 + x, 33 + y, 38, 38, 38);
	PutPixel(13 + x, 33 + y, 255, 255, 255);
	PutPixel(14 + x, 33 + y, 107, 107, 107);
	PutPixel(18 + x, 33 + y, 122, 122, 122);
	PutPixel(19 + x, 33 + y, 255, 255, 255);
	PutPixel(20 + x, 33 + y, 12, 12, 12);
	PutPixel(22 + x, 33 + y, 38, 38, 38);
	PutPixel(23 + x, 33 + y, 38, 38, 38);
	PutPixel(26 + x, 33 + y, 255, 255, 255);
	PutPixel(27 + x, 33 + y, 107, 107, 107);
	PutPixel(31 + x, 33 + y, 218, 218, 218);
	PutPixel(32 + x, 33 + y, 218, 218, 218);
	PutPixel(33 + x, 33 + y, 255, 255, 255);
	PutPixel(34 + x, 33 + y, 12, 12, 12);
	PutPixel(36 + x, 33 + y, 51, 51, 51);
	PutPixel(37 + x, 33 + y, 255, 255, 255);
	PutPixel(38 + x, 33 + y, 51, 51, 51);
	PutPixel(40 + x, 33 + y, 255, 255, 255);
	PutPixel(41 + x, 33 + y, 174, 174, 174);
	PutPixel(44 + x, 33 + y, 51, 51, 51);
	PutPixel(45 + x, 33 + y, 255, 255, 255);
	PutPixel(46 + x, 33 + y, 51, 51, 51);
	PutPixel(4 + x, 34 + y, 107, 107, 107);
	PutPixel(5 + x, 34 + y, 255, 255, 255);
	PutPixel(8 + x, 34 + y, 78, 78, 78);
	PutPixel(9 + x, 34 + y, 255, 255, 255);
	PutPixel(10 + x, 34 + y, 51, 51, 51);
	PutPixel(13 + x, 34 + y, 195, 195, 195);
	PutPixel(14 + x, 34 + y, 174, 174, 174);
	PutPixel(18 + x, 34 + y, 107, 107, 107);
	PutPixel(19 + x, 34 + y, 255, 255, 255);
	PutPixel(20 + x, 34 + y, 195, 195, 195);
	PutPixel(21 + x, 34 + y, 107, 107, 107);
	PutPixel(22 + x, 34 + y, 25, 25, 25);
	PutPixel(26 + x, 34 + y, 255, 255, 255);
	PutPixel(27 + x, 34 + y, 107, 107, 107);
	PutPixel(30 + x, 34 + y, 38, 38, 38);
	PutPixel(31 + x, 34 + y, 255, 255, 255);
	PutPixel(32 + x, 34 + y, 78, 78, 78);
	PutPixel(33 + x, 34 + y, 255, 255, 255);
	PutPixel(34 + x, 34 + y, 78, 78, 78);
	PutPixel(36 + x, 34 + y, 51, 51, 51);
	PutPixel(37 + x, 34 + y, 255, 255, 255);
	PutPixel(38 + x, 34 + y, 92, 92, 92);
	PutPixel(39 + x, 34 + y, 64, 64, 64);
	PutPixel(40 + x, 34 + y, 255, 255, 255);
	PutPixel(41 + x, 34 + y, 122, 122, 122);
	PutPixel(44 + x, 34 + y, 51, 51, 51);
	PutPixel(45 + x, 34 + y, 255, 255, 255);
	PutPixel(46 + x, 34 + y, 51, 51, 51);
	PutPixel(4 + x, 35 + y, 107, 107, 107);
	PutPixel(5 + x, 35 + y, 255, 255, 255);
	PutPixel(8 + x, 35 + y, 107, 107, 107);
	PutPixel(9 + x, 35 + y, 255, 255, 255);
	PutPixel(10 + x, 35 + y, 51, 51, 51);
	PutPixel(13 + x, 35 + y, 174, 174, 174);
	PutPixel(14 + x, 35 + y, 174, 174, 174);
	PutPixel(18 + x, 35 + y, 12, 12, 12);
	PutPixel(19 + x, 35 + y, 139, 139, 139);
	PutPixel(20 + x, 35 + y, 255, 255, 255);
	PutPixel(21 + x, 35 + y, 255, 255, 255);
	PutPixel(22 + x, 35 + y, 255, 255, 255);
	PutPixel(23 + x, 35 + y, 51, 51, 51);
	PutPixel(26 + x, 35 + y, 255, 255, 255);
	PutPixel(27 + x, 35 + y, 107, 107, 107);
	PutPixel(30 + x, 35 + y, 122, 122, 122);
	PutPixel(31 + x, 35 + y, 255, 255, 255);
	PutPixel(32 + x, 35 + y, 51, 51, 51);
	PutPixel(33 + x, 35 + y, 255, 255, 255);
	PutPixel(34 + x, 35 + y, 156, 156, 156);
	PutPixel(36 + x, 35 + y, 51, 51, 51);
	PutPixel(37 + x, 35 + y, 255, 255, 255);
	PutPixel(38 + x, 35 + y, 195, 195, 195);
	PutPixel(39 + x, 35 + y, 255, 255, 255);
	PutPixel(40 + x, 35 + y, 195, 195, 195);
	PutPixel(44 + x, 35 + y, 51, 51, 51);
	PutPixel(45 + x, 35 + y, 255, 255, 255);
	PutPixel(46 + x, 35 + y, 51, 51, 51);
	PutPixel(4 + x, 36 + y, 107, 107, 107);
	PutPixel(5 + x, 36 + y, 255, 255, 255);
	PutPixel(8 + x, 36 + y, 51, 51, 51);
	PutPixel(9 + x, 36 + y, 255, 255, 255);
	PutPixel(10 + x, 36 + y, 78, 78, 78);
	PutPixel(13 + x, 36 + y, 255, 255, 255);
	PutPixel(14 + x, 36 + y, 156, 156, 156);
	PutPixel(18 + x, 36 + y, 25, 25, 25);
	PutPixel(21 + x, 36 + y, 107, 107, 107);
	PutPixel(22 + x, 36 + y, 255, 255, 255);
	PutPixel(23 + x, 36 + y, 107, 107, 107);
	PutPixel(26 + x, 36 + y, 255, 255, 255);
	PutPixel(27 + x, 36 + y, 107, 107, 107);
	PutPixel(30 + x, 36 + y, 218, 218, 218);
	PutPixel(31 + x, 36 + y, 218, 218, 218);
	PutPixel(32 + x, 36 + y, 174, 174, 174);
	PutPixel(33 + x, 36 + y, 218, 218, 218);
	PutPixel(34 + x, 36 + y, 255, 255, 255);
	PutPixel(35 + x, 36 + y, 12, 12, 12);
	PutPixel(36 + x, 36 + y, 51, 51, 51);
	PutPixel(37 + x, 36 + y, 255, 255, 255);
	PutPixel(38 + x, 36 + y, 51, 51, 51);
	PutPixel(39 + x, 36 + y, 107, 107, 107);
	PutPixel(40 + x, 36 + y, 255, 255, 255);
	PutPixel(41 + x, 36 + y, 51, 51, 51);
	PutPixel(44 + x, 36 + y, 51, 51, 51);
	PutPixel(45 + x, 36 + y, 255, 255, 255);
	PutPixel(46 + x, 36 + y, 51, 51, 51);
	PutPixel(4 + x, 37 + y, 107, 107, 107);
	PutPixel(5 + x, 37 + y, 255, 255, 255);
	PutPixel(9 + x, 37 + y, 195, 195, 195);
	PutPixel(10 + x, 37 + y, 218, 218, 218);
	PutPixel(11 + x, 37 + y, 64, 64, 64);
	PutPixel(12 + x, 37 + y, 139, 139, 139);
	PutPixel(13 + x, 37 + y, 255, 255, 255);
	PutPixel(14 + x, 37 + y, 51, 51, 51);
	PutPixel(18 + x, 37 + y, 174, 174, 174);
	PutPixel(19 + x, 37 + y, 122, 122, 122);
	PutPixel(20 + x, 37 + y, 51, 51, 51);
	PutPixel(21 + x, 37 + y, 122, 122, 122);
	PutPixel(22 + x, 37 + y, 255, 255, 255);
	PutPixel(23 + x, 37 + y, 51, 51, 51);
	PutPixel(26 + x, 37 + y, 255, 255, 255);
	PutPixel(27 + x, 37 + y, 107, 107, 107);
	PutPixel(29 + x, 37 + y, 25, 25, 25);
	PutPixel(30 + x, 37 + y, 255, 255, 255);
	PutPixel(31 + x, 37 + y, 92, 92, 92);
	PutPixel(33 + x, 37 + y, 64, 64, 64);
	PutPixel(34 + x, 37 + y, 255, 255, 255);
	PutPixel(35 + x, 37 + y, 78, 78, 78);
	PutPixel(36 + x, 37 + y, 51, 51, 51);
	PutPixel(37 + x, 37 + y, 255, 255, 255);
	PutPixel(38 + x, 37 + y, 51, 51, 51);
	PutPixel(39 + x, 37 + y, 12, 12, 12);
	PutPixel(40 + x, 37 + y, 218, 218, 218);
	PutPixel(41 + x, 37 + y, 218, 218, 218);
	PutPixel(42 + x, 37 + y, 12, 12, 12);
	PutPixel(44 + x, 37 + y, 51, 51, 51);
	PutPixel(45 + x, 37 + y, 255, 255, 255);
	PutPixel(46 + x, 37 + y, 51, 51, 51);
	PutPixel(4 + x, 38 + y, 78, 78, 78);
	PutPixel(5 + x, 38 + y, 174, 174, 174);
	PutPixel(9 + x, 38 + y, 12, 12, 12);
	PutPixel(10 + x, 38 + y, 122, 122, 122);
	PutPixel(11 + x, 38 + y, 174, 174, 174);
	PutPixel(12 + x, 38 + y, 156, 156, 156);
	PutPixel(13 + x, 38 + y, 51, 51, 51);
	PutPixel(18 + x, 38 + y, 51, 51, 51);
	PutPixel(19 + x, 38 + y, 139, 139, 139);
	PutPixel(20 + x, 38 + y, 174, 174, 174);
	PutPixel(21 + x, 38 + y, 156, 156, 156);
	PutPixel(22 + x, 38 + y, 64, 64, 64);
	PutPixel(26 + x, 38 + y, 174, 174, 174);
	PutPixel(27 + x, 38 + y, 78, 78, 78);
	PutPixel(29 + x, 38 + y, 64, 64, 64);
	PutPixel(30 + x, 38 + y, 174, 174, 174);
	PutPixel(31 + x, 38 + y, 25, 25, 25);
	PutPixel(33 + x, 38 + y, 12, 12, 12);
	PutPixel(34 + x, 38 + y, 174, 174, 174);
	PutPixel(35 + x, 38 + y, 107, 107, 107);
	PutPixel(36 + x, 38 + y, 38, 38, 38);
	PutPixel(37 + x, 38 + y, 174, 174, 174);
	PutPixel(38 + x, 38 + y, 38, 38, 38);
	PutPixel(40 + x, 38 + y, 51, 51, 51);
	PutPixel(41 + x, 38 + y, 174, 174, 174);
	PutPixel(42 + x, 38 + y, 78, 78, 78);
	PutPixel(44 + x, 38 + y, 38, 38, 38);
	PutPixel(45 + x, 38 + y, 174, 174, 174);
	PutPixel(46 + x, 38 + y, 38, 38, 38);

}

void Graphics::DrawGameOver(RectF& walls)
{
	int x = int(walls.right + walls.left) / 2 - 25;
	int y = int(walls.top + walls.bottom) / 2 - 25;
	PutPixel(3 + x, 8 + y, 12, 12, 12);
	PutPixel(4 + x, 8 + y, 92, 92, 92);
	PutPixel(5 + x, 8 + y, 195, 195, 195);
	PutPixel(6 + x, 8 + y, 255, 255, 255);
	PutPixel(7 + x, 8 + y, 255, 255, 255);
	PutPixel(8 + x, 8 + y, 255, 255, 255);
	PutPixel(9 + x, 8 + y, 195, 195, 195);
	PutPixel(10 + x, 8 + y, 122, 122, 122);
	PutPixel(11 + x, 8 + y, 38, 38, 38);
	PutPixel(16 + x, 8 + y, 64, 64, 64);
	PutPixel(17 + x, 8 + y, 255, 255, 255);
	PutPixel(18 + x, 8 + y, 255, 255, 255);
	PutPixel(19 + x, 8 + y, 255, 255, 255);
	PutPixel(20 + x, 8 + y, 38, 38, 38);
	PutPixel(25 + x, 8 + y, 107, 107, 107);
	PutPixel(26 + x, 8 + y, 255, 255, 255);
	PutPixel(27 + x, 8 + y, 255, 255, 255);
	PutPixel(28 + x, 8 + y, 255, 255, 255);
	PutPixel(29 + x, 8 + y, 12, 12, 12);
	PutPixel(33 + x, 8 + y, 38, 38, 38);
	PutPixel(34 + x, 8 + y, 255, 255, 255);
	PutPixel(35 + x, 8 + y, 255, 255, 255);
	PutPixel(36 + x, 8 + y, 255, 255, 255);
	PutPixel(37 + x, 8 + y, 107, 107, 107);
	PutPixel(40 + x, 8 + y, 174, 174, 174);
	PutPixel(41 + x, 8 + y, 255, 255, 255);
	PutPixel(42 + x, 8 + y, 255, 255, 255);
	PutPixel(43 + x, 8 + y, 255, 255, 255);
	PutPixel(44 + x, 8 + y, 255, 255, 255);
	PutPixel(45 + x, 8 + y, 255, 255, 255);
	PutPixel(46 + x, 8 + y, 255, 255, 255);
	PutPixel(47 + x, 8 + y, 255, 255, 255);
	PutPixel(48 + x, 8 + y, 51, 51, 51);
	PutPixel(2 + x, 9 + y, 12, 12, 12);
	PutPixel(3 + x, 9 + y, 195, 195, 195);
	PutPixel(4 + x, 9 + y, 255, 255, 255);
	PutPixel(5 + x, 9 + y, 195, 195, 195);
	PutPixel(6 + x, 9 + y, 51, 51, 51);
	PutPixel(8 + x, 9 + y, 12, 12, 12);
	PutPixel(9 + x, 9 + y, 107, 107, 107);
	PutPixel(10 + x, 9 + y, 255, 255, 255);
	PutPixel(11 + x, 9 + y, 174, 174, 174);
	PutPixel(16 + x, 9 + y, 139, 139, 139);
	PutPixel(17 + x, 9 + y, 255, 255, 255);
	PutPixel(18 + x, 9 + y, 255, 255, 255);
	PutPixel(19 + x, 9 + y, 255, 255, 255);
	PutPixel(20 + x, 9 + y, 122, 122, 122);
	PutPixel(25 + x, 9 + y, 107, 107, 107);
	PutPixel(26 + x, 9 + y, 255, 255, 255);
	PutPixel(27 + x, 9 + y, 255, 255, 255);
	PutPixel(28 + x, 9 + y, 255, 255, 255);
	PutPixel(29 + x, 9 + y, 78, 78, 78);
	PutPixel(33 + x, 9 + y, 139, 139, 139);
	PutPixel(34 + x, 9 + y, 255, 255, 255);
	PutPixel(35 + x, 9 + y, 255, 255, 255);
	PutPixel(36 + x, 9 + y, 255, 255, 255);
	PutPixel(37 + x, 9 + y, 107, 107, 107);
	PutPixel(40 + x, 9 + y, 174, 174, 174);
	PutPixel(41 + x, 9 + y, 255, 255, 255);
	PutPixel(42 + x, 9 + y, 255, 255, 255);
	PutPixel(43 + x, 9 + y, 51, 51, 51);
	PutPixel(2 + x, 10 + y, 195, 195, 195);
	PutPixel(3 + x, 10 + y, 255, 255, 255);
	PutPixel(4 + x, 10 + y, 255, 255, 255);
	PutPixel(5 + x, 10 + y, 25, 25, 25);
	PutPixel(10 + x, 10 + y, 38, 38, 38);
	PutPixel(11 + x, 10 + y, 156, 156, 156);
	PutPixel(16 + x, 10 + y, 255, 255, 255);
	PutPixel(17 + x, 10 + y, 255, 255, 255);
	PutPixel(18 + x, 10 + y, 255, 255, 255);
	PutPixel(19 + x, 10 + y, 255, 255, 255);
	PutPixel(20 + x, 10 + y, 218, 218, 218);
	PutPixel(25 + x, 10 + y, 107, 107, 107);
	PutPixel(26 + x, 10 + y, 255, 255, 255);
	PutPixel(27 + x, 10 + y, 255, 255, 255);
	PutPixel(28 + x, 10 + y, 255, 255, 255);
	PutPixel(29 + x, 10 + y, 195, 195, 195);
	PutPixel(32 + x, 10 + y, 12, 12, 12);
	PutPixel(33 + x, 10 + y, 255, 255, 255);
	PutPixel(34 + x, 10 + y, 255, 255, 255);
	PutPixel(35 + x, 10 + y, 255, 255, 255);
	PutPixel(36 + x, 10 + y, 255, 255, 255);
	PutPixel(37 + x, 10 + y, 107, 107, 107);
	PutPixel(40 + x, 10 + y, 174, 174, 174);
	PutPixel(41 + x, 10 + y, 255, 255, 255);
	PutPixel(42 + x, 10 + y, 255, 255, 255);
	PutPixel(43 + x, 10 + y, 51, 51, 51);
	PutPixel(1 + x, 11 + y, 64, 64, 64);
	PutPixel(2 + x, 11 + y, 255, 255, 255);
	PutPixel(3 + x, 11 + y, 255, 255, 255);
	PutPixel(4 + x, 11 + y, 139, 139, 139);
	PutPixel(15 + x, 11 + y, 64, 64, 64);
	PutPixel(16 + x, 11 + y, 255, 255, 255);
	PutPixel(17 + x, 11 + y, 255, 255, 255);
	PutPixel(18 + x, 11 + y, 195, 195, 195);
	PutPixel(19 + x, 11 + y, 255, 255, 255);
	PutPixel(20 + x, 11 + y, 255, 255, 255);
	PutPixel(21 + x, 11 + y, 38, 38, 38);
	PutPixel(25 + x, 11 + y, 107, 107, 107);
	PutPixel(26 + x, 11 + y, 255, 255, 255);
	PutPixel(27 + x, 11 + y, 255, 255, 255);
	PutPixel(28 + x, 11 + y, 255, 255, 255);
	PutPixel(29 + x, 11 + y, 255, 255, 255);
	PutPixel(30 + x, 11 + y, 25, 25, 25);
	PutPixel(32 + x, 11 + y, 78, 78, 78);
	PutPixel(33 + x, 11 + y, 255, 255, 255);
	PutPixel(34 + x, 11 + y, 255, 255, 255);
	PutPixel(35 + x, 11 + y, 255, 255, 255);
	PutPixel(36 + x, 11 + y, 255, 255, 255);
	PutPixel(37 + x, 11 + y, 107, 107, 107);
	PutPixel(40 + x, 11 + y, 174, 174, 174);
	PutPixel(41 + x, 11 + y, 255, 255, 255);
	PutPixel(42 + x, 11 + y, 255, 255, 255);
	PutPixel(43 + x, 11 + y, 51, 51, 51);
	PutPixel(1 + x, 12 + y, 139, 139, 139);
	PutPixel(2 + x, 12 + y, 255, 255, 255);
	PutPixel(3 + x, 12 + y, 255, 255, 255);
	PutPixel(4 + x, 12 + y, 78, 78, 78);
	PutPixel(15 + x, 12 + y, 139, 139, 139);
	PutPixel(16 + x, 12 + y, 255, 255, 255);
	PutPixel(17 + x, 12 + y, 255, 255, 255);
	PutPixel(18 + x, 12 + y, 51, 51, 51);
	PutPixel(19 + x, 12 + y, 255, 255, 255);
	PutPixel(20 + x, 12 + y, 255, 255, 255);
	PutPixel(21 + x, 12 + y, 122, 122, 122);
	PutPixel(25 + x, 12 + y, 107, 107, 107);
	PutPixel(26 + x, 12 + y, 255, 255, 255);
	PutPixel(27 + x, 12 + y, 255, 255, 255);
	PutPixel(28 + x, 12 + y, 156, 156, 156);
	PutPixel(29 + x, 12 + y, 255, 255, 255);
	PutPixel(30 + x, 12 + y, 122, 122, 122);
	PutPixel(32 + x, 12 + y, 174, 174, 174);
	PutPixel(33 + x, 12 + y, 255, 255, 255);
	PutPixel(34 + x, 12 + y, 139, 139, 139);
	PutPixel(35 + x, 12 + y, 255, 255, 255);
	PutPixel(36 + x, 12 + y, 255, 255, 255);
	PutPixel(37 + x, 12 + y, 107, 107, 107);
	PutPixel(40 + x, 12 + y, 174, 174, 174);
	PutPixel(41 + x, 12 + y, 255, 255, 255);
	PutPixel(42 + x, 12 + y, 255, 255, 255);
	PutPixel(43 + x, 12 + y, 51, 51, 51);
	PutPixel(1 + x, 13 + y, 174, 174, 174);
	PutPixel(2 + x, 13 + y, 255, 255, 255);
	PutPixel(3 + x, 13 + y, 255, 255, 255);
	PutPixel(4 + x, 13 + y, 51, 51, 51);
	PutPixel(15 + x, 13 + y, 255, 255, 255);
	PutPixel(16 + x, 13 + y, 255, 255, 255);
	PutPixel(17 + x, 13 + y, 195, 195, 195);
	PutPixel(19 + x, 13 + y, 255, 255, 255);
	PutPixel(20 + x, 13 + y, 255, 255, 255);
	PutPixel(21 + x, 13 + y, 218, 218, 218);
	PutPixel(25 + x, 13 + y, 107, 107, 107);
	PutPixel(26 + x, 13 + y, 255, 255, 255);
	PutPixel(27 + x, 13 + y, 255, 255, 255);
	PutPixel(28 + x, 13 + y, 78, 78, 78);
	PutPixel(29 + x, 13 + y, 255, 255, 255);
	PutPixel(30 + x, 13 + y, 218, 218, 218);
	PutPixel(31 + x, 13 + y, 25, 25, 25);
	PutPixel(32 + x, 13 + y, 255, 255, 255);
	PutPixel(33 + x, 13 + y, 195, 195, 195);
	PutPixel(34 + x, 13 + y, 107, 107, 107);
	PutPixel(35 + x, 13 + y, 255, 255, 255);
	PutPixel(36 + x, 13 + y, 255, 255, 255);
	PutPixel(37 + x, 13 + y, 107, 107, 107);
	PutPixel(40 + x, 13 + y, 174, 174, 174);
	PutPixel(41 + x, 13 + y, 255, 255, 255);
	PutPixel(42 + x, 13 + y, 255, 255, 255);
	PutPixel(43 + x, 13 + y, 255, 255, 255);
	PutPixel(44 + x, 13 + y, 255, 255, 255);
	PutPixel(45 + x, 13 + y, 255, 255, 255);
	PutPixel(46 + x, 13 + y, 255, 255, 255);
	PutPixel(47 + x, 13 + y, 174, 174, 174);
	PutPixel(1 + x, 14 + y, 174, 174, 174);
	PutPixel(2 + x, 14 + y, 255, 255, 255);
	PutPixel(3 + x, 14 + y, 255, 255, 255);
	PutPixel(4 + x, 14 + y, 51, 51, 51);
	PutPixel(6 + x, 14 + y, 108, 107, 107);
	PutPixel(7 + x, 14 + y, 255, 255, 255);
	PutPixel(8 + x, 14 + y, 255, 255, 255);
	PutPixel(9 + x, 14 + y, 255, 255, 255);
	PutPixel(10 + x, 14 + y, 255, 255, 255);
	PutPixel(11 + x, 14 + y, 174, 174, 174);
	PutPixel(14 + x, 14 + y, 64, 64, 64);
	PutPixel(15 + x, 14 + y, 255, 255, 255);
	PutPixel(16 + x, 14 + y, 255, 255, 255);
	PutPixel(17 + x, 14 + y, 107, 107, 107);
	PutPixel(19 + x, 14 + y, 156, 156, 156);
	PutPixel(20 + x, 14 + y, 255, 255, 255);
	PutPixel(21 + x, 14 + y, 255, 255, 255);
	PutPixel(22 + x, 14 + y, 38, 38, 38);
	PutPixel(25 + x, 14 + y, 107, 107, 107);
	PutPixel(26 + x, 14 + y, 255, 255, 255);
	PutPixel(27 + x, 14 + y, 255, 255, 255);
	PutPixel(28 + x, 14 + y, 51, 51, 51);
	PutPixel(29 + x, 14 + y, 218, 218, 218);
	PutPixel(30 + x, 14 + y, 255, 255, 255);
	PutPixel(31 + x, 14 + y, 195, 195, 195);
	PutPixel(32 + x, 14 + y, 255, 255, 255);
	PutPixel(33 + x, 14 + y, 92, 92, 92);
	PutPixel(34 + x, 14 + y, 107, 107, 107);
	PutPixel(35 + x, 14 + y, 255, 255, 255);
	PutPixel(36 + x, 14 + y, 255, 255, 255);
	PutPixel(37 + x, 14 + y, 107, 107, 107);
	PutPixel(40 + x, 14 + y, 174, 174, 174);
	PutPixel(41 + x, 14 + y, 255, 255, 255);
	PutPixel(42 + x, 14 + y, 255, 255, 255);
	PutPixel(43 + x, 14 + y, 51, 51, 51);
	PutPixel(1 + x, 15 + y, 139, 139, 139);
	PutPixel(2 + x, 15 + y, 255, 255, 255);
	PutPixel(3 + x, 15 + y, 255, 255, 255);
	PutPixel(4 + x, 15 + y, 78, 78, 78);
	PutPixel(8 + x, 15 + y, 51, 51, 51);
	PutPixel(9 + x, 15 + y, 255, 255, 255);
	PutPixel(10 + x, 15 + y, 255, 255, 255);
	PutPixel(11 + x, 15 + y, 174, 174, 174);
	PutPixel(14 + x, 15 + y, 139, 139, 139);
	PutPixel(15 + x, 15 + y, 255, 255, 255);
	PutPixel(16 + x, 15 + y, 255, 255, 255);
	PutPixel(17 + x, 15 + y, 255, 255, 255);
	PutPixel(18 + x, 15 + y, 255, 255, 255);
	PutPixel(19 + x, 15 + y, 255, 255, 255);
	PutPixel(20 + x, 15 + y, 255, 255, 255);
	PutPixel(21 + x, 15 + y, 255, 255, 255);
	PutPixel(22 + x, 15 + y, 122, 122, 122);
	PutPixel(25 + x, 15 + y, 107, 107, 107);
	PutPixel(26 + x, 15 + y, 255, 255, 255);
	PutPixel(27 + x, 15 + y, 255, 255, 255);
	PutPixel(28 + x, 15 + y, 51, 51, 51);
	PutPixel(29 + x, 15 + y, 122, 122, 122);
	PutPixel(30 + x, 15 + y, 255, 255, 255);
	PutPixel(31 + x, 15 + y, 255, 255, 255);
	PutPixel(32 + x, 15 + y, 255, 255, 255);
	PutPixel(33 + x, 15 + y, 25, 25, 25);
	PutPixel(34 + x, 15 + y, 107, 107, 107);
	PutPixel(35 + x, 15 + y, 255, 255, 255);
	PutPixel(36 + x, 15 + y, 255, 255, 255);
	PutPixel(37 + x, 15 + y, 107, 107, 107);
	PutPixel(40 + x, 15 + y, 174, 174, 174);
	PutPixel(41 + x, 15 + y, 255, 255, 255);
	PutPixel(42 + x, 15 + y, 255, 255, 255);
	PutPixel(43 + x, 15 + y, 51, 51, 51);
	PutPixel(1 + x, 16 + y, 78, 78, 78);
	PutPixel(2 + x, 16 + y, 255, 255, 255);
	PutPixel(3 + x, 16 + y, 255, 255, 255);
	PutPixel(4 + x, 16 + y, 139, 139, 139);
	PutPixel(8 + x, 16 + y, 51, 51, 51);
	PutPixel(9 + x, 16 + y, 255, 255, 255);
	PutPixel(10 + x, 16 + y, 255, 255, 255);
	PutPixel(11 + x, 16 + y, 174, 174, 174);
	PutPixel(14 + x, 16 + y, 218, 218, 218);
	PutPixel(15 + x, 16 + y, 255, 255, 255);
	PutPixel(16 + x, 16 + y, 255, 255, 255);
	PutPixel(19 + x, 16 + y, 25, 25, 25);
	PutPixel(20 + x, 16 + y, 255, 255, 255);
	PutPixel(21 + x, 16 + y, 255, 255, 255);
	PutPixel(22 + x, 16 + y, 218, 218, 218);
	PutPixel(25 + x, 16 + y, 107, 107, 107);
	PutPixel(26 + x, 16 + y, 255, 255, 255);
	PutPixel(27 + x, 16 + y, 255, 255, 255);
	PutPixel(28 + x, 16 + y, 51, 51, 51);
	PutPixel(29 + x, 16 + y, 38, 38, 38);
	PutPixel(30 + x, 16 + y, 255, 255, 255);
	PutPixel(31 + x, 16 + y, 255, 255, 255);
	PutPixel(32 + x, 16 + y, 195, 195, 195);
	PutPixel(34 + x, 16 + y, 107, 107, 107);
	PutPixel(35 + x, 16 + y, 255, 255, 255);
	PutPixel(36 + x, 16 + y, 255, 255, 255);
	PutPixel(37 + x, 16 + y, 107, 107, 107);
	PutPixel(40 + x, 16 + y, 174, 174, 174);
	PutPixel(41 + x, 16 + y, 255, 255, 255);
	PutPixel(42 + x, 16 + y, 255, 255, 255);
	PutPixel(43 + x, 16 + y, 51, 51, 51);
	PutPixel(2 + x, 17 + y, 195, 195, 195);
	PutPixel(3 + x, 17 + y, 255, 255, 255);
	PutPixel(4 + x, 17 + y, 255, 255, 255);
	PutPixel(5 + x, 17 + y, 38, 38, 38);
	PutPixel(8 + x, 17 + y, 51, 51, 51);
	PutPixel(9 + x, 17 + y, 255, 255, 255);
	PutPixel(10 + x, 17 + y, 255, 255, 255);
	PutPixel(11 + x, 17 + y, 174, 174, 174);
	PutPixel(13 + x, 17 + y, 38, 38, 38);
	PutPixel(14 + x, 17 + y, 255, 255, 255);
	PutPixel(15 + x, 17 + y, 255, 255, 255);
	PutPixel(16 + x, 17 + y, 156, 156, 156);
	PutPixel(20 + x, 17 + y, 195, 195, 195);
	PutPixel(21 + x, 17 + y, 255, 255, 255);
	PutPixel(22 + x, 17 + y, 255, 255, 255);
	PutPixel(23 + x, 17 + y, 38, 38, 38);
	PutPixel(25 + x, 17 + y, 107, 107, 107);
	PutPixel(26 + x, 17 + y, 255, 255, 255);
	PutPixel(27 + x, 17 + y, 255, 255, 255);
	PutPixel(28 + x, 17 + y, 51, 51, 51);
	PutPixel(30 + x, 17 + y, 218, 218, 218);
	PutPixel(31 + x, 17 + y, 255, 255, 255);
	PutPixel(32 + x, 17 + y, 92, 92, 92);
	PutPixel(34 + x, 17 + y, 107, 107, 107);
	PutPixel(35 + x, 17 + y, 255, 255, 255);
	PutPixel(36 + x, 17 + y, 255, 255, 255);
	PutPixel(37 + x, 17 + y, 107, 107, 107);
	PutPixel(40 + x, 17 + y, 174, 174, 174);
	PutPixel(41 + x, 17 + y, 255, 255, 255);
	PutPixel(42 + x, 17 + y, 255, 255, 255);
	PutPixel(43 + x, 17 + y, 51, 51, 51);
	PutPixel(2 + x, 18 + y, 38, 38, 38);
	PutPixel(3 + x, 18 + y, 218, 218, 218);
	PutPixel(4 + x, 18 + y, 255, 255, 255);
	PutPixel(5 + x, 18 + y, 255, 255, 255);
	PutPixel(6 + x, 18 + y, 78, 78, 78);
	PutPixel(8 + x, 18 + y, 51, 51, 51);
	PutPixel(9 + x, 18 + y, 255, 255, 255);
	PutPixel(10 + x, 18 + y, 255, 255, 255);
	PutPixel(11 + x, 18 + y, 174, 174, 174);
	PutPixel(13 + x, 18 + y, 122, 122, 122);
	PutPixel(14 + x, 18 + y, 255, 255, 255);
	PutPixel(15 + x, 18 + y, 255, 255, 255);
	PutPixel(16 + x, 18 + y, 78, 78, 78);
	PutPixel(20 + x, 18 + y, 107, 107, 107);
	PutPixel(21 + x, 18 + y, 255, 255, 255);
	PutPixel(22 + x, 18 + y, 255, 255, 255);
	PutPixel(23 + x, 18 + y, 122, 122, 122);
	PutPixel(25 + x, 18 + y, 107, 107, 107);
	PutPixel(26 + x, 18 + y, 255, 255, 255);
	PutPixel(27 + x, 18 + y, 255, 255, 255);
	PutPixel(28 + x, 18 + y, 51, 51, 51);
	PutPixel(34 + x, 18 + y, 107, 107, 107);
	PutPixel(35 + x, 18 + y, 255, 255, 255);
	PutPixel(36 + x, 18 + y, 255, 255, 255);
	PutPixel(37 + x, 18 + y, 107, 107, 107);
	PutPixel(40 + x, 18 + y, 174, 174, 174);
	PutPixel(41 + x, 18 + y, 255, 255, 255);
	PutPixel(42 + x, 18 + y, 255, 255, 255);
	PutPixel(43 + x, 18 + y, 51, 51, 51);
	PutPixel(3 + x, 19 + y, 12, 12, 12);
	PutPixel(4 + x, 19 + y, 107, 107, 107);
	PutPixel(5 + x, 19 + y, 195, 195, 195);
	PutPixel(6 + x, 19 + y, 255, 255, 255);
	PutPixel(7 + x, 19 + y, 255, 255, 255);
	PutPixel(8 + x, 19 + y, 255, 255, 255);
	PutPixel(9 + x, 19 + y, 174, 174, 174);
	PutPixel(10 + x, 19 + y, 122, 122, 122);
	PutPixel(11 + x, 19 + y, 38, 38, 38);
	PutPixel(13 + x, 19 + y, 218, 218, 218);
	PutPixel(14 + x, 19 + y, 255, 255, 255);
	PutPixel(15 + x, 19 + y, 255, 255, 255);
	PutPixel(16 + x, 19 + y, 12, 12, 12);
	PutPixel(20 + x, 19 + y, 38, 38, 38);
	PutPixel(21 + x, 19 + y, 255, 255, 255);
	PutPixel(22 + x, 19 + y, 255, 255, 255);
	PutPixel(23 + x, 19 + y, 218, 218, 218);
	PutPixel(25 + x, 19 + y, 107, 107, 107);
	PutPixel(26 + x, 19 + y, 255, 255, 255);
	PutPixel(27 + x, 19 + y, 255, 255, 255);
	PutPixel(28 + x, 19 + y, 51, 51, 51);
	PutPixel(34 + x, 19 + y, 107, 107, 107);
	PutPixel(35 + x, 19 + y, 255, 255, 255);
	PutPixel(36 + x, 19 + y, 255, 255, 255);
	PutPixel(37 + x, 19 + y, 107, 107, 107);
	PutPixel(40 + x, 19 + y, 174, 174, 174);
	PutPixel(41 + x, 19 + y, 255, 255, 255);
	PutPixel(42 + x, 19 + y, 255, 255, 255);
	PutPixel(43 + x, 19 + y, 255, 255, 255);
	PutPixel(44 + x, 19 + y, 255, 255, 255);
	PutPixel(45 + x, 19 + y, 255, 255, 255);
	PutPixel(46 + x, 19 + y, 255, 255, 255);
	PutPixel(47 + x, 19 + y, 255, 255, 255);
	PutPixel(48 + x, 19 + y, 51, 51, 51);
	PutPixel(5 + x, 27 + y, 92, 92, 92);
	PutPixel(6 + x, 27 + y, 195, 195, 195);
	PutPixel(7 + x, 27 + y, 255, 255, 255);
	PutPixel(8 + x, 27 + y, 255, 255, 255);
	PutPixel(9 + x, 27 + y, 218, 218, 218);
	PutPixel(10 + x, 27 + y, 122, 122, 122);
	PutPixel(11 + x, 27 + y, 12, 12, 12);
	PutPixel(14 + x, 27 + y, 78, 78, 78);
	PutPixel(15 + x, 27 + y, 174, 174, 174);
	PutPixel(16 + x, 27 + y, 174, 174, 174);
	PutPixel(17 + x, 27 + y, 78, 78, 78);
	PutPixel(22 + x, 27 + y, 107, 107, 107);
	PutPixel(23 + x, 27 + y, 174, 174, 174);
	PutPixel(24 + x, 27 + y, 174, 174, 174);
	PutPixel(25 + x, 27 + y, 51, 51, 51);
	PutPixel(27 + x, 27 + y, 174, 174, 174);
	PutPixel(28 + x, 27 + y, 174, 174, 174);
	PutPixel(29 + x, 27 + y, 174, 174, 174);
	PutPixel(30 + x, 27 + y, 174, 174, 174);
	PutPixel(31 + x, 27 + y, 174, 174, 174);
	PutPixel(32 + x, 27 + y, 174, 174, 174);
	PutPixel(33 + x, 27 + y, 174, 174, 174);
	PutPixel(34 + x, 27 + y, 174, 174, 174);
	PutPixel(37 + x, 27 + y, 174, 174, 174);
	PutPixel(38 + x, 27 + y, 174, 174, 174);
	PutPixel(39 + x, 27 + y, 174, 174, 174);
	PutPixel(40 + x, 27 + y, 174, 174, 174);
	PutPixel(41 + x, 27 + y, 174, 174, 174);
	PutPixel(42 + x, 27 + y, 174, 174, 174);
	PutPixel(43 + x, 27 + y, 122, 122, 122);
	PutPixel(44 + x, 27 + y, 38, 38, 38);
	PutPixel(49 + x, 27 + y, 1, 1, 1);
	PutPixel(3 + x, 28 + y, 12, 12, 12);
	PutPixel(4 + x, 28 + y, 195, 195, 195);
	PutPixel(5 + x, 28 + y, 255, 255, 255);
	PutPixel(6 + x, 28 + y, 255, 255, 255);
	PutPixel(7 + x, 28 + y, 78, 78, 78);
	PutPixel(8 + x, 28 + y, 64, 64, 64);
	PutPixel(9 + x, 28 + y, 195, 195, 195);
	PutPixel(10 + x, 28 + y, 255, 255, 255);
	PutPixel(11 + x, 28 + y, 255, 255, 255);
	PutPixel(12 + x, 28 + y, 38, 38, 38);
	PutPixel(14 + x, 28 + y, 38, 38, 38);
	PutPixel(15 + x, 28 + y, 255, 255, 255);
	PutPixel(16 + x, 28 + y, 255, 255, 255);
	PutPixel(17 + x, 28 + y, 174, 174, 174);
	PutPixel(22 + x, 28 + y, 218, 218, 218);
	PutPixel(23 + x, 28 + y, 255, 255, 255);
	PutPixel(24 + x, 28 + y, 255, 255, 255);
	PutPixel(25 + x, 28 + y, 12, 12, 12);
	PutPixel(27 + x, 28 + y, 255, 255, 255);
	PutPixel(28 + x, 28 + y, 255, 255, 255);
	PutPixel(29 + x, 28 + y, 255, 255, 255);
	PutPixel(30 + x, 28 + y, 51, 51, 51);
	PutPixel(31 + x, 28 + y, 51, 51, 51);
	PutPixel(32 + x, 28 + y, 51, 51, 51);
	PutPixel(33 + x, 28 + y, 51, 51, 51);
	PutPixel(34 + x, 28 + y, 51, 51, 51);
	PutPixel(37 + x, 28 + y, 255, 255, 255);
	PutPixel(38 + x, 28 + y, 255, 255, 255);
	PutPixel(39 + x, 28 + y, 255, 255, 255);
	PutPixel(40 + x, 28 + y, 51, 51, 51);
	PutPixel(41 + x, 28 + y, 51, 51, 51);
	PutPixel(42 + x, 28 + y, 195, 195, 195);
	PutPixel(43 + x, 28 + y, 255, 255, 255);
	PutPixel(44 + x, 28 + y, 255, 255, 255);
	PutPixel(45 + x, 28 + y, 64, 64, 64);
	PutPixel(48 + x, 28 + y, 1, 1, 1);
	PutPixel(49 + x, 28 + y, 3, 3, 3);
	PutPixel(3 + x, 29 + y, 122, 122, 122);
	PutPixel(4 + x, 29 + y, 255, 255, 255);
	PutPixel(5 + x, 29 + y, 255, 255, 255);
	PutPixel(6 + x, 29 + y, 51, 51, 51);
	PutPixel(9 + x, 29 + y, 12, 12, 12);
	PutPixel(10 + x, 29 + y, 255, 255, 255);
	PutPixel(11 + x, 29 + y, 255, 255, 255);
	PutPixel(12 + x, 29 + y, 195, 195, 195);
	PutPixel(15 + x, 29 + y, 218, 218, 218);
	PutPixel(16 + x, 29 + y, 255, 255, 255);
	PutPixel(17 + x, 29 + y, 255, 255, 255);
	PutPixel(18 + x, 29 + y, 12, 12, 12);
	PutPixel(21 + x, 29 + y, 25, 25, 25);
	PutPixel(22 + x, 29 + y, 255, 255, 255);
	PutPixel(23 + x, 29 + y, 255, 255, 255);
	PutPixel(24 + x, 29 + y, 156, 156, 156);
	PutPixel(27 + x, 29 + y, 255, 255, 255);
	PutPixel(28 + x, 29 + y, 255, 255, 255);
	PutPixel(29 + x, 29 + y, 255, 255, 255);
	PutPixel(37 + x, 29 + y, 255, 255, 255);
	PutPixel(38 + x, 29 + y, 255, 255, 255);
	PutPixel(39 + x, 29 + y, 255, 255, 255);
	PutPixel(42 + x, 29 + y, 25, 25, 25);
	PutPixel(43 + x, 29 + y, 255, 255, 255);
	PutPixel(44 + x, 29 + y, 255, 255, 255);
	PutPixel(45 + x, 29 + y, 218, 218, 218);
	PutPixel(49 + x, 29 + y, 1, 1, 1);
	PutPixel(2 + x, 30 + y, 12, 12, 12);
	PutPixel(3 + x, 30 + y, 255, 255, 255);
	PutPixel(4 + x, 30 + y, 255, 255, 255);
	PutPixel(5 + x, 30 + y, 218, 218, 218);
	PutPixel(10 + x, 30 + y, 139, 139, 139);
	PutPixel(11 + x, 30 + y, 255, 255, 255);
	PutPixel(12 + x, 30 + y, 255, 255, 255);
	PutPixel(13 + x, 30 + y, 64, 64, 64);
	PutPixel(15 + x, 30 + y, 122, 122, 122);
	PutPixel(16 + x, 30 + y, 255, 255, 255);
	PutPixel(17 + x, 30 + y, 255, 255, 255);
	PutPixel(18 + x, 30 + y, 64, 64, 64);
	PutPixel(21 + x, 30 + y, 92, 92, 92);
	PutPixel(22 + x, 30 + y, 255, 255, 255);
	PutPixel(23 + x, 30 + y, 255, 255, 255);
	PutPixel(24 + x, 30 + y, 78, 78, 78);
	PutPixel(27 + x, 30 + y, 255, 255, 255);
	PutPixel(28 + x, 30 + y, 255, 255, 255);
	PutPixel(29 + x, 30 + y, 255, 255, 255);
	PutPixel(37 + x, 30 + y, 255, 255, 255);
	PutPixel(38 + x, 30 + y, 255, 255, 255);
	PutPixel(39 + x, 30 + y, 255, 255, 255);
	PutPixel(43 + x, 30 + y, 255, 255, 255);
	PutPixel(44 + x, 30 + y, 255, 255, 255);
	PutPixel(45 + x, 30 + y, 255, 255, 255);
	PutPixel(2 + x, 31 + y, 64, 64, 64);
	PutPixel(3 + x, 31 + y, 255, 255, 255);
	PutPixel(4 + x, 31 + y, 255, 255, 255);
	PutPixel(5 + x, 31 + y, 139, 139, 139);
	PutPixel(10 + x, 31 + y, 78, 78, 78);
	PutPixel(11 + x, 31 + y, 255, 255, 255);
	PutPixel(12 + x, 31 + y, 255, 255, 255);
	PutPixel(13 + x, 31 + y, 122, 122, 122);
	PutPixel(15 + x, 31 + y, 38, 38, 38);
	PutPixel(16 + x, 31 + y, 255, 255, 255);
	PutPixel(17 + x, 31 + y, 255, 255, 255);
	PutPixel(18 + x, 31 + y, 122, 122, 122);
	PutPixel(21 + x, 31 + y, 156, 156, 156);
	PutPixel(22 + x, 31 + y, 255, 255, 255);
	PutPixel(23 + x, 31 + y, 255, 255, 255);
	PutPixel(24 + x, 31 + y, 12, 12, 12);
	PutPixel(27 + x, 31 + y, 255, 255, 255);
	PutPixel(28 + x, 31 + y, 255, 255, 255);
	PutPixel(29 + x, 31 + y, 255, 255, 255);
	PutPixel(37 + x, 31 + y, 255, 255, 255);
	PutPixel(38 + x, 31 + y, 255, 255, 255);
	PutPixel(39 + x, 31 + y, 255, 255, 255);
	PutPixel(43 + x, 31 + y, 255, 255, 255);
	PutPixel(44 + x, 31 + y, 255, 255, 255);
	PutPixel(45 + x, 31 + y, 255, 255, 255);
	PutPixel(2 + x, 32 + y, 107, 107, 107);
	PutPixel(3 + x, 32 + y, 255, 255, 255);
	PutPixel(4 + x, 32 + y, 255, 255, 255);
	PutPixel(5 + x, 32 + y, 107, 107, 107);
	PutPixel(10 + x, 32 + y, 51, 51, 51);
	PutPixel(11 + x, 32 + y, 255, 255, 255);
	PutPixel(12 + x, 32 + y, 255, 255, 255);
	PutPixel(13 + x, 32 + y, 174, 174, 174);
	PutPixel(16 + x, 32 + y, 218, 218, 218);
	PutPixel(17 + x, 32 + y, 255, 255, 255);
	PutPixel(18 + x, 32 + y, 218, 218, 218);
	PutPixel(21 + x, 32 + y, 255, 255, 255);
	PutPixel(22 + x, 32 + y, 255, 255, 255);
	PutPixel(23 + x, 32 + y, 156, 156, 156);
	PutPixel(27 + x, 32 + y, 255, 255, 255);
	PutPixel(28 + x, 32 + y, 255, 255, 255);
	PutPixel(29 + x, 32 + y, 255, 255, 255);
	PutPixel(30 + x, 32 + y, 174, 174, 174);
	PutPixel(31 + x, 32 + y, 174, 174, 174);
	PutPixel(32 + x, 32 + y, 174, 174, 174);
	PutPixel(33 + x, 32 + y, 174, 174, 174);
	PutPixel(34 + x, 32 + y, 78, 78, 78);
	PutPixel(37 + x, 32 + y, 255, 255, 255);
	PutPixel(38 + x, 32 + y, 255, 255, 255);
	PutPixel(39 + x, 32 + y, 255, 255, 255);
	PutPixel(42 + x, 32 + y, 12, 12, 12);
	PutPixel(43 + x, 32 + y, 255, 255, 255);
	PutPixel(44 + x, 32 + y, 255, 255, 255);
	PutPixel(45 + x, 32 + y, 174, 174, 174);
	PutPixel(2 + x, 33 + y, 107, 107, 107);
	PutPixel(3 + x, 33 + y, 255, 255, 255);
	PutPixel(4 + x, 33 + y, 255, 255, 255);
	PutPixel(5 + x, 33 + y, 107, 107, 107);
	PutPixel(10 + x, 33 + y, 51, 51, 51);
	PutPixel(11 + x, 33 + y, 255, 255, 255);
	PutPixel(12 + x, 33 + y, 255, 255, 255);
	PutPixel(13 + x, 33 + y, 174, 174, 174);
	PutPixel(16 + x, 33 + y, 122, 122, 122);
	PutPixel(17 + x, 33 + y, 255, 255, 255);
	PutPixel(18 + x, 33 + y, 255, 255, 255);
	PutPixel(19 + x, 33 + y, 25, 25, 25);
	PutPixel(20 + x, 33 + y, 51, 51, 51);
	PutPixel(21 + x, 33 + y, 255, 255, 255);
	PutPixel(22 + x, 33 + y, 255, 255, 255);
	PutPixel(23 + x, 33 + y, 78, 78, 78);
	PutPixel(27 + x, 33 + y, 255, 255, 255);
	PutPixel(28 + x, 33 + y, 255, 255, 255);
	PutPixel(29 + x, 33 + y, 255, 255, 255);
	PutPixel(30 + x, 33 + y, 51, 51, 51);
	PutPixel(31 + x, 33 + y, 51, 51, 51);
	PutPixel(32 + x, 33 + y, 51, 51, 51);
	PutPixel(33 + x, 33 + y, 51, 51, 51);
	PutPixel(34 + x, 33 + y, 25, 25, 25);
	PutPixel(37 + x, 33 + y, 255, 255, 255);
	PutPixel(38 + x, 33 + y, 255, 255, 255);
	PutPixel(39 + x, 33 + y, 255, 255, 255);
	PutPixel(42 + x, 33 + y, 107, 107, 107);
	PutPixel(43 + x, 33 + y, 255, 255, 255);
	PutPixel(44 + x, 33 + y, 255, 255, 255);
	PutPixel(45 + x, 33 + y, 38, 38, 38);
	PutPixel(2 + x, 34 + y, 92, 92, 92);
	PutPixel(3 + x, 34 + y, 255, 255, 255);
	PutPixel(4 + x, 34 + y, 255, 255, 255);
	PutPixel(5 + x, 34 + y, 107, 107, 107);
	PutPixel(10 + x, 34 + y, 51, 51, 51);
	PutPixel(11 + x, 34 + y, 255, 255, 255);
	PutPixel(12 + x, 34 + y, 255, 255, 255);
	PutPixel(13 + x, 34 + y, 156, 156, 156);
	PutPixel(16 + x, 34 + y, 38, 38, 38);
	PutPixel(17 + x, 34 + y, 255, 255, 255);
	PutPixel(18 + x, 34 + y, 255, 255, 255);
	PutPixel(19 + x, 34 + y, 78, 78, 78);
	PutPixel(20 + x, 34 + y, 122, 122, 122);
	PutPixel(21 + x, 34 + y, 255, 255, 255);
	PutPixel(22 + x, 34 + y, 255, 255, 255);
	PutPixel(23 + x, 34 + y, 12, 12, 12);
	PutPixel(27 + x, 34 + y, 255, 255, 255);
	PutPixel(28 + x, 34 + y, 255, 255, 255);
	PutPixel(29 + x, 34 + y, 255, 255, 255);
	PutPixel(37 + x, 34 + y, 255, 255, 255);
	PutPixel(38 + x, 34 + y, 255, 255, 255);
	PutPixel(39 + x, 34 + y, 255, 255, 255);
	PutPixel(40 + x, 34 + y, 174, 174, 174);
	PutPixel(41 + x, 34 + y, 195, 195, 195);
	PutPixel(42 + x, 34 + y, 255, 255, 255);
	PutPixel(43 + x, 34 + y, 255, 255, 255);
	PutPixel(44 + x, 34 + y, 25, 25, 25);
	PutPixel(2 + x, 35 + y, 38, 38, 38);
	PutPixel(3 + x, 35 + y, 255, 255, 255);
	PutPixel(4 + x, 35 + y, 255, 255, 255);
	PutPixel(5 + x, 35 + y, 174, 174, 174);
	PutPixel(10 + x, 35 + y, 92, 92, 92);
	PutPixel(11 + x, 35 + y, 255, 255, 255);
	PutPixel(12 + x, 35 + y, 255, 255, 255);
	PutPixel(13 + x, 35 + y, 92, 92, 92);
	PutPixel(17 + x, 35 + y, 218, 218, 218);
	PutPixel(18 + x, 35 + y, 255, 255, 255);
	PutPixel(19 + x, 35 + y, 156, 156, 156);
	PutPixel(20 + x, 35 + y, 195, 195, 195);
	PutPixel(21 + x, 35 + y, 255, 255, 255);
	PutPixel(22 + x, 35 + y, 156, 156, 156);
	PutPixel(27 + x, 35 + y, 255, 255, 255);
	PutPixel(28 + x, 35 + y, 255, 255, 255);
	PutPixel(29 + x, 35 + y, 255, 255, 255);
	PutPixel(37 + x, 35 + y, 255, 255, 255);
	PutPixel(38 + x, 35 + y, 255, 255, 255);
	PutPixel(39 + x, 35 + y, 255, 255, 255);
	PutPixel(40 + x, 35 + y, 51, 51, 51);
	PutPixel(41 + x, 35 + y, 195, 195, 195);
	PutPixel(42 + x, 35 + y, 255, 255, 255);
	PutPixel(43 + x, 35 + y, 255, 255, 255);
	PutPixel(44 + x, 35 + y, 139, 139, 139);
	PutPixel(3 + x, 36 + y, 218, 218, 218);
	PutPixel(4 + x, 36 + y, 255, 255, 255);
	PutPixel(5 + x, 36 + y, 255, 255, 255);
	PutPixel(6 + x, 36 + y, 12, 12, 12);
	PutPixel(10 + x, 36 + y, 174, 174, 174);
	PutPixel(11 + x, 36 + y, 255, 255, 255);
	PutPixel(12 + x, 36 + y, 255, 255, 255);
	PutPixel(13 + x, 36 + y, 25, 25, 25);
	PutPixel(17 + x, 36 + y, 122, 122, 122);
	PutPixel(18 + x, 36 + y, 255, 255, 255);
	PutPixel(19 + x, 36 + y, 255, 255, 255);
	PutPixel(20 + x, 36 + y, 255, 255, 255);
	PutPixel(21 + x, 36 + y, 255, 255, 255);
	PutPixel(22 + x, 36 + y, 78, 78, 78);
	PutPixel(27 + x, 36 + y, 255, 255, 255);
	PutPixel(28 + x, 36 + y, 255, 255, 255);
	PutPixel(29 + x, 36 + y, 255, 255, 255);
	PutPixel(37 + x, 36 + y, 255, 255, 255);
	PutPixel(38 + x, 36 + y, 255, 255, 255);
	PutPixel(39 + x, 36 + y, 255, 255, 255);
	PutPixel(41 + x, 36 + y, 38, 38, 38);
	PutPixel(42 + x, 36 + y, 255, 255, 255);
	PutPixel(43 + x, 36 + y, 255, 255, 255);
	PutPixel(44 + x, 36 + y, 255, 255, 255);
	PutPixel(45 + x, 36 + y, 92, 92, 92);
	PutPixel(3 + x, 37 + y, 51, 51, 51);
	PutPixel(4 + x, 37 + y, 255, 255, 255);
	PutPixel(5 + x, 37 + y, 255, 255, 255);
	PutPixel(6 + x, 37 + y, 139, 139, 139);
	PutPixel(9 + x, 37 + y, 78, 78, 78);
	PutPixel(10 + x, 37 + y, 255, 255, 255);
	PutPixel(11 + x, 37 + y, 255, 255, 255);
	PutPixel(12 + x, 37 + y, 107, 107, 107);
	PutPixel(17 + x, 37 + y, 38, 38, 38);
	PutPixel(18 + x, 37 + y, 255, 255, 255);
	PutPixel(19 + x, 37 + y, 255, 255, 255);
	PutPixel(20 + x, 37 + y, 255, 255, 255);
	PutPixel(21 + x, 37 + y, 255, 255, 255);
	PutPixel(22 + x, 37 + y, 12, 12, 12);
	PutPixel(27 + x, 37 + y, 255, 255, 255);
	PutPixel(28 + x, 37 + y, 255, 255, 255);
	PutPixel(29 + x, 37 + y, 255, 255, 255);
	PutPixel(37 + x, 37 + y, 255, 255, 255);
	PutPixel(38 + x, 37 + y, 255, 255, 255);
	PutPixel(39 + x, 37 + y, 255, 255, 255);
	PutPixel(42 + x, 37 + y, 64, 64, 64);
	PutPixel(43 + x, 37 + y, 255, 255, 255);
	PutPixel(44 + x, 37 + y, 255, 255, 255);
	PutPixel(45 + x, 37 + y, 255, 255, 255);
	PutPixel(46 + x, 37 + y, 64, 64, 64);
	PutPixel(4 + x, 38 + y, 64, 64, 64);
	PutPixel(5 + x, 38 + y, 255, 255, 255);
	PutPixel(6 + x, 38 + y, 255, 255, 255);
	PutPixel(7 + x, 38 + y, 218, 218, 218);
	PutPixel(8 + x, 38 + y, 195, 195, 195);
	PutPixel(9 + x, 38 + y, 255, 255, 255);
	PutPixel(10 + x, 38 + y, 255, 255, 255);
	PutPixel(11 + x, 38 + y, 107, 107, 107);
	PutPixel(18 + x, 38 + y, 218, 218, 218);
	PutPixel(19 + x, 38 + y, 255, 255, 255);
	PutPixel(20 + x, 38 + y, 255, 255, 255);
	PutPixel(21 + x, 38 + y, 156, 156, 156);
	PutPixel(27 + x, 38 + y, 255, 255, 255);
	PutPixel(28 + x, 38 + y, 255, 255, 255);
	PutPixel(29 + x, 38 + y, 255, 255, 255);
	PutPixel(30 + x, 38 + y, 174, 174, 174);
	PutPixel(31 + x, 38 + y, 174, 174, 174);
	PutPixel(32 + x, 38 + y, 174, 174, 174);
	PutPixel(33 + x, 38 + y, 174, 174, 174);
	PutPixel(34 + x, 38 + y, 174, 174, 174);
	PutPixel(37 + x, 38 + y, 255, 255, 255);
	PutPixel(38 + x, 38 + y, 255, 255, 255);
	PutPixel(39 + x, 38 + y, 255, 255, 255);
	PutPixel(43 + x, 38 + y, 107, 107, 107);
	PutPixel(44 + x, 38 + y, 255, 255, 255);
	PutPixel(45 + x, 38 + y, 255, 255, 255);
	PutPixel(46 + x, 38 + y, 255, 255, 255);
	PutPixel(47 + x, 38 + y, 38, 38, 38);
	PutPixel(6 + x, 39 + y, 64, 64, 64);
	PutPixel(7 + x, 39 + y, 107, 107, 107);
	PutPixel(8 + x, 39 + y, 107, 107, 107);
	PutPixel(9 + x, 39 + y, 78, 78, 78);
	PutPixel(10 + x, 39 + y, 12, 12, 12);
	PutPixel(18 + x, 39 + y, 38, 38, 38);
	PutPixel(19 + x, 39 + y, 51, 51, 51);
	PutPixel(20 + x, 39 + y, 51, 51, 51);
	PutPixel(21 + x, 39 + y, 25, 25, 25);
	PutPixel(27 + x, 39 + y, 51, 51, 51);
	PutPixel(28 + x, 39 + y, 51, 51, 51);
	PutPixel(29 + x, 39 + y, 51, 51, 51);
	PutPixel(30 + x, 39 + y, 51, 51, 51);
	PutPixel(31 + x, 39 + y, 51, 51, 51);
	PutPixel(32 + x, 39 + y, 51, 51, 51);
	PutPixel(33 + x, 39 + y, 51, 51, 51);
	PutPixel(34 + x, 39 + y, 51, 51, 51);
	PutPixel(37 + x, 39 + y, 51, 51, 51);
	PutPixel(38 + x, 39 + y, 51, 51, 51);
	PutPixel(39 + x, 39 + y, 51, 51, 51);
	PutPixel(44 + x, 39 + y, 51, 51, 51);
	PutPixel(45 + x, 39 + y, 51, 51, 51);
	PutPixel(46 + x, 39 + y, 51, 51, 51);
	PutPixel(47 + x, 39 + y, 38, 38, 38);

}

void Graphics::DrawWalls(RectF& walls, float thickness, Color c)
{
	DrawRect(int(walls.left            ), int(walls.top - thickness), int(walls.right            ), int(walls.top               ), c);
	DrawRect(int(walls.left - thickness), int(walls.top - thickness), int(walls.left             ), int(walls.bottom + thickness), c);
	DrawRect(int(walls.right           ), int(walls.top - thickness), int(walls.right + thickness), int(walls.bottom + thickness), c);
	DrawRect(int(walls.left            ), int(walls.bottom         ), int(walls.right            ), int(walls.bottom + thickness), c);
}

void Graphics::DrawHeart(int x, int y)
{
	PutPixel(2 + x, 0 + y, 149, 0, 0);
	PutPixel(3 + x, 0 + y, 149, 0, 0);
	PutPixel(4 + x, 0 + y, 149, 0, 0);
	PutPixel(10 + x, 0 + y, 149, 0, 0);
	PutPixel(11 + x, 0 + y, 149, 0, 0);
	PutPixel(12 + x, 0 + y, 149, 0, 0);
	PutPixel(1 + x, 1 + y, 149, 0, 0);
	PutPixel(2 + x, 1 + y, 255, 0, 0);
	PutPixel(3 + x, 1 + y, 255, 0, 0);
	PutPixel(4 + x, 1 + y, 255, 0, 0);
	PutPixel(5 + x, 1 + y, 149, 0, 0);
	PutPixel(9 + x, 1 + y, 149, 0, 0);
	PutPixel(10 + x, 1 + y, 255, 0, 0);
	PutPixel(11 + x, 1 + y, 255, 0, 0);
	PutPixel(12 + x, 1 + y, 255, 0, 0);
	PutPixel(13 + x, 1 + y, 149, 0, 0);
	PutPixel(0 + x, 2 + y, 149, 0, 0);
	PutPixel(1 + x, 2 + y, 255, 0, 0);
	PutPixel(2 + x, 2 + y, 255, 0, 0);
	PutPixel(3 + x, 2 + y, 255, 0, 0);
	PutPixel(4 + x, 2 + y, 255, 0, 0);
	PutPixel(5 + x, 2 + y, 255, 0, 0);
	PutPixel(6 + x, 2 + y, 149, 0, 0);
	PutPixel(8 + x, 2 + y, 149, 0, 0);
	PutPixel(9 + x, 2 + y, 255, 0, 0);
	PutPixel(10 + x, 2 + y, 255, 0, 0);
	PutPixel(11 + x, 2 + y, 255, 0, 0);
	PutPixel(12 + x, 2 + y, 255, 0, 0);
	PutPixel(13 + x, 2 + y, 255, 0, 0);
	PutPixel(14 + x, 2 + y, 149, 0, 0);
	PutPixel(0 + x, 3 + y, 149, 0, 0);
	PutPixel(1 + x, 3 + y, 255, 0, 0);
	PutPixel(2 + x, 3 + y, 255, 0, 0);
	PutPixel(3 + x, 3 + y, 255, 0, 0);
	PutPixel(4 + x, 3 + y, 255, 0, 0);
	PutPixel(5 + x, 3 + y, 255, 0, 0);
	PutPixel(6 + x, 3 + y, 255, 0, 0);
	PutPixel(7 + x, 3 + y, 149, 0, 0);
	PutPixel(8 + x, 3 + y, 255, 0, 0);
	PutPixel(9 + x, 3 + y, 255, 0, 0);
	PutPixel(10 + x, 3 + y, 255, 0, 0);
	PutPixel(11 + x, 3 + y, 255, 0, 0);
	PutPixel(12 + x, 3 + y, 255, 0, 0);
	PutPixel(13 + x, 3 + y, 255, 0, 0);
	PutPixel(14 + x, 3 + y, 149, 0, 0);
	PutPixel(0 + x, 4 + y, 149, 0, 0);
	PutPixel(1 + x, 4 + y, 255, 0, 0);
	PutPixel(2 + x, 4 + y, 255, 0, 0);
	PutPixel(3 + x, 4 + y, 255, 0, 0);
	PutPixel(4 + x, 4 + y, 255, 0, 0);
	PutPixel(5 + x, 4 + y, 255, 0, 0);
	PutPixel(6 + x, 4 + y, 255, 0, 0);
	PutPixel(7 + x, 4 + y, 255, 0, 0);
	PutPixel(8 + x, 4 + y, 255, 0, 0);
	PutPixel(9 + x, 4 + y, 255, 0, 0);
	PutPixel(10 + x, 4 + y, 255, 0, 0);
	PutPixel(11 + x, 4 + y, 255, 0, 0);
	PutPixel(12 + x, 4 + y, 255, 0, 0);
	PutPixel(13 + x, 4 + y, 255, 0, 0);
	PutPixel(14 + x, 4 + y, 149, 0, 0);
	PutPixel(0 + x, 5 + y, 149, 0, 0);
	PutPixel(1 + x, 5 + y, 255, 0, 0);
	PutPixel(2 + x, 5 + y, 255, 0, 0);
	PutPixel(3 + x, 5 + y, 255, 0, 0);
	PutPixel(4 + x, 5 + y, 255, 0, 0);
	PutPixel(5 + x, 5 + y, 255, 0, 0);
	PutPixel(6 + x, 5 + y, 255, 0, 0);
	PutPixel(7 + x, 5 + y, 255, 0, 0);
	PutPixel(8 + x, 5 + y, 255, 0, 0);
	PutPixel(9 + x, 5 + y, 255, 0, 0);
	PutPixel(10 + x, 5 + y, 255, 0, 0);
	PutPixel(11 + x, 5 + y, 255, 0, 0);
	PutPixel(12 + x, 5 + y, 255, 0, 0);
	PutPixel(13 + x, 5 + y, 255, 0, 0);
	PutPixel(14 + x, 5 + y, 149, 0, 0);
	PutPixel(0 + x, 6 + y, 149, 0, 0);
	PutPixel(1 + x, 6 + y, 255, 0, 0);
	PutPixel(2 + x, 6 + y, 255, 0, 0);
	PutPixel(3 + x, 6 + y, 255, 0, 0);
	PutPixel(4 + x, 6 + y, 255, 0, 0);
	PutPixel(5 + x, 6 + y, 255, 0, 0);
	PutPixel(6 + x, 6 + y, 255, 0, 0);
	PutPixel(7 + x, 6 + y, 255, 0, 0);
	PutPixel(8 + x, 6 + y, 255, 0, 0);
	PutPixel(9 + x, 6 + y, 255, 0, 0);
	PutPixel(10 + x, 6 + y, 255, 0, 0);
	PutPixel(11 + x, 6 + y, 255, 0, 0);
	PutPixel(12 + x, 6 + y, 255, 0, 0);
	PutPixel(13 + x, 6 + y, 255, 0, 0);
	PutPixel(14 + x, 6 + y, 149, 0, 0);
	PutPixel(0 + x, 7 + y, 149, 0, 0);
	PutPixel(1 + x, 7 + y, 255, 0, 0);
	PutPixel(2 + x, 7 + y, 255, 0, 0);
	PutPixel(3 + x, 7 + y, 255, 0, 0);
	PutPixel(4 + x, 7 + y, 255, 0, 0);
	PutPixel(5 + x, 7 + y, 255, 0, 0);
	PutPixel(6 + x, 7 + y, 255, 0, 0);
	PutPixel(7 + x, 7 + y, 255, 0, 0);
	PutPixel(8 + x, 7 + y, 255, 0, 0);
	PutPixel(9 + x, 7 + y, 255, 0, 0);
	PutPixel(10 + x, 7 + y, 255, 0, 0);
	PutPixel(11 + x, 7 + y, 255, 0, 0);
	PutPixel(12 + x, 7 + y, 255, 0, 0);
	PutPixel(13 + x, 7 + y, 255, 0, 0);
	PutPixel(14 + x, 7 + y, 149, 0, 0);
	PutPixel(0 + x, 8 + y, 149, 0, 0);
	PutPixel(1 + x, 8 + y, 255, 0, 0);
	PutPixel(2 + x, 8 + y, 255, 0, 0);
	PutPixel(3 + x, 8 + y, 255, 0, 0);
	PutPixel(4 + x, 8 + y, 255, 0, 0);
	PutPixel(5 + x, 8 + y, 255, 0, 0);
	PutPixel(6 + x, 8 + y, 255, 0, 0);
	PutPixel(7 + x, 8 + y, 255, 0, 0);
	PutPixel(8 + x, 8 + y, 255, 0, 0);
	PutPixel(9 + x, 8 + y, 255, 0, 0);
	PutPixel(10 + x, 8 + y, 255, 0, 0);
	PutPixel(11 + x, 8 + y, 255, 0, 0);
	PutPixel(12 + x, 8 + y, 255, 0, 0);
	PutPixel(13 + x, 8 + y, 255, 0, 0);
	PutPixel(14 + x, 8 + y, 149, 0, 0);
	PutPixel(1 + x, 9 + y, 149, 0, 0);
	PutPixel(2 + x, 9 + y, 255, 0, 0);
	PutPixel(3 + x, 9 + y, 255, 0, 0);
	PutPixel(4 + x, 9 + y, 255, 0, 0);
	PutPixel(5 + x, 9 + y, 255, 0, 0);
	PutPixel(6 + x, 9 + y, 255, 0, 0);
	PutPixel(7 + x, 9 + y, 255, 0, 0);
	PutPixel(8 + x, 9 + y, 255, 0, 0);
	PutPixel(9 + x, 9 + y, 255, 0, 0);
	PutPixel(10 + x, 9 + y, 255, 0, 0);
	PutPixel(11 + x, 9 + y, 255, 0, 0);
	PutPixel(12 + x, 9 + y, 255, 0, 0);
	PutPixel(13 + x, 9 + y, 149, 0, 0);
	PutPixel(2 + x, 10 + y, 149, 0, 0);
	PutPixel(3 + x, 10 + y, 255, 0, 0);
	PutPixel(4 + x, 10 + y, 255, 0, 0);
	PutPixel(5 + x, 10 + y, 255, 0, 0);
	PutPixel(6 + x, 10 + y, 255, 0, 0);
	PutPixel(7 + x, 10 + y, 255, 0, 0);
	PutPixel(8 + x, 10 + y, 255, 0, 0);
	PutPixel(9 + x, 10 + y, 255, 0, 0);
	PutPixel(10 + x, 10 + y, 255, 0, 0);
	PutPixel(11 + x, 10 + y, 255, 0, 0);
	PutPixel(12 + x, 10 + y, 149, 0, 0);
	PutPixel(3 + x, 11 + y, 149, 0, 0);
	PutPixel(4 + x, 11 + y, 255, 0, 0);
	PutPixel(5 + x, 11 + y, 255, 0, 0);
	PutPixel(6 + x, 11 + y, 255, 0, 0);
	PutPixel(7 + x, 11 + y, 255, 0, 0);
	PutPixel(8 + x, 11 + y, 255, 0, 0);
	PutPixel(9 + x, 11 + y, 255, 0, 0);
	PutPixel(10 + x, 11 + y, 255, 0, 0);
	PutPixel(11 + x, 11 + y, 149, 0, 0);
	PutPixel(4 + x, 12 + y, 149, 0, 0);
	PutPixel(5 + x, 12 + y, 255, 0, 0);
	PutPixel(6 + x, 12 + y, 255, 0, 0);
	PutPixel(7 + x, 12 + y, 255, 0, 0);
	PutPixel(8 + x, 12 + y, 255, 0, 0);
	PutPixel(9 + x, 12 + y, 255, 0, 0);
	PutPixel(10 + x, 12 + y, 149, 0, 0);
	PutPixel(5 + x, 13 + y, 149, 0, 0);
	PutPixel(6 + x, 13 + y, 255, 0, 0);
	PutPixel(7 + x, 13 + y, 255, 0, 0);
	PutPixel(8 + x, 13 + y, 255, 0, 0);
	PutPixel(9 + x, 13 + y, 149, 0, 0);
	PutPixel(6 + x, 14 + y, 149, 0, 0);
	PutPixel(7 + x, 14 + y, 149, 0, 0);
	PutPixel(8 + x, 14 + y, 149, 0, 0);

}


//////////////////////////////////////////////////
//           Graphics Exception
Graphics::Exception::Exception( HRESULT hr,const std::wstring& note,const wchar_t* file,unsigned int line )
	:
	ChiliException( file,line,note ),
	hr( hr )
{}

std::wstring Graphics::Exception::GetFullMessage() const
{
	const std::wstring empty = L"";
	const std::wstring errorName = GetErrorName();
	const std::wstring errorDesc = GetErrorDescription();
	const std::wstring& note = GetNote();
	const std::wstring location = GetLocation();
	return    (!errorName.empty() ? std::wstring( L"Error: " ) + errorName + L"\n"
		: empty)
		+ (!errorDesc.empty() ? std::wstring( L"Description: " ) + errorDesc + L"\n"
			: empty)
		+ (!note.empty() ? std::wstring( L"Note: " ) + note + L"\n"
			: empty)
		+ (!location.empty() ? std::wstring( L"Location: " ) + location
			: empty);
}

std::wstring Graphics::Exception::GetErrorName() const
{
	return DXGetErrorString( hr );
}

std::wstring Graphics::Exception::GetErrorDescription() const
{
	std::array<wchar_t,512> wideDescription;
	DXGetErrorDescription( hr,wideDescription.data(),wideDescription.size() );
	return wideDescription.data();
}

std::wstring Graphics::Exception::GetExceptionType() const
{
	return L"Chili Graphics Exception";
}