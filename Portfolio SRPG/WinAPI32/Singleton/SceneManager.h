#pragma once
#include "Singleton/Singleton.h"

//과제
//메모리영역,할당시점 해제시점,크기가정해지는시점

//자료구조
//List,Tree

//stl
//vector,map


class SceneManager: public  SingletonBase <SceneManager>
{
private:
    //vector 동적배열
    //추가적인 크기를 잡고 늘리거나 줄일수있다.
    //버퍼크기가 다 사용되었을때는 이사
    //emplaceback(이동생성자),pushback(복사생성자)

    //map 장점
    //추가 생성이 쉽다. (포인터로 접근)
    //탐색에 있어서 리스트보다 낫더라

    //  pair<키,    값>
    map<string ,Scene*> mapScene;
public:
    ~SceneManager();
    //Scene*  SceneArray[2];      //씬이 담긴 배열
    Scene*  CurrentScene;       //현재 씬

    void AddScene(string key,Scene* p);
    Scene* ChageScene(string key);
    Scene* GetScene(string key);

};

