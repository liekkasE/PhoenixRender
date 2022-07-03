

struct VertexIn
{
	float3 VertexPos    : POSITION;
    float4 Color        : COLOR;
    uint instance_id : SV_InstanceID;
};


struct PSIn
{
	float4 Pos : SV_POSITION;
	float4 Color : COLOR;
};


cbuffer cbPreView : register(b0)
{
    float3 viewPosition;
	float pad;
	float4 ViewOri;
	row_major float4x4 ViewMat;
    row_major float4x4 ViewProj;
}

struct mseh_instance
{
    row_major float4x4 mat;
};

StructuredBuffer<mseh_instance> sb_instances : register(t0);

PSIn VS_Main(VertexIn vs_in)
{
	PSIn vs_out;

    float4x4 instance_mat = sb_instances[vs_in.instance_id].mat;
    float3 world_vertex_pos = mul(instance_mat, float4(vs_in.VertexPos.xyz, 1.0f));
    float4 mvp_res = mul(ViewProj, float4(world_vertex_pos, 1.0));
    vs_out.Pos = float4(mvp_res.xyz / mvp_res.z, 1.0f);
    vs_out.Color = vs_in.Color;
    
	return vs_out;
}


float4 PS_Main(PSIn ps_in) : SV_TARGET
{
    return ps_in.Color;
}


