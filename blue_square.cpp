#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

const char* vertexShaderSource = R"(
#version 330 core
layout (location = 0) in vec2 vPosition;
layout (location = 1) in vec3 vColor;
out vec3 passColor;
void main() {
    gl_Position = vec4(vPosition, 0.0, 1.0);
    passColor = vColor;
}
)";

const char* fragmentShaderSource = R"(
#version 330 core
in vec3 passColor;
out vec4 FragColor;
void main() {
    FragColor = vec4(passColor, 1.0);
}
)";

int main() {
    // 1. Инициализация GLFW
    if (!glfwInit()) {
        std::cerr << "Ошибка: GLFW не инициализировался!\n";
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // 2. Создаём окно
    GLFWwindow* window = glfwCreateWindow(500, 500, "Blue Square", NULL, NULL);
    if (!window) {
        std::cerr << "Ошибка: не удалось создать окно!\n";
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // 3. Загружаем функции OpenGL
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Ошибка: не удалось инициализировать GLAD!\n";
        return -1;
    }

    // 4. Компиляция шейдеров
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // 🔹 5. Массив вершин (4 точки квадрата)
    float vertices[] = {
        // x,    y,     r, g, b
        -0.6f,  0.6f,  0, 0, 1,   // левый верх
        -0.6f, -0.6f,  0, 0, 1,   // левый низ
         0.6f,  0.6f,  0, 0, 1,   // правый верх
         0.6f, -0.6f,  0, 0, 1    // правый низ
    };

    // 6. VAO + VBO
    unsigned int VAO, VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // позиция (x,y)
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // цвет (r,g,b)
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // 7. Цикл рендера
    while (!glfwWindowShouldClose(window)) {
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);

        // 🔹 рисуем квадрат (4 вершины, TRIANGLE_STRIP)
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // 8. Очистка
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);

    glfwTerminate();
    return 0;
}
