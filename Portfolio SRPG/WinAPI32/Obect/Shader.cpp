#include "stdafx.h"
#include "Shader.h"


Shader::Shader(_tstring file, string vs, string ps)
{
    //Create Shader
    {
        HRESULT hr;

        hr = D3DX11CompileFromFile(file.c_str(),//읽어올 셰이더파일
                                   0,//특별히 사용되지 않음
                                   0,//인클루드 파일 필요할때 
                                   vs.c_str(),//셰이더 프로그램 개별적으로 컴파일할때 진입함수이름설정 Effect Framework 을 사용하는 경우에 
                                              //Effect File에 정의된 technique passes에 
                                              //	Shader Entry Point 가 정의 되어 있으므로 이 Parameter를 NULL로 지정한다.
                                   "vs_5_0",//셰이더 버전
                                   0,//Shader Code 의 Compile 방식에 영향을 미치는 Flags를 지정한다.
                                   0, //특별히 사용되지 않으므로 0을 지정한다.
                                   0, //Shader를 비동기적으로 Compile 하기 위한 Option 으로 사용된다.
                                   &VsBlob,//Compile된 Shader를 담은 ID3D10Blob(COM 객체임을 상기) Structure 를 가리키는 Pointer
                                   0,//Error Message를 담은 ID3D10Blobk(역시 COM 객체임을 상기) Structure를 가리키는 Pointer
                                   0);//비동기 Compile시 Error Code 를 조회하는데 쓰인다
                                      /*비동기 컴파일을 진행하지 않을 경우에는 무난하게 0을 지정한다.
                                      즉 아무것도 수행하지 않는다.*/
        assert(SUCCEEDED(hr));

        hr = D3DX11CompileFromFile(file.c_str(), 0, 0, ps.c_str(), "ps_5_0", 0, 0, 0, &PsBlob, 0, 0);
        assert(SUCCEEDED(hr));


        hr = g_Device->CreateVertexShader(VsBlob->GetBufferPointer(), //컴파일 된 셰이더에 대한 포인터.
                                          VsBlob->GetBufferSize()//컴파일 된 버텍스 쉐이더의 크기
                                          , NULL, //클래스 연계 인터페이스에 대한 포인터 ( ID3D11ClassLinkage 참조 ); 값은 NULL 일 수 있습니다 .
                                          &VertexShader);//ID3D11VertexShader 인터페이스 에 대한 포인터의 주소입니다
        assert(SUCCEEDED(hr));

        hr = g_Device->CreatePixelShader(PsBlob->GetBufferPointer(), PsBlob->GetBufferSize(), NULL, &PixelShader);
        assert(SUCCEEDED(hr));



    }



}


Shader::~Shader()
{
    SAFE_RELEASE(VertexShader);
    SAFE_RELEASE(PixelShader);
    SAFE_RELEASE(VsBlob);
    SAFE_RELEASE(PsBlob);
    SAFE_RELEASE(VertexLayout);
}

void Shader::CreateInputLayout(D3D11_INPUT_ELEMENT_DESC * desc, UINT count)
{
    //Create InputLayout
    {
        HRESULT hr = g_Device->CreateInputLayout
        (
            desc,//정점구조체배열
            count,//배열원소갯수
            VsBlob->GetBufferPointer(),//정점셰이더 포인터
            VsBlob->GetBufferSize(),//셰이더크기
            &VertexLayout//입력배치를 포인터를 통해 돌려줌
        );
        assert(SUCCEEDED(hr));
    }
}

void Shader::SetShader()
{
    g_DeviceContext->VSSetShader(VertexShader,//꼭짓점 셰이더를 가리키는 포인터 ( ID3D11VertexShader 참조 ) NULL을 전달 하면이 파이프 라인 단계에 대한 셰이더가 비활성화됩니다.
                                 0, //클래스 인스턴스 인터페이스의 배열에 대한 포인터입니다 ,셰이더가 인터페이스를 사용하지 않는 경우 ppClassInstances를 NULL로 설정하십시오 .
                                 0);//배열의 클래스 인스턴스 인터페이스 수입니다
    g_DeviceContext->PSSetShader(PixelShader,//픽셀 셰이더의 포인터 
                                 0, 0);
    g_DeviceContext->IASetInputLayout(VertexLayout);
}
