#include "stdafx.h"
#include "Object.h"

Object::Object()
{
}

Object::~Object()
{
}

HRESULT Object::init()
{

    //이동할 좌표의 원점
    Pos.x = 0.0f;
    Pos.y = 0.0f;
    //Pos.z = 0.0f;

    //확대할 크기
    Scale.x =1.0f;
    Scale.y =1.0f;

    //회전값
    Rot.x = 0.0f;
    Rot.y = 0.0f;
    Rot.z = 0.0f;

    //처음 만들땐 부모가 없다고 하기
    P = nullptr;

    color = Color(1,1,1,1);

    Up = Vector2(0.0f,1.0f);
    Right = Vector2(1.0f,0.0f);

    //상수버퍼
    //CreateConstantBuffer
    {
        D3D11_BUFFER_DESC desc = { 0 };
        desc.Usage = D3D11_USAGE_DEFAULT;
        desc.ByteWidth = sizeof(Matrix);
        desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;//상수버퍼
        desc.CPUAccessFlags = 0;
        desc.MiscFlags = 0;
        desc.StructureByteStride = 0;

        HRESULT hr = g_Device->CreateBuffer(&desc, NULL, &WBuffer);
        assert(SUCCEEDED(hr));
    }

    //상수버퍼
    //CreateConstantBuffer
    {
        D3D11_BUFFER_DESC desc = { 0 };
        desc.Usage = D3D11_USAGE_DEFAULT;
        desc.ByteWidth = sizeof(Color);
        desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;//상수버퍼
        desc.CPUAccessFlags = 0;
        desc.MiscFlags = 0;
        desc.StructureByteStride = 0;

        HRESULT hr = g_Device->CreateBuffer(&desc, NULL, &ColorBuffer);
        assert(SUCCEEDED(hr));
    }
    return S_OK;
}

void Object::release()
{
    SAFE_RELEASE(WBuffer);
    SAFE_RELEASE(ColorBuffer);
    SAFE_DELETE(shader);
}

void Object::update()
{
    //Rot 각도로 z축회전하는 행렬 만드는 함수
    //D3DXMatrixRotationZ(&R,Rot);

    //Rot x,y,z 3개축으로 회전행렬 만드는 함수
    D3DXMatrixRotationYawPitchRoll(&R,
        Rot.y,Rot.x,Rot.z);

    //Scale 크기로 S 행렬 만드는 함수
    D3DXMatrixScaling(&S,Scale.x,Scale.y,1.0f);
    //Pos 위치로 T행렬 만드는 함수
    D3DXMatrixTranslation(&T,Pos.x,Pos.y,0.0f);

    //ctrl + space 검색
    //최종 행렬(월드행렬)에 S*R*T 값을 준다.
    W = S*R*T;
    //회전이동행렬 값 주기
    RT = R*T;

    //부모행렬이 있는 녀석들만
    if(P)
    {
        //기존 행렬값에서 부모행렬값 곱하기
        W *= *P;
        RT *= *P;
    }

    Up = Vector2(0.0f,1.0f);
    Right = Vector2(1.0f,0.0f);
    D3DXVec2TransformNormal(&Up,&Up,&RT);//w = 0
   // D3DXVec2TransformCoord(&Up,&Up,&RT);//w = 1
    D3DXVec2TransformNormal(&Right,&Right,&RT);

}

void Object::render()
{
    shader->SetShader();

    //상수버퍼 업데이트 W값을 WBuffer에 전달
    g_DeviceContext->UpdateSubresource(WBuffer, 0, NULL, &W, 0, 0);
    //                            연결될레지스터 번호 0
    g_DeviceContext->VSSetConstantBuffers(0, 1, &WBuffer);

    //상수버퍼 업데이트 W값을 WBuffer에 전달
    g_DeviceContext->UpdateSubresource(ColorBuffer, 0, NULL, &color, 0, 0);
    //                            연결될레지스터 번호 0
    g_DeviceContext->PSSetConstantBuffers(0, 1, &ColorBuffer);

}
