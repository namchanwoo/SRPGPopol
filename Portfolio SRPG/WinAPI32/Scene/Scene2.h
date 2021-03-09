#pragma once



class UIButton
{

public:
	
	int numIdx;										//������ �̹��� �ε���
	POINT imgIdx;									//�̹��� �ε���
	Vector2 Pos;									//�̹����� ��ġ�� ��ġ
	Vector2 ScreenPos;									//����ķ�� ����ǵ� ������ ��ġ
	Vector2 Scale;									//�̹����� ũ��
	COL_RECT col;									//�浹ü
	bool IsActive = true;							//Ȱ��ȭ ����

	UIButton* parent;								//�θ� UI��ư

	static int btsCount;

	UIButton() {
		UIButton::btsCount++;
	}

	//��ư�� �̹����� �ε����� �����ش�.
	void ButtonImageInit(int numIdx_, POINT imgIdx_);
	//��ư�� ������Ʈ
	void Update();


};


//���
class Scene2 :public Scene
{

private:

	float uiMovespeed = 5.0f;			//�̹����� �����̴� �ӵ�
	bool isUi = false;					//���콺 �������� �޴��ȿ� ������

	vector<Image*>	 uiImg;				//ui �̹�����
	vector<UIButton*> bts;				//ui ��ư��


	TileMapEdit* edit;					//�����Ͽ� �ٲ��� Ÿ�ϸ�
	Image*   bg;						//Ÿ�ϸ����� �� ����̹���

	int width;							//�� ����
	int height;							//�� ����
	float alphaColor;

	Vector2 TileSize;					//xŸ�� ������
	

	Vector2 AlphaUiPos;					//���� �����̴� ��ũ����ġ


	
	queue<HexTile*> curTiles;			//���õ� �ٻ�Ÿ�ϵ��� ���� ����

	//Fade
	Color Fade;
	ID3D11Buffer*		FadeBuffer;



	void AddImage(_tstring file, UINT MaxFrameX, UINT MaxFrameY, string vs = "VS", string ps = "PS");

	//Ÿ�� ���� ����
	void TileIndexResize();
	//Ÿ�� ��ġ ����
	void TileInitPosition();
	//Ÿ�� ������ ����
	void TileScaleResize();

	//����ķ�� �������� ������ ��ġ
	Vector2 SettingPos(Vector2 ScreenPos);

	//���콺�� ui�� �ִ���
	bool PtInUi();

	//��ư �ٿ�
	void ButtonIsKeyDown();
	//��ư ��� �ٿ�
	void ButtonStayKeyDown();


	

public:

	Scene2();
	~Scene2();

	HRESULT init()override;		//�����ǵ� �Լ�
	void release()override;
	void update()override;
	void render()override;

	

	

};

