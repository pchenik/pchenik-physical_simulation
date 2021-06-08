#include <GL/glut.h>
#include <openacc.h>
#include <stdio.h>

extern void start();

int main(int argc, char** argv)
{

    glutInit(&argc,argv);
    glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGB|GLUT_DEPTH);
    glutInitWindowSize(500,500);
    glutCreateWindow("Corpuscles");
    start();
    glutMainLoop();
    return 0;
}


