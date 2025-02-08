#include <GL/glut.h>

static int day = 200; // day�ı仯����0��359

void myDisplay(void)
{
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
}

void updateDay(int value)
{
    day = (day + 1) % 360;  // ÿ������ 1
    glutPostRedisplay();    // ǿ�����»�����Ļ
    glutTimerFunc(50, updateDay, 0);  // 50 ������ٴε��ã������ٶ�
}

int main(int argc, char* argv[])
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(600, 600); // �������ڴ�С����Ӧ��Ұ
    glutCreateWindow("�����OpenGL����");
    glutDisplayFunc(&myDisplay);
    glutTimerFunc(50, updateDay, 0); // ��ʼ����ʱ���ص�����
    glEnable(GL_DEPTH_TEST);        // ������Ȳ���
    glutMainLoop();
    return 0;
}
