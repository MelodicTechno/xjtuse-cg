#include <GL/glut.h>

static int day = 200; // day的变化：从0到359

void myDisplay(void)
{
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
}

void updateDay(int value)
{
    day = (day + 1) % 360;  // 每次增加 1
    glutPostRedisplay();    // 强制重新绘制屏幕
    glutTimerFunc(50, updateDay, 0);  // 50 毫秒后再次调用，调节速度
}

int main(int argc, char* argv[])
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(600, 600); // 调整窗口大小以适应视野
    glutCreateWindow("第五个OpenGL程序");
    glutDisplayFunc(&myDisplay);
    glutTimerFunc(50, updateDay, 0); // 初始化定时器回调函数
    glEnable(GL_DEPTH_TEST);        // 启用深度测试
    glutMainLoop();
    return 0;
}
