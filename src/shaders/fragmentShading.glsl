#version 330 core

out vec4 FragColor;

in vec3 ourColor;
in vec2 TexCoord;

vec2 newCoord = vec2(-TexCoord.x, TexCoord.y);

uniform sampler2D ourTexture;
uniform sampler2D ourTexture2;

void main()
{
    FragColor = mix(texture(ourTexture, TexCoord), texture(ourTexture2, newCoord), 0.2);
}