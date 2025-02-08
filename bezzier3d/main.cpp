#include <stdlib.h>
#include <time.h>
#include <GL/glut.h>
/* ���Ƶ������ */
//GLfloat ctrlpoints[4][4][3] = {
//    { { -1.5, -1.5, 2.0 },
//    { -0.5, -1.5, 2.0 },
//    { 0.5, -1.5, -1.0 },
//    { 1.5, -1.5, 2.0 } },
//
//    { { -1.5, -0.5, 1.0 },
//    { -0.5, 1.5, 2.0 },
//    { 0.5, 0.5, 1.0 },
//    { 1.5, -0.5, -1.0 } },
//
//    { { -1.5, 0.5, 2.0 },
//    { -0.5, 0.5, 1.0 },
//    { 0.5, 0.5, 3.0 },
//    { 1.5, -1.5, 1.5 } },
//
//    { { -1.5, 1.5, -2.0 },
//    { -0.5, 1.5, -2.0 },
//    { 0.5, 0.5, 1.0 },
//    { 1.5, 1.5, -1.0 } } };

GLfloat ctrlpoints[4][4][3] = {
    { { 1.5, 1.5, 2.0 },
    { -0.5, -1.5, 2.0 },
    { 0.5, -1.5, -1.0 },
    { 1.5, -1.5, 2.0 } },

    { { 1.5, 0.5, 1.0 },
    { 0.5, 1.5, 2.0 },
    { 0.5, 0.5, 1.0 },
    { 1.5, -0.5, -1.0 } },

    { { -1.5, 0.5, 2.0 },
    { -0.5, 0.5, 1.0 },
    { 0.5, 0.5, 3.0 },
    { 1.5, -1.5, 1.5 } },

    { { -1.5, 1.5, -2.0 },
    { -0.5, 1.5, -2.0 },
    { 0.5, 0.5, 1.0 },
    { 1.5, 1.5, -1.0 } } };

void init(void)
{
    //����ɫ
    glClearColor(0.0, 0.0, 0.0, 1.0);
    //�����Ƶ�����ӳ��Ϊ��������
    //����1��GL_MAP1_VERTEX_3��3ά������
    //����2��3�����Ʋ���u��ȡֵ��Χ[0, 1]
    //����4��x����Ԫ�ؼ�Ĳ���Ϊ3��GLfloat
    //����5��x�������߼�Ĳ���Ϊ4�����Ƶ㡪��������4�����Ƶ�ȷ��
    //����6-7�����Ʋ���v��ȡֵ��Χ[0, 1]
    //����8��y����Ԫ�ؼ�Ĳ���Ϊ12��GLfloatԪ��
    //����9��y����ÿ�����ߵĿ��Ƶ�����Ϊ4
    //note: ������������������ز�����������ctrlpoints���ݸ������߲���
    glMap2f(GL_MAP2_VERTEX_3, 0, 1, 3, 4, 0, 1, 12, 4, &ctrlpoints[0][0][0]);
    //�����άӳ��
    glEnable(GL_MAP2_VERTEX_3);
    //��άӳ�䣺x��y����U��V�Ĳ���[0, 1]�����м��ֵ����Ϊ��20��
    glMapGrid2f(20, 0.0, 1.0, 20, 0.0, 1.0);
    //������Ȳ���
    glDepthFunc(GL_LESS);
    glEnable(GL_DEPTH_TEST);
    //���뿪��4��ȡ���������д��룬�鿴������ʾЧ������
    //���Զ���ʸ������
    //glEnable(GL_AUTO_NORMAL);
    //�������򻯷�ʸ��
    //glEnable(GL_NORMALIZE);

    //���뿪��3�����ò������Դ
    GLfloat ambient[] = { 0.4, 0.6, 0.2, 1.0 };
    GLfloat position[] = { 0.0, 1.0, 3.0, 1.0 };
    GLfloat mat_diffuse[] = { 0.2, 0.4, 0.8, 1.0 };
    GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat mat_shininess[] = { 80.0 };
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
    glLightfv(GL_LIGHT0, GL_POSITION, position);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
}

void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glColor3f(0.0, 1.0, 0.0);
    //�����ϣ����ת��������push��pop���������ע�͵�glRotatef��
    glPushMatrix();
    //���뿪��1��ȥ��ע�Ͳ鿴Ч����������ת�ǶȲ������鿴Ч��
    //glRotatef(1.0, 1.0, 1.0, 1.0);
    glEvalMesh2(GL_FILL, 0, 20, 0, 20);
    glPopMatrix();
    glutSwapBuffers();
}

void reshape(GLsizei w, GLsizei h)
{
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    if (w <= h)
        glOrtho(-5.0, 5.0, -5.0 * (GLfloat)h / (GLfloat)w, 5.0 * (GLfloat)h / (GLfloat)w, -5.0, 5.0);
    else
        glOrtho(-5.0 * (GLfloat)w / (GLfloat)h, 5.0 * (GLfloat)w / (GLfloat)h, -5.0, 5.0, -5.0, 5.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void keyboard(unsigned char key, int x, int y)
{
    switch (key)
    {
    case 'x':
    case 'X':
    case 27:   //ESC��
        exit(0);
        break;
    default:
        break;
    }
}

int main(int argc, char** argv)
{
    srand((unsigned int)time(0));
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);//ʹ��˫����ģʽ����Ȼ���
    glutInitWindowSize(800, 800);
    glutInitWindowPosition(0, 0);
    glutCreateWindow("Bezier����");
    init();
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutIdleFunc(display);//���ÿ���ʱ���õĺ���
    glutMainLoop();
    return 0;
}