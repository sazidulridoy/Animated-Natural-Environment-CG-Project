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
void drawCloud(float cx, float cy, float scale)
{
    if (isNight) glColor3f(0.55f, 0.55f, 0.65f);
    else glColor3f(0.98f, 0.98f, 0.98f);

    // Fluffy multi-circle cloud
    drawEllipse(cx, cy, 10 * scale, 6 * scale);
    drawEllipse(cx - 7 * scale, cy - 1 * scale, 7 * scale, 4.5f * scale);
    drawEllipse(cx + 7 * scale, cy - 1 * scale, 7 * scale, 4.5f * scale);
    drawEllipse(cx - 4 * scale, cy + 2.5f * scale, 6 * scale, 3.5f * scale);
    drawEllipse(cx + 4 * scale, cy + 2.5f * scale, 6 * scale, 3.5f * scale);

    // Slightly darker base for depth
    if (!isNight)
        glColor3f(0.88f, 0.90f, 0.92f);
    else
        glColor3f(0.45f, 0.46f, 0.52f);
    drawEllipse(cx, cy + 3.5f * scale, 9 * scale, 3 * scale);
}

void drawClouds()
{
    drawCloud(-55 + cloudPos, 78, 1.0f);
    drawCloud(25 + cloudPos, 72, 0.75f);
    drawCloud(-120 + cloudPos, 85, 0.60f);
    drawCloud(80 + cloudPos, 80, 0.85f);
}
void drawMountains()
{
    // Far mountains (lighter, atmospheric haze)
    glColor3f(0.55f, 0.52f, 0.60f);
    glBegin(GL_TRIANGLES);
    glVertex2f(-100, 0); glVertex2f(-45, 55); glVertex2f(10, 0);
    glVertex2f(-10, 0);  glVertex2f(45, 48);  glVertex2f(100, 0);
    glEnd();

    // Near mountains (richer brown)
    glColor3f(0.38f, 0.28f, 0.20f);
    glBegin(GL_TRIANGLES);
    glVertex2f(-100, 0); glVertex2f(-62, 48); glVertex2f(-22, 0);
    glVertex2f(-32, 0);  glVertex2f(8, 58);   glVertex2f(48, 0);
    glVertex2f(18, 0);   glVertex2f(62, 40);  glVertex2f(100, 0);
    glEnd();

    // Rocky detail overlay (slightly lighter mid-tone)
    glColor3f(0.50f, 0.38f, 0.28f);
    glBegin(GL_TRIANGLES);
    glVertex2f(-75, 10); glVertex2f(-62, 48); glVertex2f(-50, 10);
    glVertex2f(-10, 8);  glVertex2f(8, 58);   glVertex2f(25, 8);
    glVertex2f(35, 8);   glVertex2f(62, 40);  glVertex2f(80, 8);
    glEnd();

    // Snow caps
    glColor3f(0.95f, 0.96f, 0.98f);
    glBegin(GL_TRIANGLES);
    // Peak 1
    glVertex2f(-70, 30); glVertex2f(-62, 48); glVertex2f(-54, 30);
    // Peak 2
    glVertex2f(0, 40);  glVertex2f(8, 58);  glVertex2f(16, 40);
    // Peak 3 (far)
    glVertex2f(-50, 38); glVertex2f(-45, 55); glVertex2f(-40, 38);
    glEnd();
}

void drawFog()
{
    // Subtle atmospheric fog at mountain base
    glColor4f(0.80f, 0.85f, 0.92f, 0.18f);
    glBegin(GL_QUADS);
    glVertex2f(-100, 0);
    glVertex2f(100, 0);
    glVertex2f(100, 15);
    glVertex2f(-100, 15);
    glEnd();

    glColor4f(0.80f, 0.85f, 0.92f, 0.08f);
    glBegin(GL_QUADS);
    glVertex2f(-100, 15);
    glVertex2f(100, 15);
    glVertex2f(100, 25);
    glVertex2f(-100, 25);
    glEnd();
}
void drawGround()
{
    // Rich green with slight gradient
    glBegin(GL_QUADS);
    glColor3f(0.18f, 0.65f, 0.18f);
    glVertex2f(-100, 0);
    glVertex2f(100, 0);
    glColor3f(0.22f, 0.72f, 0.22f);
    glVertex2f(100, -20);
    glVertex2f(-100, -20);
    glEnd();

    // Ground texture dots
    glColor3f(0.15f, 0.55f, 0.15f);
    glPointSize(1.5f);
    glBegin(GL_POINTS);
    for (int i = -95; i < 100; i += 4)
        for (int j = -19; j < 0; j += 3)
            if ((i * 7 + j * 13) % 5 == 0)
                glVertex2f(i + 0.5f, j + 0.5f);
    glEnd();
}
void drawRiverBank()
{
    // Sandy base
    glBegin(GL_QUADS);
    glColor3f(0.78f, 0.68f, 0.45f);
    glVertex2f(-100, -100);
    glVertex2f(100, -100);
    glColor3f(0.72f, 0.62f, 0.40f);
    glVertex2f(100, -40);
    glVertex2f(-100, -40);
    glEnd();

    // Wet sand near water edge
    glBegin(GL_QUADS);
    glColor3f(0.60f, 0.52f, 0.36f);
    glVertex2f(-100, -40);
    glVertex2f(100, -40);
    glVertex2f(100, -36);
    glVertex2f(-100, -36);
    glEnd();

    // Sand texture
    glColor3f(0.68f, 0.58f, 0.38f);
    glPointSize(1.5f);
    glBegin(GL_POINTS);
    for (int i = -95; i < 100; i += 3)
        for (int j = -99; j < -40; j += 3)
            if ((i * 11 + j * 7) % 8 == 0)
                glVertex2f(i, j);
    glEnd();
}
void drawRiver()
{
    // Base water
    glBegin(GL_QUADS);
    if (!isNight)
        glColor3f(0.18f, 0.50f, 0.85f);
    else
        glColor3f(0.08f, 0.12f, 0.35f);
    glVertex2f(-100, -40);
    glVertex2f(100, -40);
    glVertex2f(100, -20);
    glVertex2f(-100, -20);
    glEnd();

    // River shimmer / depth gradient
    glBegin(GL_QUADS);
    if (!isNight)
        glColor4f(0.25f, 0.65f, 0.95f, 0.30f);
    else
        glColor4f(0.10f, 0.20f, 0.55f, 0.30f);
    glVertex2f(-100, -40);
    glVertex2f(100, -40);
    glVertex2f(100, -32);
    glVertex2f(-100, -32);
    glEnd();

    // Water ripple lines
    glLineWidth(1.2f);
    for (int i = -100; i < 100; i += 8)
    {
        float offset = waterOffset;
        glColor4f(1.0f, 1.0f, 1.0f, 0.50f);
        glBegin(GL_LINE_STRIP);
        for (int j = 0; j < 6; j++)
        {
            float x = i + j * 1.0f + offset;
            float y = -30.0f + sin((x + offset) * 0.5f) * 0.4f;
            glVertex2f(x, y);
        }
        glEnd();

        // Second wave layer
        glColor4f(1.0f, 1.0f, 1.0f, 0.25f);
        glBegin(GL_LINE_STRIP);
        for (int j = 0; j < 4; j++)
        {
            float x = i + j * 1.0f + offset * 0.6f + 3.0f;
            float y = -26.0f + sin((x + offset * 0.7f) * 0.8f) * 0.3f;
            glVertex2f(x, y);
        }
        glEnd();
    }
    glLineWidth(1.0f);

    // Moon/sun reflection on water
    if (!isNight)
    {
        glColor4f(1.0f, 1.0f, 0.60f, 0.20f);
        drawEllipse(0, -30, 18, 4);
    }
    else
    {
        glColor4f(0.90f, 0.90f, 0.80f, 0.15f);
        drawEllipse(0, -30, 12, 3);
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
