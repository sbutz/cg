#version 150

// TODO 
// Teilaufgabe 2
// Uniform aus Framework
uniform vec4 k_diff;

out vec4 out_col;

void main() {
	// TODO 
	// Teilaufgabe 2
	//out_col = vec4(0.6,0.1,0.1,1);
	out_col = k_diff;
}
