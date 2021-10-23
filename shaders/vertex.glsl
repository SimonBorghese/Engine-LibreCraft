#version 330 core

layout (location=0) in vec3 aPos;
layout (location=1) in vec2 aTex;
layout (location=2) in float aTextNum;

out vec2 textCord;
flat out float textNum;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

void main(){
    gl_Position = projection * view * model * vec4(aPos, 1.0f);
    
    //gl_Position = vec4(aPos, 1.0f);

    textCord = vec2(aTex.x, 1.0 - aTex.y);
    textNum = aTextNum;
}
