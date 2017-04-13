//-----------------------------------------------------------------------------
// File: Matrices.cpp
//
// Desc: Now that we know how to create a device and render some 2D vertices,
//       this tutorial goes the next step and renders 3D geometry. To deal with
//       3D geometry we need to introduce the use of 4x4 Matrices to transform
//       the geometry with translations, rotations, scaling, and setting up our
//       camera.
//
//       Geometry is defined in model space. We can move it (translation),
//       rotate it (rotation), or stretch it (scaling) using a world transform.
//       The geometry is then said to be in world space. Next, we need to
//       position the camera, or eye point, somewhere to look at the geometry.
//       Another transform, via the view matrix, is used, to position and
//       rotate our view. With the geometry then in view space, our last
//       transform is the projection transform, which "projects" the 3D scene
//       into our 2D viewport.
//
//       Note that in this tutorial, we are introducing the use of D3DX, which
//       is a set of helper utilities for D3D. In this case, we are using some
//       of D3DX's useful matrix initialization functions. To use D3DX, simply
//       include <d3dx9.h> and link with d3dx9.lib.
//
// Copyright (c) Microsoft Corporation. All rights reserved.
//-----------------------------------------------------------------------------
#include <Windows.h>
#include <mmsystem.h>
#include <d3dx9.h>
#pragma warning( disable : 4996 ) // disable deprecated warning 
#include <strsafe.h>
#pragma warning( default : 4996 )



float iSpeed = 1;

//-----------------------------------------------------------------------------
// Global variables
//-----------------------------------------------------------------------------
LPDIRECT3D9             g_pD3D = NULL; // Used to create the D3DDevice
LPDIRECT3DDEVICE9       g_pd3dDevice = NULL; // Our rendering device
LPDIRECT3DVERTEXBUFFER9 g_pVB = NULL; // Buffer to hold vertices
LPDIRECT3DVERTEXBUFFER9 g_pVB2 = NULL; // Buffer to hold vertices
LPDIRECT3DVERTEXBUFFER9 g_pVB3 = NULL; // Buffer to hold vertices
LPDIRECT3DVERTEXBUFFER9 g_pVB4 = NULL; // Buffer to hold vertices
LPDIRECT3DVERTEXBUFFER9 g_pVB5 = NULL; // Buffer to hold vertices
LPDIRECT3DVERTEXBUFFER9 g_pVB6 = NULL; // Buffer to hold vertices
LPDIRECT3DVERTEXBUFFER9 g_pVB7 = NULL; // Buffer to hold vertices
LPDIRECT3DVERTEXBUFFER9 g_pVB8 = NULL; // Buffer to hold vertices

// A structure for our custom vertex type
struct CUSTOMVERTEX
{
    FLOAT x, y, z;      // The untransformed, 3D position for the vertex
    DWORD color;        // The vertex color
};

// Our custom FVF, which describes our custom vertex structure
#define D3DFVF_CUSTOMVERTEX (D3DFVF_XYZ|D3DFVF_DIFFUSE)




//-----------------------------------------------------------------------------
// Name: InitD3D()
// Desc: Initializes Direct3D
//-----------------------------------------------------------------------------
HRESULT InitD3D( HWND hWnd )
{
    // Create the D3D object.
    if( NULL == ( g_pD3D = Direct3DCreate9( D3D_SDK_VERSION ) ) )
        return E_FAIL;

    // Set up the structure used to create the D3DDevice
    D3DPRESENT_PARAMETERS d3dpp;
    ZeroMemory( &d3dpp, sizeof( d3dpp ) );
    d3dpp.Windowed = TRUE;
    d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
    d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;

    // Create the D3DDevice
    if( FAILED( g_pD3D->CreateDevice( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd,
                                      D3DCREATE_SOFTWARE_VERTEXPROCESSING,
                                      &d3dpp, &g_pd3dDevice ) ) )
    {
        return E_FAIL;
    }

    // Turn off culling, so we see the front and back of the triangle
    g_pd3dDevice->SetRenderState( D3DRS_CULLMODE, D3DCULL_NONE );

    // Turn off D3D lighting, since we are providing our own vertex colors
    g_pd3dDevice->SetRenderState( D3DRS_LIGHTING, FALSE );

    return S_OK;
}




//-----------------------------------------------------------------------------
// Name: InitGeometry()
// Desc: Creates the scene geometry
//-----------------------------------------------------------------------------
HRESULT InitGeometry()
{
    // Initialize three vertices for rendering a triangle
    CUSTOMVERTEX g_Vertices[] =
    {
        { -1.0f,-1.0f, 0.0f, 0x0058FA58 , }, //0xff00ff00
        {  1.0f,-1.0f, 0.0f, 0x000040FF, },
        {  0.0f, 1.0f, 0.0f, 0xFF00FF, },
    };

    // Create the vertex buffer.
    if( FAILED( g_pd3dDevice->CreateVertexBuffer( 3 * sizeof( CUSTOMVERTEX ),
                                                  0, D3DFVF_CUSTOMVERTEX,
                                                  D3DPOOL_DEFAULT, &g_pVB, NULL ) ) )
    {
        return E_FAIL;
    }

    // Fill the vertex buffer.
    VOID* pVertices;
    if( FAILED( g_pVB->Lock( 0, sizeof( g_Vertices ), ( void** )&pVertices, 0 ) ) )
        return E_FAIL;
    memcpy( pVertices, g_Vertices, sizeof( g_Vertices ) );
    g_pVB->Unlock();

    
	// µÎ¹øÂ° »ï°¢Çü
	CUSTOMVERTEX g_Vertices2[] =
	{
		{ -1.0f,-1.0f, 0.0f, 0xFE2E64, },
		{ 1.0f,-1.0f, 0.0f, 0x0B610B, },
		{ 0.0f, 1.0f, 0.0f, 0x610B0B, },
	};

	// Create the vertex buffer.
	if (FAILED(g_pd3dDevice->CreateVertexBuffer(3 * sizeof(CUSTOMVERTEX),
		0, D3DFVF_CUSTOMVERTEX,
		D3DPOOL_DEFAULT, &g_pVB2, NULL)))
	{
		return E_FAIL;
	}

	// Fill the vertex buffer.
	VOID* pVertices2;
	if (FAILED(g_pVB2->Lock(0, sizeof(g_Vertices2), (void**)&pVertices2, 0)))
		return E_FAIL;
	memcpy(pVertices2, g_Vertices2, sizeof(g_Vertices2));
	g_pVB2->Unlock();

	//¼¼¹øÂ° »ï°¢Çü
	CUSTOMVERTEX g_Vertices3[] =
	{
		{ -1.0f,-1.0f, 0.0f, 0xFF0000, },
		{ 1.0f,-1.0f, 0.0f, 0x00BFFF, },
		{ 0.0f, 1.0f, 0.0f, 0xD7DF01, },
	};

	// Create the vertex buffer.
	if (FAILED(g_pd3dDevice->CreateVertexBuffer(3 * sizeof(CUSTOMVERTEX),
		0, D3DFVF_CUSTOMVERTEX,
		D3DPOOL_DEFAULT, &g_pVB3, NULL)))
	{
		return E_FAIL;
	}

	// Fill the vertex buffer.
	VOID* pVertices3;
	if (FAILED(g_pVB3->Lock(0, sizeof(g_Vertices3), (void**)&pVertices3, 0)))
		return E_FAIL;
	memcpy(pVertices3, g_Vertices3, sizeof(g_Vertices3));
	g_pVB3->Unlock();

	//³×¹øÂ°»ï°¢Çü(Áß½É »ï°¢Çü À§)
	CUSTOMVERTEX g_Vertices4[] =
	{
		{ -1.0f,-1.0f, 0.0f, 0xF5A9F2, },
		{ 1.0f,-1.0f, 0.0f, 0xDA81F5, },
		{ 0.0f, 1.0f, 0.0f, 0x8A0829, },
	};

	// Create the vertex buffer.
	if (FAILED(g_pd3dDevice->CreateVertexBuffer(3 * sizeof(CUSTOMVERTEX),
		0, D3DFVF_CUSTOMVERTEX,
		D3DPOOL_DEFAULT, &g_pVB4, NULL)))
	{
		return E_FAIL;
	}

	// Fill the vertex buffer.
	VOID* pVertices4;
	if (FAILED(g_pVB4->Lock(0, sizeof(g_Vertices4), (void**)&pVertices4, 0)))
		return E_FAIL;
	memcpy(pVertices4, g_Vertices4, sizeof(g_Vertices4));
	g_pVB4->Unlock();

	//´Ù¼¸¹øÂ°»ï°¢Çü(³×¹øÂ°²¨¿Í ´ëÄª)
	CUSTOMVERTEX g_Vertices5[] =
	{
		{ -1.0f,-1.0f, 0.0f, 0x8181F7, },
		{ 1.0f,-1.0f, 0.0f, 0x2ECCFA, },
		{ 0.0f, 1.0f, 0.0f, 0xF5A9A9, },
	};

	// Create the vertex buffer.
	if (FAILED(g_pd3dDevice->CreateVertexBuffer(3 * sizeof(CUSTOMVERTEX),
		0, D3DFVF_CUSTOMVERTEX,
		D3DPOOL_DEFAULT, &g_pVB5, NULL)))
	{
		return E_FAIL;
	}

	// Fill the vertex buffer.
	VOID* pVertices5;
	if (FAILED(g_pVB5->Lock(0, sizeof(g_Vertices5), (void**)&pVertices5, 0)))
		return E_FAIL;
	memcpy(pVertices5, g_Vertices5, sizeof(g_Vertices5));
	g_pVB5->Unlock();

	//¿©¼¸¹ø¤ŠÁß¾Ó À§ »ï°¢Çü
	CUSTOMVERTEX g_Vertices6[] =
	{
		{ -1.0f,-1.0f, 0.0f, 0x58FAAC, },
		{ 1.0f,-1.0f, 0.0f, 0x58D3F7, },
		{ 0.0f, 1.0f, 0.0f, 0x0B4C5F, },
	};

	// Create the vertex buffer.
	if (FAILED(g_pd3dDevice->CreateVertexBuffer(3 * sizeof(CUSTOMVERTEX),
		0, D3DFVF_CUSTOMVERTEX,
		D3DPOOL_DEFAULT, &g_pVB6, NULL)))
	{
		return E_FAIL;
	}

	// Fill the vertex buffer.
	VOID* pVertices6;
	if (FAILED(g_pVB6->Lock(0, sizeof(g_Vertices6), (void**)&pVertices6, 0)))
		return E_FAIL;
	memcpy(pVertices6, g_Vertices6, sizeof(g_Vertices6));
	g_pVB6->Unlock();

	//ÀÏ°ö¹ø¤ŠÁß¾Ó ¾Æ·¡ »ï°¢Çü
	CUSTOMVERTEX g_Vertices7[] =
	{
		{ -1.0f,-1.0f, 0.0f, 0xFE642E, },
		{ 1.0f,-1.0f, 0.0f, 0xF3F781,},
		{ 0.0f, 1.0f, 0.0f, 0xFACC2E, },
	};

	// Create the vertex buffer.
	if (FAILED(g_pd3dDevice->CreateVertexBuffer(3 * sizeof(CUSTOMVERTEX),
		0, D3DFVF_CUSTOMVERTEX,
		D3DPOOL_DEFAULT, &g_pVB7, NULL)))
	{
		return E_FAIL;
	}

	// Fill the vertex buffer.
	VOID* pVertices7;
	if (FAILED(g_pVB7->Lock(0, sizeof(g_Vertices7), (void**)&pVertices7, 0)))
		return E_FAIL;
	memcpy(pVertices7, g_Vertices7, sizeof(g_Vertices7));
	g_pVB7->Unlock();


	//ÀÏ°ö¹ø¤ŠÁß¾Ó ¾Æ·¡ »ï°¢Çü
	CUSTOMVERTEX g_Vertices8[] =
	{
		{ -1.0f,-1.0f, 0.0f, 0x610B0B, },
		{ 1.0f,-2.0f, 0.0f, 0xDF0101, },
		{ 0.0f, 1.0f, 0.0f, 0xF78181, },
	};

	// Create the vertex buffer.
	if (FAILED(g_pd3dDevice->CreateVertexBuffer(3 * sizeof(CUSTOMVERTEX),
		0, D3DFVF_CUSTOMVERTEX,
		D3DPOOL_DEFAULT, &g_pVB8, NULL)))
	{
		return E_FAIL;
	}

	// Fill the vertex buffer.
	VOID* pVertices8;
	if (FAILED(g_pVB8->Lock(0, sizeof(g_Vertices8), (void**)&pVertices8, 0)))
		return E_FAIL;
	memcpy(pVertices8, g_Vertices8, sizeof(g_Vertices8));
	g_pVB8->Unlock();

	return S_OK;
}




//-----------------------------------------------------------------------------
// Name: Cleanup()
// Desc: Releases all previously initialized objects
//-----------------------------------------------------------------------------
VOID Cleanup()
{
    if( g_pVB != NULL )
        g_pVB->Release();

	
	if (g_pVB2 != NULL)
		g_pVB2->Release();

	if (g_pVB3 != NULL)
		g_pVB3->Release();

	if (g_pVB4 != NULL)
		g_pVB4->Release();

	if (g_pVB5 != NULL)
		g_pVB5->Release();

	if (g_pVB6 != NULL)
		g_pVB6->Release();

	if (g_pVB7 != NULL)
		g_pVB7->Release();

	if (g_pVB8 != NULL)
		g_pVB8->Release();
	
    if( g_pd3dDevice != NULL )
        g_pd3dDevice->Release();

    if( g_pD3D != NULL )
        g_pD3D->Release();
}



//-----------------------------------------------------------------------------
// Name: SetupMatrices()
// Desc: Sets up the world, view, and projection transform Matrices.
//-----------------------------------------------------------------------------
//Ã¹¹øÂ° »ï°¢Çü
VOID SetupMatrices()
{
    // For our world matrix, we will just rotate the object about the y-axis.
    D3DXMATRIXA16 matWorld1;
	D3DXMATRIXA16 matWorld2;
	D3DXMATRIXA16 matWorld3;
	D3DXMATRIXA16 matWorld4;
	D3DXMATRIXA16 matWorld5;
	D3DXMATRIXA16 output;

    // Set up the rotation matrix to generate 1 full rotation (2*PI radians) 
    // every 1000 ms. To avoid the loss of precision inherent in very high 
    // floating point numbers, the system time is modulated by the rotation 
    // period before conversion to a radian angle.
    UINT iTime = timeGetTime() % 1000;
    FLOAT fAngle = iTime * ( 2.0f * D3DX_PI ) / 1000.0f;
	D3DXMatrixTranslation(&matWorld2, 3.0f, 0.0f, 0.0f);
	D3DXMatrixScaling(&matWorld3, 0.35f, 0.35f, 0.35f);
    D3DXMatrixRotationY( &matWorld1, fAngle );
	D3DXMatrixRotationX(&matWorld5, fAngle);
	D3DXMatrixRotationX(&matWorld4, 2);
	
	D3DXMatrixMultiply(&output, &matWorld3, &matWorld1);
	D3DXMatrixMultiply(&output, &matWorld5, &output);
	D3DXMatrixMultiply(&output, &matWorld2, &output);
	D3DXMatrixMultiply(&output, &matWorld4, &output);
	


    g_pd3dDevice->SetTransform( D3DTS_WORLD, &output);

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

//µÎ¹øÂ° »ï°¢Çü(Áß½É»ï°¢Çü)
VOID SetupMatrices2()
{
	// For our world matrix, we will just rotate the object about the y-axis.
	D3DXMATRIXA16 matWorld1;
	D3DXMATRIXA16 matWorld2;
	D3DXMATRIXA16 matWorld3;
	D3DXMATRIXA16 matWorld4;
	D3DXMATRIXA16 matWorld5;
	D3DXMATRIXA16 output;
	

	// Set up the rotation matrix to generate 1 full rotation (2*PI radians) 
	// every 1000 ms. To avoid the loss of precision inherent in very high 
	// floating point numbers, the system time is modulated by the rotation 
	// period before conversion to a radian angle.
	UINT iTime = timeGetTime() % 1000;
	FLOAT fAngle = iTime * (2.0f * D3DX_PI) / 1000.0f;
	D3DXMatrixTranslation(&matWorld2, 0.0f, 0.0f, 0.0f);
	D3DXMatrixRotationY(&matWorld1, fAngle);
	D3DXMatrixRotationX(&matWorld4, fAngle);
	D3DXMatrixScaling(&matWorld3, 0.5f, 0.5f, 0.5f);
	D3DXMatrixMultiply(&output, &matWorld3, &matWorld2);
	D3DXMatrixMultiply(&output, &matWorld1, &output);
	D3DXMatrixMultiply(&output, &matWorld4, &output);

	g_pd3dDevice->SetTransform(D3DTS_WORLD, &output);

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


VOID SetupMatrices3()
{
	// For our world matrix, we will just rotate the object about the y-axis.
	D3DXMATRIXA16 matWorld1;
	D3DXMATRIXA16 matWorld2;
	D3DXMATRIXA16 matWorld3;
	D3DXMATRIXA16 matWorld4;
	D3DXMATRIXA16 matWorld5;
	D3DXMATRIXA16 output;

	/*
	D3DXMatrixTranslation(&matWorld2, 3.0f, 0.0f, 0.0f);
	D3DXMatrixScaling(&matWorld3, 0.5f, 0.5f, 0.5f);
	D3DXMatrixRotationY(&matWorld1, fAngle );
	D3DXMatrixRotationX(&matWorld5, fAngle);
	D3DXMatrixRotationX(&matWorld4, 2);

	D3DXMatrixMultiply(&output, &matWorld3, &matWorld1);
	D3DXMatrixMultiply(&output, &matWorld5, &output);
	D3DXMatrixMultiply(&output, &matWorld2, &output);
	D3DXMatrixMultiply(&output, &matWorld4, &output);
	*/

	// Set up the rotation matrix to generate 1 full rotation (2*PI radians) 
	// every 1000 ms. To avoid the loss of precision inherent in very high 
	// floating point numbers, the system time is modulated by the rotation 
	// period before conversion to a radian angle.
	UINT iTime = timeGetTime() % 1000;
	FLOAT fAngle = iTime * (2.0f * D3DX_PI) / 1000.0f;
	D3DXMatrixTranslation(&matWorld1, -3.0f, 0.0f, 0.0f);
	D3DXMatrixScaling(&matWorld2, 0.45f, 0.45f, 0.45f);
	D3DXMatrixRotationY(&matWorld3, -fAngle);
	D3DXMatrixRotationX(&matWorld4, -fAngle);
	D3DXMatrixRotationX(&matWorld5, 2);
	
	D3DXMatrixMultiply(&output, &matWorld2, &matWorld3);
	D3DXMatrixMultiply(&output, &matWorld5, &output);
	D3DXMatrixMultiply(&output, &matWorld1, &output);
	D3DXMatrixMultiply(&output, &matWorld4, &output);

	g_pd3dDevice->SetTransform(D3DTS_WORLD, &output);

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


VOID SetupMatrices4()
{
	// For our world matrix, we will just rotate the object about the y-axis.
	D3DXMATRIXA16 matWorld1;
	D3DXMATRIXA16 matWorld2;
	D3DXMATRIXA16 matWorld3;
	D3DXMATRIXA16 matWorld4;
	D3DXMATRIXA16 matWorld5;
	D3DXMATRIXA16 matWorld6;
	D3DXMATRIXA16 matWorld7;
	D3DXMATRIXA16 output;

	/*
	D3DXMatrixTranslation(&matWorld2, 3.0f, 0.0f, 0.0f);
	D3DXMatrixScaling(&matWorld3, 0.5f, 0.5f, 0.5f);
	D3DXMatrixRotationY(&matWorld1, fAngle );
	D3DXMatrixRotationX(&matWorld5, fAngle);
	D3DXMatrixRotationX(&matWorld4, 2);

	D3DXMatrixMultiply(&output, &matWorld3, &matWorld1);
	D3DXMatrixMultiply(&output, &matWorld5, &output);
	D3DXMatrixMultiply(&output, &matWorld2, &output);
	D3DXMatrixMultiply(&output, &matWorld4, &output);
	*/

	// Set up the rotation matrix to generate 1 full rotation (2*PI radians) 
	// every 1000 ms. To avoid the loss of precision inherent in very high 
	// floating point numbers, the system time is modulated by the rotation 
	// period before conversion to a radian angle.
	UINT iTime = timeGetTime() % 1000;
	FLOAT fAngle = iTime * (2.0f * D3DX_PI) / 1000.0f;
	D3DXMatrixTranslation(&matWorld1, 3.0f, 3.0f, 5.0f);
	D3DXMatrixScaling(&matWorld2, 0.25f, 0.25f, 0.25f);
	D3DXMatrixRotationY(&matWorld3, fAngle);
	D3DXMatrixRotationY(&matWorld7, fAngle);
	D3DXMatrixRotationX(&matWorld4, -fAngle);
	D3DXMatrixRotationX(&matWorld6, fAngle+2);
	D3DXMatrixRotationZ(&matWorld5, 2);

	D3DXMatrixMultiply(&output, &matWorld3, &matWorld5);
	D3DXMatrixMultiply(&output, &matWorld7, &output);
	D3DXMatrixMultiply(&output, &matWorld2, &output);
	D3DXMatrixMultiply(&output, &matWorld1, &output);
	D3DXMatrixMultiply(&output, &matWorld4, &output);
	D3DXMatrixMultiply(&output, &matWorld6, &output);

	g_pd3dDevice->SetTransform(D3DTS_WORLD, &output);

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

VOID SetupMatrices5()
{
	// For our world matrix, we will just rotate the object about the y-axis.
	D3DXMATRIXA16 matWorld1;
	D3DXMATRIXA16 matWorld2;
	D3DXMATRIXA16 matWorld3;
	D3DXMATRIXA16 matWorld4;
	D3DXMATRIXA16 matWorld5;
	D3DXMATRIXA16 matWorld6;
	D3DXMATRIXA16 matWorld7;
	D3DXMATRIXA16 output;

	/*
	D3DXMatrixTranslation(&matWorld2, 3.0f, 0.0f, 0.0f);
	D3DXMatrixScaling(&matWorld3, 0.5f, 0.5f, 0.5f);
	D3DXMatrixRotationY(&matWorld1, fAngle );
	D3DXMatrixRotationX(&matWorld5, fAngle);
	D3DXMatrixRotationX(&matWorld4, 2);

	D3DXMatrixMultiply(&output, &matWorld3, &matWorld1);
	D3DXMatrixMultiply(&output, &matWorld5, &output);
	D3DXMatrixMultiply(&output, &matWorld2, &output);
	D3DXMatrixMultiply(&output, &matWorld4, &output);
	*/

	// Set up the rotation matrix to generate 1 full rotation (2*PI radians) 
	// every 1000 ms. To avoid the loss of precision inherent in very high 
	// floating point numbers, the system time is modulated by the rotation 
	// period before conversion to a radian angle.
	UINT iTime = timeGetTime() % 1000;
	FLOAT fAngle = iTime * (2.0f * D3DX_PI) / 1000.0f;
	D3DXMatrixTranslation(&matWorld1, -3.0f, -3.0f, -5.0f);
	D3DXMatrixScaling(&matWorld2, 0.25f, 0.25f, 0.25f);
	D3DXMatrixRotationY(&matWorld3, -fAngle);
	D3DXMatrixRotationY(&matWorld7, -fAngle);
	D3DXMatrixRotationX(&matWorld4, fAngle);
	D3DXMatrixRotationX(&matWorld6, -fAngle + 2);
	D3DXMatrixRotationZ(&matWorld5, 2);

	D3DXMatrixMultiply(&output, &matWorld5, &matWorld3);
	D3DXMatrixMultiply(&output, &matWorld7, &output);
	D3DXMatrixMultiply(&output, &matWorld2, &output);
	D3DXMatrixMultiply(&output, &matWorld1, &output);
	D3DXMatrixMultiply(&output, &matWorld4, &output);
	D3DXMatrixMultiply(&output, &matWorld6, &output);

	g_pd3dDevice->SetTransform(D3DTS_WORLD, &output);

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

VOID SetupMatrices6()
{
	// For our world matrix, we will just rotate the object about the y-axis.
	D3DXMATRIXA16 matWorld1;
	D3DXMATRIXA16 matWorld2;
	D3DXMATRIXA16 matWorld3;
	D3DXMATRIXA16 matWorld4;
	D3DXMATRIXA16 matWorld5;
	D3DXMATRIXA16 matWorld6;
	D3DXMATRIXA16 matWorld7;
	D3DXMATRIXA16 output;

	/*
	D3DXMatrixTranslation(&matWorld2, 3.0f, 0.0f, 0.0f);
	D3DXMatrixScaling(&matWorld3, 0.5f, 0.5f, 0.5f);
	D3DXMatrixRotationY(&matWorld1, fAngle );
	D3DXMatrixRotationX(&matWorld5, fAngle);
	D3DXMatrixRotationX(&matWorld4, 2);

	D3DXMatrixMultiply(&output, &matWorld3, &matWorld1);
	D3DXMatrixMultiply(&output, &matWorld5, &output);
	D3DXMatrixMultiply(&output, &matWorld2, &output);
	D3DXMatrixMultiply(&output, &matWorld4, &output);
	*/

	// Set up the rotation matrix to generate 1 full rotation (2*PI radians) 
	// every 1000 ms. To avoid the loss of precision inherent in very high 
	// floating point numbers, the system time is modulated by the rotation 
	// period before conversion to a radian angle.
	UINT iTime = timeGetTime() % 1000;
	FLOAT fAngle = iTime * (2.0f * D3DX_PI) / 1000.0f;
	D3DXMatrixTranslation(&matWorld1, 0.0f, 3.0f, 0.0f);
	D3DXMatrixScaling(&matWorld2, 0.4f, 0.4f, 0.4f);
	D3DXMatrixRotationY(&matWorld3, fAngle);
	D3DXMatrixRotationX(&matWorld4, fAngle);
	D3DXMatrixRotationY(&matWorld5, fAngle);
	D3DXMatrixRotationZ(&matWorld6, -fAngle);



	D3DXMatrixMultiply(&output, &matWorld5, &matWorld4);
	D3DXMatrixMultiply(&output, &matWorld2, &output);
	D3DXMatrixMultiply(&output, &matWorld1, &output);
	D3DXMatrixMultiply(&output, &matWorld5, &output);
	D3DXMatrixMultiply(&output, &matWorld6, &output);
	//D3DXMatrixMultiply(&output, &matWorld4, &output);

	g_pd3dDevice->SetTransform(D3DTS_WORLD, &output);

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

VOID SetupMatrices7()
{
	// For our world matrix, we will just rotate the object about the y-axis.
	D3DXMATRIXA16 matWorld1;
	D3DXMATRIXA16 matWorld2;
	D3DXMATRIXA16 matWorld3;
	D3DXMATRIXA16 matWorld4;
	D3DXMATRIXA16 matWorld5;
	D3DXMATRIXA16 matWorld6;
	D3DXMATRIXA16 matWorld7;
	D3DXMATRIXA16 output;

	/*
	D3DXMatrixTranslation(&matWorld2, 3.0f, 0.0f, 0.0f);
	D3DXMatrixScaling(&matWorld3, 0.5f, 0.5f, 0.5f);
	D3DXMatrixRotationY(&matWorld1, fAngle );
	D3DXMatrixRotationX(&matWorld5, fAngle);
	D3DXMatrixRotationX(&matWorld4, 2);

	D3DXMatrixMultiply(&output, &matWorld3, &matWorld1);
	D3DXMatrixMultiply(&output, &matWorld5, &output);
	D3DXMatrixMultiply(&output, &matWorld2, &output);
	D3DXMatrixMultiply(&output, &matWorld4, &output);
	*/

	// Set up the rotation matrix to generate 1 full rotation (2*PI radians) 
	// every 1000 ms. To avoid the loss of precision inherent in very high 
	// floating point numbers, the system time is modulated by the rotation 
	// period before conversion to a radian angle.
	UINT iTime = timeGetTime() % 1000;
	FLOAT fAngle = iTime * (2.0f * D3DX_PI) / 1000.0f;
	D3DXMatrixTranslation(&matWorld1, 0.0f, -3.0f, 0.0f);
	D3DXMatrixScaling(&matWorld2, 0.43f, 0.43f, 0.43f);
	D3DXMatrixRotationY(&matWorld3, -fAngle);
	D3DXMatrixRotationX(&matWorld4, -fAngle);
	D3DXMatrixRotationY(&matWorld5, -fAngle);
	D3DXMatrixRotationZ(&matWorld6, fAngle);



	D3DXMatrixMultiply(&output, &matWorld5, &matWorld4);
	D3DXMatrixMultiply(&output, &matWorld2, &output);
	D3DXMatrixMultiply(&output, &matWorld1, &output);
	D3DXMatrixMultiply(&output, &matWorld5, &output);
	D3DXMatrixMultiply(&output, &matWorld6, &output);
	//D3DXMatrixMultiply(&output, &matWorld4, &output);

	g_pd3dDevice->SetTransform(D3DTS_WORLD, &output);

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


VOID SetupMatrices8()
{
	// For our world matrix, we will just rotate the object about the y-axis.
	D3DXMATRIXA16 matWorld1;
	D3DXMATRIXA16 matWorld2;
	D3DXMATRIXA16 matWorld3;
	D3DXMATRIXA16 matWorld4;
	D3DXMATRIXA16 matWorld5;
	D3DXMATRIXA16 output;


	// Set up the rotation matrix to generate 1 full rotation (2*PI radians) 
	// every 1000 ms. To avoid the loss of precision inherent in very high 
	// floating point numbers, the system time is modulated by the rotation 
	// period before conversion to a radian angle.
	UINT iTime = timeGetTime() % 1000;
	FLOAT fAngle = iTime * (2.0f * D3DX_PI) / 1000.0f;
	D3DXMatrixTranslation(&matWorld1, 1.0f, 1.0f, 1.0f);
	D3DXMatrixRotationY(&matWorld2, -fAngle);
	D3DXMatrixRotationX(&matWorld3, -fAngle);
	D3DXMatrixRotationZ(&matWorld4, -fAngle);
	D3DXMatrixScaling(&matWorld5, 0.2f, 0.2f, 0.2f);
	D3DXMatrixMultiply(&output, &matWorld3, &matWorld4);
	//D3DXMatrixMultiply(&output, &matWorld2, &matWorld3);
	D3DXMatrixMultiply(&output, &matWorld1, &output);
	D3DXMatrixMultiply(&output, &matWorld5, &output);

	g_pd3dDevice->SetTransform(D3DTS_WORLD, &output);

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
    // Clear the backbuffer to a black color
    g_pd3dDevice->Clear( 0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB( 0, 0, 0 ), 1.0f, 0 );

    // Begin the scene
    if( SUCCEEDED( g_pd3dDevice->BeginScene() ) )
    {
        // Setup the world, view, and projection Matrices
		//½Ã°è¹æÇâ»ï°¢Çü
        SetupMatrices();
        // Render the vertex buffer contents
        g_pd3dDevice->SetStreamSource( 0, g_pVB, 0, sizeof( CUSTOMVERTEX ) );
        g_pd3dDevice->SetFVF( D3DFVF_CUSTOMVERTEX );
        g_pd3dDevice->DrawPrimitive( D3DPT_TRIANGLESTRIP, 0, 1 );

		
		//Áß½É »ï°¢Çü
		SetupMatrices2();
		// Render the vertex buffer contents
		g_pd3dDevice->SetStreamSource(0, g_pVB2, 0, sizeof(CUSTOMVERTEX));
		g_pd3dDevice->SetFVF(D3DFVF_CUSTOMVERTEX);
		g_pd3dDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 1);


		//¹Ý½Ã°è »ï°¢Çü(ÁÂ¿ì)
		SetupMatrices3();
		// Render the vertex buffer contents
		g_pd3dDevice->SetStreamSource(0, g_pVB3, 0, sizeof(CUSTOMVERTEX));
		g_pd3dDevice->SetFVF(D3DFVF_CUSTOMVERTEX);
		g_pd3dDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 1);

		//»ó´Ü »ï°¢Çü
		SetupMatrices4();
		// Render the vertex buffer contents
		g_pd3dDevice->SetStreamSource(0, g_pVB4, 0, sizeof(CUSTOMVERTEX));
		g_pd3dDevice->SetFVF(D3DFVF_CUSTOMVERTEX);
		g_pd3dDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 1);

		SetupMatrices5();
		// Render the vertex buffer contents
		g_pd3dDevice->SetStreamSource(0, g_pVB5, 0, sizeof(CUSTOMVERTEX));
		g_pd3dDevice->SetFVF(D3DFVF_CUSTOMVERTEX);
		g_pd3dDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 1);

		SetupMatrices6();
		// Render the vertex buffer contents
		g_pd3dDevice->SetStreamSource(0, g_pVB6, 0, sizeof(CUSTOMVERTEX));
		g_pd3dDevice->SetFVF(D3DFVF_CUSTOMVERTEX);
		g_pd3dDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 1);

		SetupMatrices7();
		// Render the vertex buffer contents
		g_pd3dDevice->SetStreamSource(0, g_pVB7, 0, sizeof(CUSTOMVERTEX));
		g_pd3dDevice->SetFVF(D3DFVF_CUSTOMVERTEX);
		g_pd3dDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 1);

		SetupMatrices8();
		// Render the vertex buffer contents
		g_pd3dDevice->SetStreamSource(0, g_pVB8, 0, sizeof(CUSTOMVERTEX));
		g_pd3dDevice->SetFVF(D3DFVF_CUSTOMVERTEX);
		g_pd3dDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 1);


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
    HWND hWnd = CreateWindow( L"D3D Tutorial", L"D3D Tutorial 03: Matrices",
                              WS_OVERLAPPEDWINDOW, 100, 100, 1200, 900,
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



