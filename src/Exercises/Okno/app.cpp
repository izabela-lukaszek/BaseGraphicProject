//
// Created by pbialas on 25.09.2020.
//

#include "app.h"
#include <iostream>
#include <vector>
#include <tuple>
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/string_cast.hpp"
#include "glm/glm.hpp"
#include "Application/utils.h"
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>

void SimpleShapeApplication::init() {


    auto program = xe::create_program(std::string(PROJECT_DIR) + "/shaders/base_vs.glsl",
                                      std::string(PROJECT_DIR) + "/shaders/base_fs.glsl");


    if (!program) {
        std::cerr << "Cannot create program from " << std::string(PROJECT_DIR) + "/shaders/base_vs.glsl" << " and ";
        std::cerr << std::string(PROJECT_DIR) + "/shaders/base_fs.glsl" << " shader files" << std::endl;
    }

    std::vector<GLushort> indices = {
            0,2,1, 2,3,1, 4,5,6, 7,9,8, 10,11,12, 13,15,14
    };

    std::vector<GLfloat> vertices = {

            0.4f, 0.4f, 0.0f,
            0.2f, 1.0f, 0.4f,//kolor
            -0.4f, 0.4f,0.0f,
            0.2f, 1.0f, 0.4f,//kolor
            0.4f, -0.4f, 0.0f,
            0.2f, 1.0f, 0.4f,//kolor
            -0.4f, -0.4f,0.0f,
            0.2f, 1.0f, 0.4f,//kolor

            0.4f, 0.4f, 0.0f,
            1.0f, 0.5f, 0.4f,//kolor
            -0.4f, 0.4f,0.0f,
            1.0f, 0.5f, 0.4f,//kolor
            0.0f, 0.0f, -0.8f,
            1.0f, 0.5f, 0.4f,//kolor

            0.4f, -0.4f, 0.0f,
            0.0f, 0.5f, 1.0f,//kolor
            -0.4f, -0.4f,0.0f,
            0.0f, 0.5f, 1.0f,//kolor
            0.0f, 0.0f, -0.8f,
            0.0f, 0.5f, 1.0f,//kolor

            0.4f, -0.4f, 0.0f,
            0.6f, 0.2f, 0.8f,//kolor
            0.4f, 0.4f,0.0f,
            0.6f, 0.2f, 0.8f,//kolor
            0.0f, 0.0f, -0.8f,
            0.6f, 0.2f, 0.8f,//kolor

            -0.4f, -0.4f, 0.0f,
            0.9f, 0.9f, 0.2f,//kolor
            -0.4f, 0.4f,0.0f,
            0.9f, 0.9f, 0.2f,//kolor
            0.0f, 0.0f, -0.8f,
            0.9f, 0.9f, 0.2f,//kolor


    };

    GLuint v_buffer_handle;
    glGenBuffers(1, &v_buffer_handle);
    glBindBuffer(GL_ARRAY_BUFFER, v_buffer_handle);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), vertices.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    GLuint idx_buffer_handle;
    glGenBuffers(1,&idx_buffer_handle);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, idx_buffer_handle);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLushort), indices.data(),GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    glGenVertexArrays(1, &vao_);
    glBindVertexArray(vao_);
    glBindBuffer(GL_ARRAY_BUFFER, v_buffer_handle);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, idx_buffer_handle);

    auto u_modifiers_index = glGetUniformBlockIndex(program, "Modifiers");
    if (u_modifiers_index == GL_INVALID_INDEX){
        std::cout << "Cannot find Modifiers uniform block in program" << std::endl;
    }
    else {
        glUniformBlockBinding(program, u_modifiers_index, 0);
    }

    GLuint ubo_handle;
    glGenBuffers(1,&ubo_handle);
    glBindBuffer(GL_UNIFORM_BUFFER, ubo_handle);
    glBufferData(GL_UNIFORM_BUFFER, 8 * sizeof(float), nullptr, GL_STATIC_DRAW);

    float light_intensity=1.0;
    float light_color[3] = {0.5,0.5,0.5};
    glBufferSubData(GL_UNIFORM_BUFFER,0,sizeof(float),&light_intensity);
    glBufferSubData(GL_UNIFORM_BUFFER,4 * sizeof(float),3 * sizeof(float),light_color);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
    glBindBufferBase(GL_UNIFORM_BUFFER, 0, ubo_handle);


    auto u_transformations_index = glGetUniformBlockIndex(program, "Transformations");
    if (u_transformations_index == GL_INVALID_INDEX)
    {
        std::cout << "Cannot find Transformations uniform block in program" << std::endl;
    }
    else
    {
        glUniformBlockBinding(program, u_transformations_index, 1);
    }

    int w, h;
    std::tie(w, h) = frame_buffer_size();
    aspect_ = (float)w/h;
    fov_ = glm::pi<float>()/4.0;
    near_ = 0.1f;
    far_ = 100.0f;
    V_ = glm::lookAt(glm::vec3{0.5,1.0,-3.0},glm::vec3{0.0,0.0,0.0},glm::vec3{0.0,0.0,-0.1});
    P_ = glm::perspective(fov_, aspect_, near_, far_);

    glGenBuffers(1, &u_pvm_buffer_);
    glBindBuffer(GL_UNIFORM_BUFFER,u_pvm_buffer_);
    glBufferData(GL_UNIFORM_BUFFER,sizeof(glm::mat4), nullptr, GL_STATIC_DRAW);
    glBindBuffer(GL_UNIFORM_BUFFER,0);
    glBindBufferBase(GL_UNIFORM_BUFFER, 1, u_pvm_buffer_);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), reinterpret_cast<GLvoid *>(0));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), reinterpret_cast<GLvoid *>(3 * sizeof(GLfloat)));
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    glClearColor(0.81f, 0.81f, 0.8f, 1.0f);
    glViewport(0, 0, w, h);

    glEnable(GL_CULL_FACE);
    glFrontFace(GL_CW);
    glCullFace(GL_BACK);

    glEnable(GL_DEPTH_TEST);
    glUseProgram(program);
}

void SimpleShapeApplication::frame() {
    glBindVertexArray(vao_);
    glDrawElements(GL_TRIANGLES,18,GL_UNSIGNED_SHORT,nullptr);
    glBindVertexArray(0);

    auto PVM = P_ * V_;
    glBindBuffer(GL_UNIFORM_BUFFER, u_pvm_buffer_);
    glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), &PVM[0]);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void SimpleShapeApplication::framebuffer_resize_callback(int w, int h) {
    Application::framebuffer_resize_callback(w, h);
    glViewport(0,0,w,h);
    aspect_ = (float) w / h;
    P_ = glm::perspective(fov_, aspect_, near_, far_);
}