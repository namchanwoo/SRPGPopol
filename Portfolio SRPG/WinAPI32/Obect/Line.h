#pragma once
class Line : public Object
{
    //���ؽ�(����,������) ����
    //���� �������̽� ���۴� �Ϲ������� ���� �Ǵ� �ε��� �����͸� �����մϴ�.
    ID3D11Buffer*		VertexBuffer; 
    UINT				vertexCount;


public:

    HRESULT init()override;		//������ ȣ��
    void release()override;		//�Ҹ�� ȣ��
    void update()override;		//���ŵǴ°��� ������ ȣ��
    void render()override;		//�׷��� �Ǵ°��� ������ ȣ��


};

