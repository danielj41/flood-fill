#include <cstdlib>
#include <iostream>

#include "GLIncludes.h"
#include "debug_macros.h"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_PURE
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "load_manager.hpp"
#include "director.hpp"
#include "global_variables.hpp"
//#include "main_level.hpp"
#include "demo_level.hpp"
#include "time_manager.hpp"
#include "render_engine.hpp"
#include "material_manager.hpp"
#include "material.hpp"
#include "mesh.hpp"
#include "image.hpp"
#include "texture.hpp"

using namespace std;

//TODO: Encapsulate ALL opengl functions in a library with error checking

void setupGLFW();
void setupGLEW();
void loadContent();
void createScenes();

int main()
{
    setupGLFW();
    setupGLEW();

    RenderEngine::setup();

    loadContent();
    createScenes();

    /* OpenGL 3.3 Vertex Array Object
    // Each mesh should have one vao to improve performance
    // Responsible to hold all vertex data
    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    */


    TimeManager::setTimeStamp();
    TimeManager::setDeltaTime();

    int FPS = 0;
    double timeStamp = TimeManager::getTimeStamp();

    LoadManager::loadSound("rain.wav");
    LoadManager::getSound("rain.wav")->playSound();

    do{
        Director::updateScene();
        Director::renderScene();

        if(TimeManager::getTimeStamp() - timeStamp >= 1.0f){
            DEBUG("FPS: " << FPS);
            DEBUG("Frame Time: " << 1.0f/FPS);
            timeStamp = TimeManager::getTimeStamp();
            FPS = 0;
        }

        TimeManager::setDeltaTime();
        TimeManager::setTimeStamp();
        FPS++;

        // Swap buffers
        glfwSwapBuffers(Global::window);
        glfwPollEvents();
    }
    while (glfwGetKey(Global::window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
    glfwWindowShouldClose(Global::window) == 0);

    /* OpenGL 3.3 VAO
    glDeleteVertexArrays(1, &vao);*/

    // Close OpenGL window and terminate GLFW
    glfwTerminate();
    return 0;
}


void createScenes(){
    INFO("Creating Scenes...");

    DemoLevel * demoLevel = new DemoLevel();
    Director::addScene(demoLevel);
    Director::setScene("DemoLevel");
}

/**
* Load Files
*/

void loadMeshes(){
    INFO("Loading all meshes...");

    Mesh * mesh;

    LoadManager::loadMesh("cube.obj");
    mesh = LoadManager::getMesh("cube.obj");
    mesh->resize();
    mesh->calculateNormals();
    mesh->generateVertexBuffer();
    mesh->generateIndexBuffer();
    mesh->generateNormalBuffer();
    mesh->generateTextureCoordinateBuffer();
    mesh->calculateLimits();
    RenderEngine::addMesh(mesh);

    LoadManager::loadMesh("plane.obj");
    mesh = LoadManager::getMesh("plane.obj");
    mesh->resize();
    mesh->calculateNormals();
    mesh->generateVertexBuffer();
    mesh->generateIndexBuffer();
    mesh->generateNormalBuffer();
    mesh->generateTextureCoordinateBuffer();
    mesh->calculateLimits();


    RenderEngine::addMesh(mesh);

    LoadManager::loadMesh("sphere.obj");
    mesh = LoadManager::getMesh("sphere.obj");
    mesh->resize();
    mesh->calculateNormals();
    mesh->generateVertexBuffer();
    mesh->generateIndexBuffer();
    mesh->generateNormalBuffer();
    mesh->generateTextureCoordinateBuffer();
    mesh->calculateLimits();
    RenderEngine::addMesh(mesh);
}

void loadTextures(){
    INFO("Loading all textures...");

    LoadManager::loadImage("default_voxel.bmp");
    LoadManager::loadTexture("VoxelTexture", LoadManager::getImage("default_voxel.bmp"));
}

void createMaterials(){
    INFO("Creating materials...");

    /*More Materials: http://devernay.free.fr/cours/opengl/materials.html */

    Material * material;
    material = new Material(glm::vec3(0.4, 0.4, 0.4),
                            glm::vec3(0.3, 0.3, 0.4),
                            glm::vec3(0.3, 0.3, 0.4),
                            4.0f);
    MaterialManager::addMaterial("FlatGrey", material);

    Material * material2;
    material2 = new Material(glm::vec3(0.13, 0.13, 0.34),
                            glm::vec3(0.3, 0.3, 0.8),
                            glm::vec3(0.3, 0.3, 0.8),
                            4.0f);
    MaterialManager::addMaterial("FlatBlue", material2);

}

void loadContent(){
    loadMeshes();
    loadTextures();
    createMaterials();
}

/**
* GLFW and GLEW setup
*/

double lastX = Global::ScreenWidth/2.0f, lastY = Global::ScreenHeight/2.0f;
double mouse_sensitivity = 1.0f;
void mouse_movement_callback(GLFWwindow *, double xpos, double ypos){
    if(Director::getScene()->hasMainCamera()){
        Camera * cam = Director::getScene()->getCamera();

        double xoffset = (xpos - lastX)*mouse_sensitivity;
        double yoffset = (ypos - lastY)*mouse_sensitivity;

        lastX = xpos;
        lastY = ypos;

        float phi = cam->getPhi(), theta = cam->getTheta();
        phi   -= yoffset * 360.0f / Global::ScreenHeight;
        theta += xoffset * 360.0f / Global::ScreenWidth;

        if (phi > 80.0f)  phi =  80.0f;
        if (phi < -80.0f) phi = -80.0f;

        cam->setAngles(theta, phi);
    }
}

void error_callback(int, const char* description)
{
    ASSERT(false, description);
}

void setupGLFW(){
    INFO("Initializing GLFW...");

    // Initialise GLFW
    if (!glfwInit())
    {
        std::cerr << "Failed to initialize GLFW\n";
        exit(EXIT_FAILURE);
    }

    glfwSetErrorCallback(error_callback);

    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    /*OpenGL 3.3 Profile Context
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);*/

    // Open a window and create its OpenGL context
    Global::window = glfwCreateWindow(Global::ScreenWidth, Global::ScreenHeight,
                "Lab Project", NULL, NULL);
    if (Global::window == NULL){
        DEBUG("Failed to open GLFW window.");
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    glfwMakeContextCurrent(Global::window);
    glfwSetInputMode(Global::window, GLFW_STICKY_KEYS, GL_TRUE);
    glfwSetInputMode(Global::window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    glfwSetCursorPosCallback(Global::window, mouse_movement_callback);
}

void setupGLEW(){
    INFO("Initializing GLEW...");
    // Initialize glew
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        DEBUG("Unable to initialize glew.");
        glfwDestroyWindow(Global::window);
        glfwTerminate();
        exit(EXIT_FAILURE);
    }
}
