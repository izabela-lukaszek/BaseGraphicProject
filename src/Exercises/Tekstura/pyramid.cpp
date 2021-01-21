#include "Exercises/Tekstura/pyramid.h"
#include <vector>
#include <iostream>
#define STB_IMAGE_IMPLEMENTATION
#include "3rdParty/src/stb/stb_image.h"

Pyramid::Pyramid() {
    /*
        Tu dodajemy kod tworzący bufory indeksów i wierzchołków oraz obiekt VAO,
        Potem ładujemy dane wierzchołków i indeksow do buforów  i  podłaczamy je do atrybutów w szaderze
   */
    std::vector<GLushort> indices = {
            0,2,1, 2,3,1, 0,1,4, 2,5,3, 2,1,6, 3,7,1
    };

    std::vector<GLfloat> vertices = {

            0.4f, 0.4f, 0.0f,//0
            0.8090f, 0.5000f,//kolor
            -0.4f, 0.4f,0.0f,//1
            0.5000f, 0.8090f,//kolor
            0.4f, -0.4f, 0.0f,//2
            0.5000f, 0.1910f,//kolor
            -0.4f, -0.4f,0.0f,//3
            0.1910f, 0.5000f, //kolor

            //4//0.4f, 0.4f, 0.0f,//0
            //1.0f, 0.5f, 0.4f,//kolor
            //5//-0.4f, 0.4f,0.0f,//1
           // 1.0f, 0.5f, 0.4f,//kolor
            0.0f, 0.0f, -0.8f,//4
            1.0f, 1.0f, //kolor

           //7 //0.4f, -0.4f, 0.0f,//2
            //0.0f, 0.5f, 1.0f,//kolor
            //8//-0.4f, -0.4f,0.0f,//3
            //0.0f, 0.5f, 1.0f,//kolor
            0.0f, 0.0f, -0.8f,//5
            0.0f, 1.0f,//kolor

           //10// 0.4f, -0.4f, 0.0f,//2
            //0.6f, 0.2f, 0.8f,//kolor
            //11//0.4f, 0.4f,0.0f,//1
            //0.6f, 0.2f, 0.8f,//kolor
            0.0f, 0.0f, -0.8f,//6
            1.0f, 0.0f, //kolor

            //13//-0.4f, -0.4f, 0.0f,//3
            //0.9f, 0.9f, 0.2f,//kolor
            //14//-0.4f, 0.4f,0.0f,//1
            //0.9f, 0.9f, 0.2f,//kolor
            0.0f, 0.0f, -0.8f,//7
            0.0f, 0.0f, //kolor


    };


    glGenBuffers(2, this->buffer_);
    glBindBuffer(GL_ARRAY_BUFFER, this->buffer_[0]);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), vertices.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

   // GLuint idx_buffer_handle;
   // glGenBuffers(1,&idx_buffer_handle);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->buffer_[1]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLushort), indices.data(),GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    glGenVertexArrays(1, &vao_);
    glBindVertexArray(vao_);
    glBindBuffer(GL_ARRAY_BUFFER, this->buffer_[0]);
    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, idx_buffer_handle);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), reinterpret_cast<GLvoid *>(0));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), reinterpret_cast<GLvoid *>(3 * sizeof(GLfloat)));
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    int width, height, n_channels;
    auto texture_filename = std::string(PROJECT_DIR) + "/Textures/multicolor.png";
    uint8_t  *data = stbi_load(texture_filename.c_str(), &width, &height, &n_channels, 0);
    glBindTexture(GL_TEXTURE_2D, diffuse_texture_);
    glTexImage2D(GL_TEXTURE_2D,0,GL_RGB, width, height,0, GL_RGB, GL_UNSIGNED_BYTE,data);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    glBindTexture(GL_TEXTURE_2D, 0);


    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,this->buffer_[1]);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);

}

Pyramid::~Pyramid() {
    //Tu usuwamy VAO i bufory
    glDeleteBuffers(2, this->buffer_);
    glDeleteVertexArrays(1,&vao_);
}

void Pyramid::draw() {
    //Tu wywolujemy polecenie glDrawElements
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D,diffuse_texture_);

    glBindVertexArray(vao_);
    glDrawElements(GL_TRIANGLES,18,GL_UNSIGNED_SHORT,nullptr);
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D,0);


}  