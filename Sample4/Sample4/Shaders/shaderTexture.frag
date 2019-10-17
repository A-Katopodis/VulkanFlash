
#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(binding = 1) uniform sampler2D texSampler[3];

layout(location = 0) in vec3 fragColor;
layout(location = 1) in vec2 fragTexCoord;
layout(push_constant) uniform PER_OBJECT
{
	layout(offset=0)int imgIdx;
}pc;


layout(location = 0) out vec4 outColor;

void main() {
    outColor = texture(texSampler[pc.imgIdx], fragTexCoord);
}

