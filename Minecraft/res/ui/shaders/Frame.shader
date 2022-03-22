#shader shared
#version 330 core

vec2 RotatePosition(vec2 original, float theta)
{
	return vec2(
		(original.x * cos(theta) - original.y * sin(theta)),
		(original.x * sin(theta) + original.y * cos(theta))
	);
}

vec2 ConvertCoords(vec2 screenSpace, vec2 resolution)
{
	vec2 aRes = resolution - vec2(1.0f, 1.0f);
	vec2 aPos = vec2(screenSpace.x, aRes.y - screenSpace.y);
	return vec2(((aPos.x * 2.0f) - aRes.x) / aRes.x, ((aPos.y * 2.0f) - aRes.y) / aRes.y);
}

#shader single
#version 430 core

// Input
layout(location = 0) in vec4 position;

uniform float u_InstanceData[11];
uniform vec2 u_Resolution;

// Output
flat out vec2 v_Scale;
out vec2 v_FragPosition;
flat out float v_Roundness;
out vec4 v_Color;

// Shared
vec2 RotatePosition(vec2 original, float theta);
vec2 ConvertCoords(vec2 screenSpace, vec2 resolution);

void main()
{
	// Extracting data from instance data
	float zIndex = u_InstanceData[0];
	float rotation = u_InstanceData[1];
	vec2 scale = vec2(u_InstanceData[2], u_InstanceData[3]);
	vec2 offset = vec2(u_InstanceData[4], u_InstanceData[5]);
	v_Color = vec4(u_InstanceData[6], u_InstanceData[7],
		u_InstanceData[8], u_InstanceData[9]);
	v_Roundness = min(min(u_InstanceData[10], scale.x / 2.0f), scale.y / 2.0f);

	// Calculate and set vertex coordinate
	vec2 vPos = RotatePosition(vec2(position.x, position.y) - vec2(0.5f, 0.5f), rotation) + vec2(0.5f, 0.5f);
	vPos = vPos * scale + offset;
	vPos = ConvertCoords(vPos, u_Resolution);
	gl_Position = vec4(vPos.x, vPos.y, zIndex, position.w);

	// Set left over values to be passed into fragment shader
	v_FragPosition = (vec2(position.x, position.y) * scale) - scale / 2.0f;
	v_Scale = scale;
};

#shader instanced
#version 430 core

// Input
layout(location = 0) in vec4 position;

layout(binding = 0) buffer instanceBuffer
{
	float instanceData[];
};

uniform int u_InstanceSize;
uniform vec2 u_Resolution;

// Output
flat out vec2 v_Scale;
out vec2 v_FragPosition;
flat out float v_Roundness;
out vec4 v_Color;

// Shared
vec2 RotatePosition(vec2 original, float theta);
vec2 ConvertCoords(vec2 screenSpace, vec2 resolution);

void main()
{
	int instanceIndex = gl_InstanceID * u_InstanceSize;

	// Extracting data from instance data
	float zIndex = instanceData[instanceIndex];
	float rotation = instanceData[instanceIndex + 1];
	vec2 scale = vec2(instanceData[instanceIndex + 2], instanceData[instanceIndex + 3]);
	vec2 offset = vec2(instanceData[instanceIndex + 4], instanceData[instanceIndex + 5]);
	v_Color = vec4(instanceData[instanceIndex + 6], instanceData[instanceIndex + 7],
		instanceData[instanceIndex + 8], instanceData[instanceIndex + 9]);
	v_Roundness = min(min(instanceData[instanceIndex + 10], scale.x / 2.0f), scale.y / 2.0f);

	// Calculate and set vertex coordinate
	vec2 vPos = RotatePosition(vec2(position.x, position.y) - vec2(0.5f, 0.5f), rotation) + vec2(0.5f, 0.5f);
	vPos = vPos * scale + offset;
	vPos = ConvertCoords(vPos, u_Resolution);
	gl_Position = vec4(vPos.x, vPos.y, zIndex, position.w);

	// Set left over values to be passed into fragment shader
	v_FragPosition = (vec2(position.x, position.y) * scale) - scale / 2.0f;
	v_Scale = scale;
};

#shader fragment
#version 330 core

layout(location = 0) out vec4 fragColor;

flat in vec2 v_Scale;
in vec2 v_FragPosition;
flat in float v_Roundness;
in vec4 v_Color;

// val < 0: -1, val == 0: 1, val > 0: 1
float mSign(float val)
{
	return max(float(val == 0) * val * 2 - val, val) / max(float(val == 0), abs(val));
}

void main()
{
	vec2 posSign = vec2(mSign(v_FragPosition.x), mSign(v_FragPosition.y));

	vec2 closestCorner = (v_Scale / 2.0f) * posSign;
	vec2 cornerCenter = closestCorner - vec2(v_Roundness, v_Roundness) * posSign;

	vec2 absCornerCenter = abs(cornerCenter);
	vec2 absCoord = abs(v_FragPosition);

	float withinCorner = float(max(max(int(distance(absCornerCenter, absCoord) < v_Roundness),
		int(absCoord.x < absCornerCenter.x)),
		int(absCoord.y < absCornerCenter.y)));

	fragColor = vec4(v_Color.x, v_Color.y, v_Color.z, min(v_Color.w, withinCorner));
};
