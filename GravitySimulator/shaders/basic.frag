#version 330 core
out vec4 FragColor;

uniform float brightness;
uniform vec3 circleColor;

void main()
{
    FragColor = vec4(circleColor * brightness, 1.0);  // grayscale color
}