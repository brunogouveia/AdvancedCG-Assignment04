#version 430 core

//  Transformation matrices
uniform Tranformations {
	// Projection Matrix
	mat4 ProjectionMatrix;
	// ModelView Matrix
	mat4 ModelViewMatrix;
	// ModelViewProjection Matrix
	mat4 MVP;
	// Normal matrix;
	mat4 normalMatrix;
} tranformations;

uniform Light {
	// Position
	vec4 position;
	
	// Colors
	vec4 global;
	vec4 ambient;
	vec4 diffuse;
	vec4 specular;
} light;


uniform vec3 center;
uniform float radius;

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
   // Compute the normalized vector
   //   The idea is to use a cube to get a lot of distributed vertices and normalize those vertices
   // so then all the vertices will be equally distant from center (a sphere).
   //   This is definitely not the best way to do this, I just thought that would be interesting do this
   // in the shader
   vec4 v = vec4(radius*normalize(Vertex.xyz) + center, 1.0);

   //  Pass colors to fragment shader (will be interpolated)
   IPosition = vec3(tranformations.ModelViewMatrix * v);
   // The normal is a vector parallel to vertex
   INormal = normalize(mat3(tranformations.normalMatrix) * Vertex.xyz);
   FrontColor = Color;
   ITextCoord = TextCoord;

   //  Set transformed vertex location
   gl_Position =  (tranformations.MVP * v);
}
