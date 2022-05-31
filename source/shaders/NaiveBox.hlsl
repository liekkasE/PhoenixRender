

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
    float3 viewPosition;
	float pad;
	float4 ViewOri;
	float4x4 ViewMat;
	float4x4 ViewProj;
}

VertexOut VS_Main(VertexIn vs_in)
{
    VertexOut vs_out;

	float3 world_vertex_pos = float3(0.0, 0.0, -1000.0) + vs_in.VertexPos.xyz;
	vs_out.Pos = mul(ViewProj, float4(world_vertex_pos, 1.0));
    vs_out.Color = vs_in.Color;
    
	return vs_out;
}


float4 PS_Main(VertexOut ps_in) : SV_TARGET
{
    return ps_in.Color;
}