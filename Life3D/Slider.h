#pragma once
#include <GLFW/glfw3.h>
#include <Shader/Shader.h>


class Slider {
public:
    // Konstruktor
    Slider(int posX, int posY, int width, int height, int minVal, int maxVal, int initVal = 0, int step = 1, glm::vec3 barColor = glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3 sliderColor = glm::vec3(1.0f, 0.0f, 0.0f))
        :posX_(posX), posY_(posY), width_(width), height_(height), minVal_(minVal), maxVal_(maxVal), initVal_(initVal), step_(step), barColor_(barColor), sliderColor_(sliderColor) {
        projection_ = glm::ortho(0.0f, (float)1200, 0.0f, (float)800);
        initBuffers();
    }

    void Draw(Shader &shader) {
        
        shader.use();
        shader.setMat4("projection", projection_);
        shader.setVec3("color", sliderColor_);
        glBindVertexArray(sliderVAO);
        glDrawArrays(GL_TRIANGLES, 0,6);

        //std::cout << posX_ << std::endl;
    }

    void setPos() {

    }

    glm::vec3 getPos() {

    }

private:
    int posX_;
    int posY_;
    int width_;
    int height_;
    int minVal_;
    int maxVal_;
    int initVal_;
    int step_;
    Shader shader_;
    glm::vec3 barColor_;
    glm::vec3 sliderColor_;

    glm::mat4 projection_;

    unsigned int sliderVAO;
    unsigned int sliderVBO;

    void initBuffers() {
        GLfloat sliderVertices[18];

        GLfloat x1 = posX_;
        GLfloat y1 = posY_;
        GLfloat x2 = posX_ + width_;
        GLfloat y2 = posY_;
        GLfloat x3 = posX_ + width_;
        GLfloat y3 = posY_ + height_;
        GLfloat x4 = posX_;
        GLfloat y4 = posY_ + height_;


        // Erstes Dreieck
        sliderVertices[0] = x1;
        sliderVertices[1] = y1;
        sliderVertices[2] = 0.0f;
        sliderVertices[3] = x2;
        sliderVertices[4] = y2;
        sliderVertices[5] = 0.0f;
        sliderVertices[6] = x3;
        sliderVertices[7] = y3;
        sliderVertices[8] = 0.0f;

        // Zweites Dreieck
        sliderVertices[9] = x1;
        sliderVertices[10] = y1;
        sliderVertices[11] = 0.0f;
        sliderVertices[12] = x3;
        sliderVertices[13] = y3;
        sliderVertices[14] = 0.0f;
        sliderVertices[15] = x4;
        sliderVertices[16] = y4;
        sliderVertices[17] = 0.0f;

        glGenBuffers(1, &sliderVAO);
        glGenBuffers(1, &sliderVBO);

        glBindVertexArray(sliderVAO);
        glBindBuffer(GL_ARRAY_BUFFER, sliderVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(sliderVertices), sliderVertices, GL_STATIC_DRAW);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    
    }
};


