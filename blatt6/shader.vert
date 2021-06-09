#version 130

in vec3 local_vertex;
in vec3 local_color;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

out vec3 color;

void main() {

	// TODO: Aufgabe 1.5
	// local_vertex soll im World-Space verstanden werden
	vec4 eye = view * vec4(local_vertex, 1.0);

	// TODO: Aufgabe 1.4
	// local_vertex soll im Eye-Space verstanden werden
	vec4 clip = proj * eye;

	gl_Position = clip;

	color = local_color;
}
