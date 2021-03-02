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
    

    //�θ�Ŭ������ init ȣ��
    Object::init();

    //���� ���� Ư¡

    //��ġ���� �ִ� ���� �ټ���
    VertexP  Star[6];

    vertexCount = 6;

    //�׷��ߵɼ������ �־��ֱ�
    Star[0].Position.x = 0.5f * (cosf(72.0f* 0.0f * TORADIAN));
    Star[0].Position.y = 0.5f * (sinf(72.0f* 0.0f * TORADIAN));

    //2��
    Star[1].Position.x = 0.5f * (cosf(72.0f* 2.0f * TORADIAN));
    Star[1].Position.y = 0.5f * (sinf(72.0f* 2.0f * TORADIAN));

    //4��
    Star[2].Position.x = 0.5f * (cosf(72.0f* 4.0f * TORADIAN));
    Star[2].Position.y = 0.5f * (sinf(72.0f* 4.0f * TORADIAN));

    //1��
    Star[3].Position.x = 0.5f * (cosf(72.0f* 1.0f * TORADIAN));
    Star[3].Position.y = 0.5f * (sinf(72.0f* 1.0f * TORADIAN));

    //3��
    Star[4].Position.x = 0.5f * (cosf(72.0f* 3.0f * TORADIAN));
    Star[4].Position.y = 0.5f * (sinf(72.0f* 3.0f * TORADIAN));

    //0��
    Star[5].Position.x = 0.5f * (cosf(72.0f* 0.0f * TORADIAN));
    Star[5].Position.y = 0.5f * (sinf(72.0f* 0.0f * TORADIAN));

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
    };
    //CreateVertexBuffer
    {
        D3D11_BUFFER_DESC desc;
        desc = { 0 };
        desc.Usage = D3D11_USAGE_DEFAULT;//���۸� �а� ���� ����� �ĺ��Ͻʽÿ�. ������Ʈ �󵵰� �ٽ� ����Դϴ�. ���� �Ϲ����� ���� �Ϲ������� D3D11_USAGE_DEFAULT�Դϴ�. 
        desc.ByteWidth = sizeof(VertexP) * vertexCount; //���� ũ�� (����Ʈ)�Դϴ�.
        desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;//���۰� ������ ���ο� ���ε��Ǵ� ����� �ĺ��Ͻʽÿ�

        D3D11_SUBRESOURCE_DATA data = { 0 };
        //���� ���ҽ��� �ʱ�ȭ�ϱ����� �����͸� �����մϴ�.
        data.pSysMem = Star;
        //�ʱ�ȭ �������� ������.

        //���� �����
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
    //�θ�Ŭ������ release ȣ��
    Object::release();
    SAFE_RELEASE(VertexBuffer);
}

void Star::update()
{
    //�θ�Ŭ������ update ȣ��
    Object::update();
}

void Star::render()
{
    //�θ�Ŭ������ render ȣ��
    Object::render();

    UINT stride = sizeof(VertexP);
    UINT offset = 0;

    g_DeviceContext->IASetVertexBuffers(0,//�Է½���(16~32������ ��������)
                                        1,//�Է½��Կ� ���̰��� �ϴ� ������ ����
                                        &VertexBuffer,
                                        &stride,//���������� �� ������ ����Ʈ���� ũ��
                                        &offset);//�������� �迭�� ������(�迭��������) �������� �̿��� ������ �պκ��� �����ϰ� ����Ҽ����ִ�.


    //������ �׷��� ����
    g_DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP);
    //g_DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
    //g_DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    //�׷��� ��� ���
    g_DeviceContext->Draw(vertexCount, 0);
}
