#version 150

in vec3 in_pos;
in vec3 in_norm;
in vec2 in_tc;
in vec3 in_tan;

uniform mat4 model;
uniform mat4 model_normal;
uniform mat4 view;
uniform mat4 view_normal;
uniform mat4 proj;

uniform vec3 cam_pos;
uniform vec3 dirlight_dir;
uniform vec3 pointlight_pos;

out vec3 pos_ws;
out vec3 to_dirlight;
out vec3 to_pointlight;
out vec3 v;
out vec2 tc;

void main() {
	vec3 bi_tan = cross(in_norm, in_tan);
	// TODO:
	// Transformieren Sie die T, B und N Komponenten in den World-Space und setzen Sie die TBN Matrix auf

	pos_ws = (model * vec4(in_pos, 1.0)).xyz;

	// TODO:
	// Überführen Sie die Richtung zur Kamera und die beiden Lichtrichtungen in den TS

	tc = in_tc;
	gl_Position = proj * view * vec4(pos_ws, 1);
}
