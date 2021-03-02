#pragma once
class Shader
{
private:

    //쉐이더 사용에 필요한 com 인터페이스
    ID3D11VertexShader*			VertexShader;
    ID3D11PixelShader*			PixelShader;
    ID3D10Blob*					VsBlob;
    ID3D10Blob*					PsBlob;
    ID3D11InputLayout*			VertexLayout;// 메모리에 배치 된 정점 데이터를 그래픽 파이프 라인 의 IA 에 공급하는 방법에 대한 정의를 보유

public:
    Shader(_tstring file, string vs = "VS", string ps = "PS");
    ~Shader();
    void CreateInputLayout(D3D11_INPUT_ELEMENT_DESC* desc, UINT count);
    void SetShader();
};

