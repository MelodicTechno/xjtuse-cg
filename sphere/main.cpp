#include <GL/glut.h>

// ��ʼ������
void initLighting() {
    glEnable(GL_LIGHT0); // ����0�Ź�Դ
    glEnable(GL_LIGHTING); // ���ù���

    // ���û�����
    GLfloat ambientLight[] = { 0.2, 0.2, 0.2, 1.0 };
    glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);

    // ���������
    GLfloat diffuseLight[] = { 0.8, 0.8, 0.8, 1.0 };
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);

    // ���ù�Դλ��
    GLfloat lightPosition[] = { 1.0, 1.0, 1.0, 0.0 };
    glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
}

// ��ʼ������
void initMaterial() {
    // ���ò��ʵ���������ɫ
    GLfloat materialDiffuse[] = { 1.0, 0.0, 0.0, 1.0 }; // ��ɫ
    glMaterialfv(GL_FRONT, GL_DIFFUSE, materialDiffuse);
}

void init() {
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glEnable(GL_DEPTH_TEST);
    initLighting();
    initMaterial();
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glutSolidSphere(1.0, 50, 50); // ����ʵ����
    glFlush();
}

void reshape(int w, int h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, (GLfloat)w / (GLfloat)h, 1.0, 100.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(0.0, 0.0, 5.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("OpenGL Colored Sphere with Lighting");
    init();
    glutDisplayFunc(&display);
    glutReshapeFunc(reshape);
    glutMainLoop();
    return 0;
}
