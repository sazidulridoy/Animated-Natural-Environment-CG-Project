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
void drawWaterEdge()
{
    glBegin(GL_QUADS);
    glColor3f(0.08f, 0.28f, 0.65f);
    glVertex2f(-100, -40);
    glVertex2f(100, -40);
    glVertex2f(100, -39);
    glVertex2f(-100, -39);
    glEnd();

    glBegin(GL_QUADS);
    glColor4f(0.10f, 0.35f, 0.75f, 0.50f);
    glVertex2f(-100, -20);
    glVertex2f(100, -20);
    glVertex2f(100, -21);
    glVertex2f(-100, -21);
    glEnd();
}

void drawEdgeGrass()
{
    glLineWidth(1.2f);
    for (int i = -100; i < 100; i += 2)
    {
        float h = (float)((i * 17 + 43) % 6) + 3.0f;
        float lean = sin(i * 0.7f) * 1.5f;

        // Darker back blades
        glColor3f(0.10f, 0.55f, 0.10f);
        glBegin(GL_LINES);
        glVertex2f(i, -40);
        glVertex2f(i + lean * 0.5f, -40 + h * 0.7f);
        glEnd();

        // Bright front blades
        glColor3f(0.20f, 0.75f, 0.20f);
        glBegin(GL_LINES);
        glVertex2f(i + 0.5f, -40);
        glVertex2f(i + lean + 0.5f, -40 + h);
        glEnd();
    }
    glLineWidth(1.0f);
}

void drawStones()
{
    // Varied stone colors and sizes
    struct Stone { float x, y, r; float r1, g1, b1; };
    Stone stones[] = {
        {-72, -55, 5.0f,  0.50f, 0.48f, 0.46f},
        {-22, -65, 3.5f,  0.45f, 0.43f, 0.41f},
        {28,  -50, 6.0f,  0.52f, 0.50f, 0.48f},
        {62,  -70, 3.0f,  0.48f, 0.46f, 0.44f},
        {10,  -60, 2.5f,  0.42f, 0.40f, 0.38f},
        {-50, -75, 4.0f,  0.55f, 0.53f, 0.50f},
        {45,  -62, 2.0f,  0.46f, 0.44f, 0.42f},
    };

    for (auto& s : stones)
    {
        // Stone shadow
        glColor3f(s.r1 - 0.12f, s.g1 - 0.12f, s.b1 - 0.12f);
        drawEllipse(s.x + 1, s.y - 0.5f, s.r * 1.05f, s.r * 0.65f);
        // Stone body
        glColor3f(s.r1, s.g1, s.b1);
        drawEllipse(s.x, s.y, s.r, s.r * 0.65f);
        // Stone highlight
        glColor3f(s.r1 + 0.15f, s.g1 + 0.15f, s.b1 + 0.15f);
        drawEllipse(s.x - s.r * 0.25f, s.y + s.r * 0.15f, s.r * 0.35f, s.r * 0.2f);
    }
}

void drawSmallPlants()
{
    glLineWidth(1.5f);
    for (int i = -90; i < 90; i += 15)
    {
        float base = -60.0f + sin(i * 0.4f) * 3;
        glColor3f(0.05f, 0.50f, 0.05f);
        glBegin(GL_LINES);
        glVertex2f(i, base);       glVertex2f(i, base + 12);
        glVertex2f(i, base + 6);   glVertex2f(i - 5, base + 12);
        glVertex2f(i, base + 6);   glVertex2f(i + 5, base + 12);
        glEnd();

        // Second offset plant
        glColor3f(0.10f, 0.45f, 0.10f);
        glBegin(GL_LINES);
        glVertex2f(i + 6, base - 4); glVertex2f(i + 6, base + 8);
        glVertex2f(i + 6, base + 3); glVertex2f(i + 3, base + 8);
        glVertex2f(i + 6, base + 3); glVertex2f(i + 9, base + 8);
        glEnd();
    }
    glLineWidth(1.0f);
}

void drawTree(float x, float y)
{
    // Root flare
    glColor3f(0.38f, 0.22f, 0.08f);
    glBegin(GL_TRIANGLES);
    glVertex2f(x - 2, y);
    glVertex2f(x + 2, y);
    glVertex2f(x - 3, y - 3);
    glEnd();

    // Trunk with texture
    glColor3f(0.45f, 0.28f, 0.10f);
    glBegin(GL_QUADS);
    glVertex2f(x - 1.5f, y);
    glVertex2f(x + 1.5f, y);
    glVertex2f(x + 1.2f, y + 10);
    glVertex2f(x - 1.2f, y + 10);
    glEnd();

    // Bark detail lines
    glColor3f(0.35f, 0.20f, 0.07f);
    glLineWidth(0.8f);
    glBegin(GL_LINES);
    glVertex2f(x - 0.5f, y + 2); glVertex2f(x + 0.5f, y + 4);
    glVertex2f(x - 0.5f, y + 6); glVertex2f(x + 0.5f, y + 8);
    glEnd();
    glLineWidth(1.0f);

    // Foliage layers (dark backing -> mid -> highlight tip)
    // Back / shadow layer
    glColor3f(0.05f, 0.38f, 0.05f);
    drawCircle(x, y + 14, 5.5f);
    drawCircle(x - 3.5f, y + 11, 4.0f);
    drawCircle(x + 3.5f, y + 11, 4.0f);

    // Mid layer
    glColor3f(0.10f, 0.55f, 0.10f);
    drawCircle(x, y + 15, 4.5f);
    drawCircle(x - 3, y + 12, 3.5f);
    drawCircle(x + 3, y + 12, 3.5f);
    drawCircle(x, y + 11, 3.8f);

    // Highlight top
    glColor3f(0.18f, 0.70f, 0.18f);
    drawCircle(x, y + 16.5f, 3.0f);
    drawCircle(x - 1.5f, y + 13.5f, 2.0f);
    drawCircle(x + 1.5f, y + 13.5f, 2.0f);
}

void drawBushes()
{
    struct Bush { float x, y, s; };
    Bush bushes[] = {
        {-88, -20, 1.0f},
        {-68, -20, 0.8f},
        {55,  -20, 0.9f},
        {85,  -20, 0.75f},
        {-35, -20, 0.70f},
    };

    for (auto& b : bushes)
    {
        glColor3f(0.06f, 0.40f, 0.06f);
        drawCircle(b.x, b.y, 5 * b.s);
        drawCircle(b.x - 4 * b.s, b.y - 1 * b.s, 3.5f * b.s);
        drawCircle(b.x + 4 * b.s, b.y - 1 * b.s, 3.5f * b.s);

        glColor3f(0.12f, 0.58f, 0.12f);
        drawCircle(b.x, b.y + 1 * b.s, 4 * b.s);
        drawCircle(b.x - 3 * b.s, b.y, 3 * b.s);
        drawCircle(b.x + 3 * b.s, b.y, 3 * b.s);
    }
}

void drawBirds()
{
    if (isNight) return;

    glColor3f(0.05f, 0.05f, 0.10f);
    glLineWidth(1.8f);

    // Bird 1 - animated wing flap with sine
    float flap1 = sin(birdPos * 0.15f) * 3.0f;
    float bx = -65 + birdPos;
    float by = 62;
    glBegin(GL_LINE_STRIP);
    glVertex2f(bx - 7, by + flap1 * 0.5f);
    glVertex2f(bx - 3.5f, by + flap1);
    glVertex2f(bx, by);
    glVertex2f(bx + 3.5f, by + flap1);
    glVertex2f(bx + 7, by + flap1 * 0.5f);
    glEnd();

    // Bird 2 - slightly offset phase
    float flap2 = sin(birdPos * 0.15f + 1.2f) * 3.0f;
    float bx2 = -75 + birdPos;
    float by2 = 57;
    glBegin(GL_LINE_STRIP);
    glVertex2f(bx2 - 5, by2 + flap2 * 0.5f);
    glVertex2f(bx2 - 2.5f, by2 + flap2);
    glVertex2f(bx2, by2);
    glVertex2f(bx2 + 2.5f, by2 + flap2);
    glVertex2f(bx2 + 5, by2 + flap2 * 0.5f);
    glEnd();

    // Bird 3
    float flap3 = sin(birdPos * 0.15f + 2.5f) * 2.5f;
    float bx3 = -55 + birdPos;
    float by3 = 65;
    glBegin(GL_LINE_STRIP);
    glVertex2f(bx3 - 4, by3 + flap3 * 0.5f);
    glVertex2f(bx3 - 2, by3 + flap3);
    glVertex2f(bx3, by3);
    glVertex2f(bx3 + 2, by3 + flap3);
    glVertex2f(bx3 + 4, by3 + flap3 * 0.5f);
    glEnd();

    glLineWidth(1.0f);
}

void drawFireflies()
{
    if (!isNight) return;

    glPointSize(3.5f);
    glBegin(GL_POINTS);
    for (int i = 0; i < 20; i++)
    {
        float b = ffBright[i];
        glColor4f(0.70f, 1.0f, 0.40f, b);
        glVertex2f(ffX[i], ffY[i]);
    }
    glEnd();

    // Glow rings
    for (int i = 0; i < 20; i++)
    {
        if (ffBright[i] > 0.5f)
        {
            glColor4f(0.60f, 0.90f, 0.30f, ffBright[i] * 0.15f);
            drawCircle(ffX[i], ffY[i], 2.0f);
        }
    }
}


void drawRain()
{
    if (!isRaining) return;

    glLineWidth(1.2f);
    glBegin(GL_LINES);
    for (int i = 0; i < 300; i++)
    {
        float alpha = 0.5f + (i % 3) * 0.15f;
        glColor4f(0.65f, 0.72f, 1.0f, alpha);
        glVertex2f(rainX[i], rainY[i]);
        glVertex2f(rainX[i] + 1.0f, rainY[i] - 4.5f);  // Diagonal for realism
    }
    glEnd();
    glLineWidth(1.0f);

    // Splash dots at ground level
    glPointSize(2.0f);
    glBegin(GL_POINTS);
    for (int i = 0; i < 300; i++)
    {
        if (rainY[i] < -39 && rainY[i] > -42)
        {
            glColor4f(0.70f, 0.80f, 1.0f, 0.5f);
            glVertex2f(rainX[i], -40);
        }
    }
    glEnd();
}


void display()
{
    glClear(GL_COLOR_BUFFER_BIT);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    drawSky();
    drawStars();
    drawSunMoon();
    drawClouds();
    drawMountains();
    drawFog();
    drawGround();

    drawTree(-80, -20);
    drawTree(-52, -20);
    drawTree(40, -20);
    drawTree(72, -20);
    drawBushes();

    drawRiverBank();
    drawRiver();
    drawWaterEdge();
    drawEdgeGrass();
    drawStones();
    drawSmallPlants();

    drawBirds();
    drawFireflies();
    drawRain();

    glFlush();
}


void update(int v)
{
    cloudPos  += 0.15f;
    birdPos   += 0.4f;
    sunAngle  += 0.03f;
    waterOffset += 0.35f;

    if (cloudPos  > 220) cloudPos  = -220;
    if (birdPos   > 200) birdPos   = -200;
    if (sunAngle  > 200) sunAngle  = -200;
    if (waterOffset > 10) waterOffset = 0;

    // Rain update
    for (int i = 0; i < 300; i++)
    {
        rainY[i] -= 2.2f;
        rainX[i] += 0.5f;  // Slight diagonal
        if (rainY[i] < -100)
        {
            rainY[i] = 100;
            rainX[i] = (float)(rand() % 220) - 110;
        }
    }
 // Star twinkle
    for (int i = 0; i < 150; i++)
    {
        starBright[i] = 0.5f + 0.5f * sin(v * 0.003f + i * 1.3f);
    }

    // Firefly update
    for (int i = 0; i < 20; i++)
    {
        ffTimer[i] -= 0.016f;
        if (ffTimer[i] < 0)
        {
            ffBright[i] = 0.0f;
            if (ffTimer[i] < -1.5f)
            {
                ffTimer[i] = (float)(rand() % 30) / 10.0f + 0.5f;
                ffX[i] = (float)(rand() % 160) - 80;
                ffY[i] = (float)(rand() % 40) - 60;
                ffBright[i] = 1.0f;
            }
        }
        else
        {
            ffBright[i] = ffTimer[i] > 0.5f ? 1.0f : ffTimer[i] * 2.0f;
            // Gentle drift
            ffX[i] += sin(v * 0.01f + i) * 0.05f;
            ffY[i] += cos(v * 0.008f + i * 1.5f) * 0.03f;
        }
    }

    glutPostRedisplay();
    glutTimerFunc(16, update, v + 1);

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
