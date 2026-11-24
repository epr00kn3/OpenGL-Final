#include"Texture.h"

Texture::Texture(const char* image, const char* texType, GLuint slot)
{
	// Assigns the type of the texture ot the texture object
	type = texType;
	target = GL_TEXTURE_2D;

	//Almacenar ancho y altura de la imagen y el numero de canales de color
	int widthImg, heightImg, numColCh;
	//Flip vertical
	//ARREGLO ERROR
	stbi_set_flip_vertically_on_load(false);

	//Almacenar la imagen en una matriz de caracteres //ERROR
	unsigned char* bytes = stbi_load(image, &widthImg, &heightImg, &numColCh, 0);

	//Generar objeto de textura (num texturas, puntero de referencia)
	glGenTextures(1, &ID);

	//Insertar textura en la ranura de una unidad de textura
	glActiveTexture(GL_TEXTURE0 + slot);
	unit = slot;
	//Vincular textura (tipo, referencia)
	glBindTexture(GL_TEXTURE_2D, ID);

	//Ajustar configuración de textura (tipo, configuración a modificar, valor a la configuración)
	//ARREGLO ERROR
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	//------
	//CASOS

	//1- Comprobar el numero de canales de color que tiene una textura y se le asigna un tipo de color correspondiente
	//2- Reemplazar en todas partes con el GL_UNSIGNED_BYTE
	if (numColCh == 4)
		glTexImage2D
		(
			GL_TEXTURE_2D,
			0,
			GL_RGBA,
			widthImg,
			heightImg,
			0,
			GL_RGBA,
			GL_UNSIGNED_BYTE,
			bytes
		);
	else if (numColCh == 3)
		glTexImage2D
		(
			GL_TEXTURE_2D,
			0,
			GL_RGBA,
			widthImg,
			heightImg,
			0,
			GL_RGB,
			GL_UNSIGNED_BYTE,
			bytes
		);
	else if (numColCh == 1)
		glTexImage2D
		(
			GL_TEXTURE_2D,
			0,
			GL_RGBA,
			widthImg,
			heightImg,
			0,
			GL_RED,
			GL_UNSIGNED_BYTE,
			bytes
		);
	else
		throw std::invalid_argument("Automatic Texture type recognition failed");



	//-----


	//Generar textura (tipo, nivel, canal, tamaño, tipo de dato de pixeles, datos imagen)
	glGenerateMipmap(GL_TEXTURE_2D);

	//Eliminar datos y desvincular textura
	stbi_image_free(bytes);
	glBindTexture(GL_TEXTURE_2D, 0);

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

//Cubemap
Texture::Texture(const std::vector<std::string>& faces, const char* texType, GLuint slot)
{
	type = texType;
	target = GL_TEXTURE_CUBE_MAP;
	unit = slot;

	stbi_set_flip_vertically_on_load(false);

	glGenTextures(1, &ID);
	glActiveTexture(GL_TEXTURE0 + slot);
	glBindTexture(target, ID);

	for (unsigned int i = 0; i < 6; i++)
	{
		int w, h, ch;
		unsigned char* data = stbi_load(faces[i].c_str(), &w, &h, &ch, 0);
		if (!data)
			throw std::runtime_error("No se pudo cargar la cara: " + faces[i]);

		GLenum fmt = (ch == 4) ? GL_RGBA : GL_RGB;
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, fmt, w, h, 0, fmt, GL_UNSIGNED_BYTE, data);
		stbi_image_free(data);
	}

	glTexParameteri(target, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(target, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(target, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(target, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	glBindTexture(target, 0);
}
//--------

void Texture::Bind()
{
	glActiveTexture(GL_TEXTURE0 + unit);
	glBindTexture(target, ID);
}

void Texture::Unbind()
{
	glBindTexture(target, 0);
}

void Texture::Delete()
{
	glDeleteTextures(1, &ID);
}