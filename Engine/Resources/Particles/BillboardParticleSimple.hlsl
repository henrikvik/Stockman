cbuffer Camera : register(b0)
{
	float4x4 View;
	float4x4 Proj;
};

struct VSIn {
	float3 pos : POSITION;
	float2 scale : SIZE;
	float age : AGE;

	int idx : IDX;
};

struct GSOut {
	float4 pos : SV_POSITION;
	float2 uv : TEXCOORD;
	float age : AGE;
};

VSIn VS(VSIn input)
{
	return input;
}

static const float4 UV = float4(0, 0, 1, 1);

[maxvertexcount(4)]
void GS(point VSIn inp[1], inout TriangleStream<GSOut> outstream)
{
	VSIn input = inp[0];

	GSOut output;
	output.age = input.age;

	float w = input.scale.x;
	float h = input.scale.y;
	/*float rot = input.rotation;

	float4x4 rotate = {
		cos(rot), -sin(rot), 0.0, 0.0,
		sin(rot), cos(rot),  0.0, 0.0,
		0.0,        0.0,     1.0, 0.0,
		0.0,        0.0,     0.0, 1.0
	};

	float4 N = mul(rotate, float4(0, w, 0, 0));
	float4 S = mul(rotate, float4(0, -w, 0, 0));
	float4 E = mul(rotate, float4(-h, 0, 0, 0));
	float4 W = mul(rotate, float4(h, 0, 0, 0));*/

	float4 N = float4(0, w, 0, 0);
	float4 S = float4(0, -w, 0, 0);
	float4 E = float4(-h, 0, 0, 0);
	float4 W = float4(h, 0, 0, 0);

	float4 pos = mul(View, float4(input.pos.xyz, 1.0));

	output.pos = mul(Proj, pos + N + W);
	output.uv = UV.xy;
	outstream.Append(output);

	output.pos = mul(Proj, pos + S + W);
	output.uv = UV.xw;
	outstream.Append(output);

	output.pos = mul(Proj, pos + N + E);
	output.uv = UV.zy;
	outstream.Append(output);

	output.pos = mul(Proj, pos + S + E);
	output.uv = UV.zw;
	outstream.Append(output);

}

Texture2D Plane : register(t0);
Texture2D Noise : register(t1);
Texture2D Mask : register(t2);

SamplerState Sampler : register(s1);
SamplerState SamplerClamp : register(s0);


float4 PS(GSOut input) : SV_Target0
{
	float4 mask = Mask.Sample(Sampler, input.uv);
	float t = input.age * 0.05;

	float4 noise1 = Noise.Sample(Sampler, 0.25*input.uv+t*float2(0, 0.5));
	float4 noise2 = Noise.Sample(Sampler, 0.15*input.uv+t*float2(0, 1));

	float distort = pow(noise1.r + noise2.g, 4);
	float falloff = input.uv.y;
	
	float bsize = 3.f;

	float N = ((mask.b*bsize) + ((3.9*distort * mask.r) * (mask.g * falloff))) * falloff;
	clip(saturate(step(0.5 + input.uv.y, N)) - 0.5);

	float innerFactor = step(1.9, N * pow(mask.g * falloff, 1));
	float outerFactor = 1 - innerFactor;

	float3 inner = float3(1.000, 0.894, 0.710);
	float3 innerEnd = float3(0.902, 0.902, 0.980);


	float3 outer = float3(1.000, 0.549, 0.000);
	float3 outerEnd = float3(1.000, 0.647, 0.000);

	float3 emissive = ((innerFactor * lerp(inner, innerEnd, falloff - 0.49)) + (outerFactor * lerp(outerEnd, outer, falloff-0.7)));


	return float4(emissive, 1);
}

void PS_depth(GSOut input)
{
    float4 mask = Mask.Sample(Sampler, input.uv);
    float t = input.age * 0.05;

    float4 noise1 = Noise.Sample(Sampler, 0.25*input.uv + t*float2(0, 0.5));
    float4 noise2 = Noise.Sample(Sampler, 0.15*input.uv + t*float2(0, 1));

    float distort = pow(noise1.r + noise2.g, 4);
    float falloff = input.uv.y;

    float bsize = 3.f;

    float N = ((mask.b*bsize) + ((3.9*distort * mask.r) * (mask.g * falloff))) * falloff;
    clip(saturate(step(0.5 + input.uv.y, N)) - 0.5);
}