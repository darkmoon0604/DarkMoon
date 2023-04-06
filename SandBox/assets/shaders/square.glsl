// square shader

#type vertex
#version 330 core
			
layout(location = 0) in vec3 aPos;

uniform mat4 uViewProjection;		
uniform mat4 uTransform;	

out vec3 vPos;

void main()
{
    vPos = aPos;
    gl_Position = uViewProjection * uTransform * vec4(aPos, 1.0);
}

//--------------------------------------------------------------------//

#type fragment
#version 330 core

layout(location = 0) out vec4 color;

in vec3 vPos;

uniform vec3 uColor;

void main()
{
    color = vec4(uColor, 1.0);
}
