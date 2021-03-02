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

    //�̵��� ��ǥ�� ����
    Pos.x = 0.0f;
    Pos.y = 0.0f;
    //Pos.z = 0.0f;

    //Ȯ���� ũ��
    Scale.x =1.0f;
    Scale.y =1.0f;

    //ȸ����
    Rot.x = 0.0f;
    Rot.y = 0.0f;
    Rot.z = 0.0f;

    //ó�� ���鶩 �θ� ���ٰ� �ϱ�
    P = nullptr;

    color = Color(1,1,1,1);

    Up = Vector2(0.0f,1.0f);
    Right = Vector2(1.0f,0.0f);

    //�������
    //CreateConstantBuffer
    {
        D3D11_BUFFER_DESC desc = { 0 };
        desc.Usage = D3D11_USAGE_DEFAULT;
        desc.ByteWidth = sizeof(Matrix);
        desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;//�������
        desc.CPUAccessFlags = 0;
        desc.MiscFlags = 0;
        desc.StructureByteStride = 0;

        HRESULT hr = g_Device->CreateBuffer(&desc, NULL, &WBuffer);
        assert(SUCCEEDED(hr));
    }

    //�������
    //CreateConstantBuffer
    {
        D3D11_BUFFER_DESC desc = { 0 };
        desc.Usage = D3D11_USAGE_DEFAULT;
        desc.ByteWidth = sizeof(Color);
        desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;//�������
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
    //Rot ������ z��ȸ���ϴ� ��� ����� �Լ�
    //D3DXMatrixRotationZ(&R,Rot);

    //Rot x,y,z 3�������� ȸ����� ����� �Լ�
    D3DXMatrixRotationYawPitchRoll(&R,
        Rot.y,Rot.x,Rot.z);

    //Scale ũ��� S ��� ����� �Լ�
    D3DXMatrixScaling(&S,Scale.x,Scale.y,1.0f);
    //Pos ��ġ�� T��� ����� �Լ�
    D3DXMatrixTranslation(&T,Pos.x,Pos.y,0.0f);

    //ctrl + space �˻�
    //���� ���(�������)�� S*R*T ���� �ش�.
    W = S*R*T;
    //ȸ���̵���� �� �ֱ�
    RT = R*T;

    //�θ������ �ִ� �༮�鸸
    if(P)
    {
        //���� ��İ����� �θ���İ� ���ϱ�
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

    //������� ������Ʈ W���� WBuffer�� ����
    g_DeviceContext->UpdateSubresource(WBuffer, 0, NULL, &W, 0, 0);
    //                            ����ɷ������� ��ȣ 0
    g_DeviceContext->VSSetConstantBuffers(0, 1, &WBuffer);

    //������� ������Ʈ W���� WBuffer�� ����
    g_DeviceContext->UpdateSubresource(ColorBuffer, 0, NULL, &color, 0, 0);
    //                            ����ɷ������� ��ȣ 0
    g_DeviceContext->PSSetConstantBuffers(0, 1, &ColorBuffer);

}
