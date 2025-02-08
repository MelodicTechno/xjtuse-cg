#include <Windows.h>
#include <GL/glut.h>
#include <cmath>
#include <cstdio>
#include <cstdlib>

static int day = 200; // day�ı仯����0��359
void myDisplay(void)
{
    glClearColor(1.0f, 1.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glTranslatef(0.00001, 0.0f, 0.0f); // ÿ�ε���myDisplay()ʱ������ͼ����x��������ƽ��0.00001����λ
    glColor3f(1.0f, 0.0f, 0.0f);
    glRectf(-0.3f, 0.2f, -0.1f, 0.4f);
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

int main(int argc, char* argv[])
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE); // �޸��˲���ΪGLUT_DOUBLE
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(400, 400);
    glutCreateWindow("ʹ�� glTranslatef()������ʵ��2Dͼ��ƽ��"); // ���˴��ڱ���
    glutDisplayFunc(&myDisplay);
    glutIdleFunc(&myIdle); // �¼��������
    glutMainLoop();
    return 0;
}