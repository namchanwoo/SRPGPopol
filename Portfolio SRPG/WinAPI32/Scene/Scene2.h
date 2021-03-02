#pragma once

//장면
class Scene2 :public Scene
{
private:
    //Fade
    Color               Fade;
    ID3D11Buffer*		FadeBuffer;   
  


public:
    Scene2();
    ~Scene2();
    HRESULT init()override;		//재정의된 함수
    void release()override;		
    void update()override;		
    void render()override;
 
};

