#include "stdafx.h"
#include "TimeManager.h"
#include <mmsystem.h>
#pragma comment(lib, "winmm.lib")

//cast�����ڴ� (c��Ÿ�� ����ȯ)���� �����ϸ� 
//�����Ͻ� ������ ����ش�.



//timeGetTime �Լ��� ����ϱ� ���� ���̺귯�� �߰�
//�����찡 ���۵ǰ� �� ���� �ð��� ����

TimeManager::TimeManager()
{
    //���ļ�,Ŭ���� ����


    //frequency ���ļ�

    // ���� http://www.tipssoft.com/bulletin/board.php?bo_table=FAQ&wr_id=735

    //Ŭ���� ������ ������ ������ �ݺ��Ǵ� ���ļ��� Ŭ�� ���ļ��� �ϴµ� �ٿ��� Ŭ���̶���

    /*��ǻ���� ���κ��忡�� �ϳ��� ���ػ� Ÿ�̸Ӱ� �����մϴ�.�� Ÿ�̸Ӵ� ��Ȯ�ϰ�
    ������ ���ļ�
    (��, 1�ʿ� �� Ÿ�̸��� ���ɿ� �´� ������)�� ���� ������ �����ϰ��� �ϴ� �ڵ����
    ���۰� ������
    CPU Ŭ������ ��� �� ���� ����ð��� ���� �� �ֽ��ϴ�.*/

    //QueryPerformanceFrequency���ػ� Ÿ�̸��� ���ļ�(1�ʴ� ������)�� �Ű������� ��� �Լ�
    //�������������� 0�� ��ȯ��
    //QueryPerformanceCounter:���ػ� Ÿ�̸��� ���� CPU�� Ŭ������ �Ű������� ��� �Լ�
    //�������������� 0�� ��ȯ��

    //���� Ÿ�̸� �������θ� üũ�Ѵ�.
    //���� Ÿ�̸Ӹ� �����ϸ� �ʴ� ����ũ�μ�������� ���� ����.
    //(1/1000000)
    //�ð� ����
    __int64 _periodFrequency;
    

    //�����Ǵ� ��ǻ�� ���
    if (QueryPerformanceFrequency(reinterpret_cast<LARGE_INTEGER*>(&_periodFrequency)))
    //if(QueryPerformanceFrequency((LARGE_INTEGER*)&_periodFrequency))
    {
        //64��Ʈ ������ ����� �� �ֵ��� ������ ������
        //����ũ�μ����带 (1/1000000)�� ���� �����Ͽ� ���� ��Ȯ�� �ð����� �����ϱ� ����
        _isHardware = true;
        //�ʱ�ȭ �ɶ� �������ð���(Ŭ������) ���� �ʱ�ȭ ����
        QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER*>(&_lastTime));

        //���� ������ ��� �Ǵ��� �ʴ� �������� ������ �ʱ�ȭ
        _timeScale = 1.0f / _periodFrequency;
    }
    //�������� �ʴ� ��ǻ�Ͷ��
    else
    {
        _isHardware = false;

        //timeGetTime:������(�ü��)�� ���۵Ǿ ���ݱ��� �帥 �ð��� 
        //1 / 1000 ��(milliseconds) ������ DWORD���� �����ϴ� �Լ���.

        //�ʱ�ȭ �ɶ� �������ð���(�и����������) ���� �ʱ�ȭ ����
        _lastTime = timeGetTime();
        //�и������� ũ��� ����̶�°� �ʱ�ȭ
        _timeScale = 0.001f;
    }

    //Ÿ�̸Ӱ� �����ɶ� 0������ ����ڴ�.
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
    //���� Ÿ�̸Ӹ� �����Ѵٸ�
    if (_isHardware)
    {
        //����ð���(Ŭ������) ���� �ʱ�ȭ ����
        QueryPerformanceCounter((reinterpret_cast<LARGE_INTEGER*>(&_curTime)));
    }
    else
    {
        //�������� ������ time�Լ��� �̿� �и�������(1/1000)
        _curTime = timeGetTime();
    }

    //�������ð��� ����ð��� �������Ӵ� �����(�ʴ���)
    _timeElapsed = (_curTime - _lastTime) * _timeScale;


    //��� 1������ �̻� ������ ���ӿ� ���Ͽ�
    if (lock > 0.0f)
    {
        //�̰��� �ݺ��������� ����ð��� ������ �̰����� �ݺ����� ���ٰ� ������ ���߸� ������Ʈ�� ������ ���Ե�
        //���� �������� �ð��� �����Ҷ����� �������Ѷ�
        //       0.1        < 0.1
        while (_timeElapsed < (1.0f / lock))
        {
            if (_isHardware) QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER*>(&_curTime));
            else _curTime = timeGetTime();

            //������ �ð��� ���� �ð��� �����
            _timeElapsed = (_curTime - _lastTime) * _timeScale;
        }
    }

    _lastTime = _curTime;				//������ �ð��� ���
    _FPSFrameCount++;					//�ڵ����� ������ ī��Ʈ ����
    _FPStimeElapsed += _timeElapsed;	//�ʴ� ������ �ð� ����� ����
    _worldTime += _timeElapsed;			//��ü �ð� ����� ����

    //������ �ʱ�ȭ�� 1�ʸ��� ����
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
    ////����� �ؽ�Ʈ �ڽ� (l,t,r,b)
    RECT rect = { 0,0,800,300 };
    RECT rect2 = { 0,20,800,300 };
    RECT rect3 = { 0,40,800,300 };
    Color brush = { 1.0f,0.0f,0.0f,1.0f };//������
    //                                          //�⺻
    WRITEMANAGER->RenderText(str, rect,30.0f,L"����ü",brush);
    WRITEMANAGER->RenderText(str2, rect2);
    WRITEMANAGER->RenderText(str3, rect3);
}
