#pragma once

//���
class Scene2 :public Scene
{
private:
    //Fade
    Color               Fade;
    ID3D11Buffer*		FadeBuffer;   
  


public:
    Scene2();
    ~Scene2();
    HRESULT init()override;		//�����ǵ� �Լ�
    void release()override;		
    void update()override;		
    void render()override;
 
};

