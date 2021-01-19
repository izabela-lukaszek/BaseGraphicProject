#pragma once

#include "glad/glad.h"


class Pyramid  {
public:
    Pyramid();
    ~Pyramid();
    void draw();
private:
    GLuint vao_;
    GLuint buffer_[2];

};