#pragma once
#include "stdafx.h"
//�������̽�
class Scene
{
public:

   virtual    ~Scene(){};
   virtual HRESULT init() = 0;
   virtual void release()=0;	//�Ҹ�� ȣ��
   virtual void update()=0;	    //���ŵǴ°��� ������ ȣ��
   virtual void render()=0;	    //�׷��� �Ǵ°��� ������ ȣ��

};