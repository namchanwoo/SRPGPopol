#pragma once


//하나의 장면
class MainGame : public Scene
{
private:


public:
	HRESULT init()override;		//재정의된 함수
	void release()override;		
	void update()override;		
	void render()override;		

};

