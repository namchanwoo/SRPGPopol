#pragma once
#include "Singleton/Singleton.h"

//����ī�޶� 
//dx���� ������ȯ�� �����Ҷ� �������� ���� ī�޶�
//������ ������ ���ֱ����� �̱��� ����
class CamManager : public  SingletonBase <CamManager>
{
private:
    Matrix              V;          //�� ���
    ID3D11Buffer*		VBuffer;    //�� ����

    Matrix              P;          //�������� ���
    ID3D11Buffer*		PBuffer;    //�������� ����
public:

    CamManager();
    ~CamManager();

    void                CameraUpdate();
    Vector2             Pos;        //ī�޶��� ��ġ
};

