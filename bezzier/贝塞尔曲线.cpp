#include <GL/glut.h>

// 定义贝塞尔曲线的控制点
//GLfloat controlPoints[4][3] = {
//    { -1.5, -1.5, 0.0 },
//    { -0.5, 0.5, 0.0 },
//    { 0.5, -1.5, 0.0 },
//    { 1.5, 2.5, 0.0 }
//};

GLfloat controlPoints[4][3] = {
    { -1.5, -1.5, 0.0 },
    { -0.5, 0.2, 0.0 },
    { 0.5, -1.2, 0.0 },
    { -1.1, 2.5, 0.0 }
};

// 定义节点向量，对于三次贝塞尔曲线，节点向量应该是{0, 0, 0, 0, 1, 1, 1, 1}
GLfloat knots[8] = { 0.0, 0.0, 0.0, 0.0, 1.0, 1.0, 1.0, 1.0 };

// 初始化OpenGL设置
void init() {
    glClearColor(1.0, 1.0, 1.0, 1.0); // 设置背景颜色为白色
    glColor3f(0.0, 0.0, 0.0); // 设置绘图颜色为黑色
}

// 显示回调函数
void display() {
    glClear(GL_COLOR_BUFFER_BIT); // 清除颜色缓冲区

    // 绘制控制点
    glBegin(GL_POINTS);
    for (int i = 0; i < 4; i++) {
        glVertex3fv(controlPoints[i]);
    }
    glEnd();

    // 绘制贝塞尔曲线
    GLUnurbs* nurbs = gluNewNurbsRenderer();
    gluBeginCurve(nurbs);
    gluNurbsCurve(nurbs, 8, knots, 3, &controlPoints[0][0], 4, GL_MAP1_VERTEX_3);
    gluEndCurve(nurbs);
    gluDeleteNurbsRenderer(nurbs);

    glFlush(); // 强制执行OpenGL命令
}

// 主函数
int main(int argc, char** argv) {
    glutInit(&argc, argv); // 初始化GLUT
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB); // 设置显示模式
    glutInitWindowSize(400, 400); // 设置窗口大小
    glutCreateWindow("Bezier Curve Example"); // 创建窗口
    init(); // 初始化OpenGL设置
    glutDisplayFunc(display); // 设置显示回调函数
    glutMainLoop(); // 进入GLUT事件处理循环
    return 0;
}
