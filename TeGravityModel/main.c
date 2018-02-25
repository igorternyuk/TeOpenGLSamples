#include <stdio.h>
#include <GL/glut.h>
#include <GL/gl.h>
#include <math.h>
#include <time.h>

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
#define TIMER_DELAY 10
#define GRAVITY 0.01
#define PI 3.14

enum { NUM_PARTICLES = 4, SUN = 0, VENUS = 1, EARTH = 2, MOON = 3};

typedef struct
{
    float x, y;
    float vx, vy;
    float mass, radius;
} Particle;

Particle particles[NUM_PARTICLES];

void draw_filled_circle(float cx, float cy, float radius)
{
    glBegin(GL_TRIANGLES);
    for(int i = 0; i < 36; ++i)
    {
        float x1 = cx + radius * sin(2 * PI / 36 * i);
        float y1 = cy + radius * cos(2 * PI / 36 * i);
        float x2 = cx + radius * sin(2 * PI / 36 * (i + 1));
        float y2 = cy + radius * cos(2 * PI / 36 * (i + 1));
        glVertex2f(x1,y1);
        glVertex2f(cx,cy);
        glVertex2f(x2,y2);
    }
    glEnd();
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT);
    for(int i = 0; i < NUM_PARTICLES; ++i)
    {
        switch (i) {
        case SUN:
            glColor3f(1.0f, 1.0f, 0);
            break;
        case VENUS:
            glColor3f(1.0f, 0.55f, 0);
            break;
        case EARTH:
            glColor3f(56.f / 255, 172.f / 255, 215.f / 255);
            break;
        case MOON:
            glColor3f(255.f / 255, 233.f / 255, 127.f / 255);
            break;
        default:
            break;
        }
        draw_filled_circle(particles[i].x, particles[i].y, particles[i].radius);
    }
    glutSwapBuffers();
}

void timer(int t)
{
    for(int i = 0; i < NUM_PARTICLES; ++i)
        for(int j = 0; j < NUM_PARTICLES; ++j)
        {
            if(i != j)
            {
                const float dx = particles[j].x - particles[i].x;
                const float dy = particles[j].y - particles[i].y;
                const float dist = sqrt(dx * dx + dy * dy);
                const float acceleration = GRAVITY * particles[j].mass / (dist * dist);
                //Beacuse of the repulsive forces
                if(dist > 3)
                {
                    particles[i].vx += acceleration * dx / dist;
                    particles[i].vy += acceleration * dy / dist;
                }
            }
        }

    for(int i = 0; i < NUM_PARTICLES; ++i)
    {
        particles[i].x += particles[i].vx;
        particles[i].y += particles[i].vy;
    }

    display();
    glutTimerFunc(TIMER_DELAY, timer, 0);
}

void init_particles()
{
    particles[SUN].x = WINDOW_WIDTH / 2;
    particles[SUN].y = WINDOW_HEIGHT / 2;
    particles[SUN].vx = 0;
    particles[SUN].vy = 0;
    particles[SUN].mass = 3000;
    particles[SUN].radius = 14;

    particles[VENUS].x = WINDOW_WIDTH / 2 + 70;
    particles[VENUS].y = WINDOW_HEIGHT / 2;
    particles[VENUS].vx = 0;
    particles[VENUS].vy = -0.5f;
    particles[VENUS].mass = 5;
    particles[VENUS].radius = 5;

    particles[EARTH].x = 150;
    particles[EARTH].y = WINDOW_HEIGHT / 2;
    particles[EARTH].vx = 0;
    particles[EARTH].vy = 0.3f;
    particles[EARTH].mass = 28;
    particles[EARTH].radius = 5;

    particles[MOON].x = 145;
    particles[MOON].y = WINDOW_HEIGHT / 2;
    particles[MOON].vx = 0;
    particles[MOON].vy = 0.6f;
    particles[MOON].mass = 0.01;
    particles[MOON].radius = 1;
}

int main(int argc, char *argv[])
{
    init_particles();
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    glutInitWindowPosition(20, 20);
    glutCreateWindow("Gravity model");
    glClearColor(0,0,0,1);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0,WINDOW_WIDTH, WINDOW_HEIGHT,0,-1,1);
    glutDisplayFunc(display);
    glutTimerFunc(TIMER_DELAY, timer, 0);
    glutMainLoop();
    return 0;
}
