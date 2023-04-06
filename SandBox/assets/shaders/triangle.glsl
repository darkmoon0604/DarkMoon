// triangle shader

#type vertex
#version 330 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec4 aColor;

uniform mat4 uViewProjection;
uniform mat4 uTransform;

out vec3 vPos;
out vec4 vColor;

void main()
{
    vPos = aPos;
    vColor = aColor;
    gl_Position = uViewProjection * uTransform * vec4(aPos, 1.0);
}

//--------------------------------------------------------------------//

#type fragment
#version 330 core
layout(location = 0) out vec4 fColor;

in vec3 vPos;
in vec4 vColor;

void main()
{
    fColor = vec4(vPos * 0.5 + 0.5, 1.0);
    fColor = vColor;
}
