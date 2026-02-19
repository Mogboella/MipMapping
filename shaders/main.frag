#version 330 core
out vec4 FragColor;

in vec3 Position;
in vec2 TexCoords;
in mat3 TBN;

uniform sampler2D tex;

void main(){
  FragColor = texture(tex, TexCoords);
}
