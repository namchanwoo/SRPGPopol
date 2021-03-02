#pragma once
#include "Singleton/Singleton.h"

//메인카메라 
//dx상의 공간변환을 구현할때 메인으로 쓰는 카메라
//전역적 접근을 해주기위해 싱글톤 선언
class CamManager : public  SingletonBase <CamManager>
{
private:
    Matrix              V;          //뷰 행렬
    ID3D11Buffer*		VBuffer;    //뷰 버퍼

    Matrix              P;          //프로젝션 행렬
    ID3D11Buffer*		PBuffer;    //프로젝션 버퍼
public:

    CamManager();
    ~CamManager();

    void                CameraUpdate();
    Vector2             Pos;        //카메라의 위치
};

