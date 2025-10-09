// 1. Подключаем библиотеки
#include <glad/glad.h>   // загрузчик функций OpenGL
#include <GLFW/glfw3.h>  // для окна и ввода
#include <iostream>      // для вывода ошибок/сообщений

// 2. Шейдеры (минимальные: позиция + цвет)
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
    // 3. Инициализация GLFW
    if (!glfwInit()) {
        std::cerr << "Ошибка: GLFW не инициализировался!\n";
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // OpenGL 3.3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // 4. Создаём окно
    GLFWwindow* window = glfwCreateWindow(500, 500, "Red Triangle", NULL, NULL);
    if (!window) {
        std::cerr << "Ошибка: не удалось создать окно!\n";
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // 5. Загружаем функции OpenGL через GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Ошибка: не удалось инициализировать GLAD!\n";
        return -1;
    }

    // 6. Компиляция шейдеров
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    // 7. Линкуем шейдерную программу
    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // 8. ⬇️ Вот здесь СОЗДАЁМ массив вершин (3 точки треугольника)
    float vertices[] = {
        // x,     y,     r, g, b
         0.0f,  0.6f,   1, 0, 0,  // верхняя вершина (красная)
        -0.6f, -0.4f,   1, 0, 0,  // левая нижняя
         0.6f, -0.4f,   1, 0, 0   // правая нижняя
    };

    // 9. Создаём VAO и VBO
    unsigned int VAO, VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // позиция: 2 float-а
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // цвет: 3 float-а (начинаются после 2 координат)
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // 10. Цикл рендера
    while (!glfwWindowShouldClose(window)) {
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f); // цвет фона
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3); // ⬅️ рисуем треугольник

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // 11. Очистка ресурсов
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);

    glfwTerminate();
    return 0;
}
