#version 150

// TODO
// Teilaufgabe 3
// Uniforms und Input-Attribute
in vec3 n_ws;
in vec3 pos_ws;

uniform vec4 k_diff;
uniform vec4 k_spec;
uniform vec3 cam_pos;
uniform vec3 pointlight_pos;
uniform vec3 pointlight_col;
uniform float pointlight_scale;
uniform vec3 dirlight_dir;
uniform vec3 dirlight_col;
uniform float dirlight_scale;

out vec4 out_col;

void main() {
	// TODO
	// Teilaufgabe 3
	// Phong-Beleuchtung für zwei Lichtquellen mit Diffuse und Spekularteil.
	//out_col = vec4(0.6,0.1,0.1,1);

	vec3 col = vec3(0);
	vec3 v = normalize(cam_pos - pos_ws);
	vec3 n = normalize(n_ws);
	vec3 l, r;

	// Directional Light
	l = -dirlight_dir;
	r = 2*n*dot(n,l) - l;
	col += dirlight_scale * k_diff.rgb * max(dot(n,l),0) * dirlight_col;
	col += dirlight_scale * k_spec.rgb* pow(max(dot(v,r),0),4) * dirlight_col;

	// Point Light
	l = normalize(pointlight_pos - pos_ws);
	r = 2*n*dot(n,l) - l;
	col += pointlight_scale * k_diff.rgb * max(dot(n,l),0) * pointlight_col;
	col += pointlight_scale * k_spec.rgb * pow(max(dot(v,r),0),4) * pointlight_col;

	out_col = vec4(col, 1.0);
}
