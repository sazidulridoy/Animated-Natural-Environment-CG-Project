#include <GL/glut.h>
#include <math.h>
#include <stdlib.h>

float sunPos = 0.0f;

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
