#define WindowWidth  400
#define WindowHeight 400
#define WindowTitle  "OpenGL纹理测试"

#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>

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

int power_of_two(int n)
{
    if (n <= 0)
        return 0;
    return (n & (n - 1)) == 0;
}

/* 函数load_texture
 * 读取一个BMP文件作为纹理
 * 如果失败，返回0，如果成功，返回纹理编号
 */
GLuint load_texture(const char* file_name)
{
    GLint width, height, total_bytes;
    GLubyte* pixels = 0;
    GLint last_texture_ID;

    // 打开文件，如果失败，返回
    FILE* pFile = fopen(file_name, "rb");
    if (pFile == 0)
        return 0;

    // 读取文件中图象的宽度和高度
    fseek(pFile, 0x0012, SEEK_SET);
    fread(&width, 4, 1, pFile);
    fread(&height, 4, 1, pFile);
    fseek(pFile, BMP_Header_Length, SEEK_SET);

    // 计算每行像素所占字节数，并根据此数据计算总像素字节数
    {
        GLint line_bytes = width * 3;
        while (line_bytes % 4 != 0)
            ++line_bytes;
        total_bytes = line_bytes * height;
    }

    // 根据总像素字节数分配内存
    pixels = (GLubyte*)malloc(total_bytes);
    if (pixels == 0)
    {
        fclose(pFile);
        return 0;
    }

    // 读取像素数据
    if (fread(pixels, total_bytes, 1, pFile) <= 0)
    {
        free(pixels);
        fclose(pFile);
        return 0;
    }

    // 在旧版本的OpenGL中
    // 如果图象的宽度和高度不是的整数次方，则需要进行缩放
    // 这里并没有检查OpenGL版本，出于对版本兼容性的考虑，按旧版本处理
    // 另外，无论是旧版本还是新版本，
    // 当图象的宽度和高度超过当前OpenGL实现所支持的最大值时，也要进行缩放
    {
        GLint max;
        glGetIntegerv(GL_MAX_TEXTURE_SIZE, &max);
        if (!power_of_two(width)
            || !power_of_two(height)
            || width > max
            || height > max)
        {
            const GLint new_width = 256;
            const GLint new_height = 256; // 规定缩放后新的大小为边长的正方形
            GLint new_line_bytes, new_total_bytes;
            GLubyte* new_pixels = 0;

            // 计算每行需要的字节数和总字节数
            new_line_bytes = new_width * 3;
            while (new_line_bytes % 4 != 0)
                ++new_line_bytes;
            new_total_bytes = new_line_bytes * new_height;

            // 分配内存
            new_pixels = (GLubyte*)malloc(new_total_bytes);
            if (new_pixels == 0)
            {
                free(pixels);
                fclose(pFile);
                return 0;
            }

            // 进行像素缩放
            gluScaleImage(GL_RGB,
                width, height, GL_UNSIGNED_BYTE, pixels,
                new_width, new_height, GL_UNSIGNED_BYTE, new_pixels);

            // 释放原来的像素数据，把pixels指向新的像素数据，并重新设置width和height
            free(pixels);
            pixels = new_pixels;
            width = new_width;
            height = new_height;
        }
    }

    // 分配一个新的纹理编号
    GLuint texture_ID = 0;  // Change from GLint to GLuint
    glGenTextures(1, &texture_ID);
    if (texture_ID == 0)
    {
        free(pixels);
        fclose(pFile);
        return 0;
    }

    // 绑定新的纹理，载入纹理并设置纹理参数
    // 在绑定前，先获得原来绑定的纹理编号，以便在最后进行恢复
    glGetIntegerv(GL_TEXTURE_BINDING_2D, &last_texture_ID);
    glBindTexture(GL_TEXTURE_2D, texture_ID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0,
        GL_BGR_EXT, GL_UNSIGNED_BYTE, pixels);
    glBindTexture(GL_TEXTURE_2D, last_texture_ID);

    // 之前为pixels分配的内存可在使用glTexImage2D以后释放
    // 因为此时像素数据已经被OpenGL另行保存了一份（可能被保存到专门的图形硬件中）
    free(pixels);
    return texture_ID;
}

/* 将当前纹理BGR格式转换为BGRA格式
 * 纹理中像素的RGB值如果与指定rgb相差不超过absolute，则将Alpha设置为0.0，否则设置为1.0
 */
void texture_colorkey(GLubyte r, GLubyte g, GLubyte b, GLubyte absolute)
{
    GLint width, height;
    GLubyte* pixels = 0;

    // 获得纹理的大小信息
    glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &width);
    glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &height);

    // 分配空间并获得纹理像素
    pixels = (GLubyte*)malloc(width * height * 4);
    if (pixels == 0)
        return;
    glGetTexImage(GL_TEXTURE_2D, 0, GL_BGRA_EXT, GL_UNSIGNED_BYTE, pixels);

    // 修改像素中的Alpha值
    // 其中pixels[i*4], pixels[i*4+1], pixels[i*4+2], pixels[i*4+3]
    //   分别表示第i个像素的蓝、绿、红、Alpha四种分量，0表示最小，255表示最大
    {
        GLint i;
        GLint count = width * height;
        for (i = 0; i < count; ++i)
        {
            if (abs(pixels[i * 4] - b) <= absolute
                && abs(pixels[i * 4 + 1] - g) <= absolute
                && abs(pixels[i * 4 + 2] - r) <= absolute)
                pixels[i * 4 + 3] = 0;
            else
                pixels[i * 4 + 3] = 255;
        }
    }

    // 将修改后的像素重新设置到纹理中，释放内存
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0,
        GL_BGRA_EXT, GL_UNSIGNED_BYTE, pixels);
    free(pixels);
}

void display(void)
{
    static int initialized = 0;
    static GLuint texWindow = 0;
    static GLuint texPicture = 0;

    // 执行初始化操作，包括：读取相片，读取相框，将相框由BGR颜色转换为BGRA，启用二维纹理
    if (!initialized)
    {
        texPicture = load_texture("pic.bmp");
        texWindow = load_texture("window.bmp");
        glBindTexture(GL_TEXTURE_2D, texWindow);
        texture_colorkey(255, 255, 255, 10);

        glEnable(GL_TEXTURE_2D);

        initialized = 1;
    }

    // 清除屏幕
    glClear(GL_COLOR_BUFFER_BIT);

    // 绘制相片，此时不需要进行Alpha测试，所有的像素都进行绘制
    glBindTexture(GL_TEXTURE_2D, texPicture);
    glDisable(GL_ALPHA_TEST);
    glBegin(GL_QUADS);
    glTexCoord2f(0, 0);     glVertex2f(-1.0f, -1.0f);
    glTexCoord2f(0, 1);     glVertex2f(-1.0f, 1.0f);
    glTexCoord2f(1, 1);     glVertex2f(1.0f, 1.0f);
    glTexCoord2f(1, 0);     glVertex2f(1.0f, -1.0f);
    glEnd();

    // 绘制相框，此时进行Alpha测试，只绘制不透明部分的像素
    glBindTexture(GL_TEXTURE_2D, texWindow);
    glEnable(GL_ALPHA_TEST);
    glAlphaFunc(GL_GREATER, 0.5f);
    glBegin(GL_QUADS);
    glTexCoord2f(0, 0);     glVertex2f(-1.0f, -1.0f);
    glTexCoord2f(0, 1);     glVertex2f(-1.0f, 1.0f);
    glTexCoord2f(1, 1);     glVertex2f(1.0f, 1.0f);
    glTexCoord2f(1, 0);     glVertex2f(1.0f, -1.0f);
    glEnd();

    // 交换缓冲
    glutSwapBuffers();
}

int main(int argc, char* argv[])
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(400, 400);
    glutCreateWindow("图片组合");
    glutDisplayFunc(&display);
    glutMainLoop();
    return 0;
}