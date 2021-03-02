#pragma once

enum AnimState
{
    ANI_STOP,		//정지
    ANI_LOOP,		//재생
    ANI_REVERSE		//역방향재생
};


//해상도는 가로픽셀수 세로픽셀수
//디스플레이에따라 크기가 다 다르다.

//그림파일도 해상도로 되어있다.
//그림파일경우 크기가 다 제각각이므로
//노멀라이즈된 Uv좌표로 정점의 이미지좌표를 잡아준다.

class Image:public Object
{
    //버텍스(정점,꼭지점) 버퍼
    //버퍼 인터페이스 버퍼는 일반적으로 정점 또는 인덱스 데이터를 저장합니다.
    ID3D11Buffer*		VertexBuffer; 
    UINT				vertexCount;
    VertexPT*           Vertex;

   

    //샘플러
    ID3D11SamplerState*			Sampler;
   
    //추가 투명도에따라 앞뒤색을 섞어줄놈
    ID3D11BlendState*       blendState[2];

   
    ID3D11Buffer*		        UVBuffer;

public:
    D3D11_SAMPLER_DESC			SamplerDesc;
    Vector4				        UV;     //변경하고싶은 uv값
                                        //쉐이더에서 접근할 자원뷰
    ID3D11ShaderResourceView*	srv;

    //분할될 갯수
    UINT				MaxFrameX = 1;
    UINT				MaxFrameY = 1;

    //현재 인덱스
    UINT				CurrentFrameX = 0;
    UINT				CurrentFrameY = 0;
    //애니메이션 재생시 필요한 시간변수
    float				time = 0.0f;



    HRESULT init(_tstring file,   string vs = "VS", string ps = "PS");		//생성시 호출
    void release()override;		//소멸시 호출
    void update()override;		//갱신되는값이 있을때 호출
    void render()override;		//그려야 되는값이 있을때 호출
    void CreateSampler();
    void PlayAnimation(AnimState as, bool isFrameX, float nexttime);
};

