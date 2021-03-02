#include "stdafx.h"
#include "Circle.h"

HRESULT Circle::init(UINT slice,bool isFill)
{
    //�⺻�����δ� ä���� �׸���
    _isFill = isFill;

   

    //�θ�Ŭ������ init ȣ��
    Object::init();

    //���� ���� Ư¡

    //36

    
    //������ �迭�� ũ�⸦ �������� �����Ҵ�
    //������ ������ �޾ƿ���
    Slice = slice; 
    float Seta = 360.0f / (float)Slice;
    //�ﰢ������
    if(isFill)
    {
        vertexCount = Slice * 3;
        Vertex = new VertexPC[vertexCount];
        

        for(UINT i=0; i < Slice; i++)
        {
            //0,3,6
            Vertex[i*3].Position.x = 0.0f;
            Vertex[i*3].Position.y = 0.0f;
            Vertex[i*3].color = Color(1,0,0,1);
            //1,4,7
            Vertex[i*3+1].Position.x = 0.5f * cosf((i+1)*Seta*TORADIAN);
            Vertex[i*3+1].Position.y = 0.5f * sinf((i+1)*Seta*TORADIAN);
            Vertex[i*3+1].color = Color(0,0,1,1);
            //2,5,8
            Vertex[i*3+2].Position.x = 0.5f * cosf(i*Seta*TORADIAN);
            Vertex[i*3+2].Position.y = 0.5f * sinf(i*Seta*TORADIAN);
            Vertex[i*3+2].color = Color(0,0,1,1);
        }
    
    }
    //�� ����
    else
    {
        vertexCount = Slice * 2;
        Vertex = new VertexPC[vertexCount];
        for(UINT i=0; i < Slice; i++)
        {
            //1,4,7
            Vertex[i*2].Position.x = 0.5f * cosf((i+1)*Seta*TORADIAN);
            Vertex[i*2].Position.y = 0.5f * sinf((i+1)*Seta*TORADIAN);
            Vertex[i*2].color = Color(0,0,1,1);
            //2,5,8
            Vertex[i*2+1].Position.x = 0.5f * cosf(i*Seta*TORADIAN);
            Vertex[i*2+1].Position.y = 0.5f * sinf(i*Seta*TORADIAN);
            Vertex[i*2+1].color = Color(0,0,1,1);
        }
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

    Col.center = Vector2(W._41,W._42);
    Col.radius = Scale.x *0.5f;

    return S_OK;
}

void Circle::release()
{
    //�����Ҵ��� �迭 ����
    SAFE_DELETE_ARRAY(Vertex);

    //�θ�Ŭ������ release ȣ��
    Object::release();
    SAFE_RELEASE(VertexBuffer);
}

void Circle::update()
{
    //�θ�Ŭ������ update ȣ��
    Object::update();

    //������ǥ���� �ƴϴ� �θ� ��������
    Col.center = Pos;
    if(P != nullptr)
    {
        //��İ� ���͸� �����ִ� �Լ�
        D3DXVec2TransformCoord(&Col.center,&Col.center,P);//w 1
    }
    Col.radius = Scale.x *0.5f;
}

void Circle::render()
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
        (D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
    }
    g_DeviceContext->IASetVertexBuffers(0,//�Է½���(16~32������ ��������)
        1,//�Է½��Կ� ���̰��� �ϴ� ������ ����
        &VertexBuffer,
        &stride,//���������� �� ������ ����Ʈ���� ũ��
        &offset);//�������� �迭�� ������(�迭��������) �������� �̿��� ������ �պκ��� �����ϰ� ����Ҽ����ִ�.


                 
    //�׷��� ��� ���
    g_DeviceContext->Draw(vertexCount, 0);
}
