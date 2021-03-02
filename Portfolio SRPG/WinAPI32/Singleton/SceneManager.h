#pragma once
#include "Singleton/Singleton.h"

//����
//�޸𸮿���,�Ҵ���� ��������,ũ�Ⱑ�������½���

//�ڷᱸ��
//List,Tree

//stl
//vector,map


class SceneManager: public  SingletonBase <SceneManager>
{
private:
    //vector �����迭
    //�߰����� ũ�⸦ ��� �ø��ų� ���ϼ��ִ�.
    //����ũ�Ⱑ �� ���Ǿ������� �̻�
    //emplaceback(�̵�������),pushback(���������)

    //map ����
    //�߰� ������ ����. (�����ͷ� ����)
    //Ž���� �־ ����Ʈ���� ������

    //  pair<Ű,    ��>
    map<string ,Scene*> mapScene;
public:
    ~SceneManager();
    //Scene*  SceneArray[2];      //���� ��� �迭
    Scene*  CurrentScene;       //���� ��

    void AddScene(string key,Scene* p);
    Scene* ChageScene(string key);
    Scene* GetScene(string key);

};

