#include"Texture.h"

Texture::Texture(const char* image, GLenum texType, GLenum slot, GLenum format, GLenum pixelType)
{
	// Assigns the type of the texture ot the texture object
	type = texType;

	//Almacenar ancho y altura de la imagen y el numero de canales de color
	int widthImg, heightImg, numColCh;
	//Flip vertical
	stbi_set_flip_vertically_on_load(true);

	//Almacenar la imagen en una matriz de caracteres
	unsigned char* bytes = stbi_load(image, &widthImg, &heightImg, &numColCh, 4);

	//Generar objeto de textura (num texturas, puntero de referencia)
	glGenTextures(1, &ID);
	//Insertar textura en la ranura de una unidad de textura
	glActiveTexture(GL_TEXTURE0 + slot);
	unit = slot;
	//Vincular textura (tipo, referencia)
	glBindTexture(texType, ID);

	//Ajustar configuración de textura (tipo, configuración a modificar, valor a la configuración)
	glTexParameteri(texType, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
	glTexParameteri(texType, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glTexParameteri(texType, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(texType, GL_TEXTURE_WRAP_T, GL_REPEAT);

	//Generar textura (tipo, nivel, canal, tamaño, tipo de dato de pixeles, datos imagen)
	glTexImage2D(texType, 0, GL_RGBA, widthImg, heightImg, 0, format, pixelType, bytes);
	glGenerateMipmap(texType);

	//Eliminar datos y desvincular textura
	stbi_image_free(bytes);
	glBindTexture(texType, 0);

}

void Texture::texUnit(Shader& shader, const char* uniform, GLuint unit)
{
	// Gets the location of the uniform
	GLuint texUni = glGetUniformLocation(shader.ID, uniform);
	// Shader needs to be activated before changing the value of a uniform
	shader.Activate();
	// Sets the value of the uniform
	glUniform1i(texUni, unit);
}

void Texture::Bind()
{
	glActiveTexture(GL_TEXTURE0 + unit);
	glBindTexture(type, ID);
}

void Texture::Unbind()
{
	glBindTexture(type, 0);
}

void Texture::Delete()
{
	glDeleteTextures(1, &ID);
}