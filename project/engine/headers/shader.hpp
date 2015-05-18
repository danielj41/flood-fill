#ifndef SHADER_H
#define SHADER_H

#include <map>
#include <string>

#include "GLIncludes.h"

#include "pointer_macros.h"
DEF_PTR(Shader, ShaderPtr);

class Shader {

public:
    static const char  UNIFORM;
    static const char  ATTRIBUTE;
    static const GLint INVALID_HANDLE;

    Shader();
    Shader(std::string _vertexShader, std::string _fragmentShader);

    void load();

    GLint getHandle(std::string handleName);
    void loadHandle(std::string handleName, char type);

    GLuint getID();
    std::string getVertexShaderFileName();
    std::string getFragmentShaderFileName();
    bool isLoaded();

private:
    std::string vertexShader;
    std::string fragmentShader;

    GLuint programID;
    std::map<std::string, GLint> handles;

    bool loaded;
};

#endif

