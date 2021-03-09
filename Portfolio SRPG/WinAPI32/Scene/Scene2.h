#pragma once



class UIButton
{

public:
	
	int numIdx;										//참조할 이미지 인덱스
	POINT imgIdx;									//이미지 인덱스
	Vector2 Pos;									//이미지가 배치될 위치
	Vector2 ScreenPos;									//메인캠이 변경되도 유지될 위치
	Vector2 Scale;									//이미지의 크기
	COL_RECT col;									//충돌체
	bool IsActive = true;							//활성화 상태

	UIButton* parent;								//부모 UI버튼

	static int btsCount;

	UIButton() {
		UIButton::btsCount++;
	}

	//버튼의 이미지와 인덱스를 정해준다.
	void ButtonImageInit(int numIdx_, POINT imgIdx_);
	//버튼의 업데이트
	void Update();


};


//장면
class Scene2 :public Scene
{

private:

	float uiMovespeed = 5.0f;			//이미지가 움직이는 속도
	bool isUi = false;					//마우스 포지션이 메뉴안에 있으면

	vector<Image*>	 uiImg;				//ui 이미지들
	vector<UIButton*> bts;				//ui 버튼들


	TileMapEdit* edit;					//접근하여 바꿔줄 타일맵
	Image*   bg;						//타일맵으로 쓸 배경이미지

	int width;							//맵 가로
	int height;							//맵 세로
	float alphaColor;

	Vector2 TileSize;					//x타일 사이즈
	

	Vector2 AlphaUiPos;					//알파 슬라이더 스크린위치


	
	queue<HexTile*> curTiles;			//선택된 핵사타일들을 담을 보관

	//Fade
	Color Fade;
	ID3D11Buffer*		FadeBuffer;



	void AddImage(_tstring file, UINT MaxFrameX, UINT MaxFrameY, string vs = "VS", string ps = "PS");

	//타일 갯수 조절
	void TileIndexResize();
	//타일 위치 조절
	void TileInitPosition();
	//타일 사이즈 조절
	void TileScaleResize();

	//메인캠이 움직여도 고정될 위치
	Vector2 SettingPos(Vector2 ScreenPos);

	//마우스가 ui에 있는지
	bool PtInUi();

	//버튼 다운
	void ButtonIsKeyDown();
	//버튼 길게 다운
	void ButtonStayKeyDown();


	

public:

	Scene2();
	~Scene2();

	HRESULT init()override;		//재정의된 함수
	void release()override;
	void update()override;
	void render()override;

	

	

};

