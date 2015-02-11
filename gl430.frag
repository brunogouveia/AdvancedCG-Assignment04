#version 430 core

//  Transformation matrices
uniform Tranformations {
	// Projection Matrix
	mat4 ProjectionMatrix;
	// ModelView Matrix
	mat4 ModelViewMatrix;
	// ModelViewProjection Matrix
	mat4 MVP;
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
 

//  Input from previous shader
in vec3 IPosition;
in vec3 INormal;
in vec3 FrontColor;
in vec2 ITextCoord;

uniform sampler2D text;

//  Fragment color
layout (location=0) out vec4 Fragcolor;


vec4 phong()
{
	// Position in eye coordinates
	vec3 pos = IPosition;

	// Normal in eye coordinates
	vec3 N = INormal;

	// Light vector
	vec3 L = normalize(vec3(tranformations.ModelViewMatrix * light.position) - pos);

	// Reflection vector
	vec3 R = reflect(-L, N);

	// View vector in eye coordinates
	vec3 V = normalize(-pos);

	// Diffuse light intensity
	float Id = max(0.0, dot(N, L));

	// Specular light intensity
	float Is = (Id > 0.0) ? pow(max(0.0, dot(R, V)), 100) : 0.0;

	return light.global + light.ambient + Id*light.diffuse + Is*light.specular;
}


void main()
{
   Fragcolor = texture(text,ITextCoord.st)*vec4(FrontColor,1.0)*phong();
}
