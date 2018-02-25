#include <stdio.h>
#include <GL/gl.h>
#include <GL/glut.h>
#include <math.h>
#include <time.h>

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 480
#define TIMER_DELAY 10
#define NUM_STARS 1000
#define STAR_VELOCITY 2.5f
#define RANGE_X 150
#define RANGE_Y 150
#define STAR_Z_NEAR 50
#define STAR_Z_FAR -500

typedef struct
{
    float x, y, z;
} Star;

Star stars[NUM_STARS];

void display()
{
    glClear(GL_COLOR_BUFFER_BIT);
    for(int i = 0; i < NUM_STARS; ++i)
    {
        const float brightness = 1.0f - 0.5f * (stars[i].z - STAR_Z_NEAR) / (STAR_Z_FAR - STAR_Z_NEAR);
        glColor3f(brightness, brightness, 0.0f);
        const float star_size = 2.0f - (stars[i].z - STAR_Z_NEAR) / (STAR_Z_FAR - STAR_Z_NEAR);
        glPointSize(star_size);
        glBegin(GL_POINTS);
            glVertex3f(stars[i].x, stars[i].y, stars[i].z);
        glEnd();
    }
    glutSwapBuffers();
}

void timer(int t)
{
    display();
    for(int i = 0; i < NUM_STARS; ++i)
    {
        stars[i].z += STAR_VELOCITY;
        if(stars[i].z > STAR_Z_NEAR)
        {
            stars[i].x = 1.0f * (rand() % (2 * RANGE_X)) - RANGE_X;
            stars[i].y = 1.0f * (rand() % (2 * RANGE_Y)) - RANGE_Y;
            stars[i].z = -500;
        }
    }
    glutTimerFunc(TIMER_DELAY, timer, 0);
}

int main(int argc, char *argv[])
{
    srand(time(0));
    for(int i = 0; i < NUM_STARS; ++i)
    {
        Star star = { 1.0f * (rand() % (2 * RANGE_X)) - RANGE_X,
                      1.0f * (rand() % (2 * RANGE_Y)) - RANGE_Y,
                      -1.0f * (rand() % 500)
                    };
        stars[i] = star;
    }
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    glutInitWindowPosition(20, 20);
    glutCreateWindow("Star field");
    glClearColor(0, 0, 0, 1.0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45, 1, 0, 100);
    glutDisplayFunc(display);
    glutTimerFunc(TIMER_DELAY, timer, 0);
    glutMainLoop();
    return 0;
}
