#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>
#include <cmath>

GLFWwindow* mainWindow = nullptr;
GLFWwindow* secondWindow = nullptr;

// Переменные для второго окна
float shapesColor[3] = { 1.0f, 0.0f, 0.0f }; // Начинаем с красного
float circleScale = 1.0f;
bool animationEnabled = true;

// Шейдеры
const char* vertexShaderSource = R"(
#version 330 core
layout (location = 0) in vec2 aPos;
void main() {
    gl_Position = vec4(aPos, 0.0, 1.0);
}
)";

const char* fragmentShaderSource = R"(
#version 330 core
out vec4 FragColor;
uniform vec3 ourColor;
void main() {
    FragColor = vec4(ourColor, 1.0);
}
)";

unsigned int shaderProgram;

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

bool compileShaders() {
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return true;
}

// Отрисовка квадрата для главного окна
void drawSquare() {
    float vertices[] = {
        -0.5f, -0.5f,
         0.5f, -0.5f,
         0.5f,  0.5f,
        -0.5f,  0.5f
    };

    unsigned int indices[] = { 0, 1, 2, 2, 3, 0 };

    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glUseProgram(shaderProgram);
    int colorLoc = glGetUniformLocation(shaderProgram, "ourColor");

    // Внешний белый квадрат
    glUniform3f(colorLoc, 1.0f, 1.0f, 1.0f);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    // Внутренний черный квадрат (меньший)
    float innerVertices[] = {
        -0.3f, -0.3f,
         0.3f, -0.3f,
         0.3f,  0.3f,
        -0.3f,  0.3f
    };

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(innerVertices), innerVertices, GL_STATIC_DRAW);

    glUniform3f(colorLoc, 0.0f, 0.0f, 0.0f);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
}

// Отрисовка круга для второго окна
void drawCircle() {
    std::vector<float> vertices;
    int segments = 36;

    // Центр круга (слева)
    float centerX = -0.4f;
    float centerY = 0.0f;
    float radius = 0.2f;

    vertices.push_back(centerX);
    vertices.push_back(centerY);

    for (int i = 0; i <= segments; i++) {
        float angle = 2.0f * 3.14159f * i / segments;
        vertices.push_back(centerX + cos(angle) * radius * circleScale);
        vertices.push_back(centerY + sin(angle) * radius * circleScale);
    }

    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glUseProgram(shaderProgram);
    int colorLoc = glGetUniformLocation(shaderProgram, "ourColor");
    glUniform3f(colorLoc, shapesColor[0], shapesColor[1], shapesColor[2]);

    glDrawArrays(GL_TRIANGLE_FAN, 0, static_cast<int>(vertices.size() / 2));

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
}

// Отрисовка треугольника для второго окна
void drawTriangle() {
    // Треугольник справа
    float vertices[] = {
        0.3f,  0.2f,  // верхняя вершина
        0.1f, -0.2f,  // левая нижняя
        0.5f, -0.2f   // правая нижняя
    };

    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glUseProgram(shaderProgram);
    int colorLoc = glGetUniformLocation(shaderProgram, "ourColor");
    glUniform3f(colorLoc, shapesColor[0], shapesColor[1], shapesColor[2]);

    glDrawArrays(GL_TRIANGLES, 0, 3);

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
}

// Обновление анимации
void updateAnimation() {
    if (!animationEnabled) return;

    static double lastTime = glfwGetTime();
    double currentTime = glfwGetTime();
    float deltaTime = static_cast<float>(currentTime - lastTime);
    lastTime = currentTime;

    // Дыхание круга
    circleScale = 0.8f + 0.3f * static_cast<float>(sin(currentTime * 2.0));
}

// Обработка ввода для второго окна
void processSecondWindowInput(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
        static bool spacePressed = false;
        if (!spacePressed) {
            animationEnabled = !animationEnabled;
            std::cout << "Animation " << (animationEnabled ? "started" : "stopped") << std::endl;
            spacePressed = true;
        }
    }
    else {
        static bool spacePressed = true;
        spacePressed = false;
    }

    // Изменение цвета
    static bool rPressed = false, gPressed = false, bPressed = false, yPressed = false;

    if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS && !rPressed) {
        shapesColor[0] = 1.0f; shapesColor[1] = 0.0f; shapesColor[2] = 0.0f;
        std::cout << "Color changed to RED" << std::endl;
        rPressed = true;
    }
    else if (glfwGetKey(window, GLFW_KEY_R) == GLFW_RELEASE) {
        rPressed = false;
    }

    if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS && !gPressed) {
        shapesColor[0] = 0.0f; shapesColor[1] = 1.0f; shapesColor[2] = 0.0f;
        std::cout << "Color changed to GREEN" << std::endl;
        gPressed = true;
    }
    else if (glfwGetKey(window, GLFW_KEY_G) == GLFW_RELEASE) {
        gPressed = false;
    }

    if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS && !bPressed) {
        shapesColor[0] = 0.0f; shapesColor[1] = 0.0f; shapesColor[2] = 1.0f;
        std::cout << "Color changed to BLUE" << std::endl;
        bPressed = true;
    }
    else if (glfwGetKey(window, GLFW_KEY_B) == GLFW_RELEASE) {
        bPressed = false;
    }

    if (glfwGetKey(window, GLFW_KEY_Y) == GLFW_PRESS && !yPressed) {
        shapesColor[0] = 1.0f; shapesColor[1] = 1.0f; shapesColor[2] = 0.0f;
        std::cout << "Color changed to YELLOW" << std::endl;
        yPressed = true;
    }
    else if (glfwGetKey(window, GLFW_KEY_Y) == GLFW_RELEASE) {
        yPressed = false;
    }
}

void processMainWindowInput(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

int main() {
    std::cout << "=== OPENGL ASSIGNMENT - FIXED ===" << std::endl;

    if (!glfwInit()) {
        std::cerr << "GLFW init failed!" << std::endl;
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    mainWindow = glfwCreateWindow(800, 600, "Main Window - Black & White Square", NULL, NULL);
    secondWindow = glfwCreateWindow(400, 300, "Window 2 - Circle & Triangle", NULL, NULL);

    if (!mainWindow || !secondWindow) {
        std::cerr << "Window creation failed!" << std::endl;
        glfwTerminate();
        return -1;
    }

    // Настройка главного окна
    glfwMakeContextCurrent(mainWindow);
    glfwSetFramebufferSizeCallback(mainWindow, framebuffer_size_callback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "GLAD init failed!" << std::endl;
        glfwTerminate();
        return -1;
    }

    if (!compileShaders()) {
        std::cerr << "Shader compilation failed!" << std::endl;
        return -1;
    }

    std::cout << "\n=== CONTROLS ===" << std::endl;
    std::cout << "SPACE - Start/Stop animation (circle breathing)" << std::endl;
    std::cout << "R, G, B, Y - Change colors in second window" << std::endl;
    std::cout << "ESC - Close windows" << std::endl;
    std::cout << "===============\n" << std::endl;

    while (!glfwWindowShouldClose(mainWindow) && !glfwWindowShouldClose(secondWindow)) {
        updateAnimation();

        // Главное окно
        glfwMakeContextCurrent(mainWindow);
        processMainWindowInput(mainWindow);
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        drawSquare();
        glfwSwapBuffers(mainWindow);

        // Второе окно
        glfwMakeContextCurrent(secondWindow);
        processSecondWindowInput(secondWindow);
        glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        drawCircle();   // Круг слева
        drawTriangle(); // Треугольник справа
        glfwSwapBuffers(secondWindow);

        glfwPollEvents();
    }

    glDeleteProgram(shaderProgram);
    glfwTerminate();
    std::cout << "Application finished!" << std::endl;
    return 0;
}