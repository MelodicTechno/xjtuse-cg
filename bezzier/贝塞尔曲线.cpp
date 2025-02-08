#include <GL/glut.h>

// ���屴�������ߵĿ��Ƶ�
//GLfloat controlPoints[4][3] = {
//    { -1.5, -1.5, 0.0 },
//    { -0.5, 0.5, 0.0 },
//    { 0.5, -1.5, 0.0 },
//    { 1.5, 2.5, 0.0 }
//};

GLfloat controlPoints[4][3] = {
    { -1.5, -1.5, 0.0 },
    { -0.5, 0.2, 0.0 },
    { 0.5, -1.2, 0.0 },
    { -1.1, 2.5, 0.0 }
};

// ����ڵ��������������α��������ߣ��ڵ�����Ӧ����{0, 0, 0, 0, 1, 1, 1, 1}
GLfloat knots[8] = { 0.0, 0.0, 0.0, 0.0, 1.0, 1.0, 1.0, 1.0 };

// ��ʼ��OpenGL����
void init() {
    glClearColor(1.0, 1.0, 1.0, 1.0); // ���ñ�����ɫΪ��ɫ
    glColor3f(0.0, 0.0, 0.0); // ���û�ͼ��ɫΪ��ɫ
}

// ��ʾ�ص�����
void display() {
    glClear(GL_COLOR_BUFFER_BIT); // �����ɫ������

    // ���ƿ��Ƶ�
    glBegin(GL_POINTS);
    for (int i = 0; i < 4; i++) {
        glVertex3fv(controlPoints[i]);
    }
    glEnd();

    // ���Ʊ���������
    GLUnurbs* nurbs = gluNewNurbsRenderer();
    gluBeginCurve(nurbs);
    gluNurbsCurve(nurbs, 8, knots, 3, &controlPoints[0][0], 4, GL_MAP1_VERTEX_3);
    gluEndCurve(nurbs);
    gluDeleteNurbsRenderer(nurbs);

    glFlush(); // ǿ��ִ��OpenGL����
}

// ������
int main(int argc, char** argv) {
    glutInit(&argc, argv); // ��ʼ��GLUT
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB); // ������ʾģʽ
    glutInitWindowSize(400, 400); // ���ô��ڴ�С
    glutCreateWindow("Bezier Curve Example"); // ��������
    init(); // ��ʼ��OpenGL����
    glutDisplayFunc(display); // ������ʾ�ص�����
    glutMainLoop(); // ����GLUT�¼�����ѭ��
    return 0;
}
