#include <Windows.h>
#include <GL/glut.h>
#include <cmath>
#include <cstdio>
#include <cstdlib>

static int day = 200; // day的变化：从0到359
float M_PI = 3.1415926535;

void myDisplay(void)
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glRotatef(0.005, 0, 0, -1);
    glColor3f(1.0f, 0.0f, 0.0f);
    glBegin(GL_POLYGON);
    glVertex2f(-0.4, -0.7);
    glVertex2f(-0.8, 0.0);
    glVertex2f(-0.4, 0.7);
    glVertex2f(0.4, 0.7);
    glVertex2f(0.8, 0.0);
    glVertex2f(0.4, -0.7);
    glEnd();
    glFlush();
    glutSwapBuffers();
}

void myIdle(void)
{
    /* 新的函数，在空闲时调用，作用是把day加1并重新绘制，达到动画效果 */
    ++day;
    if (day >= 360)
        day = 0;
    myDisplay();
}

static float scale = 1.0f;       // 缩放比例
static float scaleDirection = 0.0001f; // 缩放变化量

// 缩放
void myDisplay2(void)
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // 使用 glPushMatrix 和 glPopMatrix 保护坐标系
    glPushMatrix();

    // 缩放变换
    glScalef(scale, scale, 1.0f);

    // 绘制六边形
    glColor3f(1.0f, 0.0f, 0.0f);
    glBegin(GL_POLYGON);
    glVertex2f(-0.4, -0.7);
    glVertex2f(-0.8, 0.0);
    glVertex2f(-0.4, 0.7);
    glVertex2f(0.4, 0.7);
    glVertex2f(0.8, 0.0);
    glVertex2f(0.4, -0.7);
    glEnd();

    glPopMatrix(); // 恢复坐标系

    glFlush();
    glutSwapBuffers();
}

void myIdle2(void)
{
    // 动态调整缩放比例
    scale += scaleDirection;

    // 如果缩放比例超出范围，改变缩放方向
    if (scale >= 2.0f || scale <= 0.5f) {
        scaleDirection = -scaleDirection;
    }

    // 触发重绘
    glutPostRedisplay();
}


int main(int argc, char* argv[])
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE); // 修改了参数为GLUT_DOUBLE
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(400, 400);
    glutCreateWindow("第二题");   // 改了窗口标题
    glutDisplayFunc(&myDisplay2);
    glutIdleFunc(&myIdle2);               // 新加入了这句
    glutMainLoop();
    return 0;
}