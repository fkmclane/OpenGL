#version 120

// texture
uniform sampler2D texture;

// shared
varying vec2 v_texture_coords;

void main() {
	// update pixel with color from texture corresponding to this object coordinate
	gl_FragColor = texture2D(texture, v_texture_coords);
}
