#version 150

in vec3 in_pos;
in vec3 in_norm;
// TODO shader input
in vec2 in_tc;
uniform sampler2D diffuse_texture;

uniform mat4 model;
uniform mat4 model_normal;
uniform mat4 view;
uniform mat4 view_normal;
uniform mat4 proj;

out vec3 pos_ws;
out vec3 n_ws;
out vec2 tc;
// TODO shader output


void main() {
	n_ws = normalize(mat3(model_normal) * in_norm);
	pos_ws = (model * vec4(in_pos, 1.0)).xyz;
	// TODO textur koordinate an FS weiterleiten
	tc = in_tc;
	gl_Position = proj * view * model * vec4(in_pos, 1.0);
}
