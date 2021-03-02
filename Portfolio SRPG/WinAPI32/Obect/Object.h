#pragma once
//�߻�ȭ Ŭ����
//��ü������ ���� Ŭ����
class Object
{
    //���ӻ��� ������Ʈ���� �����ߵ� ���������
    //�θ� ������ �ϱ�

protected:
    Matrix  S,R,T;      //���
                        //���̴� ?? -> �Լ�
    Shader*				shader;
    //�������۴� ��� ���ŵ� �ʿ䰡 ����
    //������۴� ��� ���ŵ� �ʿ䰡 �ִ�.
    Matrix  W;                      //�������
    ID3D11Buffer*		WBuffer;    //������� ����

    ID3D11Buffer*		ColorBuffer;    
       

public:
    Object();
    virtual ~Object();  //����ϴ� �θ�� ����Ҹ���

    Vector2	Scale;		//ũ�Ⱚ  S
    Vector3 Rot;		//ȸ����  Rx,Ry,Rz
    Vector2 Pos;		//�̵���  T

    Matrix* P;          //�θ��� ȸ���̵����
    Matrix  RT;         //ȸ���̵����

    Color	color;      //��ü�� ���� ����

    Vector2 Up;         //���� ����
    Vector2 Right;      //������ ����


    virtual HRESULT init();		//������ ȣ��
    virtual void release();		//�Ҹ�� ȣ��
    virtual void update();		//���ŵǴ°��� ������ ȣ��
    virtual void render();		//�׷��� �Ǵ°��� ������ ȣ��
};

