#include "stdafx.h"
#include "CamManager.h"

CamManager::CamManager()
{
    //CreateConstantBuffer
    {
        D3D11_BUFFER_DESC desc = { 0 };
        desc.Usage = D3D11_USAGE_DEFAULT;
        desc.ByteWidth = sizeof(Matrix);
        desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;//상수버퍼
        desc.CPUAccessFlags = 0;
        desc.MiscFlags = 0;
        desc.StructureByteStride = 0;

        HRESULT hr = g_Device->CreateBuffer(&desc, NULL, &VBuffer);
        assert(SUCCEEDED(hr));
    }

    //CreateConstantBuffer
    {
        D3D11_BUFFER_DESC desc = { 0 };
        desc.Usage = D3D11_USAGE_DEFAULT;
        desc.ByteWidth = sizeof(Matrix);
        desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;//상수버퍼
        desc.CPUAccessFlags = 0;
        desc.MiscFlags = 0;
        desc.StructureByteStride = 0;

        HRESULT hr = g_Device->CreateBuffer(&desc, NULL, &PBuffer);
        assert(SUCCEEDED(hr));
    }

    //직교투영 만들어주는 함수
    //                             좌    우 
    D3DXMatrixOrthoOffCenterLH(&P, 0, (float)WINSIZEX, 
        //  아래, 위           가까운면,멀리있는면
        0, (float)WINSIZEY, 0, 10);
    //2D에서는 원근감을 표현하지 않기에 원근투영은 사용하지 않는다

    //첫 카메라 위치
    Pos.x =0.0f;
    Pos.y =0.0f;
}

CamManager::~CamManager()
{
    SAFE_RELEASE(VBuffer);
    SAFE_RELEASE(PBuffer);
}

void CamManager::CameraUpdate()
{
    //View 를 정의
    D3DXMatrixLookAtLH(&V,
        &Vector3(Pos.x, Pos.y, -1.0f),//eye(눈의위치)
        &Vector3(Pos.x, Pos.y, 0.0f),//at(바라보는 위치)
        &Vector3(0.0f,1.0f,0.0f)); //up(카메라가 위를바라보는방향)


    g_DeviceContext->UpdateSubresource(VBuffer, 0, NULL, &V, 0, 0);
    //                            연결될레지스터 번호 1
    g_DeviceContext->VSSetConstantBuffers(1, 1, &VBuffer);


    g_DeviceContext->UpdateSubresource(PBuffer, 0, NULL, &P, 0, 0);
    //                            연결될레지스터 번호 1
    g_DeviceContext->VSSetConstantBuffers(2, 1, &PBuffer);


}
