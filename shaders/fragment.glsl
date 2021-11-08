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
  /*
  if (textNum == 0.0f){
    FragColor = texture(text0, textCord);
  }
  else if (textNum == 1.0f){
    FragColor = texture(text1, textCord);
  }
  else if (textNum == 2.0f){
    FragColor = texture(text2, textCord);
  }
  else if (textNum == 3.0f){
    FragColor = texture(text3, textCord);
  }
  else if (textNum == 4.0f){
    FragColor = texture(text4, textCord);
  }
  else if (textNum == 5.0f){
    FragColor = texture(text5, textCord);
  }
  else{
    FragColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);
  }
  */
  
  //FragColor = texture(text0, textCord);
}
else{
  FragColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);
}
    //FragColor = vec4(textCord.x, textCord.y, 0.8f, 1.0f);
}
