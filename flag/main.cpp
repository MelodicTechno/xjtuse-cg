#include <GL/glut.h>

float angle = 0.0f; // ���ڿ�����ת�Ƕ�

// ��ʼ��OpenGL����
void initOpenGL() {
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f); // ���ñ�����ɫΪ��ɫ
    glEnable(GL_DEPTH_TEST); // ������Ȳ���
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0f, 1.0f, 1.0f, 50.0f); // ����͸��ͶӰ
    glMatrixMode(GL_MODELVIEW);
}

// ���ƺ�������
void drawFlag() {
    glPushMatrix();

    // �������Ƶ���˶���
    glTranslatef(0.0f, 5.0f, 0.0f); // �������Ƶ���˶���

    // ��ת����
    glRotatef(angle, 0.0f, 1.0f, 0.0f); // ��y����ת

    // ���ƺ���ľ��β���
    glColor3f(1.0f, 0.0f, 0.0f); // ��ɫ
    glBegin(GL_QUADS);
    glVertex3f(0.0f, 1.5f, 0.0f);  // ���Ͻ�
    glVertex3f(6.0f, 1.0f, 0.0f);   // ���Ͻ�
    glVertex3f(6.0f, -1.0f, 0.0f);  // ���½�
    glVertex3f(0.0f, -1.5f, 0.0f); // ���½�
    glEnd();

    // �������
    glColor3f(0.0f, 0.0f, 0.0f); // ��ɫ
    glBegin(GL_QUADS);
    glVertex3f(0.0f, 1.5f, 0.0f);  // ���Ͻ�
    glVertex3f(-0.1f, 1.5f, 0.0f);   // ���Ͻ�
    glVertex3f(0.0f, -15.0f, 0.0f);  // ���½�
    glVertex3f(-0.1f, -15.0f, 0.0f); // ���½�
    glEnd();

    glPopMatrix();
}

// ��Ⱦ����
void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // �����ɫ����Ȼ�����
    glLoadIdentity(); // ����ģ����ͼ����
    glTranslatef(0.0f, 0.0f, -20.0f); // �ƶ���ͼ���ʵ���λ��

    drawFlag(); // ���ƺ���

    glutSwapBuffers(); // ����������
}

// ���¶���
void update(int value) {
    angle += 2.0f; // ÿ�θ�����ת�Ƕ�
    if (angle > 360.0f) angle -= 360.0f; // ȷ���ǶȲ��ᳬ��360��

    glutPostRedisplay(); // �������»���
    glutTimerFunc(16, update, 0); // ÿ16�������һ��
}

// ������
int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH); // ʹ��˫�������Ȼ���
    glutInitWindowSize(800, 600); // ���ô��ڴ�С
    glutCreateWindow("��ת�ĺ��������"); // ��������

    initOpenGL(); // ��ʼ��OpenGL����

    glutDisplayFunc(display); // ������ʾ�ص�����
    glutTimerFunc(25, update, 0); // ���ö�ʱ���ص��������¶���

    glutMainLoop(); // ������ѭ��
    return 0;
}
