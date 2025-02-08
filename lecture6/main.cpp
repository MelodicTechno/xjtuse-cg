#include <GL/glut.h>
#include <stdio.h>
#include <time.h>

// 太阳、地球和月亮
// 假设每个月都是12天
// 一年12个月，共是360天
static int day = 200; // day的变化：从0到359

double CalFrequency()
{
    static int count;
    static double save;
    static clock_t last, current;
    double timegap;

    ++count;
    if (count <= 50)
        return save;
    count = 0;
    last = current;
    current = clock();
    timegap = (current - last) / (double)CLK_TCK;
    save = 50.0 / timegap;
    return save;
}

void myDisplay(void)
{
    double FPS = CalFrequency();
    printf("FPS = %f\n", FPS);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // 设置投影矩阵
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(75, 1, 1, 1000000);  // 调整视距范围，避免过远

    // 设置视图矩阵
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(0, -500000, 500000, 0, 0, 0, 0, 0, 1); // 调整视角位置

    // 绘制红色的“太阳”
    glColor3f(1.0f, 0.0f, 0.0f);
    glutSolidSphere(10000, 20, 20);  // 调整太阳的半径

    // 绘制蓝色的“地球”
    glPushMatrix();  // 保存当前矩阵
    glColor3f(0.0f, 0.0f, 1.0f);
    glRotatef(day, 0.0f, 0.0f, -1.0f); // 按天数旋转
    glTranslatef(300000, 0.0f, 0.0f);  // 地球的平移（减少距离）
    glutSolidSphere(5000, 20, 20);   // 调整地球的半径
    glPopMatrix();   // 恢复矩阵

    // 绘制黄色的“月亮”
    glPushMatrix();  // 保存当前矩阵
    glColor3f(1.0f, 1.0f, 0.0f);
    glRotatef(day, 0.0f, 0.0f, -1.0f); // 地球围绕太阳旋转
    glTranslatef(300000, 0.0f, 0.0f);  // 地球的平移
    glRotatef(day * 12, 0.0f, 0.0f, -1.0f); // 月亮围绕地球旋转
    glTranslatef(50000, 0.0f, 0.0f);  // 月亮的平移
    glutSolidSphere(2000, 20, 20);    // 调整月亮的半径
    glPopMatrix();   // 恢复矩阵

    glFlush();
    glutSwapBuffers();
}

void myIdle(void)
{
    ++day;
    if (day >= 360)
        day = 0;
    myDisplay();
}

int main(int argc, char* argv[])
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(400, 400);
    glutCreateWindow("太阳，地球和月亮");
    glutDisplayFunc(&myDisplay);
    glutIdleFunc(&myIdle);
    glutMainLoop();
    return 0;
}
