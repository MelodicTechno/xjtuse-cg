#include <GL/glut.h>
#include <stdlib.h>
#include <stdio.h>

#pragma warning(disable : 4996)

void myDisplay4(void)
{
    static GLubyte Mask[128];
    FILE* fp;
    fp = fopen("mask.bmp", "rb");
    if (!fp)
        exit(0);
    if (fseek(fp, -(int)sizeof(Mask), SEEK_END))
        exit(0);
    if (!fread(Mask, sizeof(Mask), 1, fp))
        exit(0);
    fclose(fp);
    glClear(GL_COLOR_BUFFER_BIT);
    glEnable(GL_POLYGON_STIPPLE);
    glPolygonStipple(Mask);
    glRectf(-0.5f, -0.5f, 0.0f, 0.0f);  // �����·�����һ�����ο�Ч����������
    glDisable(GL_POLYGON_STIPPLE);
    glRectf(0.0f, 0.0f, 0.5f, 0.5f);    // �����Ϸ�����һ�����ο�Ч����������
    glFlush();
}

void myDisplay(void)
{
    glClear(GL_COLOR_BUFFER_BIT);
    glPointSize(5.0f);
    glBegin(GL_POINTS);
    glVertex2f(0.0f, 0.0f);
    glVertex2f(0.5f, 0.5f);
    glEnd();
    glFlush();
}

void myDisplay2(void)
{
    glClear(GL_COLOR_BUFFER_BIT);
    glEnable(GL_LINE_STIPPLE);
    glLineStipple(2, 0x0F0F);
    glLineWidth(10.0f);
    glBegin(GL_LINES);
    glVertex2f(0.0f, 0.0f);
    glVertex2f(0.5f, 0.5f);
    glEnd();
    glFlush();
}

void myDisplay3(void)
{
    glClear(GL_COLOR_BUFFER_BIT);
    glPolygonMode(GL_FRONT, GL_FILL); // ��������Ϊ���ģʽ
    glPolygonMode(GL_BACK, GL_LINE);  // ���÷���Ϊ����ģʽ
    glFrontFace(GL_CCW);              // ������ʱ�뷽��Ϊ����
    glBegin(GL_POLYGON);              // ����ʱ�����һ�������Σ������·�
    glVertex2f(-0.5f, -0.5f);
    glVertex2f(0.0f, -0.5f);
    glVertex2f(0.0f, 0.0f);
    glVertex2f(-0.5f, 0.0f);
    glEnd();
    glBegin(GL_POLYGON);              // ��˳ʱ�����һ�������Σ������Ϸ�
    glVertex2f(0.0f, 0.0f);
    glVertex2f(0.0f, 0.5f);
    glVertex2f(0.5f, 0.5f);
    glVertex2f(0.5f, 0.0f);
    glEnd();
    glFlush();
}



int main(int argc, char* argv[])
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(400, 400);
    glutCreateWindow("������OpenGL����"); // ���ĵ��õ�display���л���ͼ����
    glutDisplayFunc(&myDisplay4);
    glutMainLoop();
    return 0;
}
