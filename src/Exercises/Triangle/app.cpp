//
// Created by pbialas on 25.09.2020.
//

#include "app.h"

#include <iostream>
#include <vector>
#include <tuple>


#include "Application/utils.h"

void SimpleShapeApplication::init() {


    auto program = xe::create_program(std::string(PROJECT_DIR) + "/shaders/base_vs.glsl",
                                      std::string(PROJECT_DIR) + "/shaders/base_fs.glsl");


    if (!program) {
        std::cerr << "Cannot create program from " << std::string(PROJECT_DIR) + "/shaders/base_vs.glsl" << " and ";
        std::cerr << std::string(PROJECT_DIR) + "/shaders/base_fs.glsl" << " shader files" << std::endl;
    }
//Wartości wychodzące poza przedział (-1,1) spowodują że trojkąt będzie przy danym wierzchołku wychodził poza obszar okna
//Zmiana wartości z nic nie zmienia w pokazywanym na ekranie trójkącie
    std::vector<GLfloat> vertices = {
            -0.5f, -0.5f, 0.0f,
            -0.5f, 0.0f, 0.0f,
            0.0f, 0.0f, 0.0f,

    };

    std::vector<GLfloat> vertices2 = {
            0.0f, 0.0f, 0.0f,
            0.0f, -0.5f, 0.0f,
            -0.5f, -0.5f, 0.0f,

    };

    std::vector<GLfloat> vertices3 = {
            -0.7f, 0.0f, 0.0f,
            0.2f, 0.0f, 0.0f,
            -0.25f, 0.6f, 0.0f,

    };


    GLuint v_buffer_handle;
    glGenBuffers(1, &v_buffer_handle);
    glBindBuffer(GL_ARRAY_BUFFER, v_buffer_handle);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), vertices.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glGenVertexArrays(1, &vao_);
    glBindVertexArray(vao_);
    glBindBuffer(GL_ARRAY_BUFFER, v_buffer_handle);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), reinterpret_cast<GLvoid *>(0));
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    GLuint v_buffer_handle2;
    glGenBuffers(1, &v_buffer_handle2);
    glBindBuffer(GL_ARRAY_BUFFER, v_buffer_handle2);
    glBufferData(GL_ARRAY_BUFFER, vertices2.size() * sizeof(GLfloat), vertices2.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glGenVertexArrays(1, &vao_2);
    glBindVertexArray(vao_2);
    glBindBuffer(GL_ARRAY_BUFFER, v_buffer_handle2);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), reinterpret_cast<GLvoid *>(0));
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    GLuint v_buffer_handle3;
    glGenBuffers(1, &v_buffer_handle3);
    glBindBuffer(GL_ARRAY_BUFFER, v_buffer_handle3);
    glBufferData(GL_ARRAY_BUFFER, vertices3.size() * sizeof(GLfloat), vertices3.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glGenVertexArrays(1, &vao_3);
    glBindVertexArray(vao_3);
    glBindBuffer(GL_ARRAY_BUFFER, v_buffer_handle3);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), reinterpret_cast<GLvoid *>(0));
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);




    glClearColor(0.81f, 0.81f, 0.8f, 1.0f);
    int w, h;
    std::tie(w, h) = frame_buffer_size();
    glViewport(0, 0, w, h);

    glEnable(GL_DEPTH_TEST);
    glUseProgram(program);
}

void SimpleShapeApplication::frame() {
    glBindVertexArray(vao_);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glBindVertexArray(0);

    glBindVertexArray(vao_2);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glBindVertexArray(0);

    glBindVertexArray(vao_3);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glBindVertexArray(0);
}
