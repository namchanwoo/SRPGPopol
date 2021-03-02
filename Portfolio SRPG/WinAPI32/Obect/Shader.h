#pragma once
class Shader
{
private:

    //���̴� ��뿡 �ʿ��� com �������̽�
    ID3D11VertexShader*			VertexShader;
    ID3D11PixelShader*			PixelShader;
    ID3D10Blob*					VsBlob;
    ID3D10Blob*					PsBlob;
    ID3D11InputLayout*			VertexLayout;// �޸𸮿� ��ġ �� ���� �����͸� �׷��� ������ ���� �� IA �� �����ϴ� ����� ���� ���Ǹ� ����

public:
    Shader(_tstring file, string vs = "VS", string ps = "PS");
    ~Shader();
    void CreateInputLayout(D3D11_INPUT_ELEMENT_DESC* desc, UINT count);
    void SetShader();
};

