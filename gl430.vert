#version 430 core

//  Transformation matrices
uniform mat4 ModelViewMatrix;
uniform mat4 ProjectionMatrix;

uniform Light {
	// Position
	vec4 position;
	
	// Colors
	vec4 global;
	vec4 ambient;
	vec4 diffuse;
	vec4 specular;
} light;

//  Vertex attributes (input)
layout(location = 0) in vec4 Vertex;
layout(location = 1) in vec3 Normal;
layout(location = 2) in vec3 Color;
layout(location = 3) in vec2 TextCoord;


//  Output to next shader
out vec3 IPosition;
out vec3 INormal;
out vec3 FrontColor;
out vec2 ITextCoord;

void main()
{	
   //  Pass colors to fragment shader (will be interpolated)
   IPosition = vec3(ModelViewMatrix * Vertex);
   INormal = normalize(transpose(inverse(mat3(ModelViewMatrix))) * Normal);
   FrontColor = Color;
   ITextCoord = TextCoord;

	// FrontColor = abs(Normal);
   //  Set transformed vertex location
   gl_Position =  ProjectionMatrix * ModelViewMatrix * Vertex;
}
