#include <stdio.h>
#include <GL/gl.h>
#include <GL/glut.h>
#include <math.h>
#include <time.h>

#define WINDOW_WIDTH 480
#define WINDOW_HEIGHT 480
#define TIMER_DELAY 10
#define DELTA_T 0.01f
#define NUM_ELEMENTS 60
#define LOAD_RADIUS 10
#define LOAD 5
#define RUBBER_LENTH WINDOW_WIDTH / 2
#define ELEMENT_LENTH RUBBER_LENTH / NUM_ELEMENTS
#define FRICTION 0.99996f
#define PI 3.14

//const float DT = DELTA_T / 1000000.0;

typedef struct
{
    float x,y;    //coordinates
    float vx, vy; //element velocity components
} Element;

// o--o--o--o--o--o--o--o --> rubber is a set of elements

typedef struct
{
    Element elements[NUM_ELEMENTS];
} Rubber;

Rubber rubber;



void rubber_draw(const Rubber *rubber)
{
    if(rubber == NULL) return;
    glLineWidth(3);
    glBegin(GL_LINE_STRIP);
    for(int i = 0; i < NUM_ELEMENTS; ++i)
        glVertex2f(rubber->elements[i].x, rubber->elements[i].y);
    glEnd();
    glLineWidth(1);
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(0, 0.8f, 0);
    rubber_draw(&rubber);

    //Here we draw the load at the rubber end

    glColor3f(0.8f, 0, 0);
    float load_center_x = rubber.elements[NUM_ELEMENTS - 1].x;
    float load_center_y = rubber.elements[NUM_ELEMENTS - 1].y;
    float delta_alpha = 2 * PI / 36;
    glBegin(GL_TRIANGLES);
        for(int i = 0; i < 36; ++i)
        {
            float x1 = load_center_x + LOAD_RADIUS * sin(delta_alpha * i);
            float y1 = load_center_y + LOAD_RADIUS * cos(delta_alpha * i);
            float x2 = load_center_x + LOAD_RADIUS * sin(delta_alpha * (i + 1));
            float y2 = load_center_y + LOAD_RADIUS * cos(delta_alpha * (i + 1));
            glVertex2f(x1, y1);
            glVertex2f(load_center_x, load_center_y);
            glVertex2f(x2, y2);
        }
    glEnd();
    glutSwapBuffers();
}

void timer(int dt)
{
    for(int i = 0; i < 40; ++i)
        rubber_tick(&rubber);
    display();
    glutTimerFunc(TIMER_DELAY, timer, 0);
}

void rubber_init(Rubber *rubber)
{
    if(rubber == NULL) return;
    for(int i = 0; i < NUM_ELEMENTS; ++i)
    {
        rubber->elements[i].x = WINDOW_WIDTH / 2 + i * ELEMENT_LENTH * cos(PI / 12);
        rubber->elements[i].y = i * ELEMENT_LENTH * sin(PI / 12);
        rubber->elements[i].vx = 0;
        rubber->elements[i].vy = 0;
    }
    rubber->elements[NUM_ELEMENTS - 1].vy += LOAD * DELTA_T;
}

void rubber_tick(Rubber *rubber)
{
    if(rubber == NULL) return;
    for(int i = 1; i < NUM_ELEMENTS; ++i)
    {
        float a = 0;
        Element *prev = &rubber->elements[i - 1];
        Element *curr = &rubber->elements[i];
        float dx = prev->x - curr->x;
        float dy = prev->y - curr->y;
        float distance = sqrt(dx * dx + dy * dy);
        float rest = 0.9 * ELEMENT_LENTH;
        if(distance > rest)
            a = 5 * (distance - rest);

        float ax = a * dx / distance;
        float ay = a * dy / distance;

        curr->vx += ax * DELTA_T;
        curr->vy += ay * DELTA_T;

        if(i != 1)
        {
            //Nach dem dritten Gesaetz von Newton
            prev->vx -= ax * DELTA_T;
            prev->vy -= ay * DELTA_T;
        }
    }

    //Load gravity

    rubber->elements[NUM_ELEMENTS - 1].vy += LOAD * DELTA_T;

    //Here we move all elements

    for(int i = 0; i < NUM_ELEMENTS; ++i)
    {
        //Reibungskraft
        rubber->elements[i].vx *= FRICTION;
        rubber->elements[i].vy *= FRICTION;
        rubber->elements[i].x +=  rubber->elements[i].vx * DELTA_T;
        rubber->elements[i].y +=  rubber->elements[i].vy * DELTA_T;
    }
}

int main(int argc, char *argv[])
{
    rubber_init(&rubber);
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    glutInitWindowPosition(20, 20);
    glutCreateWindow("Rubber model");
    glClearColor(0,0,0,1);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0,WINDOW_WIDTH, WINDOW_HEIGHT,0,-1,1);
    glutDisplayFunc(display);
    glutTimerFunc(TIMER_DELAY, timer, 0);
    glutMainLoop();
}
