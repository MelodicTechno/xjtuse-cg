#include <GL/glut.h>

// 初始化光照
void initLighting() {
    glEnable(GL_LIGHT0); // 启用0号光源
    glEnable(GL_LIGHTING); // 启用光照

    // 设置环境光
    GLfloat ambientLight[] = { 0.2, 0.2, 0.2, 1.0 };
    glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);

    // 设置漫射光
    GLfloat diffuseLight[] = { 0.8, 0.8, 0.8, 1.0 };
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);

    // 设置光源位置
    GLfloat lightPosition[] = { 1.0, 1.0, 1.0, 0.0 };
    glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
}

// 初始化材质
void initMaterial() {
    // 设置材质的漫反射颜色
    GLfloat materialDiffuse[] = { 1.0, 0.0, 0.0, 1.0 }; // 红色
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
    glutSolidSphere(1.0, 50, 50); // 绘制实心球
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
