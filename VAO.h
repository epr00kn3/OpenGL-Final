#ifndef VAO_CLASS_H
#define VAO_CLASS_H

#include<glad/glad.h>
#include"VBO.h"

//VAO: Como OpenGL debe interpretar los datos SIEMPRE VAO ANTES DE VBO
//Bufer: (lotes grandes) zona de memoria que almacena datos temporalmente, evitar interrupciones o bloqueos en sistema
class VAO
{
public:

	// ID reference for the Vertex Array Object
	GLuint ID;
	// Constructor that generates a VAO ID
	VAO();

	// Links a VBO to the VAO using a certain layout
	void LinkAttrib(VBO& VBO, GLuint layout, GLuint numComponents, GLenum type, GLsizeiptr stride, void* offset);

	//Vinculación: Convertir un objeto determinado en el objeto actual
	// Binds the VAO
	void Bind();

	//Se desvincula por si acaso se modifica accidentalmente
	// Unbinds the VAO
	void Unbind();

	//Eliminar todos los objetos que se han creado
	// Deletes the VAO
	void Delete();
};
#endif
