#include <Windows.h>
#include <GL/glut.h>
#include <cmath>
#include <cstdio>
#include <cstdlib>

static int day = 200; // day�ı仯����0��359
float M_PI = 3.1415926535;

void myDisplay(void)
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glRotatef(0.005, 0, 0, -1);
    glColor3f(1.0f, 0.0f, 0.0f);
    glBegin(GL_POLYGON);
    glVertex2f(-0.4, -0.7);
    glVertex2f(-0.8, 0.0);
    glVertex2f(-0.4, 0.7);
    glVertex2f(0.4, 0.7);
    glVertex2f(0.8, 0.0);
    glVertex2f(0.4, -0.7);
    glEnd();
    glFlush();
    glutSwapBuffers();
}

void myIdle(void)
{
    /* �µĺ������ڿ���ʱ���ã������ǰ�day��1�����»��ƣ��ﵽ����Ч�� */
    ++day;
    if (day >= 360)
        day = 0;
    myDisplay();
}

static float scale = 1.0f;       // ���ű���
static float scaleDirection = 0.0001f; // ���ű仯��

// ����
void myDisplay2(void)
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // ʹ�� glPushMatrix �� glPopMatrix ��������ϵ
    glPushMatrix();

    // ���ű任
    glScalef(scale, scale, 1.0f);

    // ����������
    glColor3f(1.0f, 0.0f, 0.0f);
    glBegin(GL_POLYGON);
    glVertex2f(-0.4, -0.7);
    glVertex2f(-0.8, 0.0);
    glVertex2f(-0.4, 0.7);
    glVertex2f(0.4, 0.7);
    glVertex2f(0.8, 0.0);
    glVertex2f(0.4, -0.7);
    glEnd();

    glPopMatrix(); // �ָ�����ϵ

    glFlush();
    glutSwapBuffers();
}

void myIdle2(void)
{
    // ��̬�������ű���
    scale += scaleDirection;

    // ������ű���������Χ���ı����ŷ���
    if (scale >= 2.0f || scale <= 0.5f) {
        scaleDirection = -scaleDirection;
    }

    // �����ػ�
    glutPostRedisplay();
}


int main(int argc, char* argv[])
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE); // �޸��˲���ΪGLUT_DOUBLE
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(400, 400);
    glutCreateWindow("�ڶ���");   // ���˴��ڱ���
    glutDisplayFunc(&myDisplay2);
    glutIdleFunc(&myIdle2);               // �¼��������
    glutMainLoop();
    return 0;
}