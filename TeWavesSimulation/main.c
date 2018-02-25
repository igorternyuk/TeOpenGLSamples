#include <stdio.h>
#include <GL/gl.h>
#include <GL/glut.h>
#include <math.h>
#include <time.h>

#define NUM_LINES 48
#define TIMER_DELAY 10
#define WINDOW_WIDTH 480
#define WINDOW_HEIGHT 480

const float K = 20;
const float DT = 0.1;

void display();
void timer(int d);

typedef struct
{
    float x, y, z;
    float vz;
}  Point;

Point points[NUM_LINES][NUM_LINES];

int main(int argc, char *argv[])
{
    const float step = WINDOW_WIDTH / NUM_LINES;
    //printf("\nstep = %15.8f\n", step);
    for(int x = 0; x < NUM_LINES; ++x)
    {
        for(int y = 0; y < NUM_LINES; ++y)
        {
            points[x][y].x = x * step;
            points[x][y].y = y * step;
            points[x][y].z = 0;
            points[x][y].vz = 0;
        }
    }
    srand(time(0));
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    glutInitWindowPosition(20, 20);
    glutCreateWindow("Waves");
    glClearColor(0,0,0,1);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(-100, 100, -100, 100, 100, 5000);
    glMatrixMode(GL_MODELVIEW);
    glTranslatef(-WINDOW_WIDTH / 2, -WINDOW_HEIGHT / 2, -WINDOW_HEIGHT / 2);
    glRotatef(-30, 1, 0, 0);
    //glRotatef(45, 0, 0, 1);
    glutDisplayFunc(display);
    glutTimerFunc(TIMER_DELAY, timer, 0);
    glutMainLoop();
    return 0;
}

void timer(int d)
{
    if(rand() % 200 == 0)
    {
        int rand_x = rand() % (NUM_LINES - 2) + 1;
        int rand_y = rand() % (NUM_LINES - 2) + 1;
        printf("rx = %d ry = %d\n", rand_x, rand_y);
        points[rand_x][rand_y].vz += 150;
    }

    const int delta_x[] = {1, 0, -1, 0};
    const int delta_y[] = {0, 1, 0, -1};
    for(int x = 1; x < NUM_LINES - 1; ++x)
    {
        for(int y = 1; y < NUM_LINES - 1; ++y)
        {
            Point *curr_point = &points[x][y];
            /*printf("(x= %d, y= %d) -> curr_x = %6.3f curr_y = %6.3f curr_z = %6.3f vz = %6.3f\n",
                   x, y, curr_point->x, curr_point->y, curr_point->z, curr_point->vz);*/
            for(int dir = 0; dir < 4; ++dir)
            {
                const int nx = x + delta_x[dir];
                const int ny = y + delta_y[dir];
                Point *neighbour_point = &points[nx][ny];
                const float dx = neighbour_point->x - curr_point->x;
                const float dy = neighbour_point->y - curr_point->y;
                const float dz = neighbour_point->z - curr_point->z;
                const float distance = sqrt(dx * dx + dy * dy + dz * dz);
                const float cosine = dz / distance;
                curr_point->vz += K * cosine * DT;
                curr_point->vz *= 0.994;
            }
        }
    }

    for(int x = 1; x < NUM_LINES - 1; ++x)
        for(int y = 1; y < NUM_LINES - 1; ++y)
        {
            Point *curr_point = &points[x][y];
            curr_point->z += curr_point->vz;
        }
    display();
    glutTimerFunc(TIMER_DELAY, timer, 0);
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(0,1,0.57f);
    for(int x = 0; x < NUM_LINES; ++x)
    {
        glBegin(GL_LINE_STRIP);
        for(int y = 0; y < NUM_LINES; ++y)
           glVertex3f(points[x][y].x, points[x][y].y, points[x][y].z);
        glEnd();
    }

    for(int y = 0; y < NUM_LINES; ++y)
    {
        glBegin(GL_LINE_STRIP);
        for(int x = 0; x < NUM_LINES; ++x)
           glVertex3f(points[x][y].x, points[x][y].y, points[x][y].z);
        glEnd();
    }
    glutSwapBuffers();
}
