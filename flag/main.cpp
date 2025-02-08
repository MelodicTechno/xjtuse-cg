#include <GL/glut.h>

float angle = 0.0f; // 用于控制旋转角度

// 初始化OpenGL设置
void initOpenGL() {
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f); // 设置背景颜色为白色
    glEnable(GL_DEPTH_TEST); // 启用深度测试
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0f, 1.0f, 1.0f, 50.0f); // 设置透视投影
    glMatrixMode(GL_MODELVIEW);
}

// 绘制红旗和旗杆
void drawFlag() {
    glPushMatrix();

    // 将红旗移到旗杆顶端
    glTranslatef(0.0f, 5.0f, 0.0f); // 将红旗移到旗杆顶部

    // 旋转红旗
    glRotatef(angle, 0.0f, 1.0f, 0.0f); // 沿y轴旋转

    // 绘制红旗的矩形部分
    glColor3f(1.0f, 0.0f, 0.0f); // 红色
    glBegin(GL_QUADS);
    glVertex3f(0.0f, 1.5f, 0.0f);  // 左上角
    glVertex3f(6.0f, 1.0f, 0.0f);   // 右上角
    glVertex3f(6.0f, -1.0f, 0.0f);  // 右下角
    glVertex3f(0.0f, -1.5f, 0.0f); // 左下角
    glEnd();

    // 这是旗杆
    glColor3f(0.0f, 0.0f, 0.0f); // 黑色
    glBegin(GL_QUADS);
    glVertex3f(0.0f, 1.5f, 0.0f);  // 左上角
    glVertex3f(-0.1f, 1.5f, 0.0f);   // 右上角
    glVertex3f(0.0f, -15.0f, 0.0f);  // 右下角
    glVertex3f(-0.1f, -15.0f, 0.0f); // 左下角
    glEnd();

    glPopMatrix();
}

// 渲染函数
void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // 清除颜色和深度缓冲区
    glLoadIdentity(); // 重置模型视图矩阵
    glTranslatef(0.0f, 0.0f, -20.0f); // 移动视图到适当的位置

    drawFlag(); // 绘制红旗

    glutSwapBuffers(); // 交换缓冲区
}

// 更新动画
void update(int value) {
    angle += 2.0f; // 每次更新旋转角度
    if (angle > 360.0f) angle -= 360.0f; // 确保角度不会超出360度

    glutPostRedisplay(); // 请求重新绘制
    glutTimerFunc(16, update, 0); // 每16毫秒更新一次
}

// 主程序
int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH); // 使用双缓冲和深度缓冲
    glutInitWindowSize(800, 600); // 设置窗口大小
    glutCreateWindow("旋转的红旗与旗杆"); // 创建窗口

    initOpenGL(); // 初始化OpenGL设置

    glutDisplayFunc(display); // 设置显示回调函数
    glutTimerFunc(25, update, 0); // 设置定时器回调函数更新动画

    glutMainLoop(); // 进入主循环
    return 0;
}
