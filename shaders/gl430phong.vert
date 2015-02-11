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

//  Output to next shader
out vec3 FrontColor;

vec4 phong()
{
	// Position in eye coordinates
	vec3 pos = vec3(ModelViewMatrix * Vertex);

	// Normal in eye coordinates
	vec3 N = normalize(transpose(inverse(mat3(ModelViewMatrix))) * Normal);

	// Light vector
	vec3 L = normalize(vec3(ModelViewMatrix * light.position) - pos);

	// Reflection vector
	vec3 R = reflect(-L, N);

	// View vector in eye coordinates
	vec3 V = normalize(-pos);

	// Diffuse light intensity
	float Id = max(0.0, dot(N, L));

	// Specular light intensity
	float Is = (Id > 0.0) ? max(0.0, dot(R, V)) : 0.0;

	return light.global + light.ambient + Id*light.diffuse + Is*light.specular;
}

void main()
{	
   //  Pass colors to fragment shader (will be interpolated)
   FrontColor = Color * vec3(phong());

	// FrontColor = abs(Normal);
   //  Set transformed vertex location
   gl_Position =  ProjectionMatrix * ModelViewMatrix * Vertex;
}
