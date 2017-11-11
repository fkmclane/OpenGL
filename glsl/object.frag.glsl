#version 130

// texture
uniform sampler2D texture;

// shared
varying vec3 v_coords;
varying vec2 v_texture_coords;
varying vec3 v_normal;

void main() {
	// update pixel with color from texture corresponding to this object coordinate
	gl_FragColor = texture2D(texture, v_texture_coords);
}
