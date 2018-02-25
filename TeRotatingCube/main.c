#include <stdio.h>
#include <GL/gl.h>
#include <GL/glut.h>
#include <math.h>
#include <time.h>

#define TIMER_DELAY 10
#define WINDOW_WIDTH 480
#define WINDOW_HEIGHT 480

void display()
{
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(1,0,0);
    glLineWidth(3);
    glBegin(GL_LINE_STRIP);
        glVertex3f(-50, 50, 50);
        glVertex3f(50, 50, 50);
        glVertex3f(50, -50, 50);
        glVertex3f(-50, -50, 50);
        glVertex3f(-50, 50, 50);
    glEnd();
    glColor3f(0,1,0);
    glBegin(GL_LINE_STRIP);
        glVertex3f(-50, 50, -50);
        glVertex3f(50, 50, -50);
        glVertex3f(50, -50, -50);
        glVertex3f(-50, -50, -50);
        glVertex3f(-50, 50, -50);
    glEnd();
    glColor3f(0,0.65f,0.8f);
    glBegin(GL_LINES);
        glVertex3f(-50, 50, 50);
        glVertex3f(-50, 50, -50);

        glVertex3f(50, 50, 50);
        glVertex3f(50, 50, -50);

        glVertex3f(50, -50, 50);
        glVertex3f(50, -50, -50);

        glVertex3f(-50, -50, 50);
        glVertex3f(-50, -50, -50);
    glEnd();
    glLineWidth(1);
    glRotatef(1.0f, 1, 1, 0);
    glutSwapBuffers();
}

void timer(int dt)
{
    display();
    glutTimerFunc(TIMER_DELAY, timer, 0);
}

int main(int argc, char *argv[])
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    glutInitWindowPosition(20, 20);
    glutCreateWindow("Rotating cube");
    glClearColor(0,0,0,1);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(-100, 100, -100, 100, 100, 2000);
    glMatrixMode(GL_MODELVIEW);
    glTranslatef(0, 0, -WINDOW_HEIGHT * 9 / 24);
    glRotatef(-30, 1, 0, 0);
    glutDisplayFunc(display);
    glutTimerFunc(TIMER_DELAY, timer, 0);
    glutMainLoop();
    return 0;
}
