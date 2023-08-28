#include <stdio.h>
#include <stdlib.h>

// �������� GLEW. ������ ��������� ��� �� gl.h � glfw.h
#include <GLEW/glew.h>
// �������� GLFW
#include <glfw3.h>

#pragma comment(lib, "glew32.lib")

int main() {
    // �������������� GLFW
    glfwInit();
    if (!glfwInit())
    {
        fprintf(stderr, "������ ��� ������������� GLFWn");
        return -1;
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // �� �� ����� ������ OpenGL

    // ������� ���� � ������� � ��� �������� OpenGL
    GLFWwindow* window; // (� ���������������� �������� ���� ��� ���������� �������� ����������)
    window = glfwCreateWindow(1024, 768, "Tutorial 01", NULL, NULL);
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
    

    GLuint VertexArrayID;
    glGenVertexArrays(1, &VertexArrayID);
    glBindVertexArray(VertexArrayID);

    // ������ 3 ��������, ������� �������� ��������� ������������
    static const GLfloat g_vertex_buffer_data[] = {
       -1.0f, -1.0f, 0.0f,
       1.0f, -1.0f, 0.0f,
       0.0f,  1.0f, 0.0f,
    };

    // ��� ����� ��������������� ������ ������ ������
    GLuint vertexbuffer;

    // �������� 1 ����� � �������� � ���������� vertexbuffer ��� �������������
    glGenBuffers(1, &vertexbuffer);

    // ������� ������ ��� ��������� ����� �������
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);

    // ��������� ���������� � �������� � OpenGL
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);


    
    

    // ������� ����� ������������ ������� ������, ��� �������� ����
    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

    do {
        glClear(GL_COLOR_BUFFER_BIT);
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
        glDrawArrays(GL_TRIANGLES, 0, 3); // ������� � ������� 0, ����� 3 ������� -> ���� �����������

        glDisableVertexAttribArray(0);
        
        glfwSwapBuffers(window);
        glfwPollEvents();

    } // ��������� ������� ������� Escape ��� �������� ����
    while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
        glfwWindowShouldClose(window) == 0);
    return 0;
}