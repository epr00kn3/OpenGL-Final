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
//Plano
GLfloat vertices[] =
{ //     COORDINATES     /        COLORS        /    TexCoord    /       NORMALS     //
	-1.0f, 0.0f,  1.0f,		0.0f, 0.0f, 0.0f,		0.0f, 0.0f,		0.0f, 1.0f, 0.0f,
	-1.0f, 0.0f, -1.0f,		0.0f, 0.0f, 0.0f,		0.0f, 1.0f,		0.0f, 1.0f, 0.0f,
	 1.0f, 0.0f, -1.0f,		0.0f, 0.0f, 0.0f,		1.0f, 1.0f,		0.0f, 1.0f, 0.0f,
	 1.0f, 0.0f,  1.0f,		0.0f, 0.0f, 0.0f,		1.0f, 0.0f,		0.0f, 1.0f, 0.0f
};

// Indices for vertices order
GLuint indices[] =
{
	0, 1, 2,
	0, 2, 3
};

//Cubo que sirve como fuente de luz
GLfloat lightVertices[] =
{ //     COORDINATES     //
	-0.1f, -0.1f,  0.1f,
	-0.1f, -0.1f, -0.1f,
	 0.1f, -0.1f, -0.1f,
	 0.1f, -0.1f,  0.1f,
	-0.1f,  0.1f,  0.1f,
	-0.1f,  0.1f, -0.1f,
	 0.1f,  0.1f, -0.1f,
	 0.1f,  0.1f,  0.1f
};

GLuint lightIndices[] =
{
	0, 1, 2,
	0, 2, 3,
	0, 4, 7,
	0, 7, 3,
	3, 7, 6,
	3, 6, 2,
	2, 6, 5,
	2, 5, 1,
	1, 5, 4,
	1, 4, 0,
	4, 5, 6,
	4, 6, 7
};


//MAIN
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
	VAO1.LinkAttrib(VBO1, 0, 3, GL_FLOAT, 11 * sizeof(float), (void*)0);
	VAO1.LinkAttrib(VBO1, 1, 3, GL_FLOAT, 11 * sizeof(float), (void*)(3 * sizeof(float)));
	VAO1.LinkAttrib(VBO1, 2, 2, GL_FLOAT, 11 * sizeof(float), (void*)(6 * sizeof(float)));
	VAO1.LinkAttrib(VBO1, 3, 3, GL_FLOAT, 11 * sizeof(float), (void*)(8 * sizeof(float)));

	// Unbind all to prevent accidentally modifying them
	VAO1.Unbind();
	VBO1.Unbind();
	EBO1.Unbind();

	// LUZ ------
	// Shader for light cube
	Shader lightShader("light.vert", "light.frag");
	// Generates Vertex Array Object and binds it
	VAO lightVAO;
	lightVAO.Bind();
	// Generates Vertex Buffer Object and links it to vertices
	VBO lightVBO(lightVertices, sizeof(lightVertices));
	// Generates Element Buffer Object and links it to indices
	EBO lightEBO(lightIndices, sizeof(lightIndices));
	// Links VBO attributes such as coordinates and colors to VAO
	lightVAO.LinkAttrib(lightVBO, 0, 3, GL_FLOAT, 3 * sizeof(float), (void*)0);
	// Unbind all to prevent accidentally modifying them
	lightVAO.Unbind();
	lightVBO.Unbind();
	lightEBO.Unbind();

	glm::vec4 lightColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	glm::vec3 lightPos = glm::vec3(0.5f, 0.5f, 0.5f);
	glm::mat4 lightModel = glm::mat4(1.0f);
	lightModel = glm::translate(lightModel, lightPos);

	glm::vec3 pyramidPos = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::mat4 pyramidModel = glm::mat4(1.0f);
	pyramidModel = glm::translate(pyramidModel, pyramidPos);


	lightShader.Activate();
	glUniformMatrix4fv(glGetUniformLocation(lightShader.ID, "model"), 1, GL_FALSE, glm::value_ptr(lightModel));
	//Exportar al fragment shader
	glUniform4f(glGetUniformLocation(lightShader.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
	shaderProgram.Activate();
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram.ID, "model"), 1, GL_FALSE, glm::value_ptr(pyramidModel));
	//Exportar al shader Program
	glUniform4f(glGetUniformLocation(shaderProgram.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
	glUniform3f(glGetUniformLocation(shaderProgram.ID, "lightPos"), lightPos.x, lightPos.y, lightPos.z);

	//------

	// TEXTURAS
	Texture image1("brick_diffuse.jpg", GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE);
	image1.texUnit(shaderProgram, "tex0", 0);
	//importar el specular map
	Texture specularMap("brick_specular.png", GL_TEXTURE_2D, 1, GL_RGBA, GL_UNSIGNED_BYTE);
	specularMap.texUnit(shaderProgram, "tex1", 1);

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

		// Exports the camera Position to the Fragment Shader for specular lighting
		glUniform3f(glGetUniformLocation(shaderProgram.ID, "camPos"), camera.Position.x, camera.Position.y, camera.Position.z);
		//inputs (contexto)
		camera.Inputs(window);

		// Actualiza y exporta la matriz de la cámara al Vertex Shader
		camera.Matrix(45.0f, 0.1f, 100.0f, shaderProgram, "camMatrix");
		//------

		//TEXTURE
		image1.Bind();
		specularMap.Bind();
		//------

		// Bind the VAO so OpenGL knows to use it
		VAO1.Bind();
		// Draw primitives, number of indices, datatype of indices, index of indices
		glDrawElements(GL_TRIANGLES, sizeof(indices) / sizeof(int), GL_UNSIGNED_INT, 0);

		//CUBO
		// Dibujar el cubo de luz
		lightShader.Activate();
		camera.Matrix(45.0f, 0.1f, 100.0f, lightShader, "camMatrix");
		lightVAO.Bind();
		glDrawElements(GL_TRIANGLES, sizeof(lightIndices) / sizeof(int), GL_UNSIGNED_INT, 0);


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

	lightVAO.Delete();
	lightVBO.Delete();
	lightEBO.Delete();
	lightShader.Delete();

	shaderProgram.Delete();

	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}