#include "../include/shader.hpp"

shader::shader(const char *vertexFile, const char *fragmentFile)
{
    std::ifstream *shaderV = new std::ifstream(vertexFile, std::ios::binary);
    std::ifstream *shaderF = new std::ifstream(fragmentFile, std::ios::binary);

    std::string *shaderV_str = new std::string;
    shaderV_str->assign( (std::istreambuf_iterator<char>(*shaderV) ), (std::istreambuf_iterator<char>()    ) );
    std::string *shaderF_str = new std::string;
    shaderF_str->assign( (std::istreambuf_iterator<char>(*shaderF) ), (std::istreambuf_iterator<char>()    ) );

    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    mainShader = glCreateProgram();

    const char *trueVSource = shaderV_str->append("\0").c_str();
    const char *trueFSource = shaderF_str->append("\0").c_str();

    glShaderSource(vertexShader, 1, &trueVSource, NULL);
    glShaderSource(fragmentShader, 1, &trueFSource, NULL);


    glCompileShader(vertexShader);
    glCompileShader(fragmentShader);

    int success;
    char infoLog[512];

    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success){
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        printf("Vertex Shader Error: %s\n", infoLog);
    }

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success){
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        printf("Fragment Shader Error: %s\n", infoLog);
    }

    glAttachShader(mainShader, vertexShader);
    glAttachShader(mainShader, fragmentShader);

    glLinkProgram(mainShader);

    glGetProgramiv(mainShader, GL_LINK_STATUS, &success);
    if (!success){
        glGetProgramInfoLog(mainShader, 512, NULL, infoLog);
        printf("Main Error: %s\n", infoLog);
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    delete trueVSource;
    delete trueFSource;
    delete (shaderF);
    delete (shaderV);
}

shader::shader(std::vector<const char*> computeShaders){
  for (int shdr = 0; shdr < computeShaders.size(); shdr++){
    std::ifstream *shaderSrc = new std::ifstream(computeShaders.at(shdr), std::ios::binary);


    std::string *shaderSrc_str = new std::string;
    shaderSrc_str->assign( (std::istreambuf_iterator<char>(*shaderSrc) ), (std::istreambuf_iterator<char>()    ) );

    
    computeShader = glCreateShader(GL_COMPUTE_SHADER);
    mainShader = glCreateProgram();

    const char *trueShaderSource = shaderSrc_str->append("\0").c_str();
  

    glShaderSource(computeShader, 1, &trueShaderSource, NULL);


    glCompileShader(computeShader);

    int success;
    char infoLog[512];

    glGetShaderiv(computeShader, GL_COMPILE_STATUS, &success);
    if (!success){
        glGetShaderInfoLog(computeShader, 512, NULL, infoLog);
        printf("Vertex Shader Error: %s\n", infoLog);
    }


    glAttachShader(mainShader, computeShader);

    glGetProgramiv(mainShader, GL_LINK_STATUS, &success);
    if (!success){
        glGetProgramInfoLog(mainShader, 512, NULL, infoLog);
        printf("Main Error: %s\n", infoLog);
    }

    //glDeleteShader(computeShader);

    delete trueShaderSource;
    delete (shaderSrc);
  }
  glLinkProgram(mainShader);

}

shader::~shader()
{
    glDeleteProgram(mainShader);
}

void shader::useMain(){
    glUseProgram(mainShader);
}

GLint shader::getUniformLocation(const char *name){
  return glGetUniformLocation(mainShader, name);
}
void shader::setMatrix4f(GLint location, const glm::mat4 &mat){
  glUniformMatrix4fv(location, 1, GL_FALSE, &mat[0][0]);
}


void shader::bindTexture(Image *img, const char *unfi_name, int text){
  glActiveTexture(img->getImageNum());
  glBindTexture(GL_TEXTURE_2D, img->getTexture());

  glUniform1i(glGetUniformLocation(mainShader, unfi_name), text);
}

int shader::getInt(GLint location){
  int gotten_int;
  glGetUniformiv(mainShader, location, &gotten_int);
  return gotten_int;
}

void shader::setInt(GLint location, int newInt){
  glUniform1i(location, newInt);
}