#pragma once
class Rect :public Object
{
    //���ؽ�(����,������) ����
    //���� �������̽� ���۴� �Ϲ������� ���� �Ǵ� �ε��� �����͸� �����մϴ�.
    ID3D11Buffer*		VertexBuffer; 
    UINT				vertexCount;
    bool                _isFill;
    VertexPC*           Vertex;
public:

    COL_RECT            Col;


    HRESULT init(bool isFill = true,
        Vector2 pivot = Vector2(0,0));		//������ ȣ��
    void release()override;		//�Ҹ�� ȣ��
    void update()override;		//���ŵǴ°��� ������ ȣ��
    void render()override;		//�׷��� �Ǵ°��� ������ ȣ��


};

