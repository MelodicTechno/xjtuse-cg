#define WindowWidth  400
#define WindowHeight 400
#define WindowTitle  "OpenGL�������"

#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>

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

int power_of_two(int n)
{
    if (n <= 0)
        return 0;
    return (n & (n - 1)) == 0;
}

/* ����load_texture
 * ��ȡһ��BMP�ļ���Ϊ����
 * ���ʧ�ܣ�����0������ɹ�������������
 */
GLuint load_texture(const char* file_name)
{
    GLint width, height, total_bytes;
    GLubyte* pixels = 0;
    GLint last_texture_ID;

    // ���ļ������ʧ�ܣ�����
    FILE* pFile = fopen(file_name, "rb");
    if (pFile == 0)
        return 0;

    // ��ȡ�ļ���ͼ��Ŀ�Ⱥ͸߶�
    fseek(pFile, 0x0012, SEEK_SET);
    fread(&width, 4, 1, pFile);
    fread(&height, 4, 1, pFile);
    fseek(pFile, BMP_Header_Length, SEEK_SET);

    // ����ÿ��������ռ�ֽ����������ݴ����ݼ����������ֽ���
    {
        GLint line_bytes = width * 3;
        while (line_bytes % 4 != 0)
            ++line_bytes;
        total_bytes = line_bytes * height;
    }

    // �����������ֽ��������ڴ�
    pixels = (GLubyte*)malloc(total_bytes);
    if (pixels == 0)
    {
        fclose(pFile);
        return 0;
    }

    // ��ȡ��������
    if (fread(pixels, total_bytes, 1, pFile) <= 0)
    {
        free(pixels);
        fclose(pFile);
        return 0;
    }

    // �ھɰ汾��OpenGL��
    // ���ͼ��Ŀ�Ⱥ͸߶Ȳ��ǵ������η�������Ҫ��������
    // ���ﲢû�м��OpenGL�汾�����ڶ԰汾�����ԵĿ��ǣ����ɰ汾����
    // ���⣬�����Ǿɰ汾�����°汾��
    // ��ͼ��Ŀ�Ⱥ͸߶ȳ�����ǰOpenGLʵ����֧�ֵ����ֵʱ��ҲҪ��������
    {
        GLint max;
        glGetIntegerv(GL_MAX_TEXTURE_SIZE, &max);
        if (!power_of_two(width)
            || !power_of_two(height)
            || width > max
            || height > max)
        {
            const GLint new_width = 256;
            const GLint new_height = 256; // �涨���ź��µĴ�СΪ�߳���������
            GLint new_line_bytes, new_total_bytes;
            GLubyte* new_pixels = 0;

            // ����ÿ����Ҫ���ֽ��������ֽ���
            new_line_bytes = new_width * 3;
            while (new_line_bytes % 4 != 0)
                ++new_line_bytes;
            new_total_bytes = new_line_bytes * new_height;

            // �����ڴ�
            new_pixels = (GLubyte*)malloc(new_total_bytes);
            if (new_pixels == 0)
            {
                free(pixels);
                fclose(pFile);
                return 0;
            }

            // ������������
            gluScaleImage(GL_RGB,
                width, height, GL_UNSIGNED_BYTE, pixels,
                new_width, new_height, GL_UNSIGNED_BYTE, new_pixels);

            // �ͷ�ԭ�����������ݣ���pixelsָ���µ��������ݣ�����������width��height
            free(pixels);
            pixels = new_pixels;
            width = new_width;
            height = new_height;
        }
    }

    // ����һ���µ�������
    GLuint texture_ID = 0;  // Change from GLint to GLuint
    glGenTextures(1, &texture_ID);
    if (texture_ID == 0)
    {
        free(pixels);
        fclose(pFile);
        return 0;
    }

    // ���µ������������������������
    // �ڰ�ǰ���Ȼ��ԭ���󶨵������ţ��Ա��������лָ�
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

    // ֮ǰΪpixels������ڴ����ʹ��glTexImage2D�Ժ��ͷ�
    // ��Ϊ��ʱ���������Ѿ���OpenGL���б�����һ�ݣ����ܱ����浽ר�ŵ�ͼ��Ӳ���У�
    free(pixels);
    return texture_ID;
}

/* ����ǰ����BGR��ʽת��ΪBGRA��ʽ
 * ���������ص�RGBֵ�����ָ��rgb������absolute����Alpha����Ϊ0.0����������Ϊ1.0
 */
void texture_colorkey(GLubyte r, GLubyte g, GLubyte b, GLubyte absolute)
{
    GLint width, height;
    GLubyte* pixels = 0;

    // �������Ĵ�С��Ϣ
    glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &width);
    glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &height);

    // ����ռ䲢�����������
    pixels = (GLubyte*)malloc(width * height * 4);
    if (pixels == 0)
        return;
    glGetTexImage(GL_TEXTURE_2D, 0, GL_BGRA_EXT, GL_UNSIGNED_BYTE, pixels);

    // �޸������е�Alphaֵ
    // ����pixels[i*4], pixels[i*4+1], pixels[i*4+2], pixels[i*4+3]
    //   �ֱ��ʾ��i�����ص������̡��졢Alpha���ַ�����0��ʾ��С��255��ʾ���
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

    // ���޸ĺ�������������õ������У��ͷ��ڴ�
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0,
        GL_BGRA_EXT, GL_UNSIGNED_BYTE, pixels);
    free(pixels);
}

void display(void)
{
    static int initialized = 0;
    static GLuint texWindow = 0;
    static GLuint texPicture = 0;

    // ִ�г�ʼ����������������ȡ��Ƭ����ȡ��򣬽������BGR��ɫת��ΪBGRA�����ö�ά����
    if (!initialized)
    {
        texPicture = load_texture("pic.bmp");
        texWindow = load_texture("window.bmp");
        glBindTexture(GL_TEXTURE_2D, texWindow);
        texture_colorkey(255, 255, 255, 10);

        glEnable(GL_TEXTURE_2D);

        initialized = 1;
    }

    // �����Ļ
    glClear(GL_COLOR_BUFFER_BIT);

    // ������Ƭ����ʱ����Ҫ����Alpha���ԣ����е����ض����л���
    glBindTexture(GL_TEXTURE_2D, texPicture);
    glDisable(GL_ALPHA_TEST);
    glBegin(GL_QUADS);
    glTexCoord2f(0, 0);     glVertex2f(-1.0f, -1.0f);
    glTexCoord2f(0, 1);     glVertex2f(-1.0f, 1.0f);
    glTexCoord2f(1, 1);     glVertex2f(1.0f, 1.0f);
    glTexCoord2f(1, 0);     glVertex2f(1.0f, -1.0f);
    glEnd();

    // ������򣬴�ʱ����Alpha���ԣ�ֻ���Ʋ�͸�����ֵ�����
    glBindTexture(GL_TEXTURE_2D, texWindow);
    glEnable(GL_ALPHA_TEST);
    glAlphaFunc(GL_GREATER, 0.5f);
    glBegin(GL_QUADS);
    glTexCoord2f(0, 0);     glVertex2f(-1.0f, -1.0f);
    glTexCoord2f(0, 1);     glVertex2f(-1.0f, 1.0f);
    glTexCoord2f(1, 1);     glVertex2f(1.0f, 1.0f);
    glTexCoord2f(1, 0);     glVertex2f(1.0f, -1.0f);
    glEnd();

    // ��������
    glutSwapBuffers();
}

int main(int argc, char* argv[])
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(400, 400);
    glutCreateWindow("ͼƬ���");
    glutDisplayFunc(&display);
    glutMainLoop();
    return 0;
}