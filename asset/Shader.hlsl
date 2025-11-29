
struct VSInput
{
    float3 position : POSITION;
    float4 color : COLOR;
};

struct VSOutput
{
    float4 position : SV_POSITION;
    float4 color : COLOR;
};

struct PSInput
{
    float4 position : SV_POSITION;
    float4 color : COLOR;
};

VSOutput vs(VSInput input)
{
    VSOutput output;
    
    output.position = float4(input.position, 1.0f);
    output.color = input.color;
    
    return output;
}

float4 ps(PSInput input) : SV_TARGET
{
    return input.color;
}