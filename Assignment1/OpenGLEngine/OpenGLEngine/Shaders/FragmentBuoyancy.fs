#version 330 core
out vec4 FragColor;

uniform vec4 colAbove;
uniform vec4 colBelow;
uniform float liquidHeight;
in float currentHeight;

void main()
{
    if(currentHeight > liquidHeight){
        FragColor = colAbove; 
    }
    else {
        FragColor = colBelow; 
    }
}