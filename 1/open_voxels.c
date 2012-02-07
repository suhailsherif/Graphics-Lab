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
#include "libcube.h"
#include "libvoxel.h"

voxels AbstractVoxels, VoxelSphere;
int AbstractDepth=1, SphereDepth=1;
float _eyex=0.0, _eyez=2.0, _anglex, _angley;

//VoxelSphere
void renderVoxelSphere()
{
    float i, j, k, UNIT=1.0/pow(2,SphereDepth);
    VoxelSphere.begin=NULL;
    VoxelSphere.end=NULL;
    for(i=-0.5; i<0.5; i+=UNIT){
     for(j=-0.5; j<0.5; j+=UNIT){
      for(k=-0.5; k<0.5; k+=UNIT){
        if(i*i+j*j+k*k<.25) addVoxel(&VoxelSphere, i, j, k);
      }
     }
    }
}

addAbstractLevel(float x, float y, float z, int depth)
{
    float blocksize=1/pow(2, depth);
    if(depth==AbstractDepth){
        addVoxel(&AbstractVoxels, x, y, z);
        addVoxel(&AbstractVoxels, x+blocksize, y+blocksize, z);
        addVoxel(&AbstractVoxels, x, y+blocksize, z+blocksize);
        addVoxel(&AbstractVoxels, x+blocksize, y, z+blocksize);
    } else {
        addAbstractLevel(x, y, z, depth+1);
        addAbstractLevel(x+blocksize, y+blocksize, z, depth+1);
        addAbstractLevel(x, y+blocksize, z+blocksize, depth+1);
        addAbstractLevel(x+blocksize, y, z+blocksize, depth+1);
    }
}

void renderAbstractVoxels()
{
    float i, UNIT=1.0/pow(2,AbstractDepth);
    AbstractVoxels.begin=NULL;
    AbstractVoxels.end=NULL;
    addAbstractLevel(-0.5, -0.5, -0.5, 1);
}

void renderVoxels()
{
    glClearColor(0.0, 0.0, 0.0, 0.0);
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

    glRotatef(_anglex,1.0,0.0,0.0);
    glRotatef(_angley,0,1.0,0.0);
    glColor3f(1.0, 1.0, 1.0);
    
    drawVoxels(&AbstractVoxels, 0.75, 0.0, 0.0, 1.0/pow(2,AbstractDepth));
    drawVoxels(&VoxelSphere, -0.75, 0.0, 0.0, 1.0/pow(2, SphereDepth)); 
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
                renderAbstractVoxels();
                printf("Abstract depth is now %d\n",AbstractDepth);
                break;
      case 'v': if(AbstractDepth>1) AbstractDepth--;
                renderAbstractVoxels();
                printf("Abstract depth is now %d\n",AbstractDepth);
                break;
      case 'n': SphereDepth++;
                renderVoxelSphere();
                printf("Sphere depth is now %d\n",SphereDepth);
                break;
      case 'm': if(SphereDepth>1) SphereDepth--;
                renderVoxelSphere();
                printf("Sphere depth is now %d\n",SphereDepth);
                break;
  }
  glutPostRedisplay();
}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode (GLUT_SINGLE | GLUT_RGBA | GLUT_DEPTH );
    renderAbstractVoxels();
    renderVoxelSphere();
    glutInitWindowSize(500,500);
    glutInitWindowPosition(100,100);
    glutCreateWindow("OpenGL - Voxels");
    glutDisplayFunc(renderVoxels);
    glutKeyboardFunc(keyPressed);
    glutMainLoop();    
    return 0;
}
