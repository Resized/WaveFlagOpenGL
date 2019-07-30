#include "glut.h"
#include <stdlib.h>
#include <time.h>
#include <math.h>

const int W = 600; // window width
const int H = 600; // window height

const double PI = 3.14159265358979323846;

typedef struct {
	double x, y;
} POINT_2D;

typedef struct {
	unsigned char r, g, b; // 0..255
} MY_COLOR;

unsigned char pix[H][W][3]; // red,green and blue layers
double offset = 0;
POINT_2D first_point;

MY_COLOR flagBackgroundColor = { 200,200,200 };
MY_COLOR flagStarColor = { 0,0,200 };
const int BOTTOM_STRIPE = H / 6;
const int TOP_STRIPE = H / 3.5;

const double WAVE_SPEED = 0.1;
double waveIntensity = 0.4;
int starThickness = 5;


void init()
{
	int i, j;
	glClearColor(0.5, 0.5, 0.5, 0);// color of window background

	srand(time(NULL));

	for (i = 0; i < H; i++)
		for (j = 0; j < W; j++)
		{
			pix[i][j][0] = 255; // R
			pix[i][j][1] = 0; // G
			pix[i][j][2] = 0; // B
		}
}

void drawLine(POINT_2D p1, POINT_2D p2, MY_COLOR color) {
	int i, j, start, stop;
	double a, b;

	if (p1.x != p2.x)
	{
		a = ((double)p2.y - p1.y) / (p2.x - p1.x);
		if (fabs(a) < 1)
		{
			b = p1.y - a * p1.x;

			start = p1.x <= p2.x ? p1.x : p2.x;
			stop = p1.x > p2.x ? p1.x : p2.x;

			for (j = start; j <= stop; j++)
			{
				i = a * j + b;// y = ax+b
				pix[i][j][0] = color.r;
				pix[i][j][1] = color.g;
				pix[i][j][2] = color.b;
			}
		}
		else // fabs(slope) is > 1
		{
			a = 1 / a;
			b = p1.x - a * p1.y;

			start = p1.y <= p2.y ? p1.y : p2.y;
			stop = p1.y > p2.y ? p1.y : p2.y;

			for (i = start; i <= stop; i++)
			{
				j = a * i + b;
				pix[i][j][0] = color.r;
				pix[i][j][1] = color.g;
				pix[i][j][2] = color.b;
			}
		}
	}
	else
	{
		start = p1.y <= p2.y ? p1.y : p2.y;
		stop = p1.y > p2.y ? p1.y : p2.y;
		for (i = start; i < stop; i++)
		{
			pix[(int)p1.x][i][0] = color.r;
			pix[(int)p1.x][i][1] = color.g;
			pix[(int)p1.x][i][2] = color.b;
		}
	}

}

void drawFlag()
{
	int i, j;
	int radius = 80;
	POINT_2D pts[6];
	POINT_2D center = { W / 2, H / 2 };

	// background
	for (i = 0; i < H; i++)
		for (j = 0; j < W; j++)
		{
			pix[i][j][0] = flagBackgroundColor.r; // R
			pix[i][j][1] = flagBackgroundColor.g; // G
			pix[i][j][2] = flagBackgroundColor.b; // B
		}
	// the points are placed on circle of RADIUS on 
	// angles: 45,90,135, 225,270,315
	pts[0].x = center.x + radius * cos(PI / 6);
	pts[0].y = center.y - radius * sin(PI / 6);
	pts[1].x = center.x + radius * cos(PI / 2);
	pts[1].y = center.y - radius * sin(PI / 2);
	pts[2].x = center.x + radius * cos(5 * PI / 6);
	pts[2].y = center.y - radius * sin(5 * PI / 6);
	pts[3].x = center.x + radius * cos(7 * PI / 6);
	pts[3].y = center.y - radius * sin(7 * PI / 6);
	pts[4].x = center.x + radius * cos(6 * PI / 4);
	pts[4].y = center.y - radius * sin(6 * PI / 4);
	pts[5].x = center.x + radius * cos(11 * PI / 6);
	pts[5].y = center.y - radius * sin(11 * PI / 6);

	drawLine(pts[0], pts[2], flagStarColor);
	drawLine(pts[2], pts[4], flagStarColor);
	drawLine(pts[4], pts[0], flagStarColor);
	drawLine(pts[1], pts[3], flagStarColor);
	drawLine(pts[3], pts[5], flagStarColor);
	drawLine(pts[5], pts[1], flagStarColor);

	for (int i = 0; i < starThickness; i++)
	{
		for (int j = 0; j < 6; j++)
		{
			pts[j].y++;
		}
		drawLine(pts[0], pts[2], flagStarColor);
		drawLine(pts[2], pts[4], flagStarColor);
		drawLine(pts[4], pts[0], flagStarColor);
		drawLine(pts[1], pts[3], flagStarColor);
		drawLine(pts[3], pts[5], flagStarColor);
		drawLine(pts[5], pts[1], flagStarColor);
	}

	for (i = BOTTOM_STRIPE; i < TOP_STRIPE; i++)
		for (j = 0; j < W; j++)
		{
			pix[i][j][0] = flagStarColor.r; // R
			pix[i][j][1] = flagStarColor.g; // G
			pix[i][j][2] = flagStarColor.b; // B
		}

	for (i = H - BOTTOM_STRIPE; i > H - TOP_STRIPE; i--)
		for (j = 0; j < W; j++)
		{
			pix[i][j][0] = flagStarColor.r; // R
			pix[i][j][1] = flagStarColor.g; // G
			pix[i][j][2] = flagStarColor.b; // B
		}
}

void drawWave()
{
	double dist;
	int i, j;
	for (i = 0; i < H; i++)
		for (j = 0; j < W; j++)
		{
			dist = sqrt((double)(H / 2 - i)*(H / 2 - i) + (-W / 2 - j)*(-W / 2 - j));
			pix[i][j][0] = fmax(0, fmin(255, pix[i][j][0] + (127.5 * (sin(dist / 50.0 - offset)))*waveIntensity)); // R
			pix[i][j][1] = fmax(0, fmin(255, pix[i][j][1] + (127.5 * (sin(dist / 50.0 - offset)))*waveIntensity)); // G
			pix[i][j][2] = fmax(0, fmin(255, pix[i][j][2] + (127.5 * (sin(dist / 50.0 - offset)))*waveIntensity)); // B
		}
}

void display()
{
	glClear(GL_COLOR_BUFFER_BIT); // clean frame buffer

	drawFlag();
	drawWave();
	glDrawPixels(W, H, GL_RGB, GL_UNSIGNED_BYTE, pix);

	glutSwapBuffers(); // show all
}

void idle()
{
	int i, j;
	double dist, dist1;

	offset += WAVE_SPEED;

	glutPostRedisplay(); // go to display
}

void main(int argc, char* argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
	glutInitWindowSize(W, H);
	glutInitWindowPosition(200, 100);
	glutCreateWindow("City Example");

	glutDisplayFunc(display);
	glutIdleFunc(idle);

	init();

	glutMainLoop();
}