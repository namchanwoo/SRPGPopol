#pragma once


//���
class Scene2 :public Scene
{
private:
	//�����Ͽ� �ٲ��� Ÿ�ϸ�
	TileMapEdit* edit;


	//Ÿ�ϸ����� �� ����̹���
	Image*   bg1;

	//�����Ϳ��� �� �������

	//���° �̹��� ����?
	int vecIdx;
	//�̹��� ���� �ε���
	int ImgIdxX;
	int ImgIdxY;

	//Ÿ�� ����
	int MaxX;
	int MaxY;

	//���콺 ��ŷ�Ҷ� Ÿ�� ����
	int TileState;

	//Ÿ�� ������
	Vector2 Scale;
	//Ÿ���� �����̵Ǵ� ���� �Ʒ��� ��ġ
	Vector2         LB;

	//Fade
	Color Fade;
	ID3D11Buffer*		FadeBuffer;

	


public:

	Scene2();
	~Scene2();
	HRESULT init()override;		//�����ǵ� �Լ�
	void release()override;
	void update()override;
	void render()override;
	

};

