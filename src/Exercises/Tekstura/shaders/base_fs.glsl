#version 410

layout(location=0) out vec4 vFragColor;
in vec3 vertex_color;
uniform sampler2D diffuse_map;


void main() {
    vFragColor.a = 1.0;
    vFragColor.rgb =vertex_color;
    vFragColor = texture(color, texture_coords);
}