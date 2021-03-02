#include "stdafx.h"
#include "SceneManager.h"

SceneManager::~SceneManager()
{
    for(map<string ,Scene*>::iterator i =
        mapScene.begin();
        i != mapScene.end(); i++)
    {
        //����ü ����
        if(i->second)
        {
            i->second->release();
            SAFE_DELETE(i->second);
        }
    }
    mapScene.clear();
}

void SceneManager::AddScene(string key, Scene * p)
{
    //key���� �ߺ��� �ȵ˴ϴ�.

    //Ű���� ����ִ��� ã���ִ� �Լ�
    if(mapScene.find(key) == mapScene.end())
    {
        //Ű���� ���ٴ°���
        mapScene.insert(make_pair(key,p));
    }
    else
    {
        //Ű���� �ִ���?
        assert(0 && "�� �̸� �ߺ�");
    }

    //vector<int> A;

    //A.begin();
    //A.end();
    ////iter �ݺ���
    //for(int i=0; i<10; i++)
    //{
    //    A[i];
    //}
    //for(vector<int>::iterator i = A.begin();
    //    i < A.end(); i++)
    //{
    //    *i;
    //}

    //mapScene.insert();



}

Scene* SceneManager::ChageScene(string key)
{
    if(mapScene.find(key) == mapScene.end())
    {
        assert(0 && "�ش�� ����");
    }
    //map�� Ű���� �ε����� value������ ����
    CurrentScene = mapScene[key];

    return CurrentScene;

    //*mapScene.find(key)->first;
    //*(mapScene.find(key)).second;
}

Scene * SceneManager::GetScene(string key)
{
    if (mapScene.find(key) == mapScene.end())
    {
        assert(0 && "�ش�� ����");
    }
    Scene* temp = mapScene[key];
    return temp;
}
