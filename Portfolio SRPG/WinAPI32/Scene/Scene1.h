#pragma once
#define MAXPOSITION 5000.0f
#define MAXSCALE 500.0f

//장면
class Scene1 :public Scene
{
private:
	//접근하여 바꿔줄 타일맵
    TileMap* bg;

    //타일맵으로 쓸 배경이미지
    Image*   bg2;

	//에디터에서 쓸 멤버변수

	//몇번째 이미지 인지?
    int vecIdx;
	//이미지 분할 인덱스
    int ImgIdxX;
    int ImgIdxY;

    //타일 갯수
    int MaxX;
    int MaxY;

    //마우스 피킹할때 타일 상태
    int TileState;

	//타일 스케일
	Vector2 Scale;
	//타일의 기준이되는 왼쪽 아래점 위치
	Vector2         LB;

    //Fade
    Color Fade;
    ID3D11Buffer*		FadeBuffer;    

	//타일맵 에디터 버튼 함수

	//타일맵 이미지 버튼
	void TileImgButton();
	//벡터 이미지 인덱스 버튼
	void VecIdxButton();
	//리사이즈 버튼
	void ResizeButton();

	//세이브 로드 버튼
	void TileMapSaveLoad();
	
   
public:

    Scene1();
    ~Scene1();
    HRESULT init()override;		//재정의된 함수
    void release()override;		
    void update()override;		
    void render()override;		
    //정적 멤버 변수
    static int LoadCount;
    
};

