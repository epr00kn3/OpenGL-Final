#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include"texture.h"
#include"shaderClass.h"
#include"VAO.h"
#include"VBO.h"
#include"EBO.h"
#include "Camera.h"


//Ancho y altura de la ventana
const unsigned int width = 800;
const unsigned int height = 800;

// Vertices coordinates
GLfloat vertices[] =
{ //     COORDINATES     /        COLORS      /   TexCoord  //
	-0.5f, 0.0f,  0.5f,     0.83f, 0.70f, 0.44f,	0.0f, 0.0f,
	-0.5f, 0.0f, -0.5f,     0.83f, 0.70f, 0.44f,	5.0f, 0.0f,
	 0.5f, 0.0f, -0.5f,     0.83f, 0.70f, 0.44f,	0.0f, 0.0f,
	 0.5f, 0.0f,  0.5f,     0.83f, 0.70f, 0.44f,	5.0f, 0.0f,
	 0.0f, 0.8f,  0.0f,     0.92f, 0.86f, 0.76f,	2.5f, 5.0f
};

// Indices for vertices order
GLuint indices[] =
{
	0, 1, 2,
	0, 2, 3,
	0, 1, 4,
	1, 2, 4,
	2, 3, 4,
	3, 0, 4
};


int main() {

	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); //Pista de la version mayor de OpenGL que se utiliza
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); //Pista de la version menor de OpenGL que se utiliza
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); //Que perfil (paquete de funciones) de OpenGL se quiere, CORE (Funciones modernas

	//Llamar la ventana y usar la funcion crear una ventana
	GLFWwindow* window = glfwCreateWindow(width, height, "glTF", NULL, NULL);
	//Comprobación de errores
	if (window == NULL)
	{
		std::cout << "Falló en crear ventana :(" << std::endl;
		glfwTerminate();
		return -1;
	}
	//Usar la ventana (contexto actual)
	glfwMakeContextCurrent(window);
	//Activar V-Sync
	glfwSwapInterval(1);

	gladLoadGL();
	//Se le dice el área de la ventana a OpenGL en la que se quiere renderizar
	glViewport(0, 0, width, height);


	// Generates Shader object using shaders defualt.vert and default.frag
	Shader shaderProgram("default.vert", "default.frag");

	// Generates Vertex Array Object and binds it
	VAO VAO1;
	VAO1.Bind();

	// Generates Vertex Buffer Object and links it to vertices
	VBO VBO1(vertices, sizeof(vertices));
	// Generates Element Buffer Object and links it to indices
	EBO EBO1(indices, sizeof(indices));

	// Links VBO to VAO
	VAO1.LinkAttrib(VBO1, 0, 3, GL_FLOAT, 8 * sizeof(float), (void*)0);
	VAO1.LinkAttrib(VBO1, 1, 3, GL_FLOAT, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	VAO1.LinkAttrib(VBO1, 2, 2, GL_FLOAT, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	// Unbind all to prevent accidentally modifying them
	VAO1.Unbind();
	VBO1.Unbind();
	EBO1.Unbind();



	// Texture
	Texture image1("entiendo.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);
	image1.texUnit(shaderProgram, "tex0", 0);

	//Temporizador
	float rotation = 0.0f;
	double prevTime = glfwGetTime();

	//Prueba de profundidad (que triangulo va encima de cual)
	glEnable(GL_DEPTH_TEST);

	//Crear objeto camara
	Camera camera(width, height, glm::vec3(0.0f, 0.0f, 2.0f));

	//Ciclo while que termina solo cuando la ventana se cierra (pulsar botón de cierre)
	while (!glfwWindowShouldClose(window))
	{
		//Borrar el color de un bufer y darle otro color, tambien para el test de profundidad
		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Tell OpenGL which Shader Program we want to use
		shaderProgram.Activate();

		//-------
		//CAMERA

		//inputs (contexto)
		camera.Inputs(window);

		// Actualiza y exporta la matriz de la cámara al Vertex Shader
		camera.Matrix(45.0f, 0.1f, 100.0f, shaderProgram, "camMatrix");
		//------

		//Usar funcion Uniform siempre despues de activar el shaderProgram
		image1.Bind();

		// Bind the VAO so OpenGL knows to use it
		VAO1.Bind();
		// Draw primitives, number of indices, datatype of indices, index of indices
		glDrawElements(GL_TRIANGLES, sizeof(indices) / sizeof(int), GL_UNSIGNED_INT, 0);

		//Para ver el color toca intercambiar los buffers (Bufer posterior: mi color, Bufer frontal: color predeterminado)
		glfwSwapBuffers(window);
		//Cambio de tamaño, aparición de ventana, etc
		glfwPollEvents();
	}

	// Delete all the objects we've created
	VAO1.Delete();
	VBO1.Delete();
	EBO1.Delete();
	image1.Delete();
	shaderProgram.Delete();

	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}