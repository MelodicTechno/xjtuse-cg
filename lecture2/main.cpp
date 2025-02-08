#include <GL/glut.h>
#include <math.h>
const int n = 20;
const GLfloat R = 0.5f;
const GLfloat Pi = 3.1415926536f;
const GLfloat factor = 0.1f;

void myDisplay3(void)
{
    GLfloat x;
    glClear(GL_COLOR_BUFFER_BIT);
    glBegin(GL_LINES);
    glVertex2f(-1.0f, 0.0f);
    glVertex2f(1.0f, 0.0f);        // 以上两个点可以画x轴
    glVertex2f(0.0f, -1.0f);
    glVertex2f(0.0f, 1.0f);        // 以上两个点可以画y轴
    glEnd();
    glBegin(GL_LINE_STRIP);
    for (x = -1.0f / factor; x < 1.0f / factor; x += 0.01f)
    {
        glVertex2f(x * factor, sin(x) * factor);
    }
    glEnd();
    glFlush();
}

void myDisplay(void)
{
    int i;
    glClear(GL_COLOR_BUFFER_BIT);
    glBegin(GL_POLYGON);
    for (i = 0; i < n; ++i)
        glVertex2f(R * cos(2 * Pi / n * i), R * sin(2 * Pi / n * i));
    glEnd();
    glFlush();
}

void myDisplay2(void)
{
    GLfloat a = 1 / (2 - 2 * cos(72 * Pi / 180));
    GLfloat bx = a * cos(18 * Pi / 180);
    GLfloat by = a * sin(18 * Pi / 180);
    GLfloat cy = -a * cos(18 * Pi / 180);
    GLfloat
        PointA[2] = { 0, a },
        PointB[2] = { bx, by },
        PointC[2] = { 0.5, cy },
        PointD[2] = { -0.5, cy },
        PointE[2] = { -bx, by };

    glClear(GL_COLOR_BUFFER_BIT);
    // 按照A->C->E->B->D->A的顺序，可以一笔将五角星画出
    glBegin(GL_LINE_LOOP);
    glVertex2fv(PointA);
    glVertex2fv(PointC);
    glVertex2fv(PointE);
    glVertex2fv(PointB);
    glVertex2fv(PointD);
    glEnd();
    glFlush();
}

int main(int argc, char* argv[])
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(400, 400);
    glutCreateWindow("第二个OpenGL程序");
    glutDisplayFunc(&myDisplay2);
    glutMainLoop();
    return 0;
}
