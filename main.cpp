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

void drawEllipse(float cx, float cy, float rx, float ry)
{
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(cx, cy);
    for (int i = 0; i <= 100; i++)
    {
        float angle = 2 * 3.14159f * i / 100;
        glVertex2f(cx + cos(angle) * rx, cy + sin(angle) * ry);
    }
    glEnd();
}

void drawSunMoon()
{
    if (!isNight)
    {
        float sx = -85.0f + sunAngle;
        float sy = 85.0f;

        // Sun glow (soft halo)
        glColor4f(1.0f, 0.95f, 0.50f, 0.15f);
        drawCircle(sx, sy, 16);
        glColor4f(1.0f, 0.92f, 0.30f, 0.25f);
        drawCircle(sx, sy, 12);

        // Sun body
        glColor3f(1.0f, 0.95f, 0.10f);
        drawCircle(sx, sy, 8);

        // Sun rays
        glColor3f(1.0f, 0.90f, 0.20f);
        glLineWidth(1.5f);
        glBegin(GL_LINES);
        for (int i = 0; i < 12; i++)
        {
            float a = i * 3.14159f / 6.0f;
            glVertex2f(sx + cos(a) * 10, sy + sin(a) * 10);
            glVertex2f(sx + cos(a) * 15, sy + sin(a) * 15);
        }
        glEnd();
        glLineWidth(1.0f);
    }
    else
    {
        // Moon
        glColor3f(0.95f, 0.95f, 0.85f);
        drawCircle(75, 82, 7);
        // Crescent shadow
        glColor3f(0.05f, 0.05f, 0.18f);
        drawCircle(78, 82, 6);
    }
}

//sky
void drawSky()
{
    if (isNight)
    {
        // Gradient sky
        glBegin(GL_QUADS);
        glColor3f(0.15f, 0.45f, 0.90f);
        glVertex2f(-100, 100);
        glVertex2f(100, 100);
        glColor3f(0.55f, 0.80f, 1.0f);
        glVertex2f(100, 40);
        glVertex2f(-100, 40);
        glEnd();

        glBegin(GL_QUADS);
        glColor3f(0.55f, 0.80f, 1.0f);
        glVertex2f(-100, 40);
        glVertex2f(100, 40);
        glColor3f(0.80f, 0.92f, 1.0f);
        glVertex2f(100, 0);
        glVertex2f(-100, 0);
        glEnd();
    }
      else
    {
        // Night sky
        glBegin(GL_QUADS);
        glColor3f(0.02f, 0.02f, 0.12f);
        glVertex2f(-100, 100);
        glVertex2f(100, 100);
        glColor3f(0.05f, 0.05f, 0.20f);
        glVertex2f(100, 0);
        glVertex2f(-100, 0);
        glEnd();
    }

}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT);
    glPushMatrix();
    drawSky();
    drawSunMoon();

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
