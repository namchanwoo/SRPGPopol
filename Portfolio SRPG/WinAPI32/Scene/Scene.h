#pragma once
#include "stdafx.h"
//인터페이스
class Scene
{
public:

   virtual    ~Scene(){};
   virtual HRESULT init() = 0;
   virtual void release()=0;	//소멸시 호출
   virtual void update()=0;	    //갱신되는값이 있을때 호출
   virtual void render()=0;	    //그려야 되는값이 있을때 호출

};