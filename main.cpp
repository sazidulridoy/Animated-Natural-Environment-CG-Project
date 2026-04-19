#include <GL/glut.h>
#include <math.h>
#include <stdlib.h>

// Animation variables
float cloudPos = 0.0f;
float sunAngle = 0.0f;
float birdPos = 0.0f;
float waterOffset = 0.0f;
float fogAlpha = 0.0f;
float timeOfDay = 0.0f;  // 0=day, increases

bool isNight = false;
bool isRaining = false;

// Rain
float rainX[300];
float rainY[300];

// Stars
float starX[150];
float starY[150];
float starBright[150];

// Fireflies
float ffX[20];
float ffY[20];
float ffBright[20];
float ffTimer[20];


void drawCircle(float cx, float cy, float r)
{
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(cx, cy);
    for (int i = 0; i <= 100; i++)
    {
        float angle = 2 * 3.1416 * i / 100;
        glVertex2f(cx + cos(angle)*r, cy + sin(angle)*r);
    }
    glEnd();
}

void drawSun()
{

        glColor3f(1,0.9,0);
        drawCircle(70 + sunPos, 80, 8);

}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT);
    glPushMatrix();
    drawSun();

    glPopMatrix();
    glutSwapBuffers();
}

void init()
{
    glClearColor(1.0, 1.0, 1.0, 1.0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-100,100,-100,100);
}

int main(int argc,char** argv)
{
    glutInit(&argc,argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(900,600);
    glutCreateWindow("Animated Natural Environment");

    init();
    glutDisplayFunc(display);

    glutMainLoop();
    return 0;
}
