//-----------------------------------------------------------------------------
// File: Textures.cpp
//
// Desc: Better than just lights and materials, 3D objects look much more
//       convincing when texture-mapped. Textures can be thought of as a sort
//       of wallpaper, that is shrinkwrapped to fit a texture. Textures are
//       typically loaded from image files, and D3DX provides a utility to
//       function to do this for us. Like a vertex buffer, Textures have
//       Lock() and Unlock() functions to access (read or write) the image
//       data. Textures have a width, height, miplevel, and pixel format. The
//       miplevel is for "mipmapped" Textures, an advanced performance-
//       enhancing feature which uses lower resolutions of the texture for
//       objects in the distance where detail is less noticeable. The pixel
//       format determines how the colors are stored in a texel. The most
//       common formats are the 16-bit R5G6B5 format (5 bits of red, 6-bits of
//       green and 5 bits of blue) and the 32-bit A8R8G8B8 format (8 bits each
//       of alpha, red, green, and blue).
//
//       Textures are associated with geometry through texture coordinates.
//       Each vertex has one or more sets of texture coordinates, which are
//       named tu and tv and range from 0.0 to 1.0. Texture coordinates can be
//       supplied by the geometry, or can be automatically generated using
//       Direct3D texture coordinate generation (which is an advanced feature).
//
// Copyright (c) Microsoft Corporation. All rights reserved.
//-----------------------------------------------------------------------------
#include <Windows.h>
#include <mmsystem.h>
#include <d3dx9.h>
#pragma warning( disable : 4996 ) // disable deprecated warning 
#include <strsafe.h>
#pragma warning( default : 4996 )



//-----------------------------------------------------------------------------
// Global variables
//-----------------------------------------------------------------------------
LPDIRECT3D9             g_pD3D = NULL; // Used to create the D3DDevice
LPDIRECT3DDEVICE9       g_pd3dDevice = NULL; // Our rendering device
LPDIRECT3DVERTEXBUFFER9 g_pVB = NULL; // Buffer to hold vertices
LPDIRECT3DVERTEXBUFFER9 g_pVB2= NULL; // Buffer to hold vertices
LPDIRECT3DTEXTURE9      g_pTexture = NULL; // Our texture
LPDIRECT3DTEXTURE9      g_pTexture1 = NULL; // Our texture
LPDIRECT3DTEXTURE9      g_pTexture2 = NULL; // Our texture
LPDIRECT3DTEXTURE9      g_pTexture3 = NULL; // Our texture
LPDIRECT3DTEXTURE9      g_pTexture4= NULL; // Our texture
LPDIRECT3DTEXTURE9      g_pTexture5= NULL; // Our texture
LPDIRECT3DTEXTURE9      g_pTexture6 = NULL; // Our texture
LPDIRECT3DTEXTURE9      g_pTexture7 = NULL; // Our texture
LPDIRECT3DTEXTURE9      g_pTexture8 = NULL; // Our texture
LPDIRECT3DTEXTURE9      g_pTexture9 = NULL; // Our texture
LPDIRECT3DTEXTURE9      g_pTexture10 = NULL; // Our texture

LPDIRECT3DTEXTURE9      g_pTexture11 = NULL; // Our texture
LPDIRECT3DTEXTURE9      g_pTexture12 = NULL; // Our texture
LPDIRECT3DTEXTURE9      g_pTexture13 = NULL; // Our texture
LPDIRECT3DTEXTURE9      g_pTexture14 = NULL; // Our texture
LPDIRECT3DTEXTURE9      g_pTexture15 = NULL; // Our texture
LPDIRECT3DTEXTURE9      g_pTexture16 = NULL; // Our texture
LPDIRECT3DTEXTURE9      g_pTexture17 = NULL; // Our texture
LPDIRECT3DTEXTURE9      g_pTexture18 = NULL; // Our texture
LPDIRECT3DTEXTURE9      g_pTexture19 = NULL; // Our texture
LPDIRECT3DTEXTURE9      g_pTexture20 = NULL; // Our texture
LPDIRECT3DTEXTURE9      g_pTexture21 = NULL; // Our texture


LPDIRECT3DTEXTURE9      g_pTexture22 = NULL; // Our texture
LPDIRECT3DTEXTURE9      g_pTexture23= NULL; // Our texture
LPDIRECT3DTEXTURE9      g_pTexture24 = NULL; // Our texture
LPDIRECT3DTEXTURE9      g_pTexture25 = NULL; // Our texture
LPDIRECT3DTEXTURE9      g_pTexture26 = NULL; // Our texture
LPDIRECT3DTEXTURE9      g_pTexture27 = NULL; // Our texture
LPDIRECT3DTEXTURE9      g_pTexture28 = NULL; // Our texture
LPDIRECT3DTEXTURE9      g_pTexture29 = NULL; // Our texture
LPDIRECT3DTEXTURE9      g_pTexture30= NULL; // Our texture
LPDIRECT3DTEXTURE9      g_pTexture31 = NULL; // Our texture
LPDIRECT3DTEXTURE9      g_pTexture32 = NULL; // Our texture
LPDIRECT3DTEXTURE9      g_pTexture33 = NULL; // Our texture


float x;
float y;

										   // A structure for our custom vertex type. We added texture coordinates
struct CUSTOMVERTEX
{
	D3DXVECTOR3 position; // The position
	D3DCOLOR color;    // The color
#ifndef SHOW_HOW_TO_USE_TCI
	FLOAT tu, tv;   // The texture coordinates
#endif
};

// Our custom FVF, which describes our custom vertex structure
#ifdef SHOW_HOW_TO_USE_TCI
#define D3DFVF_CUSTOMVERTEX (D3DFVF_XYZ|D3DFVF_DIFFUSE)
#else
#define D3DFVF_CUSTOMVERTEX (D3DFVF_XYZ|D3DFVF_DIFFUSE|D3DFVF_TEX1)
#endif



//-----------------------------------------------------------------------------
// Name: InitD3D()
// Desc: Initializes Direct3D
//-----------------------------------------------------------------------------
HRESULT InitD3D(HWND hWnd)
{
	// Create the D3D object.
	if (NULL == (g_pD3D = Direct3DCreate9(D3D_SDK_VERSION)))
		return E_FAIL;

	// Set up the structure used to create the D3DDevice. Since we are now
	// using more complex geometry, we will create a device with a zbuffer.
	D3DPRESENT_PARAMETERS d3dpp;
	ZeroMemory(&d3dpp, sizeof(d3dpp));
	d3dpp.Windowed = TRUE;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;
	d3dpp.EnableAutoDepthStencil = TRUE;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;

	// Create the D3DDevice
	if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd,
		D3DCREATE_SOFTWARE_VERTEXPROCESSING,
		&d3dpp, &g_pd3dDevice)))
	{
		return E_FAIL;
	}

	// Turn off culling
	g_pd3dDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	// Turn off D3D lighting
	g_pd3dDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	// Turn on the zbuffer
	g_pd3dDevice->SetRenderState(D3DRS_ZENABLE, TRUE);

	return S_OK;
}




//-----------------------------------------------------------------------------
// Name: InitGeometry()
// Desc: Create the Textures and vertex buffers
//-----------------------------------------------------------------------------
HRESULT InitGeometry()
{
	// Use D3DX to create a texture from a file based image
	D3DXCreateTextureFromFile(g_pd3dDevice, L"1-1.png", &g_pTexture);
	D3DXCreateTextureFromFile(g_pd3dDevice, L"1-2.png", &g_pTexture1);
	D3DXCreateTextureFromFile(g_pd3dDevice, L"1-3.png", &g_pTexture2);
	D3DXCreateTextureFromFile(g_pd3dDevice, L"1-4.png", &g_pTexture3);
	D3DXCreateTextureFromFile(g_pd3dDevice, L"1-5.png", &g_pTexture4);
	D3DXCreateTextureFromFile(g_pd3dDevice, L"1-6.png", &g_pTexture5);
	D3DXCreateTextureFromFile(g_pd3dDevice, L"1-7.png", &g_pTexture6);
	D3DXCreateTextureFromFile(g_pd3dDevice, L"1-8.png", &g_pTexture7);
	D3DXCreateTextureFromFile(g_pd3dDevice, L"1-9.png", &g_pTexture8);
	D3DXCreateTextureFromFile(g_pd3dDevice, L"1-10.png", &g_pTexture9);
	D3DXCreateTextureFromFile(g_pd3dDevice, L"1-11.png", &g_pTexture10);

	D3DXCreateTextureFromFile(g_pd3dDevice, L"2-1.png", &g_pTexture11);
	D3DXCreateTextureFromFile(g_pd3dDevice, L"2-2.png", &g_pTexture12);
	D3DXCreateTextureFromFile(g_pd3dDevice, L"2-3.png", &g_pTexture13);
	D3DXCreateTextureFromFile(g_pd3dDevice, L"2-4.png", &g_pTexture14);
	D3DXCreateTextureFromFile(g_pd3dDevice, L"2-5.png", &g_pTexture15);
	D3DXCreateTextureFromFile(g_pd3dDevice, L"2-6.png", &g_pTexture16);
	D3DXCreateTextureFromFile(g_pd3dDevice, L"2-7.png", &g_pTexture17);
	D3DXCreateTextureFromFile(g_pd3dDevice, L"2-8.png", &g_pTexture18);
	D3DXCreateTextureFromFile(g_pd3dDevice, L"2-9.png", &g_pTexture19);
	D3DXCreateTextureFromFile(g_pd3dDevice, L"2-10.png", &g_pTexture20);
	D3DXCreateTextureFromFile(g_pd3dDevice, L"2-11.png", &g_pTexture21);


	D3DXCreateTextureFromFile(g_pd3dDevice, L"3-1.png", &g_pTexture22);
	D3DXCreateTextureFromFile(g_pd3dDevice, L"3-2.png", &g_pTexture23);
	D3DXCreateTextureFromFile(g_pd3dDevice, L"3-3.png", &g_pTexture24);
	D3DXCreateTextureFromFile(g_pd3dDevice, L"3-4.png", &g_pTexture25);
	D3DXCreateTextureFromFile(g_pd3dDevice, L"3-5.png", &g_pTexture26);
	D3DXCreateTextureFromFile(g_pd3dDevice, L"3-6.png", &g_pTexture27);
	D3DXCreateTextureFromFile(g_pd3dDevice, L"3-7.png", &g_pTexture28);
	D3DXCreateTextureFromFile(g_pd3dDevice, L"3-8.png", &g_pTexture29);
	D3DXCreateTextureFromFile(g_pd3dDevice, L"3-9.png", &g_pTexture30);
	D3DXCreateTextureFromFile(g_pd3dDevice, L"3-10.png", &g_pTexture31);
	D3DXCreateTextureFromFile(g_pd3dDevice, L"3-11.png", &g_pTexture32);
	D3DXCreateTextureFromFile(g_pd3dDevice, L"3-12.png", &g_pTexture33);




	// Create the vertex buffer.
	if (FAILED(g_pd3dDevice->CreateVertexBuffer(4 * sizeof(CUSTOMVERTEX),
		0, D3DFVF_CUSTOMVERTEX,
		D3DPOOL_DEFAULT, &g_pVB, NULL)))
	{
		return E_FAIL;
	}

	if (FAILED(g_pd3dDevice->CreateVertexBuffer(4 * sizeof(CUSTOMVERTEX),
		0, D3DFVF_CUSTOMVERTEX,
		D3DPOOL_DEFAULT, &g_pVB2, NULL)))
	{
		return E_FAIL;
	}


	// Fill the vertex buffer. We are setting the tu and tv texture
	// coordinates, which range from 0.0 to 1.0
	//첫번째 텍스처
	CUSTOMVERTEX* pVertices;
	{
	if (FAILED(g_pVB->Lock(0, 0, (void**)&pVertices, 0)))
	{
		return E_FAIL;
	}
	pVertices[0].position = D3DXVECTOR3(0.0, 1.0, 0.0f);
	pVertices[0].color = 0xffffffff;
	pVertices[0].tu = 0.0f;
	pVertices[0].tv = 0.0f;

	pVertices[1].position = D3DXVECTOR3(1.0, 1.0, 0.0f);
	pVertices[1].color = 0xffffffff;
	pVertices[1].tu = 1.0f;
	pVertices[1].tv = 0.0f;

	pVertices[2].position = D3DXVECTOR3(0.0, 0.0, 0.0f);
	pVertices[2].color = 0xffffffff;
	pVertices[2].tu = 0.0f;
	pVertices[2].tv = 1.0f;

	pVertices[3].position = D3DXVECTOR3(1.0, 0.0, 0.0f);
	pVertices[3].color = 0xffffffff;
	pVertices[3].tu = 1.0f;
	pVertices[3].tv = 1.0f;
	g_pVB->Unlock();
	}
}




//-----------------------------------------------------------------------------
// Name: Cleanup()
// Desc: Releases all previously initialized objects
//-----------------------------------------------------------------------------
VOID Cleanup()
{
	if (g_pTexture != NULL)
		g_pTexture->Release();

	if (g_pVB != NULL)
		g_pVB->Release();

	if (g_pVB2 != NULL)
		g_pVB->Release();

	if (g_pd3dDevice != NULL)
		g_pd3dDevice->Release();

	if (g_pD3D != NULL)
		g_pD3D->Release();
}



//-----------------------------------------------------------------------------
// Name: SetupMatrices()
// Desc: Sets up the world, view, and projection transform matrices.
//-----------------------------------------------------------------------------
VOID SetupMatrices()
{

	// Set up world matrix
	D3DXMATRIXA16 matWorld;
	D3DXMatrixTranslation(&matWorld, x, y, 0.0f);
	//D3DXMatrixRotationX( &matWorld,180);
	g_pd3dDevice->SetTransform(D3DTS_WORLD, &matWorld);

	// Set up our view matrix. A view matrix can be defined given an eye point,
	// a point to lookat, and a direction for which way is up. Here, we set the
	// eye five units back along the z-axis and up three units, look at the
	// origin, and define "up" to be in the y-direction.
	D3DXVECTOR3 vEyePt(0.0f, 3.0f, -5.0f);
	D3DXVECTOR3 vLookatPt(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 vUpVec(0.0f, 1.0f, 0.0f);
	D3DXMATRIXA16 matView;
	D3DXMatrixLookAtLH(&matView, &vEyePt, &vLookatPt, &vUpVec);
	g_pd3dDevice->SetTransform(D3DTS_VIEW, &matView);

	// For the projection matrix, we set up a perspective transform (which
	// transforms geometry from 3D view space to 2D viewport space, with
	// a perspective divide making objects smaller in the distance). To build
	// a perpsective transform, we need the field of view (1/4 pi is common),
	// the aspect ratio, and the near and far clipping planes (which define at
	// what distances geometry should be no longer be rendered).
	D3DXMATRIXA16 matProj;
	D3DXMatrixPerspectiveFovLH(&matProj, D3DX_PI / 4, 1.0f, 1.0f, 100.0f);
	g_pd3dDevice->SetTransform(D3DTS_PROJECTION, &matProj);
}


//-----------------------------------------------------------------------------
// Name: Render()
// Desc: Draws the scene
//-----------------------------------------------------------------------------
VOID Render()
{
	static int counter = 0;
	static int Time = 0;

	static int counter2 = 0;
	static int Time2 = 0;

	// Clear the backbuffer and the zbuffer
	g_pd3dDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
		D3DCOLOR_XRGB(255, 255, 255), 1.0f, 0);

	// Begin the scene
	if (SUCCEEDED(g_pd3dDevice->BeginScene()))
	{
		// Setup the world, view, and projection matrices
		SetupMatrices();
		g_pd3dDevice->SetStreamSource(0, g_pVB, 0, sizeof(CUSTOMVERTEX));
		g_pd3dDevice->SetFVF(D3DFVF_CUSTOMVERTEX);
		g_pd3dDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
		Time = Time + 1;
		if(Time %12 == 0)
		{
			counter = counter + 1;
		}
			switch (counter % 34)
			{
			case 0 :
				g_pd3dDevice->SetTexture(0, g_pTexture);
				break;
			case 1 :
				g_pd3dDevice->SetTexture(0, g_pTexture1);
				break;
			case 2:
				g_pd3dDevice->SetTexture(0, g_pTexture2);
				break;
			case 3:
				g_pd3dDevice->SetTexture(0, g_pTexture3);
				break;
			case 4:
				g_pd3dDevice->SetTexture(0, g_pTexture4);
				break;
			case 5:
				g_pd3dDevice->SetTexture(0, g_pTexture5);
				break;
			case 6:
				g_pd3dDevice->SetTexture(0, g_pTexture6);
				break;
			case 7:
				g_pd3dDevice->SetTexture(0, g_pTexture7);
				break;
			case 8:
				g_pd3dDevice->SetTexture(0, g_pTexture8);
				break;
			case 9:
				g_pd3dDevice->SetTexture(0, g_pTexture9);
				break;
			case 10:
				g_pd3dDevice->SetTexture(0, g_pTexture10);
				break;
			case 11:
				g_pd3dDevice->SetTexture(0, g_pTexture11);
				break;
			case 12:
				g_pd3dDevice->SetTexture(0, g_pTexture12);
				break;
			case 13:
				g_pd3dDevice->SetTexture(0, g_pTexture13);
				break;
			case 14:
				g_pd3dDevice->SetTexture(0, g_pTexture14);
				break;
			case 15:
				g_pd3dDevice->SetTexture(0, g_pTexture15);
				break;
			case 16:
				g_pd3dDevice->SetTexture(0, g_pTexture16);
				break;
			case 17:
				g_pd3dDevice->SetTexture(0, g_pTexture17);
				break;
			case 18:
				g_pd3dDevice->SetTexture(0, g_pTexture18);
				break;
			case 19:
				g_pd3dDevice->SetTexture(0, g_pTexture19);
				break;
			case 20:
				g_pd3dDevice->SetTexture(0, g_pTexture20);
				break;
			case 21:
				g_pd3dDevice->SetTexture(0, g_pTexture21);
				break;
			case 22:
				g_pd3dDevice->SetTexture(0, g_pTexture22);
				break;
			case 23:
				g_pd3dDevice->SetTexture(0, g_pTexture23);
				break;
			case 24:
				g_pd3dDevice->SetTexture(0, g_pTexture24);
				break;
			case 25:
				g_pd3dDevice->SetTexture(0, g_pTexture25);
				break;
			case 26:
				g_pd3dDevice->SetTexture(0, g_pTexture26);
				break;
			case 27:
				g_pd3dDevice->SetTexture(0, g_pTexture27);
				break;
			case 28:
				g_pd3dDevice->SetTexture(0, g_pTexture28);
				break;
			case 29:
				g_pd3dDevice->SetTexture(0, g_pTexture29);
				break;
			case 30:
				g_pd3dDevice->SetTexture(0, g_pTexture30);
				break;
			case 31:
				g_pd3dDevice->SetTexture(0, g_pTexture31);
				break;
			case 32:
				g_pd3dDevice->SetTexture(0, g_pTexture32);
				break;
			case 33:
				g_pd3dDevice->SetTexture(0, g_pTexture33);
				break;
			}

		// Setup our texture. Using Textures introduces the texture stage states,
		// which govern how Textures get blended together (in the case of multiple
		// Textures) and lighting information. In this case, we are modulating
		// (blending) our texture with the diffuse color of the vertices.
		// Render the vertex buffer contents
			/*
		SetupMatrices2();
		g_pd3dDevice->SetStreamSource(0, g_pVB2, 0, sizeof(CUSTOMVERTEX));
		g_pd3dDevice->SetFVF(D3DFVF_CUSTOMVERTEX);
		g_pd3dDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
		Time2 = Time2 + 1;
		if (Time2 % 12 == 0)
		{
			counter2 = counter2 + 1;
		}
		switch (counter % 12)
		{
		case 0:
			g_pd3dDevice->SetTexture(0, g_pTexture22);
			break;
		case 1:
			g_pd3dDevice->SetTexture(0, g_pTexture23);
			break;
		case 2:
			g_pd3dDevice->SetTexture(0, g_pTexture24);
			break;
		case 3:
			g_pd3dDevice->SetTexture(0, g_pTexture25);
			break;
		case 4:
			g_pd3dDevice->SetTexture(0, g_pTexture26);
			break;
		case 5:
			g_pd3dDevice->SetTexture(0, g_pTexture27);
			break;
		case 6:
			g_pd3dDevice->SetTexture(0, g_pTexture28);
			break;
		case 7:
			g_pd3dDevice->SetTexture(0, g_pTexture29);
			break;
		case 8:
			g_pd3dDevice->SetTexture(0, g_pTexture30);
			break;
		case 9:
			g_pd3dDevice->SetTexture(0, g_pTexture31);
			break;
		case 10:
			g_pd3dDevice->SetTexture(0, g_pTexture32);
			break;
		case 11:
			g_pd3dDevice->SetTexture(0, g_pTexture33);
			break;
		}

		*/
		
		g_pd3dDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
		g_pd3dDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
		g_pd3dDevice->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
		g_pd3dDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_DISABLE);

		

		


		// End the scene
		g_pd3dDevice->EndScene();
	}

	// Present the backbuffer contents to the display
	g_pd3dDevice->Present(NULL, NULL, NULL, NULL);
}




//-----------------------------------------------------------------------------
// Name: MsgProc()
// Desc: The window's message handler
//-----------------------------------------------------------------------------
LRESULT WINAPI MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_DESTROY:
		Cleanup();
		PostQuitMessage(0);
		return 0;
	case WM_KEYDOWN :
		if (GetKeyState(VK_UP) && wParam == VK_UP)
		{
			y = y+1;
			//D3DXMatrixTranslation(&matWorld, x+1, y, 0.0f);
		}

		if (GetKeyState(VK_DOWN) && wParam == VK_DOWN)
		{
			y = y - 1;
			//D3DXMatrixTranslation(&matWorld, x+1, y, 0.0f);
		}

		if (GetKeyState(VK_LEFT) && wParam == VK_LEFT)
		{
			x = x - 1;
			//D3DXMatrixTranslation(&matWorld, x+1, y, 0.0f);
		}

		if (GetKeyState(VK_RIGHT) && wParam == VK_RIGHT)
		{
			x = x + 1;
			//D3DXMatrixTranslation(&matWorld, x+1, y, 0.0f);
		}

		break;
	}

	return DefWindowProc(hWnd, msg, wParam, lParam);
}




//-----------------------------------------------------------------------------
// Name: WinMain()
// Desc: The application's entry point
//-----------------------------------------------------------------------------
INT WINAPI wWinMain(HINSTANCE hInst, HINSTANCE, LPWSTR, INT)
{
	UNREFERENCED_PARAMETER(hInst);

	// Register the window class
	WNDCLASSEX wc =
	{
		sizeof(WNDCLASSEX), CS_CLASSDC, MsgProc, 0L, 0L,
		GetModuleHandle(NULL), NULL, NULL, NULL, NULL,
		L"D3D Tutorial", NULL
	};
	RegisterClassEx(&wc);

	// Create the application's window
	HWND hWnd = CreateWindow(L"D3D Tutorial", L"D3D Tutorial 05: Textures",
		WS_OVERLAPPEDWINDOW, 100, 100, 900, 900,
		NULL, NULL, wc.hInstance, NULL);

	// Initialize Direct3D
	if (SUCCEEDED(InitD3D(hWnd)))
	{
		// Create the scene geometry
		if (SUCCEEDED(InitGeometry()))
		{
			// Show the window
			ShowWindow(hWnd, SW_SHOWDEFAULT);
			UpdateWindow(hWnd);

			// Enter the message loop
			MSG msg;
			ZeroMemory(&msg, sizeof(msg));
			while (msg.message != WM_QUIT)
			{
				if (PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE))
				{
					TranslateMessage(&msg);
					DispatchMessage(&msg);
				}
				else
					Render();
			}
		}
	}

	UnregisterClass(L"D3D Tutorial", wc.hInstance);
	return 0;
}



