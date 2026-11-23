#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

//vertex shader
const char* vertexShaderSource = R"(
#version 330 core
layout (location = 0) in vec3 aPos;
void main()
{
    gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
}
)";

//fragment shader
const char* fragmentShaderSource = R"(
#version 330 core
out vec4 FragColor;
void main()
{
    FragColor = vec4(0.8, 0.3, 0.02, 1.0);
}
)";


int main() {

	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); //Pista de la version mayor de OpenGL que se utiliza
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); //Pista de la version menor de OpenGL que se utiliza
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); //Que perfil (paquete de funciones) de OpenGL se quiere, CORE (Funciones modernas

	//Llamar la ventana y usar la funcion crear una ventana
	GLFWwindow* window = glfwCreateWindow(800, 600, "glTF", NULL, NULL);
	//Comprobación de errores
	if (window == NULL)
	{
		std::cout << "Falló en crear ventana :(" << std::endl;
		glfwTerminate();
		return -1;
	}
	//Usar la ventana (contexto actual)
	glfwMakeContextCurrent(window);

	gladLoadGL();
	//Se le dice el área de la ventana a OpenGL en la que se quiere renderizar
	glViewport(0, 0, 800, 600);

	//Referencia para almacenar el shader de vértices
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	//Compilar shader
	glCompileShader(vertexShader);
	glCompileShader(fragmentShader);
	//Encapsulamiento de ambos shaders en Shader Program
	GLuint shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	GLfloat vertices[] =
	{
		-0.5f, -0.5f * float(sqrt(3)) / 3, 0.0f, // Lower left corner
		0.5f, -0.5f * float(sqrt(3)) / 3, 0.0f, // Lower right corner
		0.0f, 0.5f * float(sqrt(3)) * 2 / 3, 0.0f // Upper corner
	};

	//VAO: Como OpenGL debe interpretar los datos SIEMPRE VAO ANTES DE VBO
	//Bufer: (lotes grandes) zona de memoria que almacena datos temporalmente, evitar interrupciones o bloqueos en sistema
	GLuint VAO, VBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	//Vinculación: Convertir un objeto determinado en el objeto actual
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//Almacenar los vértices en el VBO
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	//Como OpenGL debe leer el VBO
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glad_glEnableVertexAttribArray(0);

	//Se desvincula por si acaso se modifica accidentalmente
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);


	//Para ver el color toca intercambiar los buffers (Bufer posterior: mi color, Bufer frontal: color predeterminado)
	glfwSwapBuffers(window);


	//Ciclo while que termina solo cuando la ventana se cierra (pulsar botón de cierre)
	while (!glfwWindowShouldClose(window))
	{
		//Borrar el color de un bufer y darle otro color
		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		//Activar el shaderProgram
		glUseProgram(shaderProgram);
		glBindVertexArray(VAO);
		//Dibujar
		glDrawArrays(GL_TRIANGLES, 0, 3);

		glfwSwapBuffers(window);
		//Cambio de tamaño, aparición de ventana, etc
		glfwPollEvents();
	}

	//Eliminar todos los objetos que se han creado
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteProgram(shaderProgram);

	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}