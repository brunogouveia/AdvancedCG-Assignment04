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


//  Input from previous shader
in vec3 Position;
in vec3 Norm;
in vec3 FrontColor;

//  Fragment color
layout (location=0) out vec4 Fragcolor;

vec4 phong()
{
	// Position in eye coordinates
	vec3 pos = Position;

	// Normal in eye coordinates
	vec3 N = normalize(Norm);

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

	// if (Is > 0.0)
	// 	return vec4(0.0, 1.0, 0.0, 1.0);

	// return vec4(N,1.0);

	return light.global + light.ambient + Id*light.diffuse + Is*light.specular;


}

void main()
{
   Fragcolor = vec4(FrontColor*vec3(phong()),1.0);
}
