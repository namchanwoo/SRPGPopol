
//상수버퍼
//구조체 와 비슷

//상수 -> 변하지않는 수
//레지스터 값을 저장하는 아주 빠른 기억 장소
//일반상수버퍼 b 텍스쳐 t 샘플러 s
//16바이트 단위로만 크기를 만들수있다.
cbuffer VS_W : register(b0) //register(b0) 이게 상수 b0위치
{
    //열우선 행렬
    //멤버 값이 상수가 아니라
    //4x4 -> 16개의 8바이트
    row_major matrix World;
}
cbuffer VS_V : register(b1)
{
    row_major matrix View;
}
cbuffer VS_P : register(b2)
{
    row_major matrix Proj;
}

//픽셀쉐이더에서 쓸 상수버퍼 컬러
cbuffer PS_Color : register(b0)
{
    //4 개의 8바이트
    float4 Color;
}
cbuffer PS_Fade : register(b1)
{
    float4 Fade;
}


//POSITION0 -> 시멘틱 [n] n 인덱스
//gpu는 연산자체는 빠르지만 
//변수나 반환형의 용도에대해 각각 서술이 필요하다
struct VertexInput
{
    float4 Position : POSITION0;
    //float4 Position2 : POSITION1;
    float4 Color : Color0;
};

struct PixelInput
{
    float4 Position : SV_POSITION; //화면좌표계 포지션
    float4 Color : Color0;
};

//버텍스 쉐이더
//반환형  함수이름(매개변수)

// VertexInput(in) ->  VS  -> PixelInput (out)
PixelInput VS(VertexInput input)
{
    PixelInput output;
    //로컬->월드 mul곱하기
    output.Position = mul(input.Position, World);
    
    //월드->뷰
    output.Position = mul(output.Position, View);
    
    //뷰->프로젝션
    output.Position = mul(output.Position, Proj);
    
    //L -> W -> V -> P 공간변환
    
    //정점 6개만 색을 넘겨줌
    output.Color = input.Color;

    return output; //리턴되는 단계까지는 정점데이터다.
}

//RS
//->SV_POSITION 은 화면좌표계로 전환





//픽셀 쉐이더
float4 PS(PixelInput input) : SV_TARGET //SV_TARGET 은 타겟이될 색깔 
{
    //그럼 중간에 생겨난 픽셀은 어떻게 색이 정해지는가?
    
    //정점 끝에서 끝 사잇값을 선형보간
    
    //픽셀수만큼 늘어남
    
    
    //레스터라이저를 거친 스크린 좌표
    //픽셀의 좌표가 300이상이면
    //if (input.Position.y > 500)
    //{
    //    return float4(1, 0, 0, 1);
    //}
    //else if (input.Position.y > 400)
    //{
    //    return float4(1, 1, 0, 1);
    //}
    //else if (input.Position.y > 300)
    //{
    //    return float4(1, 0, 1, 1);
    //}
    
    return Color * Fade; //r g b a
}