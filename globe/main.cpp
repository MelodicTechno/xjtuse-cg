#include <Windows.h>
#include <GL/glut.h>
#include <cmath>
#include <cstdio>
#include <cstdlib>

static float _angle = 30.0f;
static float _cameraAngle = 0.0f;

static int oldX;
static int oldY;
static float _rotX = 0;
static float _rotY = 0;
static float xyz[3] = { 1, 1, 1 };
static float r = 200;

static GLUquadric* _earth;

GLuint texEarth;  // 纹理对象编号

#define BMP_Header_Length 54  // BMP 头部长度

int clickCount = 0;
unsigned int lastClickTime = 0;
const unsigned int doubleClickThreshold = 500;
float zoom = 1.0f;

// 窗口
int windowWidth = 800;
int windowHeight = 480;

// 光照
GLfloat light_position[] = { 0.0, 0.0, 0.0, 0.0 };
GLfloat light_ambient[] = { 0.2, 0.2, 0.2, 1.0 };
GLfloat light_diffuse[] = { 0.8, 0.8, 0.8, 1.0 };
GLfloat light_specular[] = { 1.0, 1.0, 1.0, 1.0 };
GLfloat high_shininess[] = { 100.0 };


// 处理按键操作
void handleKeypress(unsigned char key, int x, int y) {
	switch (key) {
	case 27:  // Esc键
		exit(0);
		break;
	case 'w':
		r += 10;
		glutPostRedisplay();
		break;
	case 's':
		r -= 10;
		glutPostRedisplay();
		break;
	}
}

// 初始化光照
void initLight() {
	GLfloat ambientLight[] = { 0.3, 0.3, 0.3, 1 }; // 环境光
	GLfloat diffuseLight[] = { 0.8, 0.8, 0.8, 1 }; // 漫射光
	GLfloat specularLight[] = { -0.5, -0.5, 0.5, 1 }; // 镜面光
	GLfloat posLight[] = { 400, 240, 1, 1 }; // 光源位置
	GLfloat specref[] = { 1, 1, 1, 1 }; // 镜面反射
	GLfloat light_position[] = { -1.0, 1.0, 1.0, 0.0 }; // 光源位置

	GLfloat mat_ambient[] = { 0.2, 0.2, 0.2, 1.0 };
	GLfloat mat_diffuse[] = { 0.8, 0.8, 0.8, 1.0 };
	//GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat high_shininess[] = { 100.0 };
	//GLfloat global_ambient[] = { 0.2, 0.2, 0.2, 1.0 };

	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientLight); // 光照模型
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);
	glLightfv(GL_LIGHT0, GL_SPECULAR, specularLight);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);

	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 128);

	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient); // 让光照也能应用在纹理上
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
	//glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, high_shininess);
}

// 判断一个数是否为2的整数次幂
int power_of_two(int n) {
	if (n <= 0)
		return 0;
	return (n & (n - 1)) == 0;
}

// 加载纹理
GLuint load_texture(const char* file_name)
{
	GLint width, height, total_bytes;
	GLubyte* pixels = 0;
	GLuint last_texture_ID = 0, texture_ID = 0;

	// 打开文件，如果失败，返回
	FILE* pFile;
	int err = fopen_s(&pFile, file_name, "rb");
	if (err != 0) { return 0; }

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
	glGenTextures(1, &texture_ID);
	if (texture_ID == 0)
	{
		free(pixels);
		fclose(pFile);
		return 0;
	}

	// 绑定新的纹理，载入纹理并设置纹理参数
	// 在绑定前，先获得原来绑定的纹理编号，以便在最后进行恢复
	GLint lastTextureID = last_texture_ID;
	glGetIntegerv(GL_TEXTURE_BINDING_2D, &lastTextureID);
	glBindTexture(GL_TEXTURE_2D, texture_ID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, pixels);
	glBindTexture(GL_TEXTURE_2D, lastTextureID);  //恢复之前的纹理绑定
	free(pixels);
	return texture_ID;
}


// 初始化渲染
void initRendering() {
	_earth = gluNewQuadric();
	initLight();

	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHTING);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LINE_SMOOTH);
}

// 释放渲染资源
void deinitRendering() {
	gluDeleteQuadric(_earth);
}

// 处理窗口调整
void handleResize(int w, int h) {
	if (h == 0) {
		h = 1;
	}
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, w, 0, h, -200, 200);
	glMatrixMode(GL_MODELVIEW);
}


//画地球
void drawEarth()
{
	static bool loaded = false;
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	//纹理绑定到目标
	glBindTexture(GL_TEXTURE_2D, texEarth);
	if (!loaded)
	{
		//纹理坐标自动生成	
		glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
		glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
		//表面生成纹理坐标
		gluQuadricDrawStyle(_earth, GL_FILL);
		gluQuadricNormals(_earth, GLU_SMOOTH);
		gluQuadricTexture(_earth, GL_TRUE);

	}
	//生成球体
	glPushMatrix();
	{
		glEnable(GL_TEXTURE_2D);
		glRotatef(-90, 1, 0, 0);
		gluSphere(_earth, r, 100, 100);
		glDisable(GL_TEXTURE_2D);

	}
	glPopMatrix();

}

// 绘制支架的函数
void drawStand() {
	GLUquadric* quadratic;
	quadratic = gluNewQuadric();
	gluQuadricNormals(quadratic, GLU_SMOOTH);
	gluQuadricTexture(quadratic, GL_TRUE);

	// 设置支架的颜色
	glColor3f(0.5, 0.5, 0.5); // 灰色

	// 绘制支架的底部
	glPushMatrix();
	glTranslatef(0.0, -1.5, 0.0); // 移动到地球仪下方
	gluDisk(quadratic, 0.0, 0.5, 20, 1); // 绘制圆柱体的底部
	glPopMatrix();

	// 绘制支架的支柱
	glPushMatrix();
	glTranslatef(0.0, -1.0, 0.0); // 移动到地球仪中心下方
	glRotatef(90.0, 1.0, 0.0, 0.0); // 旋转以使圆柱体垂直
	gluCylinder(quadratic, 0.1, 0.1, 1.0, 20, 1); // 绘制一个细长的圆柱体
	glPopMatrix();

	// 绘制支架的顶部
	glPushMatrix();
	glTranslatef(0.0, -0.5, 0.0); // 移动到地球仪中心附近
	gluDisk(quadratic, 0.0, 0.3, 20, 1); // 绘制圆柱体的顶部
	glPopMatrix();

	gluDeleteQuadric(quadratic);
}


//绘制D场景
void drawScene() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW); //切换到绘图透视图
	glLoadIdentity(); //重置绘图透视图

	glPushMatrix();
	//实现缩放
	glScalef(xyz[0], xyz[1], xyz[2]);
	//此处需要把地球往后方移动防止放大的时候中间出现黑色圆圈
	glScalef(zoom, zoom, 1.0f);
	glTranslatef(400, 240, -200);
	//实现拖动旋转
	glRotatef(_rotX / 100, 0, 1, 0);
	glRotatef(-_rotY / 100, 1, 0, 0);
	//实现自动旋转
	glRotatef(_angle, 0, 1, 0);
	drawEarth();
	glPopMatrix();

	glutSwapBuffers();
}

// 更新函数，实现自动旋转效果
void update(int value) {
	_angle += 2.0f;
	if (_angle > 360) {
		_angle -= 360;
	}

	// 重绘
	glutPostRedisplay();

	// 60FPS
	glutTimerFunc(16, update, 0);
}

// 处理鼠标点击滑动事件
void handleMotion(int x, int y) {
	int rx = x - oldX;
	int ry = 480 - y - oldY;
	_rotX += rx;
	_rotY += ry;
	// 重绘
	glutPostRedisplay();
}

// 处理鼠标点击事件
void handleMouse(int button, int state, int x, int y) {
	
	if (button == GLUT_LEFT_BUTTON) {
		oldX = x;
		oldY = 480 - y;
		if (state == GLUT_DOWN) {
			unsigned int currentTime = glutGet(GLUT_ELAPSED_TIME);
			clickCount++;
			if (currentTime - lastClickTime < doubleClickThreshold) {
				clickCount = 0;
				r += 10;
				glutPostRedisplay();
			}
			lastClickTime = currentTime;
		}
	}
	else if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) {
		r -= 10;
		glutPostRedisplay();
	}
}

int main(int argc, char** argv) {
	// 初始化
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(windowWidth, windowHeight);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	// 创建窗口
	glutCreateWindow("地球仪");
	
	initRendering();

	// 设置用户操作
	texEarth = load_texture("earth.bmp");
	glutDisplayFunc(drawScene);
	glutKeyboardFunc(handleKeypress);
	glutReshapeFunc(handleResize);
	glutMotionFunc(handleMotion);
	glutMouseFunc(handleMouse);

	// 运动效果
	glutTimerFunc(16, update, 0);

	glutMainLoop();
	// 退出时释放渲染资源
	deinitRendering();
	return 0;
}
