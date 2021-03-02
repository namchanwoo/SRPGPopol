#pragma once
#include "Singleton/Singleton.h"

class TimeManager :public SingletonBase <TimeManager>
{
    //static은 정적
    //정적멤버변수는 데이터영역 안에 할당
    //프로그램 시작할때 할당이되고 프로그램 종료할때 해제
    //즉 프로그램이 실행되고있는동안에는 메모리가 계속 상주하고 있다.

    //포인터 왜 씀?
    //주소에 접근할일이 왜 필요한가?

    //스택에 할당한 메모리(지역변수,매개변수)
    //포인터 사용 했나요?

    //new 같은경우 포인터가 필요하지


    //static TimeManager* Instance;  4바이트짜리 주소
    //static TimeManager Instance;  객체의크기


private:
    //FPS:초당 프레임 수(frames per second)
    bool	_isHardware;			//고성능 타이머를 지원하냐?
    __int64 _curTime;				//현재시간
    __int64 _lastTime;				//마지막시간

    float _FPStimeElapsed;			//FPS 마지막시간과 현재 시간의 경과량
    float _timeScale;				//시간 단위 
    DWORD _FPSFrameCount;			//FPS 카운트

                                    //리턴값
    DWORD _frameRate;				//FPS
    float _timeElapsed;				//한 프레임당 경과량
    float _worldTime;				//전체 시간 경과량

public:
    TimeManager();
    ~TimeManager();

    //초기화 하지 않으면 프레임은 60프레임으로 고정
    void SetFrame(UINT lock = 60);

    //한 프레임 경과시간 가져오기 1/60
    float GetElapsedTime() const { return _timeElapsed; }
    //전체 시간 경과량 가져오기
    float GetWorldTime() const { return _worldTime; }
    //현재 FPS 가져오기
    unsigned long GetFrameRate() { return _frameRate; }

    void Render();




};

