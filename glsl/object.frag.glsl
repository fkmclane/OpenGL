#version 120

//Texture
uniform sampler2D texture;

//Shared
varying vec2 v_texture_coords;

void main() {
	//Update pixel with color from texture corresponding to this object coordinate
	gl_FragColor = texture2D(texture, v_texture_coords);
}
