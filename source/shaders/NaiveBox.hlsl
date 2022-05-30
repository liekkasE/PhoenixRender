

struct VertexIn
{
	float3 VertexPos    : POSITION;
    float4 Color        : COLOR;
	float4 instancePosition : WORLD;
};

struct VertexOut
{
	float4 Pos     : SV_POSITION;
    float4 Color   : COLOR;
};

cbuffer cbPreView : register(b0)
{
    float4x4 Viewproj;
}

VertexOut VS_Main(VertexIn vs_in)
{
    VertexOut vs_out;

	vs_out.Pos = mul(Viewproj, float4(vs_in.instancePosition.xyz, 1.0));
    vs_out.Color = vs_in.Color;
    
	return vs_out;
}


float4 PS_Main(VertexOut ps_in) : SV_TARGET
{
    return ps_in.Color;
}