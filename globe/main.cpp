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

GLuint texEarth;  // ���������

#define BMP_Header_Length 54  // BMP ͷ������

int clickCount = 0;
unsigned int lastClickTime = 0;
const unsigned int doubleClickThreshold = 500;
float zoom = 1.0f;

// ����
int windowWidth = 800;
int windowHeight = 480;

// ����
GLfloat light_position[] = { 0.0, 0.0, 0.0, 0.0 };
GLfloat light_ambient[] = { 0.2, 0.2, 0.2, 1.0 };
GLfloat light_diffuse[] = { 0.8, 0.8, 0.8, 1.0 };
GLfloat light_specular[] = { 1.0, 1.0, 1.0, 1.0 };
GLfloat high_shininess[] = { 100.0 };


// ����������
void handleKeypress(unsigned char key, int x, int y) {
	switch (key) {
	case 27:  // Esc��
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

// ��ʼ������
void initLight() {
	GLfloat ambientLight[] = { 0.3, 0.3, 0.3, 1 }; // ������
	GLfloat diffuseLight[] = { 0.8, 0.8, 0.8, 1 }; // �����
	GLfloat specularLight[] = { -0.5, -0.5, 0.5, 1 }; // �����
	GLfloat posLight[] = { 400, 240, 1, 1 }; // ��Դλ��
	GLfloat specref[] = { 1, 1, 1, 1 }; // ���淴��
	GLfloat light_position[] = { -1.0, 1.0, 1.0, 0.0 }; // ��Դλ��

	GLfloat mat_ambient[] = { 0.2, 0.2, 0.2, 1.0 };
	GLfloat mat_diffuse[] = { 0.8, 0.8, 0.8, 1.0 };
	//GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat high_shininess[] = { 100.0 };
	//GLfloat global_ambient[] = { 0.2, 0.2, 0.2, 1.0 };

	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientLight); // ����ģ��
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);
	glLightfv(GL_LIGHT0, GL_SPECULAR, specularLight);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);

	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 128);

	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient); // �ù���Ҳ��Ӧ����������
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
	//glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, high_shininess);
}

// �ж�һ�����Ƿ�Ϊ2����������
int power_of_two(int n) {
	if (n <= 0)
		return 0;
	return (n & (n - 1)) == 0;
}

// ��������
GLuint load_texture(const char* file_name)
{
	GLint width, height, total_bytes;
	GLubyte* pixels = 0;
	GLuint last_texture_ID = 0, texture_ID = 0;

	// ���ļ������ʧ�ܣ�����
	FILE* pFile;
	int err = fopen_s(&pFile, file_name, "rb");
	if (err != 0) { return 0; }

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
	glGenTextures(1, &texture_ID);
	if (texture_ID == 0)
	{
		free(pixels);
		fclose(pFile);
		return 0;
	}

	// ���µ������������������������
	// �ڰ�ǰ���Ȼ��ԭ���󶨵������ţ��Ա��������лָ�
	GLint lastTextureID = last_texture_ID;
	glGetIntegerv(GL_TEXTURE_BINDING_2D, &lastTextureID);
	glBindTexture(GL_TEXTURE_2D, texture_ID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, pixels);
	glBindTexture(GL_TEXTURE_2D, lastTextureID);  //�ָ�֮ǰ�������
	free(pixels);
	return texture_ID;
}


// ��ʼ����Ⱦ
void initRendering() {
	_earth = gluNewQuadric();
	initLight();

	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHTING);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LINE_SMOOTH);
}

// �ͷ���Ⱦ��Դ
void deinitRendering() {
	gluDeleteQuadric(_earth);
}

// �����ڵ���
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


//������
void drawEarth()
{
	static bool loaded = false;
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	//����󶨵�Ŀ��
	glBindTexture(GL_TEXTURE_2D, texEarth);
	if (!loaded)
	{
		//���������Զ�����	
		glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
		glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
		//����������������
		gluQuadricDrawStyle(_earth, GL_FILL);
		gluQuadricNormals(_earth, GLU_SMOOTH);
		gluQuadricTexture(_earth, GL_TRUE);

	}
	//��������
	glPushMatrix();
	{
		glEnable(GL_TEXTURE_2D);
		glRotatef(-90, 1, 0, 0);
		gluSphere(_earth, r, 100, 100);
		glDisable(GL_TEXTURE_2D);

	}
	glPopMatrix();

}

// ����֧�ܵĺ���
void drawStand() {
	GLUquadric* quadratic;
	quadratic = gluNewQuadric();
	gluQuadricNormals(quadratic, GLU_SMOOTH);
	gluQuadricTexture(quadratic, GL_TRUE);

	// ����֧�ܵ���ɫ
	glColor3f(0.5, 0.5, 0.5); // ��ɫ

	// ����֧�ܵĵײ�
	glPushMatrix();
	glTranslatef(0.0, -1.5, 0.0); // �ƶ����������·�
	gluDisk(quadratic, 0.0, 0.5, 20, 1); // ����Բ����ĵײ�
	glPopMatrix();

	// ����֧�ܵ�֧��
	glPushMatrix();
	glTranslatef(0.0, -1.0, 0.0); // �ƶ��������������·�
	glRotatef(90.0, 1.0, 0.0, 0.0); // ��ת��ʹԲ���崹ֱ
	gluCylinder(quadratic, 0.1, 0.1, 1.0, 20, 1); // ����һ��ϸ����Բ����
	glPopMatrix();

	// ����֧�ܵĶ���
	glPushMatrix();
	glTranslatef(0.0, -0.5, 0.0); // �ƶ������������ĸ���
	gluDisk(quadratic, 0.0, 0.3, 20, 1); // ����Բ����Ķ���
	glPopMatrix();

	gluDeleteQuadric(quadratic);
}


//����D����
void drawScene() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW); //�л�����ͼ͸��ͼ
	glLoadIdentity(); //���û�ͼ͸��ͼ

	glPushMatrix();
	//ʵ������
	glScalef(xyz[0], xyz[1], xyz[2]);
	//�˴���Ҫ�ѵ��������ƶ���ֹ�Ŵ��ʱ���м���ֺ�ɫԲȦ
	glScalef(zoom, zoom, 1.0f);
	glTranslatef(400, 240, -200);
	//ʵ���϶���ת
	glRotatef(_rotX / 100, 0, 1, 0);
	glRotatef(-_rotY / 100, 1, 0, 0);
	//ʵ���Զ���ת
	glRotatef(_angle, 0, 1, 0);
	drawEarth();
	glPopMatrix();

	glutSwapBuffers();
}

// ���º�����ʵ���Զ���תЧ��
void update(int value) {
	_angle += 2.0f;
	if (_angle > 360) {
		_angle -= 360;
	}

	// �ػ�
	glutPostRedisplay();

	// 60FPS
	glutTimerFunc(16, update, 0);
}

// ��������������¼�
void handleMotion(int x, int y) {
	int rx = x - oldX;
	int ry = 480 - y - oldY;
	_rotX += rx;
	_rotY += ry;
	// �ػ�
	glutPostRedisplay();
}

// ����������¼�
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
	// ��ʼ��
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(windowWidth, windowHeight);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	// ��������
	glutCreateWindow("������");
	
	initRendering();

	// �����û�����
	texEarth = load_texture("earth.bmp");
	glutDisplayFunc(drawScene);
	glutKeyboardFunc(handleKeypress);
	glutReshapeFunc(handleResize);
	glutMotionFunc(handleMotion);
	glutMouseFunc(handleMouse);

	// �˶�Ч��
	glutTimerFunc(16, update, 0);

	glutMainLoop();
	// �˳�ʱ�ͷ���Ⱦ��Դ
	deinitRendering();
	return 0;
}
