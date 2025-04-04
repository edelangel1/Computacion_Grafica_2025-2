/*
Practica 6
Entrega: 16/03/2025
Perez Del Angel Joaquin Eduardo
Numero de cuenta: 422090638
*/

// Std. Includes
#include <string>

// GLEW
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

// GL includes
#include "Shader.h"
#include "Camera.h"
#include "Model.h"

// GLM Mathemtics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Other Libs
#include "SOIL2/SOIL2.h"
#include "stb_image.h"

// Properties
const GLuint WIDTH = 1200, HEIGHT = 650;
int SCREEN_WIDTH, SCREEN_HEIGHT;

// Function prototypes
void KeyCallback( GLFWwindow *window, int key, int scancode, int action, int mode );
void MouseCallback( GLFWwindow *window, double xPos, double yPos );
void DoMovement( );

// Camera
Camera camera( glm::vec3( 0.0f, 0.0f, 3.0f ) );
bool keys[1024];
GLfloat lastX = 400, lastY = 300;
bool firstMouse = true;

GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;

int main( ){
    // Init GLFW
    glfwInit( );
    // Set all the required options for GLFW
    glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 3 );
    glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 3 );
    glfwWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE );
    glfwWindowHint( GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE );
    glfwWindowHint( GLFW_RESIZABLE, GL_FALSE );
    
    // Create a GLFWwindow object that we can use for GLFW's functions
    GLFWwindow *window = glfwCreateWindow( WIDTH, HEIGHT, "Practica 6 - Eduardo", nullptr, nullptr );
    
    if ( nullptr == window ){
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate( );
        
        return EXIT_FAILURE;
    }
    
    glfwMakeContextCurrent( window );
    
    glfwGetFramebufferSize( window, &SCREEN_WIDTH, &SCREEN_HEIGHT );
    
    // Set the required callback functions
    glfwSetKeyCallback( window, KeyCallback );
    glfwSetCursorPosCallback( window, MouseCallback );
    
    // GLFW Options
    //glfwSetInputMode( window, GLFW_CURSOR, GLFW_CURSOR_DISABLED );
    
    // Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
    glewExperimental = GL_TRUE;
    // Initialize GLEW to setup the OpenGL Function pointers
    if ( GLEW_OK != glewInit( ) ){
        std::cout << "Failed to initialize GLEW" << std::endl;
        return EXIT_FAILURE;
    }
    
    // Define the viewport dimensions
    glViewport( 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT );
    
    // OpenGL options
    glEnable( GL_DEPTH_TEST );
    
    // Setup and compile our shaders
    Shader shader( "Shader/modelLoading.vs", "Shader/modelLoading.frag" );
    
    // Load models
    Model dog((char*)"Models/Perro/RedDog.obj");
    Model park((char*)"Models/Parque/Park.obj");
    Model hydrant((char*)"Models/Hidrante/Hydrant.obj");
    Model bench((char*)"Models/Banca/Bench.obj");
    Model bicycle((char*)"Models/Bici/Bicycle.obj");
    Model cat((char*)"Models/Gato/Cat.obj");
    Model house((char*)"Models/Casa/House.obj");
    glm::mat4 projection = glm::perspective( camera.GetZoom( ), ( float )SCREEN_WIDTH/( float )SCREEN_HEIGHT, 0.1f, 100.0f );

    // Game loop
    while (!glfwWindowShouldClose(window)){
        // Set frame time
        GLfloat currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // Check and call events
        glfwPollEvents();
        DoMovement();

        // Clear the colorbuffer
        glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shader.Use();

        glm::mat4 view = camera.GetViewMatrix();
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));

        // Draw the loaded model
        //// Park
        //glm::mat4 modelPark = glm::mat4(1.0f);
        //modelPark = glm::translate(modelPark, glm::vec3(1.0f, 0.0f, 0.0f));
        //modelPark = glm::rotate(modelPark, glm::radians(270.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        //modelPark = glm::scale(modelPark, glm::vec3(10.0f, 10.0f, 10.0f));
        //glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(modelPark));
        //park.Draw(shader);

        ////// Dog
        //glm::mat4 modelDog = glm::mat4(1.0f);
        //modelDog = glm::translate(modelDog, glm::vec3(3.0f, -0.4f, 3.0f));
        //modelDog = glm::scale(modelDog, glm::vec3(1.0f, 1.0f, 1.0f));
        //glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(modelDog));
        //dog.Draw(shader);

        //// Bench
        //glm::mat4 modelBench = glm::mat4(1.0f);
        //modelBench = glm::translate(modelBench, glm::vec3(0.5f, -0.5f, 0.4f));
        //modelBench = glm::rotate(modelBench, glm::radians(270.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        //modelBench = glm::scale(modelBench, glm::vec3(1.5f, 1.5f, 1.5f));
        //glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(modelBench));
        //bench.Draw(shader);

        ////// Hydrant
        //glm::mat4 modelHydrant = glm::mat4(1.0f);
        //modelHydrant = glm::translate(modelHydrant, glm::vec3(2.0f, -0.57f, -0.2f));
        //modelHydrant = glm::rotate(modelHydrant, glm::radians(270.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        //modelHydrant = glm::scale(modelHydrant, glm::vec3(0.6f, 0.6f, 0.6f));
        //glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(modelHydrant));
        //hydrant.Draw(shader);

        ////// Bicycle
        //glm::mat4 modelBicycle = glm::mat4(1.0f);
        //modelBicycle = glm::translate(modelBicycle, glm::vec3(-1.0f, -0.55f, 3.3f));
        //modelBicycle = glm::rotate(modelBicycle, glm::radians(40.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        //modelBicycle = glm::scale(modelBicycle, glm::vec3(1.0f, 1.0f, 1.0f));
        //glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(modelBicycle));
        //bicycle.Draw(shader);

        //// Cat
        //glm::mat4 modelCat = glm::mat4(1.0f);
        //modelCat = glm::translate(modelCat, glm::vec3(0.6f, -0.3f, 0.5f));
        //modelCat = glm::scale(modelCat, glm::vec3(0.5f, 0.5f, 0.5f));
        //glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(modelCat));
        //cat.Draw(shader);

        glm::mat4 modelHouse = glm::mat4(1.0f);
        modelHouse = glm::translate(modelHouse, glm::vec3(0.6f, -0.3f, 0.5f));
        modelHouse = glm::scale(modelHouse, glm::vec3(0.5f, 0.5f, 0.5f));
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(modelHouse));
        house.Draw(shader);

        // Swap the buffers
        glfwSwapBuffers( window );
    }
    
    glfwTerminate( );
    return 0;
}

// Moves/alters the camera positions based on user input
void DoMovement( ){
    // Camera controls
    if ( keys[GLFW_KEY_W] || keys[GLFW_KEY_UP] ){
        camera.ProcessKeyboard( FORWARD, deltaTime );
    }
    
    if ( keys[GLFW_KEY_S] || keys[GLFW_KEY_DOWN] ){
        camera.ProcessKeyboard( BACKWARD, deltaTime );
    }
    
    if ( keys[GLFW_KEY_A] || keys[GLFW_KEY_LEFT] ){
        camera.ProcessKeyboard( LEFT, deltaTime );
    }
    
    if ( keys[GLFW_KEY_D] || keys[GLFW_KEY_RIGHT] ){
        camera.ProcessKeyboard( RIGHT, deltaTime );
    }
}

// Is called whenever a key is pressed/released via GLFW
void KeyCallback( GLFWwindow *window, int key, int scancode, int action, int mode )
{
    if ( GLFW_KEY_ESCAPE == key && GLFW_PRESS == action ){
        glfwSetWindowShouldClose(window, GL_TRUE);
    }
    
    if ( key >= 0 && key < 1024 ){
        if ( action == GLFW_PRESS ){
            keys[key] = true;
        }
        else if ( action == GLFW_RELEASE ){
            keys[key] = false;
        }
    }
}

void MouseCallback( GLFWwindow *window, double xPos, double yPos ){
    if ( firstMouse ){
        lastX = xPos;
        lastY = yPos;
        firstMouse = false;
    }
    
    GLfloat xOffset = xPos - lastX;
    GLfloat yOffset = lastY - yPos;  // Reversed since y-coordinates go from bottom to left
    
    lastX = xPos;
    lastY = yPos;
    
    camera.ProcessMouseMovement( xOffset, yOffset );
}

