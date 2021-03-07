#pragma once

typedef void(EditManager::*ept)();



class UIButton
{

private:
	TileMapEdit* edit;

public:
	
	int numIdx;										//������ �̹��� �ε���
	POINT imgIdx;									//�̹��� �ε���
	Vector2 Pos;									//�̹����� ��ġ�� ��ġ
	Vector2 Pos2;									//����ķ�� ����ǵ� ������ ��ġ
	Vector2 Scale;									//�̹����� ũ��
	COL_RECT col;									//�浹ü
	bool IsActive = true;							//Ȱ��ȭ ����
	ept btfunc;										//��� �Լ� ������

	UIButton* parent;								//�θ� UI��ư
	
	//��ư�� ����� �־��ش�.
	void ButtonInit(TileMapEdit* edit_,ept otherFunc);
	//��ư�� �̹����� �ε����� �����ش�.
	void ButtonImageInit(int numIdx_, POINT imgIdx_);
	//����ķ�� ��ġ�� �ٲ� ������ ��ġ�� �������ش�.
	void PosSettling();
	//��ư�� ������Ʈ
	void Update();
	//��ư�� ������ �� �Լ��� ����ȴ�.
	void ButtonDown();
};


//���
class Scene2 :public Scene
{
private:

	float uiMovespeed = 5.0f;			//�̹����� �����̴� �ӵ�
	bool isUi = false;					//���콺 �������� �޴��ȿ� ������

	vector<Image*>	 uiImg;				//ui �̹�����
	vector<UIButton*> bts;			//ui ��ư��


	TileMapEdit* edit;					//�����Ͽ� �ٲ��� Ÿ�ϸ�
	Image*   bg;						//Ÿ�ϸ����� �� ����̹���

	//Fade
	Color Fade;
	ID3D11Buffer*		FadeBuffer;



	bool PtInUi();
	//Ÿ�� ������ ����
	void MapArrayResize();
	//Ÿ�� ���� ����
	void MapOriginMove();
	//Ÿ�� ������ ����
	void TileResize();


	void AddImage(_tstring file, UINT MaxFrameX, UINT MaxFrameY, string vs = "VS", string ps = "PS");
	

public:

	Scene2();
	~Scene2();
	HRESULT init()override;		//�����ǵ� �Լ�
	void release()override;
	void update()override;
	void render()override;
	

};

