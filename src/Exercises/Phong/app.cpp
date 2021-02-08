//
// Created by pbialas on 25.09.2020.
//

#include "Exercises/Phong/app.h"
#include <iostream>
#include <vector>
#include <tuple>
#include "glm/gtc/matrix_transform.hpp"
#include "Application/utils.h"
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/type_ptr.hpp>

void SimpleShapeApplication::init() {

    set_camera(new Camera);
    set_controler(new CameraControler(camera()));


    auto program = xe::create_program(std::string(PROJECT_DIR) + "/shaders/base_vs.glsl",
                                      std::string(PROJECT_DIR) + "/shaders/base_fs.glsl");


    if (!program) {
        std::cerr << "Cannot create program from " << std::string(PROJECT_DIR) + "/shaders/base_vs.glsl" << " and ";
        std::cerr << std::string(PROJECT_DIR) + "/shaders/base_fs.glsl" << " shader files" << std::endl;
    }

    auto  u_diffuse_map_location = glGetUniformLocation(program,"diffuse_map");
    if(u_diffuse_map_location==-1) {
        std::cerr<<"Cannot find uniform diffuse_map\n";
    } else {
        glUniform1ui(u_diffuse_map_location, 0);
    }

    auto u_transformations_index = glGetUniformBlockIndex(program, "Transformations");
    if (u_transformations_index == GL_INVALID_INDEX)
    {
        std::cout << "Cannot find Transformations uniform block in program" << std::endl;
    }
    else
    {
        glUniformBlockBinding(program, u_transformations_index, 1);
    }

    quad = new Quad();

    glGenBuffers(1, &u_pvm_buffer_);
    glBindBuffer(GL_UNIFORM_BUFFER,u_pvm_buffer_);
    glBufferData(GL_UNIFORM_BUFFER,3 * sizeof(glm::mat4) + 3 * sizeof(glm::vec4), nullptr, GL_STATIC_DRAW);
    glBindBuffer(GL_UNIFORM_BUFFER,0);
    glBindBufferBase(GL_UNIFORM_BUFFER, 0, u_pvm_buffer_);

    int w, h;
    std::tie(w, h) = frame_buffer_size();
    float aspect_ = (float)w/h;
    float fov_ = glm::pi<float>()/4.0;
    float near_ = 0.1f;
    float far_ = 100.0f;
    camera()->look_at(glm::vec3{0.5,1.0,-3.0},glm::vec3{0.0,0.0,0.0},glm::vec3{0.0,0.0,-0.1});
    camera()->perspective(fov_, aspect_, near_, far_);
    glBindBufferBase(GL_UNIFORM_BUFFER, 1, u_pvm_buffer_);

    auto u_light_index = glGetUniformBlockIndex(program, "Light");
    if (u_light_index == -1) {
        std::cerr << "Cannot find uniform Light" << std::endl;
    }
    else {
        glUniformBlockBinding(program, u_light_index, 2);
    }

    glGenBuffers(1, &light_buffer);
    glBindBuffer(GL_UNIFORM_BUFFER,light_buffer);
    glBufferData(GL_UNIFORM_BUFFER, 4 * sizeof(glm::vec4), nullptr, GL_STATIC_DRAW);

    auto V = camera()->view();
    light_.position = V * glm::vec4(0.0f, 0.0f, 0.5f, 1.0f);
    light_.color = glm::vec4(1.0f,1.0f,1.0f,1.0f);
    light_.a = glm::vec4(1.0f,1.0f,1.0f,1.0f);
    light_.ambient = glm::vec3(0.2f,0.2f,0.2f);
    glBindBufferBase(GL_UNIFORM_BUFFER, 2,light_buffer);
    glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::vec4), &light_.position[0]);
    glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::vec4), sizeof(glm::vec4), &light_.color[0]);
    glBufferSubData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::vec4), sizeof(glm::vec4), &light_.a[0]);
    glBufferSubData(GL_UNIFORM_BUFFER, 3 * sizeof(glm::vec4), sizeof(glm::vec4), &light_.ambient[0]);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    glClearColor(0.81f, 0.81f, 0.8f, 1.0f);
    glViewport(0, 0, w, h);

    glEnable(GL_DEPTH_TEST);
    glUseProgram(program);
}

void SimpleShapeApplication::frame() {

    glm::mat4 P = camera()->projection();
    glm::mat4 VM =  camera()->view();
    auto R = glm::mat3(VM);
    auto N = glm::transpose(glm::inverse(R));

    glBindBuffer(GL_UNIFORM_BUFFER, u_pvm_buffer_);
    glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), &P);
    glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), &VM);
    glBufferSubData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4), sizeof(glm::vec4), &N[0]);
    glBufferSubData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4) + sizeof(glm::vec4), sizeof(glm::vec3), &N[1]);
    glBufferSubData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4) + 2 * sizeof(glm::vec4), sizeof(glm::vec3), &N[2]);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
    light_.position = VM * glm::vec4(0.0f,0.0f,0.5f,1.0f);
    glBindBuffer(GL_UNIFORM_BUFFER,light_buffer);
    glBufferSubData(GL_UNIFORM_BUFFER,0,sizeof(glm::vec4),&light_.position[0]);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
    quad->draw();

}

void SimpleShapeApplication::framebuffer_resize_callback(int w, int h) {
    Application::framebuffer_resize_callback(w, h);
    glViewport(0,0,w,h);
    auto aspect_ = (float) w / h;
    camera()->set_aspect(aspect_);
}


