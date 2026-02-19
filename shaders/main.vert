#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec3 aTangent;
layout (location = 3) in vec3 aBitangent;
layout (location = 4) in vec2 aTexCoords; 

out vec3 Position;
out vec2 TexCoords;
out mat3 TBN;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
  vec3 T = normalize(mat3(model) * aTangent);
  vec3 B = normalize(mat3(model) * aBitangent);
  vec3 N = normalize(mat3(model) * aNormal);
  TBN = mat3(T, B, N);

  Position = vec3(model * vec4(aPos, 1.0));
  TexCoords = aTexCoords;

  gl_Position = projection * view * vec4(Position, 1.0);
}  
