/*
 *	Author: fkaa
 *
 *  TL;DR: Compute shader used to generate all the frustums for each "bin" used
 *         in the Forward+ light culling/sorting pass. Only executed once at
 *         start.
 *
 *
 *  TODO:
 *    - move common forward+ structs and definitions into shared file
 */


#define BLOCK_SIZE 16
#define WIDTH 1280
#define HEIGHT 720

cbuffer Camera : register(b0)
{
	float4x4 ViewProjection;
	float4x4 InvProjection;
}

cbuffer DispatchParams : register(b1)
{
	// Number of groups dispatched. (This parameter is not available as an HLSL system value!)
	uint4   numThreadGroups;

	// Total number of threads dispatched. (Also not available as an HLSL system value!)
	// Note: This value may be less than the actual number of threads executed 
	// if the screen size is not evenly divisible by the block size.
	uint4   numThreads;
}

struct Plane {
	// Plane normal
	float3 N;
	// Distance from origin
	float d;
};

// A frustrum defined by 4 planes
struct Frustum {
	Plane planes[4];
};

// Compute a plane from 3 points
Plane ComputePlane(float3 p0, float3 p1, float3 p2)
{
	Plane plane;

	float3 v0 = p1 - p0;
	float3 v1 = p2 - p0;
	
	plane.N = normalize(cross(v0, v1));
	plane.d = dot(plane.N, p0);
	
	return plane;
}

// Convert from clip-space to view-space
float4 ClipToView(float4 clip)
{
	// View space position
	float4 view = mul(InvProjection, clip);
	// Perspective projection
	view = view / view.w;

	return view;
}

// Convert from screen-space to view-space
float4 ScreenToView(float4 screen)
{
	// Convert to normalized texture coordinates
	float2 texCoord = screen.xy / float2(WIDTH, HEIGHT);

	// Convert to clip space
	float4 clip = float4(float2(texCoord.x, 1.0f - texCoord.y) * 2.0f - 1.0f, screen.z, screen.w);

	return ClipToView(clip);
}

struct CSInput {
	// 3D index of the thread group in the dispatch
	uint3 groupID           : SV_GroupID;
	// 3D index of local thread ID in a thread group
	uint3 groupThreadID     : SV_GroupThreadID;
	// 3D index of global thread ID in the dispatch
	uint3 dispatchThreadID  : SV_DispatchThreadID;
	// Flattened local index of the thread within a thread group
	uint  groupIndex        : SV_GroupIndex;
};

// Output for each grid cell's frustrum
RWStructuredBuffer<Frustum> FrustrumOutput : register(u0);

[numthreads(BLOCK_SIZE, BLOCK_SIZE, 1)]
void CS(CSInput input)
{
	float3 eye = float3(0, 0, 0);

	// 4 corner points for each frustrum's far plane in screenspace
	float4 corners[4];

	// top left
	corners[0] = float4(input.dispatchThreadID.xy * BLOCK_SIZE, -1.0f, 1.0f);
	// top right
	corners[1] = float4(float2(input.dispatchThreadID.x + 1, input.dispatchThreadID.y) * BLOCK_SIZE, -1.0f, 1.0f);
	// bottom left
	corners[2] = float4(float2(input.dispatchThreadID.x, input.dispatchThreadID.y + 1) * BLOCK_SIZE, -1.0f, 1.0f);
	// bottom right
	corners[3] = float4(float2(input.dispatchThreadID.x + 1, input.dispatchThreadID.y + 1) * BLOCK_SIZE, -1.0f, 1.0f);

	float3 viewSpace[4];
	// now convert the screen space points to view space
	for (int i = 0; i < 4; i++)
	{
		viewSpace[i] = ScreenToView(corners[i]).xyz;
	}

	// build frustrum
	Frustum frustum;

	// left plane
	frustum.planes[0] = ComputePlane(eye, viewSpace[2], viewSpace[0]);
	// right plane
	frustum.planes[1] = ComputePlane(eye, viewSpace[1], viewSpace[3]);
	// top plane
	frustum.planes[2] = ComputePlane(eye, viewSpace[0], viewSpace[1]);
	// bottom plane
	frustum.planes[3] = ComputePlane(eye, viewSpace[3], viewSpace[2]);

	// Store the computed frustum in global memory (if our thread ID is in bounds of the grid).
	if (input.dispatchThreadID.x < numThreads.x && input.dispatchThreadID.y < numThreads.y)
	{
		uint index = input.dispatchThreadID.x + (input.dispatchThreadID.y * numThreads.x);
		FrustrumOutput[index] = frustum;
	}
}