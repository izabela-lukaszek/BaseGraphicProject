#pragma once

#include "Application/application.h"
#include "glad/glad.h"
#include <vector>

class Quad{
public:
    Quad();
    ~ Quad();
    void draw();

    Quad(const  Quad& rhs) = delete;
    Quad &operator=(const  Quad& rhs) = delete;

    void operator=( Quad&& rhs) = delete;
    Quad( Quad&& rhs) = delete;


private:
    GLuint vao_;
    GLuint buffer_[2];
    GLuint diffuse_texture_;
};