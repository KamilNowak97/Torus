
#include "pch.h"
#include <iostream>
#include <windows.h>
#include <gl/gl.h>
#include <gl/glut.h>
#include <cmath>
#include <cstdlib>
#include <ctime>

static GLfloat viewer[] = { 0.0, 0.0, 15.0 };

static GLfloat thetaX = 1.0;   // k¹t obrotu obiektu
static GLfloat thetaY = 1.0;   // k¹t obrotu obiektu

static GLfloat pix2angleX;     // przelicznik pikseli na stopnie
static GLfloat pix2angleY;     // przelicznik pikseli na stopnie

static GLint status = 0;       // stan klawiszy myszy // 0 - nie naciœniêto ¿adnego klawisza // 1 - naciœniêty zostaæ lewy klawisz
static GLint model = 1;

static GLfloat x_pos_old = 0.0;       // poprzednia pozycja kursora myszy
static GLfloat y_pos_old = 0.0;
static GLfloat z_pos_old = 0.0;

static GLfloat delta_x = 0.0;
static GLfloat delta_y = 0.0;
static GLfloat delta_z = 0.0;

static GLfloat z_start_pos = 15.0;

float r = 0.05f;
float R = 0.4f;

float X = 0;
float val;
float u, v, delta_u = 0.05f, delta_v = 0.05f;
const int N = 100;

static GLfloat theta[] = { 0.0, 0.0, 0.0 };

void rotate()
{
	for (int i = 0; i < 3; i++)
	{
		theta[i] = theta[i] - 0.0001;
		if (theta[i] > 360.0)
			theta[i] -= 360.0;
	}
	glutPostRedisplay();
}
typedef float point3[3];

struct point
{
	double x;
	double y;
	double z;
	float r;
	float g;
	float b;
};
point pointsTable[N][N]; // Macierz punktów

float randFloat()
{
	return (float)(rand()) / (RAND_MAX);
}


void generateTable()
{
	for (int i = 0; i < N; ++i)
	{
		for (int j = 0; j < N; ++j)
		{
			// Zmienne wykorzystywane w równaniach
			double u = (double)i / ((double)N - 1);
			double v = (double)j / ((double)N - 1);
			// Implementacja równañ podanych w instrukcji
			pointsTable[i][j].x = (R + r * cos(2 * 3.14*v))*cos(2 * 3.14*u);
			pointsTable[i][j].y = (R + r * cos(2 * 3.14*v))*sin(2 * 3.14*u);
			pointsTable[i][j].z = r * sin(2 * 3.14*v);
			// Losowe kolory dla punktu
			pointsTable[i][j].r = randFloat();
			pointsTable[i][j].b = randFloat();
			pointsTable[i][j].g = randFloat();
		}
	}
};

static void torus(int angleX, int angleY, float value, float valuez, float X)
{

	glBegin(GL_QUAD_STRIP);
	for (int i = 0; i < N; ++i)
	{
		for (int j = 0; j < N; ++j)
		{
			glColor3f(pointsTable[i][j].r, pointsTable[i][j].g, pointsTable[i][j].b);
			glVertex3f((float)pointsTable[i][j].x + X, (float)pointsTable[i][j].y + value, (float)pointsTable[i][j].z - valuez);

			glColor3f(pointsTable[(i + 1) % N][(j + 1) % N].r, pointsTable[(i + 1) % N][(j + 1) % N].g, pointsTable[(i + 1) % N][(j + 1) % N].b);
			glVertex3f((float)pointsTable[(i + 1) % N][(j + 1) % N].x + X, (float)pointsTable[(i + 1) % N][(j + 1) % N].y + value, (float)pointsTable[(i + 1) % N][(j + 1) % N].z - valuez);

			glColor3f(pointsTable[(i + 1) % N][j].r, pointsTable[(i + 1) % N][j].g, pointsTable[(i + 1) % N][j].b);
			glVertex3f((float)pointsTable[(i + 1) % N][j].x + X, (float)pointsTable[(i + 1) % N][j].y + value, (float)pointsTable[(i + 1) % N][j].z - valuez);

			glColor3f(pointsTable[i][(j + 1) % N].r, pointsTable[i][(j + 1) % N].g, pointsTable[i][(j + 1) % N].b);
			glVertex3f((float)pointsTable[i][(j + 1) % N].x + X, (float)pointsTable[i][(j + 1) % N].y + value, (float)pointsTable[i][(j + 1) % N].z - valuez);
		}
	}
	glEnd();

}


void Motion(GLsizei x, GLsizei y)
{

	delta_x = x - x_pos_old;     // obliczenie ró¿nicy po³o¿enia kursora myszy
	x_pos_old = x;            // podstawienie bie¿¹cego po³o¿enia jako poprzednie
	delta_y = y - y_pos_old;     // obliczenie ró¿nicy po³o¿enia kursora myszy
	y_pos_old = y;            // podstawienie bie¿¹cego po³o¿enia jako poprzednie
	delta_z = y - z_pos_old;     // obliczenie ró¿nicy po³o¿enia kursora myszy
	z_pos_old = y;            // podstawienie bie¿¹cego po³o¿enia jako poprzednie
	glutPostRedisplay();     // przerysowanie obrazu sceny
}
void Mouse(int btn, int state, int x, int y)
{
	if (btn == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		x_pos_old = x;        // przypisanie aktualnie odczytanej pozycji kursora
		y_pos_old = y;					 // jako pozycji poprzedniej
		status = 1;          // wciêniêty zosta³ lewy klawisz myszy
	}
	else if (btn == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
	{
		z_pos_old = y;
		status = 2;
	}
}
static void MyInit(void)
{
	glClearColor(0.0, 0.0, 0.0, 0.0);
}

void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(1.0, 1.0, 1.0);
	glLoadIdentity();
	gluLookAt(viewer[0], viewer[1], viewer[2], 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

	if (status == 1)
	{
		thetaX += delta_x * pix2angleX;
		thetaY += delta_y * pix2angleY;
	}

	if (status == 2)
	{
		viewer[2] += delta_z * pix2angleY*0.1;
	}

	glRotatef(thetaX, 0.0, 1.0, 0.0);
	glRotatef(thetaY, 1.0, 0.0, 0.0);

	glRotatef(theta[1], 0.0, 1.0, 0.0);
	torus(0, 0, 0.0f, 0.0f, 0.0f);

	glRotated(90, 0.0, 1.0, 0.0);
	torus(20, 90, 5 * R / 4, 0.0f, -0.25f);

	glRotated(90, 0.0, 1.0, 0.0);
	torus(20, -90, 10 * R / 4, 0.0f, -0.25f);

	glRotated(90, 0.0, 1.0, 0.0);
	torus(0, 90, 15 * R / 4, 0.0f, -0.25f);

	glRotated(90, 0.0, 1.0, 0.0);
	torus(0, 90, 20 * R / 4, 0.0f, -0.25f);

	glFlush();
}


void ChangeSize(GLsizei horizontal, GLsizei vertical)
{
	pix2angleX = 360.0 / (float)horizontal;
	pix2angleY = 360.0 / (float)vertical;
	GLfloat AspectRatio;
	glViewport(0, 0, horizontal, vertical);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	AspectRatio = (GLfloat)horizontal / (GLfloat)vertical;
	gluPerspective(30, AspectRatio, 1.0, 100.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0, 0, 10, 0, 0, 0, 0, 1, 0);
}
void main(void)
{
	glutInitWindowSize(500, 500);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutCreateWindow("Torus");
	generateTable();
	glutDisplayFunc(display);
	glutReshapeFunc(ChangeSize);
	glutMouseFunc(Mouse);
	glutMotionFunc(Motion);
	MyInit();
	glutMainLoop();

}