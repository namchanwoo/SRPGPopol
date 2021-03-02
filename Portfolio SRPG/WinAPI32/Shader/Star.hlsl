//hlsl 은 쉐이더 언어

//쉐이더는 그래픽카드의 성능에 영향을 받는다(특히 픽셀쉐이더쪽이)

//쉐이더는 연산을 gpu가 하고
//메모리를 그래픽카드 메모리에 할당

//gpu를 통해 연산을 하는이유는
//cpu보다 빠르기 때문에

//??? 왜 모델명만큼이나 메모리를 강조하는가
//픽셀셰이더가 메모리를 엄청 먹기때문에



//쉐이더 라는건 함수다.


//hlsl
//시멘틱
//상수버퍼



//상수버퍼
cbuffer VS_W : register(b0)
{
    //열우선 행렬
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
cbuffer PS_Fade : register(b1)
{
    float4 Fade;
}

struct VertexInput
{
    float4 Position : POSITION0;
    //float4 Color : Color0;
};

struct PixelInput
{
    float4 Position : SV_POSITION;
};


//픽셀쉐이더에서 쓸 상수버퍼 컬러
cbuffer PS_Color : register(b0)
{
    float4 Color;
}



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

    return output; //리턴되는 단계까지는 정점데이터다.
}
//픽셀 쉐이더
float4 PS(PixelInput input) : SV_TARGET
{
    return Color * Fade; //r g b a
}