
struct VSInput
{
    float3 position : POSITION;
    float4 color : COLOR;
};

cbuffer ConstantBuffer : register(b0)
{
    matrix view;
    matrix projection;
};
cbuffer ConstantBuffer : register(b1)
{
    matrix world;
    float4 color;
}
struct VSOutput
{
    float4 position : SV_POSITION;
    float4 color : COLOR;
};

/*struct PSInput
{
    float4 position : SV_POSITION;
    float4 color : COLOR;
};*/

VSOutput vs(VSInput input)
{
    VSOutput output;
    
    float4 pos = float4(input.position, 1.0f);
    
    pos = mul(pos, world);
    pos = mul(pos, view);
    pos = mul(pos, projection);
    
    output.position = pos;
    output.color = input.color;
    
    return output;
}

float4 ps(VSOutput input) : SV_TARGET
{
    return input.color* color;
}