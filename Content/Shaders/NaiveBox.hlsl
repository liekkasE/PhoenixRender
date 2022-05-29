
struct VertexIn
{
	float3 Pos    : POSITION;
    float4 Color  : COLOR;
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

cbuffer cbPreObject : register(b1)
{
	float4x4 Worldmat;
}; 

VertexOut VS_Main(VertexIn vs_in)
{
    VertexOut out;

    float4 world_pos = mul(Worldmat, float4(vs_in.pos, 1.0f));

    out.Pos = mul(Viewproj, world_pos);
    out.Color = vs_in.Color;


}



float4 PS_Main(VertexOut ps_in)
{
    return ps_in.Color;
}