#include "stdafx.h"
#include "Star.h"

Star::Star()
{
}

Star::~Star()
{
}

HRESULT Star::init()
{
    

    //부모클래스의 init 호출
    Object::init();

    //별만 갖는 특징

    //위치값만 있는 정점 다섯개
    VertexP  Star[6];

    vertexCount = 6;

    //그려야될순서대로 넣어주기
    Star[0].Position.x = 0.5f * (cosf(72.0f* 0.0f * TORADIAN));
    Star[0].Position.y = 0.5f * (sinf(72.0f* 0.0f * TORADIAN));

    //2번
    Star[1].Position.x = 0.5f * (cosf(72.0f* 2.0f * TORADIAN));
    Star[1].Position.y = 0.5f * (sinf(72.0f* 2.0f * TORADIAN));

    //4번
    Star[2].Position.x = 0.5f * (cosf(72.0f* 4.0f * TORADIAN));
    Star[2].Position.y = 0.5f * (sinf(72.0f* 4.0f * TORADIAN));

    //1번
    Star[3].Position.x = 0.5f * (cosf(72.0f* 1.0f * TORADIAN));
    Star[3].Position.y = 0.5f * (sinf(72.0f* 1.0f * TORADIAN));

    //3번
    Star[4].Position.x = 0.5f * (cosf(72.0f* 3.0f * TORADIAN));
    Star[4].Position.y = 0.5f * (sinf(72.0f* 3.0f * TORADIAN));

    //0번
    Star[5].Position.x = 0.5f * (cosf(72.0f* 0.0f * TORADIAN));
    Star[5].Position.y = 0.5f * (sinf(72.0f* 0.0f * TORADIAN));

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
    };
    //CreateVertexBuffer
    {
        D3D11_BUFFER_DESC desc;
        desc = { 0 };
        desc.Usage = D3D11_USAGE_DEFAULT;//버퍼를 읽고 쓰는 방법을 식별하십시오. 업데이트 빈도가 핵심 요소입니다. 가장 일반적인 값은 일반적으로 D3D11_USAGE_DEFAULT입니다. 
        desc.ByteWidth = sizeof(VertexP) * vertexCount; //버퍼 크기 (바이트)입니다.
        desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;//버퍼가 파이프 라인에 바인딩되는 방법을 식별하십시오

        D3D11_SUBRESOURCE_DATA data = { 0 };
        //하위 리소스를 초기화하기위한 데이터를 지정합니다.
        data.pSysMem = Star;
        //초기화 데이터의 포인터.

        //버퍼 만들기
        HRESULT hr = g_Device->CreateBuffer(&desc, &data, &VertexBuffer);
        assert(SUCCEEDED(hr));
    }
  

    shader = new Shader(_T("./Shader/Star.hlsl"));
    UINT ElementCount = ARRAYSIZE(LayoutDesc);
    shader->CreateInputLayout(LayoutDesc, ElementCount);

    return S_OK;
}

void Star::release()
{
    //부모클래스의 release 호출
    Object::release();
    SAFE_RELEASE(VertexBuffer);
}

void Star::update()
{
    //부모클래스의 update 호출
    Object::update();
}

void Star::render()
{
    //부모클래스의 render 호출
    Object::render();

    UINT stride = sizeof(VertexP);
    UINT offset = 0;

    g_DeviceContext->IASetVertexBuffers(0,//입력슬롯(16~32개까지 설정가능)
                                        1,//입력슬롯에 붙이고자 하는 버퍼의 갯수
                                        &VertexBuffer,
                                        &stride,//정점버퍼의 한 원소의 바이트단위 크기
                                        &offset);//오프셋의 배열의 포인터(배열설정가능) 오프셋을 이용해 버퍼의 앞부분을 생략하고 출력할수가있다.


    //점으로 그려줄 도형
    g_DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP);
    //g_DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
    //g_DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    //그려라 라는 명령
    g_DeviceContext->Draw(vertexCount, 0);
}
