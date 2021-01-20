#pragma once

#include "Application/application.h"
#include "glad/glad.h"
#include <vector>

class Pyramid  {
public:
    Pyramid();
    ~Pyramid();
    void draw();

    Pyramid(const Pyramid& rhs) = delete;
    Pyramid &operator=(const Pyramid& rhs) = delete;

    void operator=(Pyramid&& rhs) = delete;
    Pyramid(Pyramid&& rhs) = delete;

    std::shared_ptr<Pyramid> pyramid_;



private:
    GLuint vao_;
    GLuint buffer_[2];

};