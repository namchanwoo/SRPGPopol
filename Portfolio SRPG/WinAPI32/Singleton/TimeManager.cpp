#include "stdafx.h"
#include "TimeManager.h"
#include <mmsystem.h>
#pragma comment(lib, "winmm.lib")

//cast연산자는 (c스타일 형변환)보다 안전하며 
//컴파일시 오류를 잡아준다.



//timeGetTime 함수를 사용하기 위한 라이브러리 추가
//윈도우가 시작되고 난 뒤의 시간을 리턴

TimeManager::TimeManager()
{
    //주파수,클럭이 뭔지


    //frequency 주파수

    // 참고 http://www.tipssoft.com/bulletin/board.php?bo_table=FAQ&wr_id=735

    //클럭은 전력이 꺼지고 켜지고 반복되는 주파수를 클럭 주파수라 하는데 줄여서 클럭이라함

    /*컴퓨터의 메인보드에는 하나의 고해상도 타이머가 존재합니다.이 타이머는 정확하고
    일정한 주파수
    (즉, 1초에 각 타이머의 성능에 맞는 진동수)를 갖기 때문에 측정하고자 하는 코드들의
    시작과 끝에서
    CPU 클럭수를 얻어 그 차로 수행시간을 얻을 수 있습니다.*/

    //QueryPerformanceFrequency고해상도 타이머의 주파수(1초당 진동수)를 매개변수가 얻는 함수
    //지원하지않으면 0을 반환함
    //QueryPerformanceCounter:고해상도 타이머의 현재 CPU의 클럭수를 매개변수가 얻는 함수
    //지원하지않으면 0을 반환함

    //고성능 타이머 지원여부를 체크한다.
    //고성능 타이머를 지원하면 초당 마이크로세컨드까지 값이 들어간다.
    //(1/1000000)
    //시간 측정
    __int64 _periodFrequency;
    

    //지원되는 컴퓨터 라면
    if (QueryPerformanceFrequency(reinterpret_cast<LARGE_INTEGER*>(&_periodFrequency)))
    //if(QueryPerformanceFrequency((LARGE_INTEGER*)&_periodFrequency))
    {
        //64비트 정수를 사용할 수 있도록 정의한 이유는
        //마이크로세컨드를 (1/1000000)초 까지 측정하여 좀더 정확한 시간값을 저장하기 위함
        _isHardware = true;
        //초기화 될때 마지막시간을(클럭단위) 먼저 초기화 해줌
        QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER*>(&_lastTime));

        //초의 단위가 어떻게 되는지 초당 진동수를 나누어 초기화
        _timeScale = 1.0f / _periodFrequency;
    }
    //지원되지 않는 컴퓨터라면
    else
    {
        _isHardware = false;

        //timeGetTime:윈도우(운영체제)가 시작되어서 지금까지 흐른 시간을 
        //1 / 1000 초(milliseconds) 단위로 DWORD형을 리턴하는 함수다.

        //초기화 될때 마지막시간을(밀리세컨드단위) 먼저 초기화 해줌
        _lastTime = timeGetTime();
        //밀리세컨드 크기로 계산이라는걸 초기화
        _timeScale = 0.001f;
    }

    //타이머가 생성될때 0값으로 만들겠다.
    _frameRate = 0;
    _FPSFrameCount = 0;
    _FPStimeElapsed = 0.0f;
    _worldTime = 0;
}


TimeManager::~TimeManager()
{
}

void TimeManager::SetFrame(UINT lock)
{
    //고성능 타이머를 지원한다면
    if (_isHardware)
    {
        //현재시간을(클럭단위) 먼저 초기화 해줌
        QueryPerformanceCounter((reinterpret_cast<LARGE_INTEGER*>(&_curTime)));
    }
    else
    {
        //지원하지 않으면 time함수를 이용 밀리세컨드(1/1000)
        _curTime = timeGetTime();
    }

    //마지막시간과 현재시간의 한프레임당 경과량(초단위)
    _timeElapsed = (_curTime - _lastTime) * _timeScale;


    //적어도 1프레임 이상 나오는 게임에 한하여
    if (lock > 0.0f)
    {
        //이곳의 반복문때문에 경과시간이 맞춰짐 이곳에서 반복문을 돌다가 조건을 맞추면 업데이트와 렌더로 가게됨
        //고정 프레임의 시간을 만족할때까지 루프시켜라
        //       0.1        < 0.1
        while (_timeElapsed < (1.0f / lock))
        {
            if (_isHardware) QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER*>(&_curTime));
            else _curTime = timeGetTime();

            //마지막 시간과 현재 시간의 경과량
            _timeElapsed = (_curTime - _lastTime) * _timeScale;
        }
    }

    _lastTime = _curTime;				//마지막 시간을 기록
    _FPSFrameCount++;					//자동으로 프레임 카운트 증가
    _FPStimeElapsed += _timeElapsed;	//초당 프레임 시간 경과량 증가
    _worldTime += _timeElapsed;			//전체 시간 경과량 증가

    //프레임 초기화를 1초마다 진행
    if (_FPStimeElapsed > 1.0f)
    {
        _frameRate = _FPSFrameCount;
        _FPSFrameCount = 0;
        _FPStimeElapsed = 0.0f;
    }
}

void TimeManager::Render()
{
    wstring str = L"FPS:"+ to_wstring(_frameRate);
    wstring str2 = L"WorldTime:" + to_wstring(_worldTime);
    wstring str3 = L"SecondPerFrameTime:" + to_wstring(_timeElapsed);
    ////출력할 텍스트 박스 (l,t,r,b)
    RECT rect = { 0,0,800,300 };
    RECT rect2 = { 0,20,800,300 };
    RECT rect3 = { 0,40,800,300 };
    Color brush = { 1.0f,0.0f,0.0f,1.0f };//빨간색
    //                                          //기본
    WRITEMANAGER->RenderText(str, rect,30.0f,L"돋움체",brush);
    WRITEMANAGER->RenderText(str2, rect2);
    WRITEMANAGER->RenderText(str3, rect3);
}
