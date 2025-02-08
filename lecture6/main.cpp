#include <GL/glut.h>
#include <stdio.h>
#include <time.h>

// ̫�������������
// ����ÿ���¶���12��
// һ��12���£�����360��
static int day = 200; // day�ı仯����0��359

double CalFrequency()
{
    static int count;
    static double save;
    static clock_t last, current;
    double timegap;

    ++count;
    if (count <= 50)
        return save;
    count = 0;
    last = current;
    current = clock();
    timegap = (current - last) / (double)CLK_TCK;
    save = 50.0 / timegap;
    return save;
}

void myDisplay(void)
{
    double FPS = CalFrequency();
    printf("FPS = %f\n", FPS);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // ����ͶӰ����
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(75, 1, 1, 1000000);  // �����Ӿ෶Χ�������Զ

    // ������ͼ����
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(0, -500000, 500000, 0, 0, 0, 0, 0, 1); // �����ӽ�λ��

    // ���ƺ�ɫ�ġ�̫����
    glColor3f(1.0f, 0.0f, 0.0f);
    glutSolidSphere(10000, 20, 20);  // ����̫���İ뾶

    // ������ɫ�ġ�����
    glPushMatrix();  // ���浱ǰ����
    glColor3f(0.0f, 0.0f, 1.0f);
    glRotatef(day, 0.0f, 0.0f, -1.0f); // ��������ת
    glTranslatef(300000, 0.0f, 0.0f);  // �����ƽ�ƣ����پ��룩
    glutSolidSphere(5000, 20, 20);   // ��������İ뾶
    glPopMatrix();   // �ָ�����

    // ���ƻ�ɫ�ġ�������
    glPushMatrix();  // ���浱ǰ����
    glColor3f(1.0f, 1.0f, 0.0f);
    glRotatef(day, 0.0f, 0.0f, -1.0f); // ����Χ��̫����ת
    glTranslatef(300000, 0.0f, 0.0f);  // �����ƽ��
    glRotatef(day * 12, 0.0f, 0.0f, -1.0f); // ����Χ�Ƶ�����ת
    glTranslatef(50000, 0.0f, 0.0f);  // ������ƽ��
    glutSolidSphere(2000, 20, 20);    // ���������İ뾶
    glPopMatrix();   // �ָ�����

    glFlush();
    glutSwapBuffers();
}

void myIdle(void)
{
    ++day;
    if (day >= 360)
        day = 0;
    myDisplay();
}

int main(int argc, char* argv[])
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(400, 400);
    glutCreateWindow("̫�������������");
    glutDisplayFunc(&myDisplay);
    glutIdleFunc(&myIdle);
    glutMainLoop();
    return 0;
}
