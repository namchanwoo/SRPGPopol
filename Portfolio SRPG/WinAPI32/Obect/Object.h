#pragma once
//추상화 클래스
//구체적이지 않은 클래스
class Object
{
    //게임상의 오브젝트들이 가져야될 공통적멤버
    //부모가 가지게 하기

protected:
    Matrix  S,R,T;      //행렬
                        //쉐이더 ?? -> 함수
    Shader*				shader;
    //정점버퍼는 계속 갱신될 필요가 없고
    //월드버퍼는 계속 갱신될 필요가 있다.
    Matrix  W;                      //최종행렬
    ID3D11Buffer*		WBuffer;    //최종행렬 버퍼

    ID3D11Buffer*		ColorBuffer;    
       

public:
    Object();
    virtual ~Object();  //상속하는 부모는 가상소멸자

    Vector2	Scale;		//크기값  S
    Vector3 Rot;		//회전값  Rx,Ry,Rz
    Vector2 Pos;		//이동값  T

    Matrix* P;          //부모의 회전이동행렬
    Matrix  RT;         //회전이동행렬

    Color	color;      //물체가 가질 색깔

    Vector2 Up;         //위쪽 방향
    Vector2 Right;      //오른쪽 방향


    virtual HRESULT init();		//생성시 호출
    virtual void release();		//소멸시 호출
    virtual void update();		//갱신되는값이 있을때 호출
    virtual void render();		//그려야 되는값이 있을때 호출
};

