/*
Practica 9: Fuentes de Luz 
Entrega: 06/04/2025
Perez Del Angel Joaquin Eduardo
Numero de cuenta: 422090638
*/

#include <iostream>
#include <cmath>
#include <string>

// GLEW
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

// Other Libs
#include "stb_image.h"
#include "SOIL2/SOIL2.h"

// GLM Mathematics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Other includes
#include "Shader.h"
#include "Camera.h"
#include "Model.h"

// Function prototypes
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode);
void MouseCallback(GLFWwindow* window, double xPos, double yPos);
void DoMovement();

// Window dimensions
const GLuint WIDTH = 1200, HEIGHT = 650;
int SCREEN_WIDTH, SCREEN_HEIGHT;

// Camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
GLfloat lastX = WIDTH / 2.0;
GLfloat lastY = HEIGHT / 2.0;
bool keys[1024];
bool firstMouse = true;

// Light attributes
glm::vec3 lightPos(0.0f, 0.0f, 0.0f);
bool active;
float reloj = 0.0f;
bool modoDia = true; 

// Positions of the point lights
glm::vec3 pointLightPositions[] = {
    glm::vec3(0.25f, 0.5f, 0.45f),   // Posición del foco del poste de luz 
};

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

glm::vec3 Light1 = glm::vec3(0);

// Deltatime
GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;

int main() {
    // Init GLFW
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    // Create window
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Practica 9: Fuentes de Luz - Eduardo Del Angel", nullptr, nullptr);
    if (window == nullptr) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return EXIT_FAILURE;
    }

    glfwMakeContextCurrent(window);
    glfwGetFramebufferSize(window, &SCREEN_WIDTH, &SCREEN_HEIGHT);

    // Set callbacks
    glfwSetKeyCallback(window, KeyCallback);
    glfwSetCursorPosCallback(window, MouseCallback);

    // Initialize GLEW
    glewExperimental = GL_TRUE;
    if (GLEW_OK != glewInit()) {
        std::cout << "Failed to initialize GLEW" << std::endl;
        return EXIT_FAILURE;
    }

    // Configure global OpenGL state
    glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    glEnable(GL_DEPTH_TEST);

    // Setup shaders
    Shader lightingShader("Shader/lighting.vs", "Shader/lighting.frag");
    Shader lampShader("Shader/lamp.vs", "Shader/lamp.frag");

    // Load models 
    Model dog((char*)"Models/Perro/RedDog.obj");
    Model park((char*)"Models/Parque/Park.obj");
    Model hydrant((char*)"Models/Hidrante/Hydrant.obj");
    Model bench((char*)"Models/Banca/Bench.obj");
    Model bicycle((char*)"Models/Bici/Bicycle.obj");
    Model cat((char*)"Models/Gato/Cat.obj");
    Model lightPole((char*)"Models/Poste/LightPole.obj");

    // Setup vertex data and buffers
    GLuint VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glBindVertexArray(0);

    // Set texture units
    lightingShader.Use();
    glUniform1i(glGetUniformLocation(lightingShader.Program, "material.diffuse"), 0);
    glUniform1i(glGetUniformLocation(lightingShader.Program, "material.specular"), 1);

    glm::mat4 projection = glm::perspective(camera.GetZoom(), (GLfloat)SCREEN_WIDTH / (GLfloat)SCREEN_HEIGHT, 0.1f, 100.0f);

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
        if (modoDia) {
            glClearColor(0.1f, 0.2f, 0.3f, 1.0f); // Day sky color
        }
        else {
            //glClearColor(0.0f, 0.0f, 0.1f, 1.0f); // Night sky color
            glClearColor(0.0f, 0.0f, 0.0f, 1.0f);  // Negro 
        }
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Use lighting shader
        lightingShader.Use();

        // Set view position
        GLint viewPosLoc = glGetUniformLocation(lightingShader.Program, "viewPos");
        glUniform3f(viewPosLoc, camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);

        // Directional light (luna para modo noche)
        glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.direction"), -0.2f, -1.0f, -0.3f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.ambient"), 0.05f, 0.05f, 0.1f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.diffuse"), 0.2f, 0.2f, 0.3f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.specular"), 0.2f, 0.2f, 0.3f);

        // Configuración especial para la luz del poste (índice 0)
        glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].position"),
            pointLightPositions[0].x, pointLightPositions[0].y, pointLightPositions[0].z);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].ambient"),
            0.2f, 0.2f, 0.2f);  // Ambiente suave para el poste de luz
        glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].diffuse"),
            1.0f, 1.0f, 0.9f);  // Luz difusa blanca brillante con leve tono amarillo
        glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].specular"),
            1.0f, 1.0f, 1.0f);   // Alta intensidad especular para simular el brillo
        glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].constant"), 1.0f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].linear"), 0.09f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].quadratic"), 0.032f);

        // Otras luces (menos intensas)
        for (GLuint i = 1; i < 4; i++) {
            std::string number = std::to_string(i);
            glUniform3f(glGetUniformLocation(lightingShader.Program, ("pointLights[" + number + "].position").c_str()),
                pointLightPositions[i].x, pointLightPositions[i].y, pointLightPositions[i].z);
            glUniform3f(glGetUniformLocation(lightingShader.Program, ("pointLights[" + number + "].ambient").c_str()),
                0.02f, 0.02f, 0.02f);
            glUniform3f(glGetUniformLocation(lightingShader.Program, ("pointLights[" + number + "].diffuse").c_str()),
                0.3f, 0.3f, 0.3f);
            glUniform3f(glGetUniformLocation(lightingShader.Program, ("pointLights[" + number + "].specular").c_str()),
                0.4f, 0.4f, 0.4f);
            glUniform1f(glGetUniformLocation(lightingShader.Program, ("pointLights[" + number + "].constant").c_str()), 1.0f);
            glUniform1f(glGetUniformLocation(lightingShader.Program, ("pointLights[" + number + "].linear").c_str()), 0.09f);
            glUniform1f(glGetUniformLocation(lightingShader.Program, ("pointLights[" + number + "].quadratic").c_str()), 0.032f);
        }

        // SpotLight (flashlight) - menos intensa para no competir con la lámpara
        glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.position"),
            camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.direction"),
            camera.GetFront().x, camera.GetFront().y, camera.GetFront().z);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.ambient"), 0.0f, 0.0f, 0.0f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.diffuse"), 0.5f, 0.5f, 0.5f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.specular"), 0.5f, 0.5f, 0.5f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.constant"), 1.0f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.linear"), 0.09f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.quadratic"), 0.032f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.cutOff"), glm::cos(glm::radians(12.5f)));
        glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.outerCutOff"), glm::cos(glm::radians(15.0f)));

        // Material properties (mayor brillo para reflejos)
        glUniform1f(glGetUniformLocation(lightingShader.Program, "material.shininess"), 128.0f);

        // Create camera transformations
        glm::mat4 view = camera.GetViewMatrix();
        glm::mat4 projectionMatrix = glm::perspective(camera.GetZoom(), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 100.0f);

        // Get the uniform locations
        GLint modelLoc = glGetUniformLocation(lightingShader.Program, "model");
        GLint viewLoc = glGetUniformLocation(lightingShader.Program, "view");
        GLint projLoc = glGetUniformLocation(lightingShader.Program, "projection");

        // Pass the matrices to the shader
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

        // Render park 
        glm::mat4 modelPark = glm::mat4(1.0f);
        modelPark = glm::translate(modelPark, glm::vec3(1.0f, 0.0f, 0.0f));
        modelPark = glm::rotate(modelPark, glm::radians(270.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        modelPark = glm::scale(modelPark, glm::vec3(10.0f, 10.0f, 10.0f));
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(modelPark));
        park.Draw(lightingShader);

        // Render bench
        glm::mat4 modelBench = glm::mat4(1.0f);
        modelBench = glm::translate(modelBench, glm::vec3(0.3f, -0.62f, 0.4f));
        modelBench = glm::rotate(modelBench, glm::radians(270.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        modelBench = glm::scale(modelBench, glm::vec3(1.0f, 1.0f, 1.0f));
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(modelBench));
        bench.Draw(lightingShader);

        // Render dog
        glm::mat4 modelDog = glm::mat4(1.0f);
        modelDog = glm::translate(modelDog, glm::vec3(3.0f, -0.4f, 3.0f));
        modelDog = glm::scale(modelDog, glm::vec3(0.7f, 0.7f, 0.7f));
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(modelDog));
        dog.Draw(lightingShader);

        // Render hydrant
        glm::mat4 modelHydrant = glm::mat4(1.0f);
        modelHydrant = glm::translate(modelHydrant, glm::vec3(2.0f, -0.57f, -0.2f));
        modelHydrant = glm::rotate(modelHydrant, glm::radians(270.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        modelHydrant = glm::scale(modelHydrant, glm::vec3(0.6f, 0.6f, 0.6f));
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(modelHydrant));
        hydrant.Draw(lightingShader);

        // Render bicycle
        glm::mat4 modelBicycle = glm::mat4(1.0f);
        modelBicycle = glm::translate(modelBicycle, glm::vec3(-1.0f, -0.55f, 3.3f));
        modelBicycle = glm::rotate(modelBicycle, glm::radians(40.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        modelBicycle = glm::scale(modelBicycle, glm::vec3(1.0f, 1.0f, 1.0f));
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(modelBicycle));
        bicycle.Draw(lightingShader);

        // Render cat
        glm::mat4 modelCat = glm::mat4(1.0f);
        modelCat = glm::translate(modelCat, glm::vec3(0.4f, -0.52f, 0.45f));
        modelCat = glm::scale(modelCat, glm::vec3(0.3f, 0.3f, 0.3f));
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(modelCat));
        cat.Draw(lightingShader);

        // Light pole
        glm::mat4 modelLightPole = glm::mat4(1.0f);
        modelLightPole = glm::translate(modelLightPole, glm::vec3(0.0f, -0.1f, 0.4f));
        modelLightPole = glm::rotate(modelLightPole, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, .0f));
        modelLightPole = glm::scale(modelLightPole, glm::vec3(1.5f, 1.5f, 1.5f));
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(modelLightPole));
        lightPole.Draw(lightingShader);

        //// Render light source cubes (opcional, para visualizar las posiciones de luz)
        //lampShader.Use();
        //glUniformMatrix4fv(glGetUniformLocation(lampShader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projectionMatrix));
        //glUniformMatrix4fv(glGetUniformLocation(lampShader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));

        //for (GLuint i = 0; i < 1; i++) {
        //    glm::mat4 modelLight = glm::mat4(1.0f);
        //    modelLight = glm::translate(modelLight, pointLightPositions[i]);
        //    modelLight = glm::scale(modelLight, glm::vec3(0.1f));
        //    glUniformMatrix4fv(glGetUniformLocation(lampShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(modelLight));

        //    if (i == 0) {
        //        glUniform3f(glGetUniformLocation(lampShader.Program, "lightColor"), 1.0f, 1.0f, 0.9f);
        //    }
        //    else {
        //        glUniform3f(glGetUniformLocation(lampShader.Program, "lightColor"), 0.3f, 0.3f, 0.3f);
        //    }

        //    glBindVertexArray(VAO);
        //    glDrawArrays(GL_TRIANGLES, 0, 36);
        //}
        //glBindVertexArray(0);

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

    // Light position controls
    if (keys[GLFW_KEY_T]) {
        pointLightPositions[0].x += 0.01f;
    }
    if (keys[GLFW_KEY_G]) {
        pointLightPositions[0].x -= 0.01f;
    }
    if (keys[GLFW_KEY_Y]) {
        pointLightPositions[0].y += 0.01f;
    }
    if (keys[GLFW_KEY_H]) {
        pointLightPositions[0].y -= 0.01f;
    }
    if (keys[GLFW_KEY_U]) {
        pointLightPositions[0].z -= 0.1f;
    }
    if (keys[GLFW_KEY_J]) {
        pointLightPositions[0].z += 0.01f;
    }

}

// Is called whenever a key is pressed/released via GLFW
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
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

    //// Toggle dynamic light
    //if (keys[GLFW_KEY_SPACE]) {
    //    active = !active;
    //    if (active) {
    //        Light1 = glm::vec3(1.0f, 1.0f, 0.0f);
    //    }
    //    else {
    //        Light1 = glm::vec3(0);
    //    }
    //}

    // Toggle day/night mode
    if (keys[GLFW_KEY_N] && action == GLFW_PRESS) {
        modoDia = !modoDia;
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