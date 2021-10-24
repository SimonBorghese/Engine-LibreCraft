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