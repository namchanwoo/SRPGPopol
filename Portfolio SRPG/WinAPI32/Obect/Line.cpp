#include "stdafx.h"
#include "Line.h"

HRESULT Line::init()
{

    //�θ�Ŭ������ init ȣ��
    Object::init();

    //���� ���� Ư¡

    //��ġ��,���� �ִ� ���� ������
    VertexPC  line[2];

    vertexCount = 2;

    //�ð�������� ������������ �����ϴº��Ͱ�
    //�����γ��ͼ� �ո��� �ǰ�

    //�ݽð�������� �޸��� �ȴ�.

    //�׷��ߵɼ������ �־��ֱ�

    line[0].Position.x = 0.0f;
    line[0].Position.y = 0.0f;
    line[0].color = Color(1,0,0,1);

    line[1].Position.x = 1.0f;
    line[1].Position.y = 0.0f;
    line[1].color = Color(0,1,0,1);


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
    { "COLOR",//�ø�ƽ���� ������ �ڷ���
    0,//�ø�ƽ���� ������ �ε��� 0���� �����߱⿡ 0 ���
    DXGI_FORMAT_R32G32B32_FLOAT,//3���� 32��Ʈ float����
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
        desc.ByteWidth = sizeof(VertexPC) * vertexCount; //���� ũ�� (����Ʈ)�Դϴ�.
        desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;//���۰� ������ ���ο� ���ε��Ǵ� ����� �ĺ��Ͻʽÿ�

        D3D11_SUBRESOURCE_DATA data = { 0 };
        //���� ���ҽ��� �ʱ�ȭ�ϱ����� �����͸� �����մϴ�.
        data.pSysMem = line;
        //�ʱ�ȭ �������� ������.

        //���� �����
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
    //�θ�Ŭ������ release ȣ��
    Object::release();
    SAFE_RELEASE(VertexBuffer);
}

void Line::update()
{
    //�θ�Ŭ������ update ȣ��
    Object::update();
}

void Line::render()
{
    //�θ�Ŭ������ render ȣ��
    Object::render();

    UINT stride = sizeof(VertexPC);
    UINT offset = 0;

    g_DeviceContext->IASetVertexBuffers(0,//�Է½���(16~32������ ��������)
        1,//�Է½��Կ� ���̰��� �ϴ� ������ ����
        &VertexBuffer,
        &stride,//���������� �� ������ ����Ʈ���� ũ��
        &offset);//�������� �迭�� ������(�迭��������) �������� �̿��� ������ �պκ��� �����ϰ� ����Ҽ����ִ�.


                 //������ �׷��� ����
                 //g_DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP);
    g_DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
    //g_DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    //�׷��� ��� ���
    g_DeviceContext->Draw(vertexCount, 0);
}
