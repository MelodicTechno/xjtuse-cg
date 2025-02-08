#include <Windows.h>
#include <GL/glut.h>
#include <cmath>
#include <cstdio>
#include <cstdlib>

static int day = 200; // day的变化：从0到359
void myDisplay(void)
{
    glClearColor(1.0f, 1.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glTranslatef(0.00001, 0.0f, 0.0f); // 每次调用myDisplay()时，都让图形沿x轴正方向平移0.00001个单位
    glColor3f(1.0f, 0.0f, 0.0f);
    glRectf(-0.3f, 0.2f, -0.1f, 0.4f);
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

int main(int argc, char* argv[])
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE); // 修改了参数为GLUT_DOUBLE
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(400, 400);
    glutCreateWindow("使用 glTranslatef()函数，实现2D图形平移"); // 改了窗口标题
    glutDisplayFunc(&myDisplay);
    glutIdleFunc(&myIdle); // 新加入了这句
    glutMainLoop();
    return 0;
}