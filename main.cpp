#include"Model.h"


//Ancho y altura de la ventana
const unsigned int width = 800;
const unsigned int height = 800;


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

	// Se ocupa de todas las cosas relacionadas con la luz
	glm::vec4 lightColor = glm::vec4(0.357, 0.459, 0.525, 1.0f);
	glm::vec3 lightPos = glm::vec3(0.0f, 5.0f, 5.0f);
	glm::mat4 lightModel = glm::mat4(1.0f);
	lightModel = glm::translate(lightModel, lightPos);

	shaderProgram.Activate();
	glUniform4f(glGetUniformLocation(shaderProgram.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
	glUniform3f(glGetUniformLocation(shaderProgram.ID, "lightPos"), lightPos.x, lightPos.y, lightPos.z);



	//Prueba de profundidad (que triangulo va encima de cual)
	glEnable(GL_DEPTH_TEST);

	//Crear objeto camara
	Camera camera(width, height, glm::vec3(0.0f, 0.0f, 2.0f));

	//MODELO
	Model boat("Models/boat_fishing/scene.gltf");


	//Fondo de degradado
	Shader bgShader("bg.vert", "bg.frag");
	VAO bgVAO;   // VAO vacío, necesario en perfil core
	bgShader.Activate();
	glUniform3f(glGetUniformLocation(bgShader.ID, "topColor"), 0.153, 0.271, 0.365f);
	glUniform3f(glGetUniformLocation(bgShader.ID, "bottomColor"), 0.031, 0.090, 0.133);
	//------

	//AGUA wazaaaaaaaaaa
	Shader waterShader("water.vert", "water.frag");

	std::vector<Vertex> waterVerts =
	{
		Vertex{ glm::vec3(-50.0f, 0.0f,  50.0f), glm::vec3(0, 1, 0), glm::vec3(1.0f), glm::vec2(0.0f, 0.0f) },
		Vertex{ glm::vec3(-50.0f, 0.0f, -50.0f), glm::vec3(0, 1, 0), glm::vec3(1.0f), glm::vec2(0.0f, 1.0f) },
		Vertex{ glm::vec3(50.0f, 0.0f, -50.0f), glm::vec3(0, 1, 0), glm::vec3(1.0f), glm::vec2(1.0f, 1.0f) },
		Vertex{ glm::vec3(50.0f, 0.0f,  50.0f), glm::vec3(0, 1, 0), glm::vec3(1.0f), glm::vec2(1.0f, 0.0f) }
	};
	std::vector<GLuint> waterIndices = { 0, 1, 2, 0, 2, 3 };
	std::vector<Texture> noTextures;
	Mesh water(waterVerts, waterIndices, noTextures);

	// altura del agua
	float waterLevel = 1.5f;
	glm::mat4 waterModel = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, waterLevel, 0.0f));
	//------


	//Ciclo while que termina solo cuando la ventana se cierra (pulsar botón de cierre)
	while (!glfwWindowShouldClose(window))
	{
		//Borrar el color de un bufer y darle otro color, tambien para el test de profundidad
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


		//Fondo degradado wazaaaa
		glDisable(GL_DEPTH_TEST);
		bgShader.Activate();
		bgVAO.Bind();
		glDrawArrays(GL_TRIANGLES, 0, 3);
		glEnable(GL_DEPTH_TEST);



		// Tell OpenGL which Shader Program we want to use
		shaderProgram.Activate();

		//-------
		//CAMERA

		// Exports the camera Position to the Fragment Shader for specular lighting
		glUniform3f(glGetUniformLocation(shaderProgram.ID, "camPos"), camera.Position.x, camera.Position.y, camera.Position.z);
		//inputs (contexto)
		camera.Inputs(window);

		// Actualiza y exporta la matriz de la cámara al Vertex Shader
		camera.updateMatrix(45.0f, 0.1f, 100.0f);

		//Dibujar MODELO
		boat.Draw(shaderProgram, camera);

		//AGUA wazaaaa
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		waterShader.Activate();
		glUniform1f(glGetUniformLocation(waterShader.ID, "time"), (float)glfwGetTime());
		glUniform3f(glGetUniformLocation(waterShader.ID, "camPos"), camera.Position.x, camera.Position.y, camera.Position.z);
		glm::vec3 lightDir = glm::normalize(glm::vec3(0.5f, 1.0f, 0.8f));
		glUniform3f(glGetUniformLocation(waterShader.ID, "lightDir"), lightDir.x, lightDir.y, lightDir.z);
		water.Draw(waterShader, camera, waterModel);

		glDisable(GL_BLEND);
		//-----



		//Para ver el color toca intercambiar los buffers (Bufer posterior: mi color, Bufer frontal: color predeterminado)
		glfwSwapBuffers(window);
		//Cambio de tamaño, aparición de ventana, etc
		glfwPollEvents();
	}

	// Delete all the objects we've created
	shaderProgram.Delete();
	bgShader.Delete();
	bgVAO.Delete();
	waterShader.Delete();
	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}