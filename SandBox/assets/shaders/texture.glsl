// base texture shader

#type vertex
#version 330 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec2 aTexcoord;

uniform mat4 uViewProjection;
uniform mat4 uTransform;

out vec2 vTexCoord;

void main()
{
    vTexCoord = aTexcoord;
    gl_Position = uViewProjection * uTransform * vec4(aPos, 1.0f);
}

//--------------------------------------------------------------------//

#type fragment
#version 330 core

layout(location = 0) out vec4 color;

in vec2 vTexCoord;

uniform sampler2D uTexture;

void main()
{
    color = texture(uTexture, vTexCoord);
}
