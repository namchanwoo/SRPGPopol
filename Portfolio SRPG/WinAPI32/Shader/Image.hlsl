
cbuffer VS_W : register(b0) 
{
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
//버텍스쉐이더 쓸 uv
cbuffer VS_UV : register(b3)
{
    float4 Uv;
}

cbuffer PS_Color : register(b0)
{
    float4 Color;
}

cbuffer PS_Fade : register(b1)
{
    float4 Fade;
}

struct VertexInput
{
    float4 Position : POSITION0;
    float2 Uv : UV0;
};

struct PixelInput
{
    float4 Position : SV_POSITION; //화면좌표계 포지션
    float2 Uv : UV0;
};

PixelInput VS(VertexInput input)
{
    PixelInput output;
    //로컬->월드 mul곱하기
    output.Position = mul(input.Position, World);
    
    //월드->뷰
    output.Position = mul(output.Position, View);
    
    //뷰->프로젝션
    output.Position = mul(output.Position, Proj);
    
    output.Uv = input.Uv;
    
    //Uv좌표의 최소값 최대값을 바꿔준다.
    if (output.Uv.x == 0.0f)
    {
        output.Uv.x = Uv.x;
    }
    else if (output.Uv.x == 1.0f)
    {
        output.Uv.x = Uv.y;
    }
    if (output.Uv.y == 0.0f)
    {
        output.Uv.y = Uv.z;
    }
    else if (output.Uv.y == 1.0f)
    {
        output.Uv.y = Uv.w;
    }
    return output; 
}
//이미지 픽셀을 어떻게 색상을잡고 좌표를 어떻게 구할지 서술
SamplerState Sampler : register(s0);
//텍스쳐 자원 이미지
Texture2D Texture : register(t0);

//픽셀 쉐이더
float4 PS(PixelInput input) : SV_TARGET //SV_TARGET 은 타겟이될 색깔 
{
    
    //Color;
    float4 TextureColor = 
    Texture.Sample(Sampler, input.Uv);
    //if(input.Position.y < 300)
    //{
    //    TextureColor.r = 1.0f;
    //}
    if (TextureColor.r == 255 / 255 &&
    TextureColor.g == 0 / 255 &&
        TextureColor.b == 255 / 255)
    {
        TextureColor.a = 0.0f;
    }
    
    //float2 A = { 400, 300 };// 조명의 중앙값
    //float2 D = input.Position.xy - A;
    ////둘간의 거리값
    //float dis = sqrt(D.x * D.x + D.y * D.y);
    //if (dis<100)
    //{
    //   // return TextureColor * Color;
    //}
    //else
    //{
    //    return float4(0, 0, 0, 1);
    //}
    
    
    //색 혼합 어두워지는 혼합
    return TextureColor * Color * Fade; //r g b a
}

float4 PS2(PixelInput input) : SV_TARGET //SV_TARGET 은 타겟이될 색깔 
{
    //Color;
    float4 TextureColor =
    Texture.Sample(Sampler, input.Uv);
    //밝아지는 혼합방법
    float4 re;
    //          흰색
    re = 1.0f - (1.0f - TextureColor)
    * (1.0f - Color);
    
    
    
    re.a = TextureColor.a * Color.a;
    
    if (TextureColor.r == 1.0 &&
    TextureColor.g == 0.0 &&
        TextureColor.b == 1.0)
    {
        re.a = 0.0f;
    }
    return re * Fade;
}