#include "stdafx.h"
#include "Scene2.h"

Scene2::Scene2()
{
    Fade = Color(1.0f,1.0f,1.0f,1.0f);
    //CreateConstantBuffer
    {
        D3D11_BUFFER_DESC desc = { 0 };
        desc.Usage = D3D11_USAGE_DEFAULT;
        desc.ByteWidth = sizeof(Color);
        desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;//상수버퍼
        desc.CPUAccessFlags = 0;
        desc.MiscFlags = 0;
        desc.StructureByteStride = 0;

        HRESULT hr = g_Device->CreateBuffer(&desc, NULL, &FadeBuffer);
        assert(SUCCEEDED(hr));
    }

}

Scene2::~Scene2()
{
    SAFE_RELEASE(FadeBuffer);
 
}

HRESULT Scene2::init()
{
  
   

    return S_OK;
}

void Scene2::release()
{
   
}

void Scene2::update()
{

	

}

void Scene2::render()
{

}


