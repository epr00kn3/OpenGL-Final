#version 330 core
out vec4 FragColor;
in vec3 crntPos;

uniform vec3 camPos;
uniform vec3 lightDir;
uniform float time;
uniform samplerCube skybox;

void main()
{
   // Ondas: se inclina la normal con cosenos que se mueven
   float dx = 0.15 * cos(crntPos.x * 1.5 + time);
   float dz = 0.15 * cos(crntPos.z * 1.9 + time * 0.8);
   vec3 normal = normalize(vec3(-dx, 1.0, -dz));

   vec3 viewDir = normalize(camPos - crntPos);

   // Fresnel: mas reflejo cuanto mas rasante se mira
   float fresnel = pow(1.0 - max(dot(viewDir, normal), 0.0), 3.0);

   // Reflejo real del cielo (cubemap)
   vec3 reflDirSky = reflect(-viewDir, normal);
   reflDirSky.y = abs(reflDirSky.y);              // nunca reflejar la mitad de abajo del cubemap
   vec3 skyReflection = texture(skybox, reflDirSky).rgb;

   // Color del agua: profundo -> reflejo del cielo
   vec3 deepColor = vec3(0.23, 0.13, 0.09);
   float reflAmount = clamp(0.25 + 0.75 * fresnel, 0.0, 1.0);
   vec3 color = mix(deepColor, skyReflection, reflAmount);

   // Brillo del sol sobre el agua
   vec3 reflDir = reflect(-lightDir, normal);
   float spec = pow(max(dot(viewDir, reflDir), 0.0), 64.0);
   color += vec3(1.0, 0.75, 0.45) * spec * 0.6;

   // Fundido con el horizonte a lo lejos (oculta el borde del plano)
   float dist = length(camPos - crntPos);
   float fade = smoothstep(30.0, 600.0, dist);
   color = mix(color, vec3(0.73, 0.34, 0.22), fade);

   FragColor = vec4(color, 0.9);
}