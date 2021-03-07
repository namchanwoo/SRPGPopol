#pragma once

typedef void(EditManager::*ept)();



class UIButton
{

private:
	TileMapEdit* edit;

public:
	
	int numIdx;										//참조할 이미지 인덱스
	POINT imgIdx;									//이미지 인덱스
	Vector2 Pos;									//이미지가 배치될 위치
	Vector2 Pos2;									//메인캠이 변경되도 유지될 위치
	Vector2 Scale;									//이미지의 크기
	COL_RECT col;									//충돌체
	bool IsActive = true;							//활성화 상태
	ept btfunc;										//멤버 함수 포인터

	UIButton* parent;								//부모 UI버튼
	
	//버튼의 기능을 넣어준다.
	void ButtonInit(TileMapEdit* edit_,ept otherFunc);
	//버튼의 이미지와 인덱스를 정해준다.
	void ButtonImageInit(int numIdx_, POINT imgIdx_);
	//메인캠의 위치가 바뀌어도 고정된 위치에 설정해준다.
	void PosSettling();
	//버튼의 업데이트
	void Update();
	//버튼을 눌렀을 때 함수가 실행된다.
	void ButtonDown();
};


//장면
class Scene2 :public Scene
{
private:

	float uiMovespeed = 5.0f;			//이미지가 움직이는 속도
	bool isUi = false;					//마우스 포지션이 메뉴안에 있으면

	vector<Image*>	 uiImg;				//ui 이미지들
	vector<UIButton*> bts;			//ui 버튼들


	TileMapEdit* edit;					//접근하여 바꿔줄 타일맵
	Image*   bg;						//타일맵으로 쓸 배경이미지

	//Fade
	Color Fade;
	ID3D11Buffer*		FadeBuffer;



	bool PtInUi();
	//타일 사이즈 조절
	void MapArrayResize();
	//타일 원점 조절
	void MapOriginMove();
	//타일 사이즈 조절
	void TileResize();


	void AddImage(_tstring file, UINT MaxFrameX, UINT MaxFrameY, string vs = "VS", string ps = "PS");
	

public:

	Scene2();
	~Scene2();
	HRESULT init()override;		//재정의된 함수
	void release()override;
	void update()override;
	void render()override;
	

};

