#include "stdafx.h"
#include "Rect.h"

HRESULT Rect::init(bool isFill, Vector2 pivot)
{
    _isFill = isFill;

    //�θ�Ŭ������ init ȣ��
    Object::init();

    //���� ���� Ư¡

    //��ġ��,���� �ִ� ���� ������
    if(isFill)
    {
        Vertex = new VertexPC[6];

        vertexCount = 6;

        Vertex[0].Position.x = -0.5f;
        Vertex[0].Position.y = -0.5f;
        Vertex[0].color = Color(1,0,0,1);

        Vertex[1].Position.x = -0.5f;
        Vertex[1].Position.y = 0.5f;
        Vertex[1].color = Color(0,1,0,1);

        Vertex[2].Position.x = 0.5f;
        Vertex[2].Position.y = -0.5f;
        Vertex[2].color = Color(0,0,2,1);

        Vertex[3].Position.x = -0.5f;
        Vertex[3].Position.y = 0.5f;
        Vertex[3].color = Color(0,1,0,1);

        Vertex[4].Position.x = 0.5f;
        Vertex[4].Position.y = 0.5f;
        Vertex[4].color = Color(1,1,0,1);

        Vertex[5].Position.x = 0.5f;
        Vertex[5].Position.y = -0.5f;
        Vertex[5].color = Color(0,0,2,1);
    }
    else
    {
        Vertex = new VertexPC[5];

        vertexCount = 5;

        Vertex[0].Position.x = -0.5f;
        Vertex[0].Position.y = -0.5f;
        Vertex[0].color = Color(1,0,0,1);

        Vertex[1].Position.x = -0.5f;
        Vertex[1].Position.y = 0.5f;
        Vertex[1].color = Color(0,1,0,1);

        Vertex[2].Position.x = 0.5f;
        Vertex[2].Position.y = 0.5f;
        Vertex[2].color = Color(0,0,2,1);

        Vertex[3].Position.x = 0.5f;
        Vertex[3].Position.y = -0.5f;
        Vertex[3].color = Color(0,1,0,1);

        Vertex[4].Position.x = -0.5f;
        Vertex[4].Position.y = -0.5f;
        Vertex[4].color = Color(1,1,0,1);

    
    }
    for (int i = 0; i < vertexCount; i++)
    {
        Vertex[i].Position.x -= pivot.x;
        Vertex[i].Position.y -= pivot.y;
    }

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
        data.pSysMem = Vertex;
        //�ʱ�ȭ �������� ������.

        //���� �����
        HRESULT hr = g_Device->CreateBuffer(&desc, &data, &VertexBuffer);
        assert(SUCCEEDED(hr));
    }
    shader = new Shader(_T("./Shader/Rect.hlsl"));
    UINT ElementCount = ARRAYSIZE(LayoutDesc);
    shader->CreateInputLayout(LayoutDesc, ElementCount);

    Col.left = Pos.x - Scale.x * 0.5f;
    Col.right = Pos.x + Scale.x * 0.5f;
    Col.top = Pos.y + Scale.y * 0.5f;
    Col.bottom = Pos.y - Scale.y * 0.5f;

    return S_OK;
}

void Rect::release()
{
    //�θ�Ŭ������ release ȣ��
    Object::release();
    SAFE_DELETE_ARRAY(Vertex);
    SAFE_RELEASE(VertexBuffer);
}

void Rect::update()
{
    //�θ�Ŭ������ update ȣ��
    Object::update();

    //W������ Col���
    if(P != nullptr)
    {
        Vector2 MovePos;
        //��İ� ���͸� �����ִ� �Լ�
        D3DXVec2TransformCoord(&MovePos,&Pos,P);//w 1
        Col.left = MovePos.x - Scale.x * 0.5f;
        Col.right = MovePos.x + Scale.x * 0.5f;
        Col.top = MovePos.y + Scale.y * 0.5f;
        Col.bottom = MovePos.y - Scale.y * 0.5f;
    }
    else
    {
        Col.left = Pos.x - Scale.x * 0.5f;
        Col.right = Pos.x + Scale.x * 0.5f;
        Col.top = Pos.y + Scale.y * 0.5f;
        Col.bottom = Pos.y - Scale.y * 0.5f;
    }
}

void Rect::render()
{
    //�θ�Ŭ������ render ȣ��
    Object::render();

    UINT stride = sizeof(VertexPC);
    UINT offset = 0;

    if(_isFill)
    {
        g_DeviceContext->IASetPrimitiveTopology
        (D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    }
    else
    {
        g_DeviceContext->IASetPrimitiveTopology
        (D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP);
    }
    g_DeviceContext->IASetVertexBuffers(0,//�Է½���(16~32������ ��������)
        1,//�Է½��Կ� ���̰��� �ϴ� ������ ����
        &VertexBuffer,
        &stride,//���������� �� ������ ����Ʈ���� ũ��
        &offset);//�������� �迭�� ������(�迭��������) �������� �̿��� ������ �պκ��� �����ϰ� ����Ҽ����ִ�.

    //�׷��� ��� ���
    g_DeviceContext->Draw(vertexCount, 0);
}
