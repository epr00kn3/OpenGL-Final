#ifndef MODEL_CLASS_H
#define MODEL_CLASS_H

#include <nlohmann/json.hpp>
#include "Mesh.h"

using json = nlohmann::json;


class Model {
public:
	//Estructura que recibe como parámetro el nombre de un archivo
	Model(const char* file);

	//Funcion de dibujo que recibe como parámetro un sombreador y una cámara
	void Draw(Shader& shader, Camera& camera, glm::mat4 modelMatrix = glm::mat4(1.0f));


private:

	//Almacenar nombre e archivo, vector de bytes con todos los datos del modelo y un objeto JSON
	const char* file;
	std::vector<unsigned char> data;
	json JSON;

	//Recibe como argumento el indice de la malla que se quiere cargar
	std::vector<Mesh> meshes;

	//Vectores para diferentes tipos de matrices
	std::vector<glm::vec3> translationsMeshes;
	std::vector<glm::quat> rotationsMeshes;
	std::vector<glm::vec3> scalesMeshes;
	std::vector<glm::mat4> matricesMeshes;

	// Evita que las texturas se carguen dos veces
	std::vector<std::string> loadedTexName;
	std::vector<Texture> loadedTex;
	std::vector<Texture> getTextures(unsigned int indMesh);

	//Vector de mallas para almacenar todas las mallas cargadas
	void loadMesh(unsigned int indMesh);

	//Funcion que recibe el indice de un nodo y una matriz
	//Recorre un nodo de manera recursiva, así que básicamente recorre todos los nodos conectados
	void traverseNode(unsigned int nextNode, glm::mat4 matrix = glm::mat4(1.0f));

	//Proporciona un vector de bytes a partir de un archivo de datos binarios externo
	std::vector<unsigned char> getData();


	//HOJAS
	std::vector<float> getFloats(json accessor);
	std::vector<GLuint> getIndices(json accessor);
	std::vector<Texture> getTextures();


	//Funcion que toma los vertices, normales y coordenadas de textura
	std::vector<Vertex> assembleVertices
	(
		std::vector<glm::vec3> positions,
		std::vector<glm::vec3> normals,
		std::vector<glm::vec2> texUVs
	);

	//Funciones que tomarán los vectores de matriz generados por la funciones get y agrupa en vectores glm
	std::vector<glm::vec2> groupFloatsVec2(std::vector<float> floatVec);
	std::vector<glm::vec3> groupFloatsVec3(std::vector<float> floatVec);
	std::vector<glm::vec4> groupFloatsVec4(std::vector<float> floatVec);
};

#endif