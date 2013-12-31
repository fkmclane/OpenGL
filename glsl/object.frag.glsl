#version 120
varying vec2 v_texture_coords;
uniform sampler2D texture;
void main() {
	gl_FragColor = texture2D(texture, v_texture_coords);
}
