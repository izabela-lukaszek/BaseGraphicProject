#version 410

layout(location=0) out vec4 vFragColor;
//in vec3 vertex_color;
in vec2  texture_uv;
uniform sampler2D diffuse_map;


void main() {
    //vFragColor.a = 1.0;
    //vFragColor.rgb =vertex_color;
    vFragColor = texture(diffuse_map,  texture_uv);
}