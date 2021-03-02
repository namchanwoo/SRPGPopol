#include "stdafx.h"
#include "SceneManager.h"

SceneManager::~SceneManager()
{
    for(map<string ,Scene*>::iterator i =
        mapScene.begin();
        i != mapScene.end(); i++)
    {
        //씬객체 해제
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
    //key값이 중복이 안됩니다.

    //키값이 어디에있는지 찾아주는 함수
    if(mapScene.find(key) == mapScene.end())
    {
        //키값이 없다는거임
        mapScene.insert(make_pair(key,p));
    }
    else
    {
        //키값이 있던데?
        assert(0 && "씬 이름 중복");
    }

    //vector<int> A;

    //A.begin();
    //A.end();
    ////iter 반복자
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
        assert(0 && "해당씬 없음");
    }
    //map도 키값을 인덱스로 value접근이 가능
    CurrentScene = mapScene[key];

    return CurrentScene;

    //*mapScene.find(key)->first;
    //*(mapScene.find(key)).second;
}

Scene * SceneManager::GetScene(string key)
{
    if (mapScene.find(key) == mapScene.end())
    {
        assert(0 && "해당씬 없음");
    }
    Scene* temp = mapScene[key];
    return temp;
}
