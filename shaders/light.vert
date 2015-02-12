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


//  Output to next shader
out vec3 FrontColor;

float max(vec3 v)
{
	float m = (v.x > v.y) ? v.x : v.y;
	return (m > v.z) ? m : v.z;
}

void main()
{	
   // Compute the normalized vector
   //   The idea is to use a cube to get a lot of distributed vertices and normalize those vertices
   // so then all the vertices will be equally distant from center (a sphere).
   //   This is definitely not the best way to do this, I just thought that would be interesting do this
   // in the shader
   vec4 v = vec4(radius*normalize(Vertex.xyz) + center, 1.0);

   //  Pass colors to fragment shader (will be interpolated)
   FrontColor = light.global.rgb + light.ambient.rgb;
   // Just make sure that Frontcolor is not dark
   FrontColor /= max(FrontColor);

   //  Set transformed vertex location
   gl_Position =  (tranformations.MVP * v);
}
