#define WindowWidth  400
#define WindowHeight 400

#include <stdio.h>
#include <stdlib.h>
#include <GL/glut.h>

#pragma warning(disable : 4996)

/* ����grab
 * ץȡ�����е�����
 * ���贰�ڿ��ΪWindowWidth���߶�ΪWindowHeight
 */
#define BMP_Header_Length 54

void grab(void)
{
    glReadBuffer(GL_FRONT);
    FILE* pDummyFile;
    FILE* pWritingFile;
    GLubyte* pPixelData;
    GLubyte  BMP_Header[BMP_Header_Length];
    GLint    i, j;
    GLint    PixelDataLength;

    // �����������ݵ�ʵ�ʳ���
    i = WindowWidth * 3;   // �õ�ÿһ�е��������ݳ���
    while (i % 4 != 0)      // �������ݣ�ֱ��i�ǵı���
        ++i;               // �������и�����㷨��
    // �������׷��ֱ�ۣ����ٶ�û��̫��Ҫ��
    PixelDataLength = i * WindowHeight;

    // �����ڴ�ʹ��ļ�
    pPixelData = (GLubyte*)malloc(PixelDataLength);
    if (pPixelData == 0)
        exit(0);

    pDummyFile = fopen("dummy.bmp", "rb");
    if (pDummyFile == 0)
        exit(0);

    pWritingFile = fopen("grab.bmp", "wb");
    if (pWritingFile == 0)
        exit(0);

    // ��ȡ����
    glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
    glReadPixels(0, 0, WindowWidth, WindowHeight,
        GL_BGR_EXT, GL_UNSIGNED_BYTE, pPixelData);

    // ��dummy.bmp���ļ�ͷ����Ϊ���ļ����ļ�ͷ
    fread(BMP_Header, sizeof(BMP_Header), 1, pDummyFile);
    fwrite(BMP_Header, sizeof(BMP_Header), 1, pWritingFile);
    fseek(pWritingFile, 0x0012, SEEK_SET);
    i = WindowWidth;
    j = WindowHeight;
    fwrite(&i, sizeof(i), 1, pWritingFile);
    fwrite(&j, sizeof(j), 1, pWritingFile);

    // д����������
    fseek(pWritingFile, 0, SEEK_END);
    fwrite(pPixelData, PixelDataLength, 1, pWritingFile);

    // �ͷ��ڴ�͹ر��ļ�
    fclose(pDummyFile);
    fclose(pWritingFile);
    free(pPixelData);
}

void draw_sphere()
{
    // ���ù�Դ
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    {
        GLfloat
            pos[] = { 5.0f, 5.0f, 0.0f, 1.0f },
            ambient[] = { 0.0f, 0.0f, 1.0f, 1.0f };
        glLightfv(GL_LIGHT0, GL_POSITION, pos);
        glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
    }

    // ����һ������
    glColor3f(1, 0, 0);
    glPushMatrix();
    glTranslatef(0, 0, 2);
    glutSolidSphere(0.5, 20, 20);
    glPopMatrix();
}

void display(void)
{
    // �����Ļ
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // ���ù۲��
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60, 1, 5, 25);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(5, 0, 6.5, 0, 0, 0, 0, 1, 0);

    glEnable(GL_DEPTH_TEST);

    // ��������
    glDisable(GL_STENCIL_TEST);
    draw_sphere();

    // ����һ��ƽ�澵���ڻ��Ƶ�ͬʱע������ģ�建�塣
    // ���⣬Ϊ�˱�֤ƽ�澵֮��ľ����ܹ���ȷ���ƣ��ڻ���ƽ�澵ʱ��Ҫ����Ȼ���������Ϊֻ���ġ�
    // �ڻ���ʱ��ʱ�رչ���Ч��
    glClearStencil(0);
    glClear(GL_STENCIL_BUFFER_BIT);
    glStencilFunc(GL_ALWAYS, 1, 0xFF);
    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
    glEnable(GL_STENCIL_TEST);

    glDisable(GL_LIGHTING);
    glColor3f(0.5f, 0.5f, 0.5f);
    glDepthMask(GL_FALSE);
    glRectf(-1.5f, -1.5f, 1.5f, 1.5f);
    glDepthMask(GL_TRUE);

    // ����һ������ǰ�������ƽ�澵�ԳƵ����壬ע���Դ��λ��ҲҪ�����ԳƸı�
    // ��Ϊƽ�澵����X���Y����ȷ����ƽ�棬����ֻҪZ����ȡ������ʵ�ֶԳ�
    // Ϊ�˱�֤����Ļ��Ʒ�Χ��������ƽ�澵�ڲ���ʹ��ģ�����
    glStencilFunc(GL_EQUAL, 1, 0xFF);
    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
    glScalef(1.0f, 1.0f, -1.0f);
    draw_sphere();

    // ��������
    glutSwapBuffers();

    // ��ͼ
    grab();
}

int main(int argc, char* argv[])
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(400, 400);
    glutCreateWindow("ģ�����");
    glutDisplayFunc(&display);
    glutMainLoop();
    return 0;
}