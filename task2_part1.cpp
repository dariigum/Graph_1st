#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>
#include <cmath>
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif


// === Шейдеры (как и в Task 1) ===
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

// === Утилита для создания VAO/VBO ===
struct Shape {
    unsigned int VAO;
    unsigned int VBO;
    int vertexCount;
    GLenum mode;
};

Shape makeShape(const std::vector<float>& data, GLenum mode) {
    Shape s;
    s.vertexCount = data.size() / 5; // 2 координаты + 3 цвета
    s.mode = mode;

    glGenVertexArrays(1, &s.VAO);
    glGenBuffers(1, &s.VBO);

    glBindVertexArray(s.VAO);
    glBindBuffer(GL_ARRAY_BUFFER, s.VBO);
    glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(float), data.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    return s;
}

// === Генерация фигур ===
Shape makeCircle(float cx, float cy, float r, int segments) {
    std::vector<float> verts;

    // центр круга
    verts.push_back(cx);
    verts.push_back(cy);
    verts.push_back(0.0f);
    verts.push_back(0.0f);
    verts.push_back(0.0f);

    // точки окружности
    for (int i = 0; i <= segments; i++) {
        float angle = (2.0f * M_PI * i) / segments;
        float x = cx + r * cos(angle);
        float y = cy + r * sin(angle);
        float R = angle / (2.0f * M_PI); // красный канал зависит от угла
        verts.push_back(x);
        verts.push_back(y);
        verts.push_back(R);
        verts.push_back(0.2f);
        verts.push_back(1.0f - R);
    }

    return makeShape(verts, GL_TRIANGLE_FAN);
}

Shape makeEllipse(float cx, float cy, float rx, float ry, int segments) {
    std::vector<float> verts;

    // центр
    verts.push_back(cx);
    verts.push_back(cy);
    verts.push_back(0.0f);
    verts.push_back(0.0f);
    verts.push_back(0.0f);

    for (int i = 0; i <= segments; i++) {
        float angle = (2.0f * M_PI * i) / segments;
        float x = cx + rx * cos(angle);
        float y = cy + ry * sin(angle) * 0.6f; // y уменьшаем на 60%
        float gray = (float)i / segments;
        verts.push_back(x);
        verts.push_back(y);
        verts.push_back(gray);
        verts.push_back(0.0f);
        verts.push_back(1.0f - gray);
    }

    return makeShape(verts, GL_TRIANGLE_FAN);
}

Shape makeSquare(float cx, float cy, float size, float gray) {
    std::vector<float> verts = {
        cx - size, cy + size, gray, gray, gray,
        cx - size, cy - size, gray, gray, gray,
        cx + size, cy + size, gray, gray, gray,
        cx + size, cy - size, gray, gray, gray,
    };
    return makeShape(verts, GL_TRIANGLE_STRIP);
}

Shape makeTriangle(float cx, float cy, float r) {
    std::vector<float> verts;
    for (int i = 0; i < 3; i++) {
        float angle = i * 2.0f * M_PI / 3.0f;
        float x = cx + r * cos(angle);
        float y = cy + r * sin(angle);
        verts.push_back(x);
        verts.push_back(y);
        verts.push_back(i == 0 ? 1.0f : 0.0f); // разные цвета на вершинах
        verts.push_back(i == 1 ? 1.0f : 0.0f);
        verts.push_back(i == 2 ? 1.0f : 0.0f);
    }
    return makeShape(verts, GL_TRIANGLES);
}

int main() {
    // === GLFW ===
    if (!glfwInit()) {
        std::cerr << "Ошибка GLFW!\n";
        return -1;
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(500, 500, "Task2 Part1", NULL, NULL);
    if (!window) {
        std::cerr << "Ошибка создания окна!\n";
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Ошибка GLAD!\n";
        return -1;
    }

    // === Шейдеры ===
    unsigned int vShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vShader);

    unsigned int fShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fShader);

    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vShader);
    glAttachShader(shaderProgram, fShader);
    glLinkProgram(shaderProgram);

    glDeleteShader(vShader);
    glDeleteShader(fShader);

    // === Фигуры ===
    Shape circle = makeCircle(-0.5f, 0.4f, 0.2f, 64);
    Shape ellipse = makeEllipse(0.5f, 0.4f, 0.25f, 0.25f, 64);

    std::vector<Shape> squares;
    for (int i = 0; i < 5; i++) {
        float size = 0.4f - i * 0.07f;
        float gray = (float)i / 4;
        squares.push_back(makeSquare(0.0f, -0.3f, size, gray));
    }

    Shape triangle = makeTriangle(0.5f, -0.4f, 0.2f);

    // === Цикл рендера ===
    while (!glfwWindowShouldClose(window)) {
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderProgram);

        // рисуем круг
        glBindVertexArray(circle.VAO);
        glDrawArrays(circle.mode, 0, circle.vertexCount);

        // рисуем эллипс
        glBindVertexArray(ellipse.VAO);
        glDrawArrays(ellipse.mode, 0, ellipse.vertexCount);

        // квадраты
        for (auto& s : squares) {
            glBindVertexArray(s.VAO);
            glDrawArrays(s.mode, 0, s.vertexCount);
        }

        // треугольник
        glBindVertexArray(triangle.VAO);
        glDrawArrays(triangle.mode, 0, triangle.vertexCount);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
