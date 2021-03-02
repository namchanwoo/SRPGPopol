#pragma once

//=================================================
//	## singletonBase ## (이 베이스로 싱글톤을 만든다) 
//=================================================

/*
싱글톤 패턴은 static member로 하나의 인스턴스만 생성되며,
프로그램 안에서 전역적으로 접근이 가능하다.
보통 클래스의 인스턴스를 하나만 유지하고자 할 때 사용한다.

전역 변수를 이용해도 하나의 인스턴스를 유지할 수 있지만,
싱글톤 패턴을 사용하면 클래스 자신이 자기의 유일한 인스턴스로
접근하는 방법을 캡슐화 하여 관리할 수 있고, 상속, 객체 생성,
객체 소멸, 인스턴스의 개수 제어등등 활용에 유리하다

더 좋은 방법은 자기 스스로가 자기의 유일한 인스턴스로 접근하는
방법을 자체적으로 관리하는 방법이다.
이 싱글톤 베이스는 다른 인스턴스가 생성되지 않도록 처리 할 수
있고, 접근 방법 또한 제한을 둘 수 있다.
*/

//싱글톤 클래스
template <typename T>
class SingletonBase
{
protected:
    //static T Instance;
    static T* Instance; //힙에 할당한 주소만 데이터영역에 담으려고

    SingletonBase() {  };
    ~SingletonBase() {};
public:
    static T* GetInstance();
    void ReleaseSingleton();
};

//정적 멤버 같은경우 함수나 클래스 밖에서 초기화 한다.
template<typename T>
T * SingletonBase<T>::Instance = nullptr;


template<typename T>
T * SingletonBase<T>::GetInstance()
{
    if (!Instance) Instance = new T;
    return Instance;
}

template<typename T>
void SingletonBase<T>::ReleaseSingleton()
{
    if (Instance)
    {
        delete Instance;
        Instance = nullptr;
    }

}
