#include <stdio.h>
#include <GL/gl.h>
#include <GL/glut.h>
#include <math.h>
#include <time.h>
#include <string.h>

#define WINDOW_WIDTH 480
#define WINDOW_HEIGHT 480
#define TIMER_DELAY 50
#define PI 3.14
#define LOAD_RADIUS 15
#define NUM_SPRING_TURNS 10
#define SPRING_DIAMETER 25
#define SPRING_LENGTH WINDOW_HEIGHT / 2 - LOAD_RADIUS
#define SPRING_CONSTANT 0.005
#define GRAVITY 0.1

typedef struct
{
    float x, y, vy;
} Load;

Load load = {350, 240, -10};
float curr_spring_force = 0;
float energy = 0;

void drawWord(char *word, float x, float y, float step,
              int red, int green, int blue)
{
    if(red < 0) red = 0;
    if(red > 255) red = 255;
    if(green < 0) green = 0;
    if(green > 255) green = 255;
    if(blue < 0) blue = 0;
    if(blue > 255) blue = 255;
    glColor3f(red / 255.f, green / 255.f, blue / 255.f);
    for(int i = 0; i < strlen(word); ++i)
    {
        glRasterPos2f(x + i * step, y);
        char ch = word[i];
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, (int)ch);
    }
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT);
    //glColor3f(0,1,0);
    //Spring
    const float spring_step = (load.y - LOAD_RADIUS) / NUM_SPRING_TURNS;
    glColor3f(1,0,0);
    glLineWidth(5);
    glBegin(GL_LINE_STRIP);
        //glVertex2f(load.x, WINDOW_HEIGHT / 2);
        //glVertex2f(load.x, load.y);
        for(int i = 0; i < NUM_SPRING_TURNS; ++i)
        {
            glVertex2f(i % 2 == 0 ? load.x : load.x + SPRING_DIAMETER, i * spring_step);
            glVertex2f(i % 2 == 0 ? load.x + SPRING_DIAMETER : load.x, (i + 1) * spring_step);
        }
    glEnd();
    glLineWidth(1);
    //Load
    glColor3f(0,1,0);
    glBegin(GL_TRIANGLES);
        for(int i = 0; i < 36; ++i)
        {
            glVertex2f(load.x, load.y);
            float x1 = load.x + LOAD_RADIUS * sin(i * 2 * PI / 36);
            float y1 = load.y + LOAD_RADIUS * cos(i * 2 * PI / 36);
            glVertex2f(x1, y1);
            float x2 = load.x + LOAD_RADIUS * sin((i + 1) * 2 * PI / 36);
            float y2 = load.y + LOAD_RADIUS * cos((i + 1) * 2 * PI / 36);
            glVertex2f(x2, y2);
        }
    glEnd();
    char buffer[50];
    sprintf(buffer, "Force: %6.3f", curr_spring_force);
    drawWord(buffer, 10, 20, 13, 255, 255, 0);
    sprintf(buffer, "Position: %6.3f", load.y);
    drawWord(buffer, 10, 50, 13, 255, 255, 0);
    sprintf(buffer, "Velocity: %6.3f", load.vy);
    drawWord(buffer, 10, 80, 13, 255, 255, 0);
    sprintf(buffer, "Elastic energy: %6.3f", energy);
    drawWord(buffer, 10, 110, 13, 255, 255, 0);
    glutSwapBuffers();
}


void timer(int dt)
{
    curr_spring_force = 0;
    if(load.y != SPRING_LENGTH)
    {
        //F = -k * dx
        curr_spring_force = SPRING_CONSTANT * (load.y - SPRING_LENGTH);
    }
    curr_spring_force -= GRAVITY;
    load.vy -= curr_spring_force;
    load.y += load.vy;
    //printf("y = %8.3f\n", load.y);
    // printf("vy = %8.3f\n", load.vy);
    //printf("Force = %8.3f\n", curr_spring_force);
    energy = 0.5 * SPRING_CONSTANT * pow(load.y - SPRING_LENGTH, 2);
    // printf("Energy = %8.3f\n", energy);
    display();
    glutTimerFunc(TIMER_DELAY, timer, 0);
}

int main(int argc, char *argv[])
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    glutInitWindowPosition(20, 20);
    glutCreateWindow("Load hanging by spring");
    glClearColor(0,0,0,0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0,WINDOW_WIDTH, WINDOW_HEIGHT,0,-1,1);
    glutDisplayFunc(display);
    glutTimerFunc(TIMER_DELAY, timer, 0);
    glutMainLoop();
}
