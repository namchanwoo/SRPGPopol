#pragma once


//�ϳ��� ���
class MainGame : public Scene
{
private:


public:
	HRESULT init()override;		//�����ǵ� �Լ�
	void release()override;		
	void update()override;		
	void render()override;		

};

