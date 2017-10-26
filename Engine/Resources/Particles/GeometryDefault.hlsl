struct PSInput {
	float4 position : SV_POSITION;
	float4 color : COLOR;
	float3 worldPos : POSITION;
	float3 normal : NORMAL;
	float2 uv : TEXCOORD;
	float age : AGE;
	float noisescale : NOISESCALE;
	float noisespeed : NOISESPEED;
	float deform : DEFORM;
};

Texture2D Noise : register(t1);
SamplerState Sampler : register(s1);

static const float3 lightDir = float3(0.2, 0.7, 0.2);

float4 PS(PSInput input) : SV_Target0
{
	float noise = Noise.SampleLevel(Sampler, input.noisescale*input.uv + float2(0, input.noisespeed), 0).g;
	float noise2 = Noise.SampleLevel(Sampler, input.noisescale*input.uv + float2(input.age*input.noisespeed, 0), 0).b;
	float noise3 = Noise.SampleLevel(Sampler, 0.1*input.uv + float2(0, input.age*input.noisespeed), 0).r;

	float emissive = 0.1;
	float ambient = 0.04;
	float diffuse = max(dot(normalize(input.normal * (noise3 * 2 - 1)), lightDir), 0);

	float cap = 1 - input.age;

	clip(
		saturate(
			step(0.13, (noise*cap*0.95))
		) - 0.005
	);

	float t = abs(input.deform*input.uv.y) + 0.04;
	float falloff = (1 - input.age);

	float innerFactor = step(input.age*1.85 - 0.01, (0.55 - input.age*0.15)*t*noise * 3);
	float outerFactor = 1 - innerFactor;

	float3 inner = float3(1, 0.6, 0.3) * 2;
	float3 innerEnd = inner;


	float3 outer = input.color.xyz;
	float3 outerEnd = input.color.xyz;

	float3 e = ((innerFactor * lerp(inner, innerEnd, falloff - 0.49)) + (outerFactor * lerp(outerEnd, outer, falloff - 0.7)));



	float f = emissive + ambient * diffuse;

	float3 col = float3(1, 1, 1);// lerp(float3(0.9, 0.3, 0.1), float3(0.23, 0.15, 0.03), cap);

								 //return float4(col * f, 1);
								 //return float4(input.uv, 0, 1);
	return float4(e * col + f, 1);
	//return float4(input.color.xyz + 0.025 * diffuse, 1);
}

void PS_depth(PSInput input)
{
    float noise = Noise.SampleLevel(Sampler, input.noisescale*input.uv + float2(0, input.noisespeed), 0).g;
    float noise2 = Noise.SampleLevel(Sampler, input.noisescale*input.uv + float2(input.age*input.noisespeed, 0), 0).b;
    float noise3 = Noise.SampleLevel(Sampler, 0.1*input.uv + float2(0, input.age*input.noisespeed), 0).r;

    float emissive = 0.1;
    float ambient = 0.04;
    float diffuse = max(dot(normalize(input.normal * (noise3 * 2 - 1)), lightDir), 0);

    float cap = 1 - input.age;

    clip(
        saturate(
            step(0.13, (noise*cap*0.95))
        ) - 0.005
    );
}