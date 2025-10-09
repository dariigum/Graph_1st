#include <GL/glut.h>
#include <iostream>

// ----- Global Variables -----
float scaleXYZ[3] = { 1.0f, 1.0f, 1.0f };
float rotateXYZ[3] = { 30.0f, 30.0f, 0.0f }; // initial rotation for 3D perspective
float translateXYZ[3] = { 0.0f, 0.0f, 0.0f };
float delta = 0.1f;

enum TransformMode { SCALE, ROTATE, TRANSLATE };
TransformMode currentMode = ROTATE;

// ----- Function Declarations -----
void display();
void reshape(int w, int h);
void keyboard(unsigned char key, int x, int y);
void menu(int option);
void printInstructions();

// ----- Main Function -----
int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(600, 600);
    glutCreateWindow("3D Cube - Rainbow Faces");

    glEnable(GL_DEPTH_TEST); // Enable depth testing

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);

    // Right-click menu
    glutCreateMenu(menu);
    glutAddMenuEntry("Scale", SCALE);
    glutAddMenuEntry("Rotate", ROTATE);
    glutAddMenuEntry("Translate", TRANSLATE);
    glutAttachMenu(GLUT_RIGHT_BUTTON);

    printInstructions();
    glutMainLoop();
    return 0;
}

// ----- Draw the cube with rainbow faces -----
void drawCube() {
    glBegin(GL_QUADS);

    // Front face (rainbow gradient)
    glColor3f(1, 0, 0); glVertex3f(-0.5f, -0.5f, 0.5f);
    glColor3f(1, 0.5f, 0); glVertex3f(0.5f, -0.5f, 0.5f);
    glColor3f(1, 1, 0); glVertex3f(0.5f, 0.5f, 0.5f);
    glColor3f(0, 1, 0); glVertex3f(-0.5f, 0.5f, 0.5f);

    // Back face
    glColor3f(0, 1, 0); glVertex3f(-0.5f, -0.5f, -0.5f);
    glColor3f(0, 1, 1); glVertex3f(-0.5f, 0.5f, -0.5f);
    glColor3f(0, 0, 1); glVertex3f(0.5f, 0.5f, -0.5f);
    glColor3f(1, 0, 1); glVertex3f(0.5f, -0.5f, -0.5f);

    // Left face
    glColor3f(1, 0, 0); glVertex3f(-0.5f, -0.5f, -0.5f);
    glColor3f(1, 0, 1); glVertex3f(-0.5f, -0.5f, 0.5f);
    glColor3f(0, 1, 1); glVertex3f(-0.5f, 0.5f, 0.5f);
    glColor3f(0, 1, 0); glVertex3f(-0.5f, 0.5f, -0.5f);

    // Right face
    glColor3f(1, 0.5f, 0); glVertex3f(0.5f, -0.5f, -0.5f);
    glColor3f(1, 1, 0); glVertex3f(0.5f, 0.5f, -0.5f);
    glColor3f(0, 1, 1); glVertex3f(0.5f, 0.5f, 0.5f);
    glColor3f(1, 0, 1); glVertex3f(0.5f, -0.5f, 0.5f);

    // Top face
    glColor3f(1, 0, 0); glVertex3f(-0.5f, 0.5f, -0.5f);
    glColor3f(1, 1, 0); glVertex3f(-0.5f, 0.5f, 0.5f);
    glColor3f(0, 1, 0); glVertex3f(0.5f, 0.5f, 0.5f);
    glColor3f(0, 1, 1); glVertex3f(0.5f, 0.5f, -0.5f);

    // Bottom face
    glColor3f(1, 0, 1); glVertex3f(-0.5f, -0.5f, -0.5f);
    glColor3f(1, 0, 0); glVertex3f(0.5f, -0.5f, -0.5f);
    glColor3f(1, 1, 0); glVertex3f(0.5f, -0.5f, 0.5f);
    glColor3f(0, 1, 0); glVertex3f(-0.5f, -0.5f, 0.5f);

    glEnd();
}

// ----- Display Function -----
void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    // Apply transformations: scale -> rotate -> translate
    glScalef(scaleXYZ[0], scaleXYZ[1], scaleXYZ[2]);
    glRotatef(rotateXYZ[0], 1, 0, 0);
    glRotatef(rotateXYZ[1], 0, 1, 0);
    glRotatef(rotateXYZ[2], 0, 0, 1);
    glTranslatef(translateXYZ[0], translateXYZ[1], translateXYZ[2]);

    drawCube();
    glutSwapBuffers();
}

// ----- Reshape Function -----
void reshape(int w, int h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    if (w <= h)
        glOrtho(-1.0, 1.0, -1.0 * (GLfloat)h / (GLfloat)w, 1.0 * (GLfloat)h / (GLfloat)w, -10.0, 10.0);
    else
        glOrtho(-1.0 * (GLfloat)w / (GLfloat)h, 1.0 * (GLfloat)w / (GLfloat)h, -1.0, 1.0, -10.0, 10.0);
    glMatrixMode(GL_MODELVIEW);
}

// ----- Keyboard Function -----
void keyboard(unsigned char key, int x, int y) {
    switch (key) {
    case 'x': if (currentMode == SCALE) scaleXYZ[0] += delta; else if (currentMode == ROTATE) rotateXYZ[0] += delta * 10; else translateXYZ[0] += delta; break;
    case 'X': if (currentMode == SCALE) scaleXYZ[0] -= delta; else if (currentMode == ROTATE) rotateXYZ[0] -= delta * 10; else translateXYZ[0] -= delta; break;
    case 'y': if (currentMode == SCALE) scaleXYZ[1] += delta; else if (currentMode == ROTATE) rotateXYZ[1] += delta * 10; else translateXYZ[1] += delta; break;
    case 'Y': if (currentMode == SCALE) scaleXYZ[1] -= delta; else if (currentMode == ROTATE) rotateXYZ[1] -= delta * 10; else translateXYZ[1] -= delta; break;
    case 'z': if (currentMode == SCALE) scaleXYZ[2] += delta; else if (currentMode == ROTATE) rotateXYZ[2] += delta * 10; else translateXYZ[2] += delta; break;
    case 'Z': if (currentMode == SCALE) scaleXYZ[2] -= delta; else if (currentMode == ROTATE) rotateXYZ[2] -= delta * 10; else translateXYZ[2] -= delta; break;
    case '+': delta += 0.01f; break;
    case '-': delta -= 0.01f; if (delta < 0.01f) delta = 0.01f; break;
    case 'r': scaleXYZ[0] = scaleXYZ[1] = scaleXYZ[2] = 1.0f;
        rotateXYZ[0] = rotateXYZ[1] = rotateXYZ[2] = 0.0f;
        translateXYZ[0] = translateXYZ[1] = translateXYZ[2] = 0.0f;
        delta = 0.1f; break;
    }
    glutPostRedisplay();
}

// ----- Menu Function -----
void menu(int option) {
    currentMode = (TransformMode)option;
    std::cout << "Current Mode: ";
    if (currentMode == SCALE) std::cout << "SCALE\n";
    else if (currentMode == ROTATE) std::cout << "ROTATE\n";
    else std::cout << "TRANSLATE\n";
}

// ----- Instructions -----
void printInstructions() {
    std::cout << "3D Cube Controls (Rainbow Faces):\n";
    std::cout << "Right-click menu to select: SCALE, ROTATE, TRANSLATE\n";
    std::cout << "X/x, Y/y, Z/z: increase/decrease component\n";
    std::cout << "+/- : increase/decrease delta\n";
    std::cout << "r : reset all transformations\n";
}
