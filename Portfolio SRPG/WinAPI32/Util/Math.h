#pragma once
#include "stdafx.h"
template <typename T>
static void Swap(T& a,T& b)
{
    T temp = a;
    a = b;
    b = temp;
}

template <typename T>
static void Shuffle(T* Array,UINT size, UINT count)
{
    for(UINT i =0; i< count; i++)
    {
        UINT sour = rand() % size ;
        UINT dest = rand() % size ;

        Swap(Array[sour],Array[dest]);
    }
}


//                     100     500
static int RndInt(int from , int to)
{
    return rand() % (to - from + 1) + from;
}
//                           0.3~   2.5
static float RndFloat(float from = 0.0f, float to = 1.0f)
{
    //0.0  ~  1.0 �Ǽ��� ����

    // 0 ~ 300 ����
    // 0/300 ~ 300/300  


    //      0 ~ 1(���̰��� �Ǽ���) 
    //      * 2.2
    //      0 ~ 2.2
    //      +   0.3
    //      0.3 ~ 2.5
    return (float)rand() / (float)RAND_MAX * (to - from) + from;
}

//saturate                 -0.1
static float Saturate(const float in, const float min = 0.0f, const float max = 1.0f)
{
	// 0�̸��� 0 
	// 1�ʰ��� 1
	if (in < min)
	{
		return min;
	}
	else if (in > max)
	{
		return max;
	}
	return in;
}

//saturate                 -0.1
static long Saturate(const long in, const long min = 0.0f, const long max = 1.0f)
{
	// 0�̸��� 0 
	// 1�ʰ��� 1
	if (in < min)
	{
		return min;
	}
	else if (in > max)
	{
		return max;
	}
	return in;
}


static float GetRadian(Vector2 dir)
{
    D3DXVec2Normalize(&dir,&dir);

    //180���� ������
    if(dir.y <0)
    {
        return 2.0f * PI -acosf(dir.x); //������� ��Ÿ���ȴ�.
    }
    else
    {
        //��ũ �ڻ��� (�� �ﰢ�Լ�)
        return  acosf(dir.x); //������� ��Ÿ���ȴ�.
    }
}

static Vector2 Linear(Vector2 from ,Vector2 to,float g)
{
    return from + (to - from) * g;
}

static Vector2 ScreenToWorld(Vector2 src)
{
    src.y = WINSIZEY - src.y;
    return src;
}

static Vector2 WorldToScreen(Vector2 src)
{
    src.y = WINSIZEY - src.y;
    return src;
}