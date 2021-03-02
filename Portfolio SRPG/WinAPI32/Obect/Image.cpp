#include "stdafx.h"
#include "Image.h"

HRESULT Image::init(_tstring file, string vs , string ps )
{
    _tstring textureFile = _T("./Images/") + file;

    HRESULT hr;

    D3DX11CreateShaderResourceViewFromFile
    (
        g_Device,//����̽�
        textureFile.c_str(),//�����̸�
        NULL, //�߰����� �̹��� ���� NULL���� ����ũ��
        NULL,//�ΰ�
        &srv,//���̴��ڿ���
        &hr
    );
    assert(SUCCEEDED(hr) && "srv ���� �Ұ�");

    //�⺻ ���÷� ��
    SamplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
    //SamplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;

    //0~1 ���̰��� ����� ��ǥ�� ����Ҷ� �ٲٴ� �ɼ�
    //D3D11_TEXTURE_ADDRESS_CLAMP ������ �ȼ������� ���� ä���.
    //D3D11_TEXTURE_ADDRESS_WRAP 0 1 �Ѿ�°� �ٽ� 0~1������ ���д�.
    //D3D11_TEXTURE_ADDRESS_MIRROR 0 1 �Ѿ�°� ����� 0~1������ ���д�.
    SamplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
    SamplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
    SamplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;

    SamplerDesc.MipLODBias = 0.0f;
    SamplerDesc.MaxAnisotropy = 1;
    SamplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
    SamplerDesc.MinLOD = -FLT_MAX;
    SamplerDesc.MaxLOD = FLT_MAX;

    //�ϳ� �̻��� ���÷� ����� �α�
    g_Device->CreateSamplerState(&SamplerDesc, &Sampler);




    //�θ�Ŭ������ init ȣ��
    Object::init();

    Vertex = new VertexPT[6];

    vertexCount = 6;

    Vertex[0].Position.x = -0.5f;
    Vertex[0].Position.y = -0.5f;
    Vertex[0].Uv = Vector2(0.0f,1.0f);

    Vertex[1].Position.x = -0.5f;
    Vertex[1].Position.y = 0.5f;
    Vertex[1].Uv = Vector2(0.0f,0.0f);

    Vertex[2].Position.x = 0.5f;
    Vertex[2].Position.y = -0.5f;
    Vertex[2].Uv = Vector2(1.0f,1.0f);

    Vertex[3].Position.x = -0.5f;
    Vertex[3].Position.y = 0.5f;
    Vertex[3].Uv =Vector2(0.0f,0.0f);

    Vertex[4].Position.x = 0.5f;
    Vertex[4].Position.y = 0.5f;
    Vertex[4].Uv =Vector2(1.0f,0.0f);

    Vertex[5].Position.x = 0.5f;
    Vertex[5].Position.y = -0.5f;
    Vertex[5].Uv = Vector2(1.0f,1.0f);
    

    //���� ����
    D3D11_INPUT_ELEMENT_DESC LayoutDesc[] =
    {
        { "POSITION",//�ø�ƽ���� ������ �ڷ���
        0,//�ø�ƽ���� ������ �ε��� 0���� �����߱⿡ 0 ���
        DXGI_FORMAT_R32G32B32_FLOAT,//3���� 32��Ʈ float����
        0,//0~15������ �ε��� ����, ���ÿ� 15������ ���۰� �Է½����� �������ִ�.
        0,//�� ��Ұ� �޸𸮵�� ������������ �󸶳� ������ ��ġ�� �ִ°��� �˷��ش�.ó������0
        D3D11_INPUT_PER_VERTEX_DATA,//�Ѿ��
        0 },//�Ѿ��
    { "UV",//�ø�ƽ���� ������ �ڷ���
    0,//�ø�ƽ���� ������ �ε��� 0���� �����߱⿡ 0 ���
    DXGI_FORMAT_R32G32_FLOAT,//2���� 32��Ʈ float����
    0,//0~15������ �ε��� ����, ���ÿ� 15������ ���۰� �Է½����� �������ִ�.
    12,//������ R32G32B32 4 ,4,4 ->12
    D3D11_INPUT_PER_VERTEX_DATA,//�Ѿ��
    0 },//�Ѿ��
    };
    //CreateVertexBuffer
    {
        D3D11_BUFFER_DESC desc;
        desc = { 0 };
        desc.Usage = D3D11_USAGE_DEFAULT;//���۸� �а� ���� ����� �ĺ��Ͻʽÿ�. ������Ʈ �󵵰� �ٽ� ����Դϴ�. ���� �Ϲ����� ���� �Ϲ������� D3D11_USAGE_DEFAULT�Դϴ�. 
        desc.ByteWidth = sizeof(VertexPT) * vertexCount; //���� ũ�� (����Ʈ)�Դϴ�.
        desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;//���۰� ������ ���ο� ���ε��Ǵ� ����� �ĺ��Ͻʽÿ�

        D3D11_SUBRESOURCE_DATA data = { 0 };
        //���� ���ҽ��� �ʱ�ȭ�ϱ����� �����͸� �����մϴ�.
        data.pSysMem = Vertex;
        //�ʱ�ȭ �������� ������.

        //���� �����
        HRESULT hr = g_Device->CreateBuffer(&desc, &data, &VertexBuffer);
        assert(SUCCEEDED(hr));
    }
    shader = new Shader(_T("./Shader/Image.hlsl"),vs,ps);
    UINT ElementCount = ARRAYSIZE(LayoutDesc);
    shader->CreateInputLayout(LayoutDesc, ElementCount);


    //CreateBlendState
    {
        D3D11_BLEND_DESC desc;
        ZeroMemory(&desc, sizeof(D3D11_BLEND_DESC));
        //���� ���� blendState[0]
        desc.RenderTarget[0].BlendEnable = false;
        desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
        g_Device->CreateBlendState(&desc, &blendState[0]);

        //������ blendState[1]
        //Dest������ Src��ó
        desc.RenderTarget[0].BlendEnable = true;
        desc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
        desc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
        desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
        desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
        desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ZERO;
        desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
        g_Device->CreateBlendState(&desc, &blendState[1]);
    }

    //CreateConstantBuffer
    {
        D3D11_BUFFER_DESC desc = { 0 };
        desc.Usage = D3D11_USAGE_DEFAULT;
        desc.ByteWidth = sizeof(Vector4);
        desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;//�������
        desc.CPUAccessFlags = 0;
        desc.MiscFlags = 0;
        desc.StructureByteStride = 0;

        HRESULT hr = g_Device->CreateBuffer(&desc, NULL, &UVBuffer);
        assert(SUCCEEDED(hr));
    }

    //U ��ǥ�� �ּ� �ִ밪
    UV.x = 0.0f;
    UV.y = 1.0f;
    //V ��ǥ�� �ּ� �ִ방
    UV.z = 0.0f;
    UV.w = 1.0f;

    return S_OK;
}

void Image::release()
{
    Object::release();
    SAFE_DELETE_ARRAY(Vertex);
    SAFE_RELEASE(VertexBuffer);
    SAFE_RELEASE(srv);
    SAFE_RELEASE(blendState[0]);
    SAFE_RELEASE(blendState[1]);
    SAFE_RELEASE(Sampler);
    SAFE_RELEASE(UVBuffer);
}

void Image::update()
{

    //�θ�Ŭ������ update ȣ��
    Object::update();

}

void Image::render()
{
    Object::render();
    UINT stride = sizeof(VertexPT);
    UINT offset = 0;

    //����ɷ������� ��ȣ 0
    g_DeviceContext->PSSetSamplers(0, 
        1, //���ҽ�����
        &Sampler);//���÷�

    //�ȼ����̴��� ���̴��ڿ��� ����
    g_DeviceContext->PSSetShaderResources(0,// ����ɷ������� ��ȣ 0
        1,//���ҽ�����
        &srv);

    g_DeviceContext->IASetPrimitiveTopology
    (D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    g_DeviceContext->IASetVertexBuffers(0,//�Է½���(16~32������ ��������)
        1,//�Է½��Կ� ���̰��� �ϴ� ������ ����
        &VertexBuffer,
        &stride,//���������� �� ������ ����Ʈ���� ũ��
        &offset);//�������� �迭�� ������(�迭��������) �������� �̿��� ������ �պκ��� �����ϰ� ����Ҽ����ִ�.



    //���γ� ���� ������ 1�� �̻��϶�
    if (MaxFrameX != 1 || MaxFrameY != 1)
    {
        UV.x = 1.0f / MaxFrameX * CurrentFrameX;
        UV.y = 1.0f / MaxFrameX * (CurrentFrameX + 1);
        UV.z = 1.0f / MaxFrameY * CurrentFrameY;
        UV.w = 1.0f / MaxFrameY * (CurrentFrameY + 1);
    }

    g_DeviceContext->UpdateSubresource(UVBuffer, 0, NULL, &UV, 0, 0);
    //                            ����ɷ������� ��ȣ3
    g_DeviceContext->VSSetConstantBuffers(3, 1, &UVBuffer);


    //���� �ѱ�
    g_DeviceContext->OMSetBlendState(blendState[1], NULL, 0xFF);
                 //�׷��� ��� ���
    g_DeviceContext->Draw(vertexCount, 0);
    //���� ����
    g_DeviceContext->OMSetBlendState(blendState[0], NULL, 0xFF);
}

void Image::CreateSampler()
{
    //���� ���÷��� �����ϸ� ���� �����ض�
    if (Sampler)
    {
        SAFE_RELEASE(Sampler);
    }
    HRESULT hr;
    //���÷��� ������
    hr = g_Device->CreateSamplerState(&SamplerDesc, &Sampler);
    assert(SUCCEEDED(hr));
}

void Image::PlayAnimation(AnimState as, bool isFrameX, float nexttime)
{
    //�������ӳ����� �ð�������
    time += DeltaTime;

    if (time >= nexttime)
    {
        //�����ð��� �ٽ� �ɸ����ְ�
        time -= nexttime;
        if (isFrameX)
        {
            switch (as)
            {
                //�������
                case ANI_LOOP:
                    if (MaxFrameX - 1 > CurrentFrameX)
                    {
                        CurrentFrameX++;
                    }
                    else
                    {
                        CurrentFrameX = 0;
                    }
                    break;
                    //�������
                case ANI_REVERSE:
                    if (CurrentFrameX > 0)
                    {
                        CurrentFrameX--;
                    }
                    else
                    {
                        CurrentFrameX = MaxFrameX - 1;
                    }
                    break;
            }


        }
        else //�������
        {
            switch (as)
            {
                //�������
                case ANI_LOOP:
                    if (MaxFrameY - 1 > CurrentFrameY)
                    {
                        CurrentFrameY++;
                    }
                    else
                    {
                        CurrentFrameY = 0;
                    }
                    break;
                    //�������
                case ANI_REVERSE:
                    if (CurrentFrameY > 0)
                    {
                        CurrentFrameY--;
                    }
                    else
                    {
                        CurrentFrameY = MaxFrameY - 1;
                    }
                    break;
            }

        }

       



    }
}
