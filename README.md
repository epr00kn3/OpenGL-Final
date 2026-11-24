# OpenGL-Final

Greisse Yuliana Mendivelso Peña
Código: 6014441

Renderizador OpenGL 3.3 Core organizado en capas: main.cpp crea la ventana con GLFW, carga los recursos y ejecuta el bucle principal, donde anima el barco sobre dos curvas Bézier encadenadas y dibuja en orden el skybox, el barco y el agua. Debajo, Model lee archivos glTF (JSON + binario) y crea un Mesh por nodo; cada Mesh agrupa un VAO, VBO y EBO junto con sus Texture (clase que maneja tanto texturas 2D como cubemaps con stb_image); Camera gestiona el input y la matriz de proyección × vista; y Shader compila los tres programas GLSL: default (iluminación Phong para el barco), cm (skybox) y water (olas con normales animadas, Fresnel y reflejo del cubemap). Todo se apoya en GLFW, glad, GLM, nlohmann/json y stb_image.
