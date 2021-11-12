#version 330 core

in vec2 textCord;
flat in float textNum;

out vec4 FragColor;

uniform sampler2D text0;
uniform sampler2D text1;
uniform sampler2D text2;
uniform sampler2D text3;
uniform sampler2D text4;
uniform sampler2D text5;

uniform int useWire;

flat in int useFog;

void main(){
  if (useWire == 0){
    switch(int(textNum)){
      case 0:
        FragColor = texture(text0, textCord);
      break;
      case 1:
        FragColor = texture(text1, textCord);
      break;
      case 2:
        FragColor = texture(text2, textCord);
      break;
      case 3:
        FragColor = texture(text3, textCord);
      break;
      case 4:
        FragColor = texture(text4, textCord);
      break;
      case 5:
        FragColor = texture(text5, textCord);
      break;

    }
  }
  else{
    FragColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);
  }

  if (useFog == 1){
    FragColor = FragColor + (vec4(0.76f, 0.99f, 1.0f, 0.99f) - FragColor) * 0.5;
  }
}
