/*
Practica 8: Materiales e Iluminacion
Entrega: 30/03/2025
Perez Del Angel Joaquin Eduardo
Numero de cuenta: 422090638
*/

// Std. Includes
#include <string>
#include <iostream>

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
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode);
void MouseCallback(GLFWwindow* window, double xPos, double yPos);
void DoMovement();

// Camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
bool keys[1024];
GLfloat lastX = 400, lastY = 300;
bool firstMouse = true;

// Light attributes
glm::vec3 lightPos(0.9f, 0.5f, 0.0f);       // Posición del sol
glm::vec3 secondLightPos(0.5f, 0.5f, 0.0f); // Posición de la luna

float reloj = 0.0f; // Reloj para el ciclo día/noche
float movelightPos = 0.0f;
GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;
float rot = 0.0f;
bool activanim = false;
bool modoDia = true; // Comienza en modo día

int main() {
    // Init GLFW
    glfwInit();
    // Set all the required options for GLFW
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    // Create a GLFWwindow object
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Practica 8: Materiales e Iluminacion - Joaquin Eduardo Perez Del Angel", nullptr, nullptr);

    if (nullptr == window) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return EXIT_FAILURE;
    }

    glfwMakeContextCurrent(window);
    glfwGetFramebufferSize(window, &SCREEN_WIDTH, &SCREEN_HEIGHT);

    // Set callback functions
    glfwSetKeyCallback(window, KeyCallback);
    glfwSetCursorPosCallback(window, MouseCallback);

    // Initialize GLEW
    glewExperimental = GL_TRUE;
    if (GLEW_OK != glewInit()) {
        std::cout << "Failed to initialize GLEW" << std::endl;
        return EXIT_FAILURE;
    }

    // Define viewport
    glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    glEnable(GL_DEPTH_TEST);

    // Setup shaders
    Shader shader("Shader/modelLoading.vs", "Shader/modelLoading.frag");
    Shader lampshader("Shader/lamp.vs", "Shader/lamp.frag");
    Shader lightingShader("Shader/lighting.vs", "Shader/lighting.frag");

    // Load models
    Model dog((char*)"Models/Perro/RedDog.obj");
    Model park((char*)"Models/Parque/Park.obj");
    Model hydrant((char*)"Models/Hidrante/Hydrant.obj");
    Model bench((char*)"Models/Banca/Bench.obj");
    Model bicycle((char*)"Models/Bici/Bicycle.obj");
    Model cat((char*)"Models/Gato/Cat.obj");
    Model moon((char*)"Models/Luna/Moon.obj");
    Model sun((char*)"Models/Sol/Sun.obj");

    glm::mat4 projection = glm::perspective(camera.GetZoom(), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 100.0f);

    // Cube vertices for light sources
    float vertices[] = {
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
         0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,

        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
    };

    // First, set the container's VAO (and VBO)
    GLuint VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    // normal attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glBindVertexArray(0);

    // Game loop
    while (!glfwWindowShouldClose(window)) {
        // Calculate delta time
        GLfloat currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // Check events
        glfwPollEvents();
        DoMovement();

        // Clear buffers
        //glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Set lighting shader
        lightingShader.Use();
        GLint lightPosLoc = glGetUniformLocation(lightingShader.Program, "light.position");
        GLint lightPosLoc2 = glGetUniformLocation(lightingShader.Program, "light2.position");
        GLint viewPosLoc = glGetUniformLocation(lightingShader.Program, "viewPos");

        // Movimiento del sol
        lightPos.x = cos(reloj) * 5.0f; // Movimiento en el eje X
        lightPos.y = sin(reloj) * 5.0f; // Movimiento en el eje Y

        // Movimiento de la luna (opuesto al sol)
        secondLightPos.x = -cos(reloj) * 5.0f; // Movimiento en el eje X
        secondLightPos.y = -sin(reloj) * 5.0f; // Movimiento en el eje Y

        // Set light positions and view pos
        glUniform3f(lightPosLoc, lightPos.x, lightPos.y, lightPos.z);
        glUniform3f(lightPosLoc2, secondLightPos.x, secondLightPos.y, secondLightPos.z);
        glUniform3f(viewPosLoc, camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);

        // Configure sun (warm light) or turn it off 
        if (modoDia) {
            // Sun properties (warm light)
            glUniform3f(glGetUniformLocation(lightingShader.Program, "light.ambient"), 0.3f, 0.3f, 0.2f);
            glUniform3f(glGetUniformLocation(lightingShader.Program, "light.diffuse"), 1.0f, 0.9f, 0.7f);
            glUniform3f(glGetUniformLocation(lightingShader.Program, "light.specular"), 0.8f, 0.6f, 0.4f);
        }
        else {
            // Turn off sun
            glUniform3f(glGetUniformLocation(lightingShader.Program, "light.ambient"), 0.0f, 0.0f, 0.0f);
            glUniform3f(glGetUniformLocation(lightingShader.Program, "light.diffuse"), 0.0f, 0.0f, 0.0f);
            glUniform3f(glGetUniformLocation(lightingShader.Program, "light.specular"), 0.0f, 0.0f, 0.0f);
        }

        // Configure moon (cool light) or turn it off
        if (!modoDia) {
            // Moon properties (cool light)
            glUniform3f(glGetUniformLocation(lightingShader.Program, "light2.ambient"), 0.1f, 0.1f, 0.2f);
            glUniform3f(glGetUniformLocation(lightingShader.Program, "light2.diffuse"), 0.5f, 0.6f, 0.8f);
            glUniform3f(glGetUniformLocation(lightingShader.Program, "light2.specular"), 0.4f, 0.5f, 0.7f);
        }
        else {
            // Turn off moon
            glUniform3f(glGetUniformLocation(lightingShader.Program, "light2.ambient"), 0.0f, 0.0f, 0.0f);
            glUniform3f(glGetUniformLocation(lightingShader.Program, "light2.diffuse"), 0.0f, 0.0f, 0.0f);
            glUniform3f(glGetUniformLocation(lightingShader.Program, "light2.specular"), 0.0f, 0.0f, 0.0f);
        }

        // Set projection and view matrices
        glm::mat4 view = camera.GetViewMatrix();
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));

        // Set material properties
        glUniform3f(glGetUniformLocation(lightingShader.Program, "material.ambient"), 0.2f, 0.2f, 0.2f); // Gris oscuro
        glUniform3f(glGetUniformLocation(lightingShader.Program, "material.diffuse"), 0.8f, 0.75f, 0.7f); // Ligero tono cálido
        glUniform3f(glGetUniformLocation(lightingShader.Program, "material.specular"), 0.4f, 0.4f, 0.4f); // Reflejos moderados
        glUniform1f(glGetUniformLocation(lightingShader.Program, "material.shininess"), 100.0f); // Brillantez moderada

        // Render park
        glm::mat4 modelPark = glm::mat4(1.0f);
        modelPark = glm::translate(modelPark, glm::vec3(1.0f, 0.0f, 0.0f));
        modelPark = glm::rotate(modelPark, glm::radians(270.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        modelPark = glm::scale(modelPark, glm::vec3(10.0f, 10.0f, 10.0f));
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(modelPark));
        park.Draw(shader);

        // Render dog
        glm::mat4 modelDog = glm::mat4(1.0f);
        modelDog = glm::translate(modelDog, glm::vec3(3.0f, -0.4f, 3.0f));
        modelDog = glm::scale(modelDog, glm::vec3(1.0f, 1.0f, 1.0f));
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(modelDog));
        dog.Draw(shader);

        // Render bench
        glm::mat4 modelBench = glm::mat4(1.0f);
        modelBench = glm::translate(modelBench, glm::vec3(0.5f, -0.5f, 0.4f));
        modelBench = glm::rotate(modelBench, glm::radians(270.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        modelBench = glm::scale(modelBench, glm::vec3(1.5f, 1.5f, 1.5f));
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(modelBench));
        bench.Draw(shader);

        // Render hydrant
        glm::mat4 modelHydrant = glm::mat4(1.0f);
        modelHydrant = glm::translate(modelHydrant, glm::vec3(2.0f, -0.57f, -0.2f));
        modelHydrant = glm::rotate(modelHydrant, glm::radians(270.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        modelHydrant = glm::scale(modelHydrant, glm::vec3(0.6f, 0.6f, 0.6f));
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(modelHydrant));
        hydrant.Draw(shader);

        // Render bicycle
        glm::mat4 modelBicycle = glm::mat4(1.0f);
        modelBicycle = glm::translate(modelBicycle, glm::vec3(-1.0f, -0.55f, 3.3f));
        modelBicycle = glm::rotate(modelBicycle, glm::radians(40.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        modelBicycle = glm::scale(modelBicycle, glm::vec3(1.0f, 1.0f, 1.0f));
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(modelBicycle));
        bicycle.Draw(shader);

        // Render cat
        glm::mat4 modelCat = glm::mat4(1.0f);
        modelCat = glm::translate(modelCat, glm::vec3(0.6f, -0.3f, 0.5f));
        modelCat = glm::scale(modelCat, glm::vec3(0.5f, 0.5f, 0.5f));
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(modelCat));
        cat.Draw(shader);

        // Render sun
        glm::mat4 modelSun = glm::mat4(1.0f);
        modelSun = glm::translate(modelSun, lightPos); 
        modelSun = glm::scale(modelSun, glm::vec3(1.0f)); 
        glUniformMatrix4fv(glGetUniformLocation(lampshader.Program, "model"), 1, GL_FALSE, glm::value_ptr(modelSun));
        sun.Draw(lampshader);

        // Render moon
        glm::mat4 modelMoon = glm::mat4(1.0f);
        modelMoon = glm::translate(modelMoon, secondLightPos);
        modelMoon = glm::scale(modelMoon, glm::vec3(1.0f));
        modelMoon = glm::scale(modelMoon, glm::vec3(0.5));
        glUniformMatrix4fv(glGetUniformLocation(lampshader.Program, "model"), 1, GL_FALSE, glm::value_ptr(modelMoon));
        moon.Draw(lampshader); 

        // Swap buffers
        glfwSwapBuffers(window);
    }

    // Clean up
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glfwTerminate();
    return 0;
}

// Moves/alters the camera positions based on user input
void DoMovement() {
    // Camera controls
    if (keys[GLFW_KEY_W] || keys[GLFW_KEY_UP]) {
        camera.ProcessKeyboard(FORWARD, deltaTime);
    }

    if (keys[GLFW_KEY_S] || keys[GLFW_KEY_DOWN]) {
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    }

    if (keys[GLFW_KEY_A] || keys[GLFW_KEY_LEFT]) {
        camera.ProcessKeyboard(LEFT, deltaTime);
    }

    if (keys[GLFW_KEY_D] || keys[GLFW_KEY_RIGHT]) {
        camera.ProcessKeyboard(RIGHT, deltaTime);
    }

    if (activanim) {
        if (rot > -90.0f)
            rot -= 0.1f;
    }
}

// Is called whenever a key is pressed/released via GLFW
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode) {
    if (GLFW_KEY_ESCAPE == key && GLFW_PRESS == action) {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }

    if (key >= 0 && key < 1024) {
        if (action == GLFW_PRESS) {
            keys[key] = true;
        }
        else if (action == GLFW_RELEASE) {
            keys[key] = false;
        }
    }

    // Controls for sun position
    if (keys[GLFW_KEY_O]) {
        reloj += 0.01f;  // Incrementar el reloj (mueve el sol y la luna hacia adelante)
    }
    if (keys[GLFW_KEY_L]) {
        reloj -= 0.01f;  // Decrementar el reloj (mueve el sol y la luna hacia atrás)
    }
}

void MouseCallback(GLFWwindow* window, double xPos, double yPos) {
    if (firstMouse) {
        lastX = xPos;
        lastY = yPos;
        firstMouse = false;
    }

    GLfloat xOffset = xPos - lastX;
    GLfloat yOffset = lastY - yPos;

    lastX = xPos;
    lastY = yPos;

    camera.ProcessMouseMovement(xOffset, yOffset);
}