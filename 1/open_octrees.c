#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include<X11/X.h>
#include<X11/Xlib.h>
#include<GL/gl.h>
#include<GL/glx.h>
#include<GL/glu.h>
#include<GL/freeglut.h>
#include<math.h>
#include "graph.h"

float _anglex, _angley, _eyex=0.0, _eyez=2.0;
octree octsphere, octabs;
int AbstractDepth=1, SphereDepth=1;

bool insideSphere(float x, float y, float z, float radius){
    return x*x+y*y+z*z<=radius*radius;
}

void recursiveOctreeSphere(octree *a, float cx, float cy, float cz, float side, int depth){
    bool inside=true;
    int i;
    //Too small
    if(depth>SphereDepth){
        if(insideSphere(cx, cy, cz, 0.5))	a->leaf=1;
        else    a->leaf=-1;
        return;
    }
    //Center inside
    if(insideSphere(cx, cy, cz, 0.5)){
        for(i=0;i<8;i++)
            inside = inside && insideSphere(cx+cubeVertices[i][0]*side/2, cy+cubeVertices[i][1]*side/2, cz+cubeVertices[i][2]*side/2, 0.5);
        //All corners inside
        if(inside)	a->leaf=1;
        //Center inside, not all corners
        else {
            addChildren(a, 1, 1, 1, 1, 1, 1, 1, 1);
            for(i=0;i<8;i++)	recursiveOctreeSphere(a->children[i],cx+cubeVertices[i][0]*side/4, cy+cubeVertices[i][1]*side/4, cz+cubeVertices[i][2]*side/4, side/2, depth+1);
        }
    } else {
        inside = false;
        for(i=0;i<8;i++)
            inside = inside || insideSphere(cx+cubeVertices[i][0]*side/2, cy+cubeVertices[i][1]*side/2, cz+cubeVertices[i][2]*side/2, 0.5);
        //Center, all corners outside
        if(!inside)	a->leaf=-1;
        //Center outside, some corners in
        else {
            addChildren(a, 1, 1, 1, 1, 1, 1, 1, 1);
            for(i=0;i<8;i++)    recursiveOctreeSphere(a->children[i],cx+cubeVertices[i][0]*side/4, cy+cubeVertices[i][1]*side/4, cz+cubeVertices[i][2]*side/4, side/2, depth+1);
        }
    }
}


//OctreeSphere
void renderOctreeSphere()
{
    recursiveOctreeSphere(&octsphere, 0.0, 0.0, 0.0, 1.0, 1);
}


//Abstract Octree
void recursiveAbstractOctree(octree *a, int depth){
    int i;
    if(depth>AbstractDepth){
        a->leaf=1;
        return;
    }
    addChildren(a, 1, 0, 0, 1, 0, 1, 1, 0);
    for(i=0;i<8;i++)
        if(a->children[i])
            if(a->children[i]->leaf!=1)
                recursiveAbstractOctree(a->children[i], depth+1);
}

void renderAbstractOctree()
{
    recursiveAbstractOctree(&octabs, 1);
}

void renderOctrees()
{

    glClearColor (0.0,0.0,0.0,1.0);
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    glDepthMask(GL_TRUE);
    glDepthFunc(GL_LEQUAL); 
    glEnable(GL_COLOR_MATERIAL);
//    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHT1);
    glEnable(GL_NORMALIZE);
    glLoadIdentity();

    

    //Add ambient light
    //GLfloat ambientColor[] = {0.2f, 0.2f, 0.2f, 1.0f}; //Color (0.2, 0.2, 0.2)
    //glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientColor);

    //Add positioned lights
    GLfloat lightColor0[] = {0.5f, 0.5f, 0.5f, 1.0f}; //Color (0.5, 0.5, 0.5)
    GLfloat lightPos0[] = {-1.0f, 1.0f, -1.0f, 1.0f};
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightColor0);
    glLightfv(GL_LIGHT0, GL_POSITION, lightPos0);
    GLfloat lightColor1[] = {0.5f, 0.2f, 0.2f, 1.0f}; //Color (0.5, 0.2, 0.2)
    GLfloat lightPos1[] = {-1.0f, 0.5f, 0.5f, 0.0f};
    glLightfv(GL_LIGHT1, GL_SPECULAR, lightColor1);
    glLightfv(GL_LIGHT1, GL_POSITION, lightPos1);


    glOrtho(-2.0, 2.0, -2.0, 2.0, 0.1, 4);

    gluLookAt(sin(_eyex)*2, 0, cos(_eyex)*2, 0, 0, 0, 0, -1, 0);

    glColor3f(1.0, 1.0, 1.0);
    glRotatef(_anglex,1.0,0.0,0.0);
    glRotatef(_angley,0,1.0,0.0);
    drawOctree(&octsphere, -0.75, 0.0, 0.0, 1.0);
    glColor3f(1, 1, 1.0);
    drawOctree(&octabs, 0.75, 0.0, 0.0, 1.0);
    glFlush();
}

void keyPressed(unsigned char x, int i, int j)
{
  switch(x){
      case 'a': _anglex-=5;
                break;
      case 'd': _anglex+=5;
                break;
      case 'w': _angley+=5;
                break;
      case 's': _angley-=5;
                break;
      case 'j': _eyex-=.05;
                break;
      case 'l': _eyex+=.05;
                break;
      case 'i': _eyez+=.05;
                break;
      case 'k': _eyez-=.05;
                break;
      case 'c': AbstractDepth++;
                renderAbstractOctree();
                printf("Abstract depth is now %d\n",AbstractDepth);
                break;
      case 'v': if(AbstractDepth>1) AbstractDepth--;
                renderAbstractOctree();
                printf("Abstract depth is now %d\n",AbstractDepth);
                break;
      case 'n': SphereDepth++;
                renderOctreeSphere();
                printf("Sphere depth is now %d\n",SphereDepth);
                break;
      case 'm': if(SphereDepth>1) SphereDepth--;
                renderOctreeSphere();
                printf("Sphere depth is now %d\n",SphereDepth);
                break;
  }
  glutPostRedisplay();
}


int main(int argc, char** argv)
{
    renderAbstractOctree();
    renderOctreeSphere();
    glutInit(&argc, argv);
    glutInitDisplayMode (GLUT_SINGLE | GLUT_RGBA | GLUT_DEPTH );
    glutInitWindowSize(500,500);
    glutInitWindowPosition(100,100);
    glutCreateWindow("OpenGL - Octrees");
    glutDisplayFunc(renderOctrees);
    glutKeyboardFunc(keyPressed);
    glutMainLoop();    
    return 0;
}

