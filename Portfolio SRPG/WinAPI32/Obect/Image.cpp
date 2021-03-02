#include "stdafx.h"
#include "Image.h"

HRESULT Image::init(_tstring file, string vs , string ps )
{
    _tstring textureFile = _T("./Images/") + file;

    HRESULT hr;

    D3DX11CreateShaderResourceViewFromFile
    (
        g_Device,//디바이스
        textureFile.c_str(),//파일이름
        NULL, //추가적인 이미지 정보 NULL값은 원본크기
        NULL,//널값
        &srv,//쉐이더자원뷰
        &hr
    );
    assert(SUCCEEDED(hr) && "srv 생성 불가");

    //기본 샘플러 값
    SamplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
    //SamplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;

    //0~1 사이값을 벗어나는 좌표를 사용할때 바꾸는 옵션
    //D3D11_TEXTURE_ADDRESS_CLAMP 마지막 픽셀값으로 색을 채운다.
    //D3D11_TEXTURE_ADDRESS_WRAP 0 1 넘어가는걸 다시 0~1값으로 가둔다.
    //D3D11_TEXTURE_ADDRESS_MIRROR 0 1 넘어가는걸 뒤집어서 0~1값으로 가둔다.
    SamplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
    SamplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
    SamplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;

    SamplerDesc.MipLODBias = 0.0f;
    SamplerDesc.MaxAnisotropy = 1;
    SamplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
    SamplerDesc.MinLOD = -FLT_MAX;
    SamplerDesc.MaxLOD = FLT_MAX;

    //하나 이상의 샘플러 만들어 두기
    g_Device->CreateSamplerState(&SamplerDesc, &Sampler);




    //부모클래스의 init 호출
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
    

    //정점 서술
    D3D11_INPUT_ELEMENT_DESC LayoutDesc[] =
    {
        { "POSITION",//시멘틱에서 정의한 자료형
        0,//시멘틱에서 정의한 인덱스 0으로 지정했기에 0 사용
        DXGI_FORMAT_R32G32B32_FLOAT,//3개의 32비트 float설정
        0,//0~15사이의 인덱스 지정, 동시에 15개까지 버퍼가 입력슬롯을 가질수있다.
        0,//이 요소가 메모리덩어리 시작지점부터 얼마나 떨어진 위치에 있는가를 알려준다.처음시작0
        D3D11_INPUT_PER_VERTEX_DATA,//넘어가기
        0 },//넘어가기
    { "UV",//시멘틱에서 정의한 자료형
    0,//시멘틱에서 정의한 인덱스 0으로 지정했기에 0 사용
    DXGI_FORMAT_R32G32_FLOAT,//2개의 32비트 float설정
    0,//0~15사이의 인덱스 지정, 동시에 15개까지 버퍼가 입력슬롯을 가질수있다.
    12,//포지션 R32G32B32 4 ,4,4 ->12
    D3D11_INPUT_PER_VERTEX_DATA,//넘어가기
    0 },//넘어가기
    };
    //CreateVertexBuffer
    {
        D3D11_BUFFER_DESC desc;
        desc = { 0 };
        desc.Usage = D3D11_USAGE_DEFAULT;//버퍼를 읽고 쓰는 방법을 식별하십시오. 업데이트 빈도가 핵심 요소입니다. 가장 일반적인 값은 일반적으로 D3D11_USAGE_DEFAULT입니다. 
        desc.ByteWidth = sizeof(VertexPT) * vertexCount; //버퍼 크기 (바이트)입니다.
        desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;//버퍼가 파이프 라인에 바인딩되는 방법을 식별하십시오

        D3D11_SUBRESOURCE_DATA data = { 0 };
        //하위 리소스를 초기화하기위한 데이터를 지정합니다.
        data.pSysMem = Vertex;
        //초기화 데이터의 포인터.

        //버퍼 만들기
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
        //섞지 않을 blendState[0]
        desc.RenderTarget[0].BlendEnable = false;
        desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
        g_Device->CreateBlendState(&desc, &blendState[0]);

        //섞어줄 blendState[1]
        //Dest목적지 Src출처
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
        desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;//상수버퍼
        desc.CPUAccessFlags = 0;
        desc.MiscFlags = 0;
        desc.StructureByteStride = 0;

        HRESULT hr = g_Device->CreateBuffer(&desc, NULL, &UVBuffer);
        assert(SUCCEEDED(hr));
    }

    //U 좌표의 최소 최대값
    UV.x = 0.0f;
    UV.y = 1.0f;
    //V 좌표의 최소 최대갑
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

    //부모클래스의 update 호출
    Object::update();

}

void Image::render()
{
    Object::render();
    UINT stride = sizeof(VertexPT);
    UINT offset = 0;

    //연결될레지스터 번호 0
    g_DeviceContext->PSSetSamplers(0, 
        1, //리소스갯수
        &Sampler);//샘플러

    //픽셀쉐이더에 쉐이더자원값 세팅
    g_DeviceContext->PSSetShaderResources(0,// 연결될레지스터 번호 0
        1,//리소스갯수
        &srv);

    g_DeviceContext->IASetPrimitiveTopology
    (D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    g_DeviceContext->IASetVertexBuffers(0,//입력슬롯(16~32개까지 설정가능)
        1,//입력슬롯에 붙이고자 하는 버퍼의 갯수
        &VertexBuffer,
        &stride,//정점버퍼의 한 원소의 바이트단위 크기
        &offset);//오프셋의 배열의 포인터(배열설정가능) 오프셋을 이용해 버퍼의 앞부분을 생략하고 출력할수가있다.



    //가로나 세로 분할이 1개 이상일때
    if (MaxFrameX != 1 || MaxFrameY != 1)
    {
        UV.x = 1.0f / MaxFrameX * CurrentFrameX;
        UV.y = 1.0f / MaxFrameX * (CurrentFrameX + 1);
        UV.z = 1.0f / MaxFrameY * CurrentFrameY;
        UV.w = 1.0f / MaxFrameY * (CurrentFrameY + 1);
    }

    g_DeviceContext->UpdateSubresource(UVBuffer, 0, NULL, &UV, 0, 0);
    //                            연결될레지스터 번호3
    g_DeviceContext->VSSetConstantBuffers(3, 1, &UVBuffer);


    //블렌드 켜기
    g_DeviceContext->OMSetBlendState(blendState[1], NULL, 0xFF);
                 //그려라 라는 명령
    g_DeviceContext->Draw(vertexCount, 0);
    //블렌드 끄기
    g_DeviceContext->OMSetBlendState(blendState[0], NULL, 0xFF);
}

void Image::CreateSampler()
{
    //기존 샘플러가 존재하면 먼저 삭제해라
    if (Sampler)
    {
        SAFE_RELEASE(Sampler);
    }
    HRESULT hr;
    //샘플러를 만들어라
    hr = g_Device->CreateSamplerState(&SamplerDesc, &Sampler);
    assert(SUCCEEDED(hr));
}

void Image::PlayAnimation(AnimState as, bool isFrameX, float nexttime)
{
    //한프레임내에서 시간증가량
    time += DeltaTime;

    if (time >= nexttime)
    {
        //다음시간에 다시 걸릴수있게
        time -= nexttime;
        if (isFrameX)
        {
            switch (as)
            {
                //증가재생
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
                    //감소재생
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
        else //세로재생
        {
            switch (as)
            {
                //증가재생
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
                    //감소재생
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
