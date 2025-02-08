#include <GL/glut.h>
#include <cmath>

const GLdouble Pi = 3.1415926536;

void myDisplay(void) {
    glClear(GL_COLOR_BUFFER_BIT);
    glShadeModel(GL_FLAT);

    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(0.0f, 0.0f);
    for (int i = 0; i <= 8; ++i) {
        float r = (i & 0x04) ? 1.0f : 0.0f;
        float g = (i & 0x02) ? 1.0f : 0.0f;
        float b = (i & 0x01) ? 1.0f : 0.0f;
        glColor3f(r, g, b);
        glVertex2f(cos(i * Pi / 4), sin(i * Pi / 4));
    }
    glEnd();

    glFlush();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(400, 400);
    glutCreateWindow("µÚËÄ½²´úÂë2");

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glutDisplayFunc(myDisplay);

    glutMainLoop();
    return 0;
}
