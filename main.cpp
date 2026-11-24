#include"Model.h"


//Ancho y altura de la ventana
const unsigned int width = 1600;
const unsigned int height = 800;

//Curva de BEZIER
glm::vec3 bezier(const glm::vec3& p0, const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& p3, float t)
{
	float u = 1.0f - t;
	return u * u * u * p0 + 3.0f * u * u * t * p1 + 3.0f * u * t * t * p2 + t * t * t * p3;
}

glm::vec3 bezierTangent(const glm::vec3& p0, const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& p3, float t)
{
	float u = 1.0f - t;
	return 3.0f * u * u * (p1 - p0) + 6.0f * u * t * (p2 - p1) + 3.0f * t * t * (p3 - p2);
}
//---

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
	glm::vec4 lightColor = glm::vec4(1.0f, 0.62f, 0.32f, 1.0f);
	glm::vec3 lightPos = glm::vec3(0.0f, 5.0f, 5.0f);
	glm::mat4 lightModel = glm::mat4(1.0f);
	lightModel = glm::translate(lightModel, lightPos);

	shaderProgram.Activate();
	glUniform4f(glGetUniformLocation(shaderProgram.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
	glUniform3f(glGetUniformLocation(shaderProgram.ID, "lightPos"), lightPos.x, lightPos.y, lightPos.z);

	

	//Prueba de profundidad (que triangulo va encima de cual)
	glEnable(GL_DEPTH_TEST);

	//Crear objeto camara y su posicion
	Camera camera(width, height, glm::vec3(0.0f, 3.0f, 4.0f));

	//MODELO
	Model boat("Models/boat_fishing/scene.gltf");


	//CUBEMAP
	Shader skyboxShader("cm.vert", "cm.frag");

	// Orden de caras (+X, -X, +Y, -Y, +Z, -Z)
	Texture skybox({
		"cubemap/right.jpg",
		"cubemap/left.jpg",
		"cubemap/top.jpg",
		"cubemap/bottom.jpg",
		"cubemap/front.jpg",
		"cubemap/back.jpg"
		}, "cubemap", 5);
	skybox.texUnit(skyboxShader, "cubemap", 5);

	// Cubo del skybox 
	std::vector<Vertex> skyboxVerts =
	{
		Vertex{ glm::vec3(-1.0f, -1.0f,  1.0f), glm::vec3(0), glm::vec3(0), glm::vec2(0) },
		Vertex{ glm::vec3(1.0f, -1.0f,  1.0f), glm::vec3(0), glm::vec3(0), glm::vec2(0) },
		Vertex{ glm::vec3(1.0f, -1.0f, -1.0f), glm::vec3(0), glm::vec3(0), glm::vec2(0) },
		Vertex{ glm::vec3(-1.0f, -1.0f, -1.0f), glm::vec3(0), glm::vec3(0), glm::vec2(0) },
		Vertex{ glm::vec3(-1.0f,  1.0f,  1.0f), glm::vec3(0), glm::vec3(0), glm::vec2(0) },
		Vertex{ glm::vec3(1.0f,  1.0f,  1.0f), glm::vec3(0), glm::vec3(0), glm::vec2(0) },
		Vertex{ glm::vec3(1.0f,  1.0f, -1.0f), glm::vec3(0), glm::vec3(0), glm::vec2(0) },
		Vertex{ glm::vec3(-1.0f,  1.0f, -1.0f), glm::vec3(0), glm::vec3(0), glm::vec2(0) }
	};
	std::vector<GLuint> skyboxIndices =
	{
		1,2,6, 6,5,1,   // derecha
		0,4,7, 7,3,0,   // izquierda
		4,5,6, 6,7,4,   // arriba
		0,3,2, 2,1,0,   // abajo
		0,1,5, 5,4,0,   // atrás
		3,7,6, 6,2,3    // frente
	};

	VAO skyboxVAO;
	skyboxVAO.Bind();
	VBO skyboxVBO(skyboxVerts);
	EBO skyboxEBO(skyboxIndices);
	skyboxVAO.LinkAttrib(skyboxVBO, 0, 3, GL_FLOAT, sizeof(Vertex), (void*)0);
	skyboxVAO.Unbind();
	skyboxVBO.Unbind();
	skyboxEBO.Unbind();
	//------

	//AGUA wazaaaaaaaaaa
	Shader waterShader("water.vert", "water.frag");
	skybox.texUnit(waterShader, "skybox", 5);

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

	//Curva de BEZIER
	// Tramo A (P0 - P3)
	glm::vec3 P0(-20.0f, 0.0f, 0.0f);
	glm::vec3 P1(-20.0f, 0.0f, -25.0f);
	glm::vec3 P2(20.0f, 0.0f, -25.0f);
	glm::vec3 P3(20.0f, 0.0f, 0.0f);
	// Tramo B (P3 - P0)
	glm::vec3 P4 = 2.0f * P3 - P2;
	glm::vec3 P5 = 2.0f * P0 - P1;

	//segundos por tramo
	float segmentDuration = 8.0f;  
	// qué tan hundido esta el barco (- baja, + sube)
	float boatY = waterLevel - 1.0f;    
	// si el barco va de lado probar glm::radians(90.0f), -90 o 180
	float headingOffset = 0.0f;     
	//------





	//Ciclo while que termina solo cuando la ventana se cierra (pulsar botón de cierre)
	while (!glfwWindowShouldClose(window))
	{
		//Borrar el color de un bufer y darle otro color, tambien para el test de profundidad
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
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
		camera.updateMatrix(45.0f, 0.1f, 100.0f);
		
		//CUBEMAP
		glDisable(GL_DEPTH_TEST);
		skyboxShader.Activate();
		glm::mat4 view = glm::mat4(glm::mat3(glm::lookAt(camera.Position, camera.Position + camera.Orientation, camera.Up)));
		glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)width / height, 0.1f, 100.0f);
		glUniformMatrix4fv(glGetUniformLocation(skyboxShader.ID, "view"), 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(glGetUniformLocation(skyboxShader.ID, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

		skybox.Bind();
		skyboxVAO.Bind();
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
		skyboxVAO.Unbind();
		skybox.Unbind();

		glEnable(GL_DEPTH_TEST);
		//-------

		//Curva de BEZIER
		float time = (float)glfwGetTime();
		float tt = fmod(time / segmentDuration, 2.0f);   // 0..2: [0,1) tramo A, [1,2) tramo B

		glm::vec3 pos, tangent;
		if (tt < 1.0f)
		{
			pos = bezier(P0, P1, P2, P3, tt);
			tangent = bezierTangent(P0, P1, P2, P3, tt);
		}
		else
		{
			float u = tt - 1.0f;
			pos = bezier(P3, P4, P5, P0, u);
			tangent = bezierTangent(P3, P4, P5, P0, u);
		}

		// Orientación: giro en Y para que el frente del barco (+Z) apunte a la tangente
		float yaw = atan2(tangent.x, tangent.z) + headingOffset;
		float bob = 0.1f * sin(time * 1.5f);             // sube y baja con las olas

		glm::mat4 boatMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(pos.x, boatY + bob, pos.z));
		boatMatrix = glm::rotate(boatMatrix, yaw, glm::vec3(0.0f, 1.0f, 0.0f));


		//Dibujar MODELO

		boat.Draw(shaderProgram, camera, boatMatrix);

		//AGUA wazaaaa
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		waterShader.Activate();
		glUniform1f(glGetUniformLocation(waterShader.ID, "time"), (float)glfwGetTime());
		glUniform3f(glGetUniformLocation(waterShader.ID, "camPos"), camera.Position.x, camera.Position.y, camera.Position.z);
		glm::vec3 lightDir = glm::normalize(glm::vec3(1.0f, 0.26f, 0.0f));
		glUniform3f(glGetUniformLocation(waterShader.ID, "lightDir"), lightDir.x, lightDir.y, lightDir.z);
		skybox.Bind();
		water.Draw(waterShader, camera, waterModel);
		skybox.Unbind();

		glDisable(GL_BLEND);
		//-----



		//Para ver el color toca intercambiar los buffers (Bufer posterior: mi color, Bufer frontal: color predeterminado)
		glfwSwapBuffers(window);
		//Cambio de tamaño, aparición de ventana, etc
		glfwPollEvents();
	}

	// Delete all the objects we've created
	shaderProgram.Delete();
	skyboxShader.Delete();
	skybox.Delete();
	skyboxVAO.Delete();
	skyboxVBO.Delete();
	skyboxEBO.Delete();
	waterShader.Delete();
	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}