#version 330 core
out vec4 FragColor;
in vec3 crntPos;

uniform vec3 camPos;
uniform vec3 lightDir;
uniform float time;

void main()
{
   // Ondas que se inclina la normal con senos y cosenos que se mueven
   float dx = 0.15 * cos(crntPos.x * 1.5 + time);
   float dz = 0.15 * cos(crntPos.z * 1.9 + time * 0.8);
   vec3 normal = normalize(vec3(-dx, 1.0, -dz));

   vec3 viewDir = normalize(camPos - crntPos);

   // Fresnel
   float fresnel = pow(1.0 - max(dot(viewDir, normal), 0.0), 3.0);

   vec3 deepColor = vec3(0.075, 0.212, 0.314);   // 133650
   vec3 skyColor  = vec3(0.357, 0.459, 0.525);   // 5B7586
   vec3 color = mix(deepColor, skyColor, fresnel);

   // Brillo de la luz sobre el agua
   vec3 reflDir = reflect(-lightDir, normal);
   float spec = pow(max(dot(viewDir, reflDir), 0.0), 64.0);
   color += vec3(1.0) * spec * 0.5;

   FragColor = vec4(color, 0.85);
}