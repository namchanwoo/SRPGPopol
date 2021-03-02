#pragma once
//�� ���� �׷����� �ﰢ���� ������ �̾
class Circle : public Object
{
    //������ ���� �ﰢ���� �ʿ�
    //? �
    ID3D11Buffer*		VertexBuffer; 
    UINT				vertexCount;

    UINT                Slice;  //������ ���� �׸�����
    //���� �̷�� ����
    VertexPC*           Vertex;
    bool                _isFill;
    
public:
    COL_CIRCLE          Col;
    

    HRESULT init(UINT slice,bool isFill = true);		//������ ȣ��
    void release()override;		//�Ҹ�� ȣ��
    void update()override;		//���ŵǴ°��� ������ ȣ��
    void render()override;		//�׷��� �Ǵ°��� ������ ȣ��

};

