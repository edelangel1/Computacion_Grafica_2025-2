/*
Práctica 2: Dibujo de primitivas		Pérez Del Angel Joaquín Eduardo
Entrega: 14/02/2025						422090638
*/

#include<iostream>
//#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Shader.h" // Shaders

void resize(GLFWwindow* window, int width, int height);
const GLint WIDTH = 800, HEIGHT = 600;

int main() {
	glfwInit();
	//Verificación de compatibilidad 
	/*glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);*/

	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Práctica 2 - Eduardo", NULL, NULL);
	glfwSetFramebufferSizeCallback(window, resize);

	//Verificación de errores de creación ventana
	if (window == NULL){
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();

		return EXIT_FAILURE;
	}

	glfwMakeContextCurrent(window);
	glewExperimental = GL_TRUE;

	//Verificación de errores de inicialización de glew
	if (GLEW_OK != glewInit()) {
		std::cout << "Failed to initialise GLEW" << std::endl;
		return EXIT_FAILURE;
	}

	// Imprimimos información de OpenGL del sistema
	std::cout << "> Version: " << glGetString(GL_VERSION) << std::endl;
	std::cout << "> Vendor: " << glGetString(GL_VENDOR) << std::endl;
	std::cout << "> Renderer: " << glGetString(GL_RENDERER) << std::endl;
	std::cout << "> SL Version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;

	// Define las dimensiones del viewport
	//glViewport(0, 0, screenWidth, screenHeight);

	Shader ourShader("Shader/core.vs", "Shader/core.frag");

	// Set up vertex data (and buffer(s)) and attribute pointers
	
	float vertices[] = {
		-0.750210845171f, 0.1451748149926f, 0.0f,  0.8f, 0.8f, 0.8f, // A0 
		-0.570282260186f, 0.243110627073f, 0.0f,   0.7f, 0.7f, 0.7f, // B1 
		-0.6408871479649f, 0.3023276297263f, 0.0f, 0.6f, 0.6f, 0.6f, // C2 
		-0.8208157329499f, 0.5050319849625f, 0.0f, 0.5f, 0.5f, 0.5f, // D3 
		-0.4700688710804f, 0.3615446323796f, 0.0f, 0.4f, 0.4f, 0.4f, // E4 
		-0.317471210397f, 0.4868113687615, 0.0f,   0.3f, 0.3f, 0.3f, // F5 
		-0.3880760981759f, 0.7601206117767f, 0.0f, 0.2f, 0.2f, 0.2f, // G6 
		-0.0487171214321f, 0.5665265646409f, 0.0f, 0.1f, 0.1f, 0.1f, // H7 
		-0.5976131844875f, 0.261331243274f, 0.0f,  0.9f, 0.9f, 0.9f, // I8 
		-0.1717062807889f, 0.2271675878971f, 0.0f, 0.8f, 0.8f, 0.8f, // J9 
		-0.0077207349798f, 0.2658863973243f, 0.0f, 0.7f, 0.7f, 0.7f, // K10 
		0.1927060432313f, 0.1292317758167f, 0.0f,  0.6f, 0.6f, 0.6f, // L11 
		0.1608199648795f, 0.3182706689022f, 0.0f,  0.5f, 0.5f, 0.5f, // M12 
		0.2450903148092f, 0.3410464391534f, 0.0f,  0.4f, 0.4f, 0.4f, // N13 
		0.4796807483972f, 0.7145690712741f, 0.0f,  0.3f, 0.3f, 0.3f, // O14 
		0.0514962676735f, 0.5870247578671f, 0.0f,  0.2f, 0.2f, 0.2f  // P15 
	};

	unsigned int indices[] = {
		0, 1, 2,		// ABC (A0, B1, C2)
		3, 4, 8,		// DEI (D3, E4, I8)
		1, 2, 5,		// BCF (B1, D3, F5)
		1, 5, 9,		// BFJ (B1, F5, J9)
		4, 5, 9,		// EFJ (E4, F5, J9)
		5, 7, 9,		// FHJ (F5, H7, J9)
		7, 9, 15,		// HJP (H7, J9, P15)
		9, 10, 15,		// JKP (J9, K10, P15) 
		5, 6, 7,		// FGH (F5, G6, H7)
		10, 12, 15,		// KMP (K10, M12, P15)
		12, 13, 15,		// MNP (M12, N13, P15)
		10, 11, 12,		// KLM (K10, L11, M12)
		13, 14, 15		// NOP (N13, O14, P15) 
	};

	GLuint VBO, VAO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	// Enlazar  Vertex Array Object
	glBindVertexArray(VAO);

	//2.- Copiamos nuestros arreglo de vertices en un buffer de vertices para que OpenGL lo use
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	// 3.Copiamos nuestro arreglo de indices en  un elemento del buffer para que OpenGL lo use
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// 4. Despues colocamos las caracteristicas de los vertices
	//Posición
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	//Color
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0); // Unbind VAO (it's always a good thing to unbind any buffer/array to prevent strange bugs)

	while (!glfwWindowShouldClose(window))
	{
		// Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
		glfwPollEvents();

		// Render
		// Clear the colorbuffer
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// Draw our first triangle
		ourShader.Use();
		glBindVertexArray(VAO);

		//Generación de triángulos
		glDrawElements(GL_TRIANGLES, 39, GL_UNSIGNED_INT, 0); // N índices (M triángulos * 3 vértices)

		glBindVertexArray(0);

		// Swap the screen buffers
		glfwSwapBuffers(window);
	}

	glfwTerminate();
	return EXIT_SUCCESS;
}

void resize(GLFWwindow* window, int width, int height)
{
	// Set the Viewport to the size of the created window
	glViewport(0, 0, width, height);
	//glViewport(0, 0, screenWidth, screenHeight);
}