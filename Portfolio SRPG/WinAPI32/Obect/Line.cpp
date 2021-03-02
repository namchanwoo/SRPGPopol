#include "stdafx.h"
#include "Line.h"

HRESULT Line::init()
{

    //부모클래스의 init 호출
    Object::init();

    //별만 갖는 특징

    //위치값,색값 있는 정점 여섯개
    VertexPC  line[2];

    vertexCount = 2;

    //시계방향으로 정점을찍으면 외적하는벡터가
    //앞으로나와서 앞면이 되고

    //반시계로찍으면 뒷면이 된다.

    //그려야될순서대로 넣어주기

    line[0].Position.x = 0.0f;
    line[0].Position.y = 0.0f;
    line[0].color = Color(1,0,0,1);

    line[1].Position.x = 1.0f;
    line[1].Position.y = 0.0f;
    line[1].color = Color(0,1,0,1);


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
    { "COLOR",//시멘틱에서 정의한 자료형
    0,//시멘틱에서 정의한 인덱스 0으로 지정했기에 0 사용
    DXGI_FORMAT_R32G32B32_FLOAT,//3개의 32비트 float설정
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
        desc.ByteWidth = sizeof(VertexPC) * vertexCount; //버퍼 크기 (바이트)입니다.
        desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;//버퍼가 파이프 라인에 바인딩되는 방법을 식별하십시오

        D3D11_SUBRESOURCE_DATA data = { 0 };
        //하위 리소스를 초기화하기위한 데이터를 지정합니다.
        data.pSysMem = line;
        //초기화 데이터의 포인터.

        //버퍼 만들기
        HRESULT hr = g_Device->CreateBuffer(&desc, &data, &VertexBuffer);
        assert(SUCCEEDED(hr));
    }
    shader = new Shader(_T("./Shader/Rect.hlsl"));
    UINT ElementCount = ARRAYSIZE(LayoutDesc);
    shader->CreateInputLayout(LayoutDesc, ElementCount);


    return S_OK;
}

void Line::release()
{
    //부모클래스의 release 호출
    Object::release();
    SAFE_RELEASE(VertexBuffer);
}

void Line::update()
{
    //부모클래스의 update 호출
    Object::update();
}

void Line::render()
{
    //부모클래스의 render 호출
    Object::render();

    UINT stride = sizeof(VertexPC);
    UINT offset = 0;

    g_DeviceContext->IASetVertexBuffers(0,//입력슬롯(16~32개까지 설정가능)
        1,//입력슬롯에 붙이고자 하는 버퍼의 갯수
        &VertexBuffer,
        &stride,//정점버퍼의 한 원소의 바이트단위 크기
        &offset);//오프셋의 배열의 포인터(배열설정가능) 오프셋을 이용해 버퍼의 앞부분을 생략하고 출력할수가있다.


                 //점으로 그려줄 도형
                 //g_DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP);
    g_DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
    //g_DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    //그려라 라는 명령
    g_DeviceContext->Draw(vertexCount, 0);
}
