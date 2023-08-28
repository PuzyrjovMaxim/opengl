#include <stdio.h>
#include <stdlib.h>
#include <iostream>
// �������� GLEW. ������ ��������� ��� �� gl.h � glfw.h
#include <GL\glew.h>
// �������� GLFW
#include <glfw3.h>
#include "shader.hpp"
#include <cmath>
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtx\transform.hpp>

using namespace glm;
const int array_width = 61;

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    std::cout << "Key pressed: " << key << ", action: " << action << std::endl;
    if (action != GLFW_RELEASE) {
        switch (key) {
        case GLFW_KEY_DOWN: glClearColor(0.1f, 0.4f, 0.8f, 0.0f); glClear(GL_COLOR_BUFFER_BIT); break;
        case GLFW_KEY_UP: glClearColor(1.0f, 1.0f, 1.0f, 0.0f); glClear(GL_COLOR_BUFFER_BIT); break;
        }
    }
}

int main() {
    float x, y;
    // �������������� GLFW
    glfwInit();
    if (!glfwInit())
    {
        fprintf(stderr, "������ ��� ������������� GLFWn");
        return -1;
    }
    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // �� �� ����� ������ OpenGL

    // ������� ���� � ������� � ��� �������� OpenGL
    GLFWwindow* window; // (� ���������������� �������� ���� ��� ���������� �������� ����������)
    window = glfwCreateWindow(1000, 1000, "Tutorial 01", NULL, NULL);
    if (window == NULL) {
        fprintf(stderr, "���������� ������� ���� GLFW. ���� � ��� Intel GPU, �� �� �� ������������ ������ 3.3. ���������� ������ ������ ��� OpenGL 2.1.n");
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);


    //�������������� GLEW
    glewInit();
    glewExperimental = true; // ���� ��������� � Core-������ OpenGL
    if (glewInit() != GLEW_OK) {
        fprintf(stderr, "���������� ���������������� GLEWn");
        return -1;
    }

    // ������� ����� ������������ ������� ������, ��� �������� ����
    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

    glClearColor(1.0f, 1.0f, 1.0f, 0.0f);

    GLuint VertexArrayID;
    glGenVertexArrays(1, &VertexArrayID);
    glBindVertexArray(VertexArrayID);

    GLuint programID = LoadShaders("SimpleVertexShader.vertexshader", "SimpleFragmentShader.fragmentshader");

    GLfloat g_vertex_buffer_data[] = {
        -1.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 0.0f,
        0.0f, -1.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
    };


    
    // ��� ����� ��������������� ������ ������ ������
    GLuint vertexbuffer;

    // �������� 1 ����� � �������� � ���������� vertexbuffer ��� �������������
    glGenBuffers(1, &vertexbuffer);

    // ������� ������ ��� ��������� ����� �������
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);

    // ��������� ���������� � �������� � OpenGL
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

    GLuint VertexArrayID2;
    glGenVertexArrays(1, &VertexArrayID2);
    glBindVertexArray(VertexArrayID2);

    glm::mat4 myScalingMatrix = glm::scale(2.0, 2.0, 2.0);
    glm::vec4 myVector(10.0f, 10.0f, 10.0f, 0.0f);

    GLuint program2ID = LoadShaders("SimpleVertexShader.vertexshader", "SimpleFragmentShader2.fragmentshader");
    
    GLfloat g_vertex_graphic_data[array_width][3];
    int count = 0;
    int mash = 30;
    for (int i = -array_width/2;i <= array_width/2; i+=1) {
        float j = i;
        x = j / mash;
        y = pow(x, 2) - 2.0 *x - abs(pow(x, 2) + x -1.0);
        std::cout << y << " " << x << std::endl;
        float x1 = x / (array_width/mash);
        float y1 = y / (array_width/mash);

        count += 1;
        g_vertex_graphic_data[i+array_width/2][0] = x1;
        g_vertex_graphic_data[i+array_width/2][1] = y1;
        g_vertex_graphic_data[i+array_width/2][2] = 0.0;
    }
    std::cout << std::endl;
    float maxy = 0;
    std::cout << std::endl;
    std::cout << count << std::endl;
    GLuint vertexbuffer2;
    glGenBuffers(1, &vertexbuffer2);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer2);
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_graphic_data), g_vertex_graphic_data, GL_STATIC_DRAW);

    glfwSetKeyCallback(window, keyCallback);

    do {
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(programID);

        // ���������, ��� ������ ������� ��������� ����� �������
        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
        glVertexAttribPointer(
            0,                  // ������� 0. ��������� �� ���� ����� ���������� � �����, ����������� ��������.
            3,                  // ������
            GL_FLOAT,           // ���
            GL_FALSE,           // ���������, ��� �������� �� �������������
            0,                  // ���
            (void*)0            // �������� ������� � ������
        );

        // ������� �����������!
        glDrawArrays(GL_LINES, 0, 4); // ������� � ������� 0, ����� 3 ������� -> ���� �����������

        glDisableVertexAttribArray(0);
        

        glUseProgram(program2ID);

        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer2);
        glVertexAttribPointer(
            0,
            3,
            GL_FLOAT,
            GL_FALSE,
            0,
            (void*)0
        );
        
        
        glDrawArrays(GL_LINES, 0, array_width);
        if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) glDrawArrays(GL_POINTS, 0, array_width);
        else if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) glDrawArrays(GL_LINE_STRIP, 0, array_width);
        glDisableVertexAttribArray(0);
        
        glfwSwapBuffers(window);
        glfwPollEvents();
    }  // ��������� ������� ������� Escape ��� �������� ����
    while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
        glfwWindowShouldClose(window) == 0);
   

    // Cleanup VBO
    glDeleteBuffers(1, &vertexbuffer);
    glDeleteVertexArrays(1, &VertexArrayID);
    glDeleteProgram(programID);
    glDeleteBuffers(1, &vertexbuffer2);
    glDeleteVertexArrays(1, &VertexArrayID2);
    glDeleteProgram(program2ID);

    // Close OpenGL window and terminate GLFW
    glfwTerminate();

    return 0;
}