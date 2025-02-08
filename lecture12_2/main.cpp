#define WindowWidth  400
#define WindowHeight 400

#include <stdio.h>
#include <stdlib.h>
#include <GL/glut.h>

#pragma warning(disable : 4996)

/* 函数grab
 * 抓取窗口中的像素
 * 假设窗口宽度为WindowWidth，高度为WindowHeight
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

    // 计算像素数据的实际长度
    i = WindowWidth * 3;   // 得到每一行的像素数据长度
    while (i % 4 != 0)      // 补充数据，直到i是的倍数
        ++i;               // 本来还有更快的算法，
    // 但这里仅追求直观，对速度没有太高要求
    PixelDataLength = i * WindowHeight;

    // 分配内存和打开文件
    pPixelData = (GLubyte*)malloc(PixelDataLength);
    if (pPixelData == 0)
        exit(0);

    pDummyFile = fopen("dummy.bmp", "rb");
    if (pDummyFile == 0)
        exit(0);

    pWritingFile = fopen("grab.bmp", "wb");
    if (pWritingFile == 0)
        exit(0);

    // 读取像素
    glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
    glReadPixels(0, 0, WindowWidth, WindowHeight,
        GL_BGR_EXT, GL_UNSIGNED_BYTE, pPixelData);

    // 把dummy.bmp的文件头复制为新文件的文件头
    fread(BMP_Header, sizeof(BMP_Header), 1, pDummyFile);
    fwrite(BMP_Header, sizeof(BMP_Header), 1, pWritingFile);
    fseek(pWritingFile, 0x0012, SEEK_SET);
    i = WindowWidth;
    j = WindowHeight;
    fwrite(&i, sizeof(i), 1, pWritingFile);
    fwrite(&j, sizeof(j), 1, pWritingFile);

    // 写入像素数据
    fseek(pWritingFile, 0, SEEK_END);
    fwrite(pPixelData, PixelDataLength, 1, pWritingFile);

    // 释放内存和关闭文件
    fclose(pDummyFile);
    fclose(pWritingFile);
    free(pPixelData);
}

void draw_sphere()
{
    // 设置光源
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    {
        GLfloat
            pos[] = { 5.0f, 5.0f, 0.0f, 1.0f },
            ambient[] = { 0.0f, 0.0f, 1.0f, 1.0f };
        glLightfv(GL_LIGHT0, GL_POSITION, pos);
        glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
    }

    // 绘制一个球体
    glColor3f(1, 0, 0);
    glPushMatrix();
    glTranslatef(0, 0, 2);
    glutSolidSphere(0.5, 20, 20);
    glPopMatrix();
}

void display(void)
{
    // 清除屏幕
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // 设置观察点
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60, 1, 5, 25);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(5, 0, 6.5, 0, 0, 0, 0, 1, 0);

    glEnable(GL_DEPTH_TEST);

    // 绘制球体
    glDisable(GL_STENCIL_TEST);
    draw_sphere();

    // 绘制一个平面镜。在绘制的同时注意设置模板缓冲。
    // 另外，为了保证平面镜之后的镜像能够正确绘制，在绘制平面镜时需要将深度缓冲区设置为只读的。
    // 在绘制时暂时关闭光照效果
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

    // 绘制一个与先前球体关于平面镜对称的球体，注意光源的位置也要发生对称改变
    // 因为平面镜是在X轴和Y轴所确定的平面，所以只要Z坐标取反即可实现对称
    // 为了保证球体的绘制范围被限制在平面镜内部，使用模板测试
    glStencilFunc(GL_EQUAL, 1, 0xFF);
    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
    glScalef(1.0f, 1.0f, -1.0f);
    draw_sphere();

    // 交换缓冲
    glutSwapBuffers();

    // 截图
    grab();
}

int main(int argc, char* argv[])
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(400, 400);
    glutCreateWindow("模板测试");
    glutDisplayFunc(&display);
    glutMainLoop();
    return 0;
}