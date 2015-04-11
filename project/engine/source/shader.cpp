#include "shader.hpp"

#include <iostream>
#include <cstdlib>
#include <stdio.h>
#include "debug_macros.h"

const char   Shader::UNIFORM        = 'u';
const char   Shader::ATTRIBUTE      = 'a';
const GLint  Shader::INVALID_HANDLE = -1;


/* Helper functions signatures */
int printGLError();
void printProgramInfoLog(GLuint program);
void printShaderInfoLog(GLuint shader);
char *textFileRead(const char *fn);


Shader::Shader() {
    DEBUG("Empty Shader!");
}

Shader::Shader(std::string _vertexShader, std::string _fragmentShader):
    vertexShader(_vertexShader), fragmentShader(_fragmentShader), loaded(false)
    {}

void Shader::load(){
    INFO("Loading shaders: " << vertexShader <<
            " and " << fragmentShader << "...");

    GLint rc;

    // Create shader handles
    GLuint VS = glCreateShader(GL_VERTEX_SHADER);
    GLuint FS = glCreateShader(GL_FRAGMENT_SHADER);

    // Read shader sources
    const char *vshader = textFileRead(vertexShader.c_str());
    const char *fshader = textFileRead(fragmentShader.c_str());
    glShaderSource(VS, 1, &vshader, NULL);
    glShaderSource(FS, 1, &fshader, NULL);

    // Compile vertex shader
    INFO("Compiling vertex shader: " << vertexShader << "...");
    glCompileShader(VS);

    printGLError();

    glGetShaderiv(VS, GL_COMPILE_STATUS, &rc);
    printShaderInfoLog(VS);
    ASSERT(rc != 0, "Error compiling vertex shader");

    // Compile fragment shader
    INFO("Compiling fragment shader: " << fragmentShader << "...");
    glCompileShader(FS);

    printGLError();

    glGetShaderiv(FS, GL_COMPILE_STATUS, &rc);
    printShaderInfoLog(FS);
    ASSERT(rc != 0, "Error compiling fragment shader");

    // Create the program and link
    INFO("Linking Shaders...")
    programID = glCreateProgram();

    glAttachShader(programID, VS);
    glAttachShader(programID, FS);

    glLinkProgram(programID);

    printGLError();

    glGetProgramiv(programID, GL_LINK_STATUS, &rc);

    printProgramInfoLog(programID);
    ASSERT(rc != 0, "Error linking shaders!");

    loaded = true;
}

GLint Shader::getHandle(std::string handleName){
    if(isLoaded()){
        if (handles.find(handleName) == handles.end()){
            DEBUG("Handle not loaded: " << handleName
                << " -- Vertex Shader:" << vertexShader
                << "  Fragment Shader: " << fragmentShader);
            return Shader::INVALID_HANDLE;
        }

        if(handles[handleName] == Shader::INVALID_HANDLE){
            DEBUG("Handle not loaded: " << handleName
                << " -- Vertex Shader:" << vertexShader
                << "  Fragment Shader: " << fragmentShader);
        }

        return handles[handleName];
    }
    DEBUG("Shader not loaded"
            << " -- Vertex Shader:" << vertexShader
            << "  Fragment Shader: " << fragmentShader);

    return Shader::INVALID_HANDLE;
}

void Shader::loadHandle(std::string handleName, char type){
    INFO("Loading handle " << handleName << "...");

    if (handles.find(handleName) == handles.end()){
        switch (type){
        case Shader::ATTRIBUTE:
            handles[handleName] = glGetAttribLocation(programID,
                                                      handleName.c_str());
            break;
        case Shader::UNIFORM:
            handles[handleName] = glGetUniformLocation(programID,
                                                       handleName.c_str());
            break;
        default:
            DEBUG("Invalid handle type: " << type <<
                  " for handle " << handleName);
            return;
        }
        if (handles[handleName] < 0){
            printGLError();
            DEBUG("Handle " << handleName << " loaded incorectly. "
            << "Handle does not exist or is not optmized "
            << "(i.e. You are not using it on the shader)");
            return;
        }
    }

    INFO("Handle " << handleName << " loaded!");
}

GLuint Shader::getID(){
    return programID;
}

std::string Shader::getVertexShaderFileName(){
    return vertexShader;
}

std::string Shader::getFragmentShaderFileName(){
    return fragmentShader;
}

bool Shader::isLoaded(){
    return loaded;
}


/** Helper Functions **/

//TODO: Those functions should only work on the DEBUG version, they should be
//     removed from the code in the release during compile time

/**
*  Functions modified from: GLSL.cpp
*  Original Author: Zoe Wood
*/

int printGLError()
{
    // Returns 1 if an OpenGL error occurred, 0 otherwise.
    GLenum glErr;
    int retCode = 0;

    glErr = glGetError();
    while(glErr != GL_NO_ERROR) {
        if(glErr != GL_INVALID_ENUM){
            DEBUG("glError in file " << __FILE__ << " @ line " << __LINE__
                << ": " << glewGetErrorString(glErr));
        }
        retCode = 1;
        glErr = glGetError();
    }
    return retCode;
}

void printShaderInfoLog(GLuint shader)
{
    GLint infologLength = 0;
    GLint charsWritten  = 0;
    GLchar *infoLog;

    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infologLength);

    if(infologLength > 0) {
        infoLog = (GLchar *)malloc(infologLength);
        ASSERT(infoLog != NULL, "ERROR: Could not allocate InfoLog buffer");

        glGetShaderInfoLog(shader, infologLength, &charsWritten, infoLog);
        DEBUG("Shader InfoLog: " << infoLog);
        free(infoLog);
    }
    printGLError();
}

void printProgramInfoLog(GLuint program)
{
    GLint infologLength = 0;
    GLint charsWritten  = 0;
    GLchar *infoLog;

    printGLError();
    glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infologLength);
    printGLError();

    if(infologLength > 0) {
        infoLog = (GLchar *)malloc(infologLength);
        ASSERT(infoLog != NULL, "ERROR: Could not allocate InfoLog buffer");

        glGetProgramInfoLog(program, infologLength, &charsWritten, infoLog);
        DEBUG("Program InfoLog: " << infoLog);

        free(infoLog);
    }
    printGLError();
}

char *textFileRead(const char *fn)
{
    FILE *fp;
    char *content = NULL;
    int count = 0;
    if(fn != NULL) {
        fp = fopen(fn,"rt");
        if(fp != NULL) {
            fseek(fp, 0, SEEK_END);
            count = (int)ftell(fp);
            rewind(fp);
            if(count > 0) {
                content = (char *)malloc(sizeof(char) * (count+1));
                count = (int)fread(content,sizeof(char),count,fp);
                content[count] = '\0';
            }
            fclose(fp);
        } else {
            DEBUG("error loading " << fn);
        }
    }
    return content;
}
