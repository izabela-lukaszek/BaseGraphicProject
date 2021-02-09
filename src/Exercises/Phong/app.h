//
// Created by pbialas on 05.08.2020.
//


#pragma once

#include <vector>
#include "Application/application.h"
#include "Exercises/Phong/quad.h"
#include "Application/utils.h"
#include "glad/glad.h"
#include "Exercises/Phong/camera.h"
#include "Exercises/Phong/camera_controler.h"
#include "glm/mat4x4.hpp"
#include "glm/vec3.hpp"
#include "glm/vec4.hpp"
#include "glm/gtc/matrix_transform.hpp"


struct Light {
    glm::vec4 position;
    glm::vec4 color;
    glm::vec4 a;
    glm::vec3 ambient;
};

class SimpleShapeApplication : public xe::Application {
public:
    SimpleShapeApplication(int width, int height, std::string title, int major = 4, int minor = 1) :
            Application(width, height, title, major, minor) {}

    void init() override;;

    void frame() override;

    void framebuffer_resize_callback(int w, int h) override;

    void cleanup() override;

    void set_camera(Camera *camera) { camera_ = camera; }
    Camera *camera() { return camera_; }
    ~SimpleShapeApplication() {
        if (camera_) {
            delete camera_;
        }
    }

    void scroll_callback(double xoffset, double yoffset) override {
        Application::scroll_callback(xoffset, yoffset);
        camera()->zoom(yoffset / 30.0f);
    }


    void set_controler(CameraControler *controler) { controler_ = controler; }

    void mouse_button_callback(int button, int action, int mods) {
        Application::mouse_button_callback(button, action, mods);

        if (controler_) {
            double x, y;
            glfwGetCursorPos(window_, &x, &y);

            if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
                controler_->LMB_pressed(x, y);

            if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE)
                controler_->LMB_released(x, y);
        }

    }

    void cursor_position_callback(double x, double y) {
        Application::cursor_position_callback(x, y);
        if (controler_) {
            controler_->mouse_moved(x, y);
        }
    }

    Quad* quad;

private:
    GLuint vao_;
    Camera *camera_;
    GLuint u_pvm_buffer_;
    CameraControler *controler_;
    Light light_;
    GLuint light_buffer;

};