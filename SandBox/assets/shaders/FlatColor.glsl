// flat color shader

#type vertex
#version 330 core

layout(location = 0) in vec3 aPos;

uniform mat4 uViewProjection;
uniform mat4 uTransform;

void main()
{
    gl_Position = uViewProjection * uTransform * vec4(aPos, 1.0);
}

//--------------------------------------------------------------------//

#type fragment
#version 330 core
layout(location = 0) out vec4 fColor;

uniform vec4 uColor;

void main()
{
    //fColor = vec4(vPos * 0.5 + 0.5, 1.0);
    fColor = uColor;
}
