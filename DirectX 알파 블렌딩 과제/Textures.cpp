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
LPDIRECT3DTEXTURE9      g_pTexture = NULL; // Our texture

float g_fElapsedTime =0.1;
double g_dCurrentTime = 1;
double g_dLastTime = 1;

// A structure for our custom vertex type. We added texture coordinates

/*
struct CUSTOMVERTEX
{
    D3DXVECTOR3 position; // The position
    D3DCOLOR color;    // The color
#ifndef SHOW_HOW_TO_USE_TCI
    FLOAT tu, tv;   // The texture coordinates
#endif
};
*/
struct Vertex
{
	float x, y, z;
	float tu, tv;
};

Vertex g_cubeVertices[] =
{
	//Quad1
	{ -1.0f, 1.0f, -1.0f,   0.0f, 0.0f, },
	{ 1.0f, 1.0f, -1.0f,    1.0f, 0.0f },
	{ -1.0f, -1.0f, -1.0f,  0.0f, 1.0f },
	{ 1.0f, -1.0f, -1.0f,   1.0f, 1.0f },

	//Quad2
	{ -1.0f, 1.0f, 1.0f,   0.0f, 0.0f },
	{ -1.0f, -1.0f, 1.0f,   1.0f, 0.0f },
	{ 1.0f, 1.0f, 1.0f,   0.0f, 1.0f },
	{ 1.0f, -1.0f, 1.0f,   1.0f, 1.0f },

	//Quad3
	{ -1.0f, 1.0f, 1.0f,   0.0f, 0.0f },
	{ 1.0f, 1.0f, 1.0f,   1.0f, 0.0f },
	{ -1.0f, 1.0f, -1.0f,   0.0f, 1.0f },
	{ 1.0f, 1.0f, -1.0f,   1.0f, 1.0f },

	//Quad4
	{ -1.0f, -1.0f, 1.0f,   0.0f, 0.0f },
	{ -1.0f, -1.0f, -1.0f,   1.0f, 0.0f },
	{ 1.0f, -1.0f, 1.0f,   0.0f, 1.0f },
	{ 1.0f, -1.0f, -1.0f,   1.0f, 1.0f },


	//Quad5
	{ 1.0f, 1.0f, -1.0f,   0.0f, 0.0f },
	{ 1.0f, 1.0f, 1.0f,   1.0f, 0.0f },
	{ 1.0f, -1.0f, -1.0f,   0.0f, 1.0f },
	{ 1.0f, -1.0f, 1.0f,   1.0f, 1.0f },

	//Quad6
	{ -1.0f, 1.0f, -1.0f,   1.0f, 0.0f },
	{ -1.0f, -1.0f, -1.0f,   1.0f, 1.0f },
	{ -1.0f, 1.0f, 1.0f,   0.0f, 0.0f },
	{ -1.0f, -1.0f, 1.0f,   0.0f, 1.0f },
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
HRESULT InitD3D( HWND hWnd )
{
    // Create the D3D object.
    if( NULL == ( g_pD3D = Direct3DCreate9( D3D_SDK_VERSION ) ) )
        return E_FAIL;

    // Set up the structure used to create the D3DDevice. Since we are now
    // using more complex geometry, we will create a device with a zbuffer.
    D3DPRESENT_PARAMETERS d3dpp;
    ZeroMemory( &d3dpp, sizeof( d3dpp ) );
    d3dpp.Windowed = TRUE;
    d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
    d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;
    d3dpp.EnableAutoDepthStencil = TRUE;
    d3dpp.AutoDepthStencilFormat = D3DFMT_D16;

    // Create the D3DDevice
    if( FAILED( g_pD3D->CreateDevice( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd,
                                      D3DCREATE_SOFTWARE_VERTEXPROCESSING,
                                      &d3dpp, &g_pd3dDevice ) ) )
    {
        return E_FAIL;
    }

    // Turn off culling
    g_pd3dDevice->SetRenderState( D3DRS_CULLMODE, D3DCULL_NONE );

    // Turn off D3D lighting
    g_pd3dDevice->SetRenderState( D3DRS_LIGHTING, FALSE );

    // Turn on the zbuffer
    g_pd3dDevice->SetRenderState( D3DRS_ZENABLE, TRUE );

    return S_OK;
}




//-----------------------------------------------------------------------------
// Name: InitGeometry()
// Desc: Create the Textures and vertex buffers
//-----------------------------------------------------------------------------
HRESULT InitGeometry()
{
	 // Use D3DX to create a texture from a file based image
        // If texture is not in current folder, try parent folder
		D3DXCreateTextureFromFile(g_pd3dDevice, L"Zero.png", &g_pTexture);
		g_pd3dDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
		g_pd3dDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	if (FAILED(g_pd3dDevice->CreateVertexBuffer(24 * sizeof(Vertex),
		0, D3DFVF_CUSTOMVERTEX,
		D3DPOOL_DEFAULT, &g_pVB, NULL)))
	{
		return E_FAIL;
	}
	void* pVertices = NULL;

	g_pVB->Lock(0, sizeof(g_cubeVertices), (void**)&pVertices, 0);
	memcpy(pVertices, g_cubeVertices, sizeof(g_cubeVertices));
	g_pVB->Unlock();

	g_pd3dDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	g_pd3dDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
	
    return S_OK;
}




//-----------------------------------------------------------------------------
// Name: Cleanup()
// Desc: Releases all previously initialized objects
//-----------------------------------------------------------------------------
VOID Cleanup()
{
    if( g_pTexture != NULL )
        g_pTexture->Release();

    if( g_pVB != NULL )
        g_pVB->Release();

    if( g_pd3dDevice != NULL )
        g_pd3dDevice->Release();

    if( g_pD3D != NULL )
        g_pD3D->Release();
}



//-----------------------------------------------------------------------------
// Name: SetupMatrices()
// Desc: Sets up the world, view, and projection transform matrices.
//-----------------------------------------------------------------------------
VOID SetupMatrices()
{


    // Set up our view matrix. A view matrix can be defined given an eye point,
    // a point to lookat, and a direction for which way is up. Here, we set the
    // eye five units back along the z-axis and up three units, look at the
    // origin, and define "up" to be in the y-direction.
    D3DXVECTOR3 vEyePt( 0.0f, 3.0f,-5.0f );
    D3DXVECTOR3 vLookatPt( 0.0f, 0.0f, 0.0f );
    D3DXVECTOR3 vUpVec( 0.0f, 1.0f, 0.0f );
    D3DXMATRIXA16 matView;
    D3DXMatrixLookAtLH( &matView, &vEyePt, &vLookatPt, &vUpVec );
    g_pd3dDevice->SetTransform( D3DTS_VIEW, &matView );

    // For the projection matrix, we set up a perspective transform (which
    // transforms geometry from 3D view space to 2D viewport space, with
    // a perspective divide making objects smaller in the distance). To build
    // a perpsective transform, we need the field of view (1/4 pi is common),
    // the aspect ratio, and the near and far clipping planes (which define at
    // what distances geometry should be no longer be rendered).
    D3DXMATRIXA16 matProj;
    D3DXMatrixPerspectiveFovLH( &matProj, D3DX_PI / 4, 1.0f, 1.0f, 100.0f );
    g_pd3dDevice->SetTransform( D3DTS_PROJECTION, &matProj );
}




//-----------------------------------------------------------------------------
// Name: Render()
// Desc: Draws the scene
//-----------------------------------------------------------------------------
VOID Render()
{
    // Clear the backbuffer and the zbuffer
    g_pd3dDevice->Clear( 0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
                         D3DCOLOR_XRGB( 255, 255, 255 ), 1.0f, 0 );

	
	static float fXrot = 0.0f;
	static float fYrot = 0.0f;
	static float fZrot = 0.0f;

	fXrot += 10.1f * g_fElapsedTime;
	fYrot += 10.2f * g_fElapsedTime;
	fZrot += 10.3f * g_fElapsedTime;


	D3DXMATRIXA16 matWorld;
	D3DXMATRIXA16 matTrans;
	D3DXMATRIXA16 matRot;

	D3DXMatrixTranslation(&matTrans, 0.0f, 0.0f, 0.0f);
	D3DXMatrixRotationYawPitchRoll(&matRot, D3DXToRadian(fXrot), D3DXToRadian(fYrot), D3DXToRadian(fZrot));

	matWorld = matRot * matTrans;

	g_pd3dDevice->SetTransform(D3DTS_WORLD, &matWorld);
	
	
    // Begin the scene
    if( SUCCEEDED( g_pd3dDevice->BeginScene() ) )
    {
        // Setup the world, view, and projection matrices
		SetupMatrices();
		g_pd3dDevice->SetTexture(0, g_pTexture);
		g_pd3dDevice->SetStreamSource(0, g_pVB, 0, sizeof(Vertex));
		g_pd3dDevice->SetFVF(D3DFVF_CUSTOMVERTEX);

		g_pd3dDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
		g_pd3dDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4, 2);
		g_pd3dDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 8, 2);
		g_pd3dDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 12, 2);
		g_pd3dDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 16, 2);
		g_pd3dDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 20, 2);
		// Setup our texture. Using Textures introduces the texture stage states,
		// which govern how Textures get blended together (in the case of multiple
		// Textures) and lighting information. In this case, we are modulating
		// (blending) our texture with the diffuse color of the vertices.
		
		if (DWORD Value = TRUE)
		{
			g_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);    // turn on the color blending
			g_pd3dDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);    // set source factor
			g_pd3dDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);    // set dest factor

			g_pd3dDevice->SetTexture(0, g_pTexture);
			g_pd3dDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
			g_pd3dDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
			g_pd3dDevice->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
			g_pd3dDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_DISABLE);

			g_pd3dDevice->SetStreamSource(0, g_pVB, 0, sizeof(Vertex));
			g_pd3dDevice->SetFVF(D3DFVF_CUSTOMVERTEX);


			if (D3DRS_CULLMODE == TRUE)
			{
				// Render the vertex buffer contents
				g_pd3dDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW);
				g_pd3dDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
				g_pd3dDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4, 2);
				g_pd3dDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 8, 2);
				g_pd3dDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 12, 2);
				g_pd3dDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 16, 2);
				g_pd3dDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 20, 2);


				g_pd3dDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
				g_pd3dDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
				g_pd3dDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4, 2);
				g_pd3dDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 8, 2);
				g_pd3dDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 12, 2);
				g_pd3dDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 16, 2);
				g_pd3dDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 20, 2);
			}
			else
			{

				g_pd3dDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
				g_pd3dDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
				g_pd3dDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4, 2);
				g_pd3dDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 8, 2);
				g_pd3dDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 12, 2);
				g_pd3dDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 16, 2);
				g_pd3dDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 20, 2);
			}
		}
		
		else
		{


		g_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);    // turn on the color blending
		g_pd3dDevice->SetRenderState(D3DRS_ZENABLE, TRUE);

		g_pd3dDevice->SetTexture(0, g_pTexture);
		g_pd3dDevice->SetStreamSource(0, g_pVB, 0, sizeof(Vertex));
		g_pd3dDevice->SetFVF(D3DFVF_CUSTOMVERTEX);

		g_pd3dDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
		g_pd3dDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
		g_pd3dDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4, 2);
		g_pd3dDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 8, 2);
		g_pd3dDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 12, 2);
		g_pd3dDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 16, 2);
		g_pd3dDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 20, 2);
	}

		
        // End the scene
        g_pd3dDevice->EndScene();
    }

    // Present the backbuffer contents to the display
    g_pd3dDevice->Present( NULL, NULL, NULL, NULL );
}




//-----------------------------------------------------------------------------
// Name: MsgProc()
// Desc: The window's message handler
//-----------------------------------------------------------------------------
LRESULT WINAPI MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
    switch( msg )
    {
        case WM_DESTROY:
            Cleanup();
            PostQuitMessage( 0 );
            return 0;
    }

    return DefWindowProc( hWnd, msg, wParam, lParam );
}




//-----------------------------------------------------------------------------
// Name: WinMain()
// Desc: The application's entry point
//-----------------------------------------------------------------------------
INT WINAPI wWinMain( HINSTANCE hInst, HINSTANCE, LPWSTR, INT )
{
    UNREFERENCED_PARAMETER( hInst );

    // Register the window class
    WNDCLASSEX wc =
    {
        sizeof( WNDCLASSEX ), CS_CLASSDC, MsgProc, 0L, 0L,
        GetModuleHandle( NULL ), NULL, NULL, NULL, NULL,
        L"D3D Tutorial", NULL
    };
    RegisterClassEx( &wc );

    // Create the application's window
    HWND hWnd = CreateWindow( L"D3D Tutorial", L"D3D Tutorial 05: Textures",
                              WS_OVERLAPPEDWINDOW, 100, 100,900, 900,
                              NULL, NULL, wc.hInstance, NULL );

    // Initialize Direct3D
    if( SUCCEEDED( InitD3D( hWnd ) ) )
    {
        // Create the scene geometry
        if( SUCCEEDED( InitGeometry() ) )
        {
            // Show the window
            ShowWindow( hWnd, SW_SHOWDEFAULT );
            UpdateWindow( hWnd );

            // Enter the message loop
            MSG msg;
            ZeroMemory( &msg, sizeof( msg ) );
            while( msg.message != WM_QUIT )
            {
                if( PeekMessage( &msg, NULL, 0U, 0U, PM_REMOVE ) )
                {
                    TranslateMessage( &msg );
                    DispatchMessage( &msg );
                }
                else
                    Render();
            }
        }
    }

    UnregisterClass( L"D3D Tutorial", wc.hInstance );
    return 0;
}



