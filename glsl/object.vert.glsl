#version 120
attribute vec3 coords;
attribute vec2 texture_coords;
varying vec2 v_texture_coords;
uniform mat4 obj_transform;
uniform mat4 world_transform;
uniform mat4 perspective;
void main() {
	gl_Position = perspective * world_transform * obj_transform * vec4(coords, 1.0);
	v_texture_coords = texture_coords;
}
