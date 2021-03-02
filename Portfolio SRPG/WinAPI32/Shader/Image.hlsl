
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
//���ؽ����̴� �� uv
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
    float4 Position : SV_POSITION; //ȭ����ǥ�� ������
    float2 Uv : UV0;
};

PixelInput VS(VertexInput input)
{
    PixelInput output;
    //����->���� mul���ϱ�
    output.Position = mul(input.Position, World);
    
    //����->��
    output.Position = mul(output.Position, View);
    
    //��->��������
    output.Position = mul(output.Position, Proj);
    
    output.Uv = input.Uv;
    
    //Uv��ǥ�� �ּҰ� �ִ밪�� �ٲ��ش�.
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
//�̹��� �ȼ��� ��� ��������� ��ǥ�� ��� ������ ����
SamplerState Sampler : register(s0);
//�ؽ��� �ڿ� �̹���
Texture2D Texture : register(t0);

//�ȼ� ���̴�
float4 PS(PixelInput input) : SV_TARGET //SV_TARGET �� Ÿ���̵� ���� 
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
    
    //float2 A = { 400, 300 };// ������ �߾Ӱ�
    //float2 D = input.Position.xy - A;
    ////�Ѱ��� �Ÿ���
    //float dis = sqrt(D.x * D.x + D.y * D.y);
    //if (dis<100)
    //{
    //   // return TextureColor * Color;
    //}
    //else
    //{
    //    return float4(0, 0, 0, 1);
    //}
    
    
    //�� ȥ�� ��ο����� ȥ��
    return TextureColor * Color * Fade; //r g b a
}

float4 PS2(PixelInput input) : SV_TARGET //SV_TARGET �� Ÿ���̵� ���� 
{
    //Color;
    float4 TextureColor =
    Texture.Sample(Sampler, input.Uv);
    //������� ȥ�չ��
    float4 re;
    //          ���
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