#ifndef SHADER_CLASS_H
#define SHADER_CLASS_H

#include<glad/glad.h>
#include<string>
#include<fstream>
#include<sstream>
#include<iostream>
#include<cerrno>


//Genera el contenido de un archivo de texto como una cadena de caracteres
std::string get_file_contents(const char* filename);

//Programa de sombreado en openGL
class Shader
{
public:
	// Reference ID of the Shader Program
	GLuint ID;
	// Constructor that build the Shader Program from 2 different shaders, Recibe los códigos fuente del sombreador
	Shader(const char* vertexFile, const char* fragmentFile);

	// Activates the Shader Program
	void Activate();
	// Deletes the Shader Program
	void Delete();

	// Checks if the different Shaders have compiled properly
	void compileErrors(unsigned int shader, const char* type);
};
#endif