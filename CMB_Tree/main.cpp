#define GLUT_DISABLE_ATEXIT_HACK
#include <iostream>
#include <windows.h>
#include <stdio.h>
#include <string.h>
#include <sstream>
#include <math.h>
#include <GL/glut.h>
#include "glm/glm/vec3.hpp"
#include "glm/glm/vec4.hpp"
#include "cluster.h"
#include "e_clust.h"

using namespace std;

glm::vec3 position_camera(-50.0, 0.0, 0.0);
glm::vec3 position_reference(0.0, 0.0, 0.0);
glm::vec3 up_vector(0.0, 1.0, 0.0);

bool bottom_number = true;

cluster clust;
extern vector<e_clust*> tree;
int y_razon = 5;



void draw_function(void);
void draw_axis(void);
void keyboard(unsigned char, int, int);
void Idle(void);
void draw_element_at_position(string, float, float);
void draw_number_at_position(string, float, float);
void draw_tree();

int main(int argc, char** argv){
  clust.read_matriz(100);
  //clust.print_data();
  clust.aglomerativo_minimo(1); //0 min, 1 max, 2 prom


  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);
  glutInitWindowSize(1350, 720);
  glutInitWindowPosition(0, 0);
  glutCreateWindow("Game");

  glutDisplayFunc(draw_function);
  glutKeyboardFunc(keyboard);
  //glutIdleFunc(Idle);
  glutMainLoop();
  return EXIT_SUCCESS ;
}


void draw_function(void){
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(30.0f, 2.0, 1.0f, 1000.0f);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  gluLookAt(position_camera.x, position_camera.y, position_camera.z, position_reference.x, position_reference.y, position_reference.z, up_vector.x, up_vector.y, up_vector.z);

  glPushMatrix();
    glRotatef(90, 0, 1, 0);
    //draw_axis();
    draw_tree();
  glPopMatrix();

  glFlush();
  glutSwapBuffers();
}

void draw_axis(void){
  glBegin(GL_LINES);
    glColor3f(1.0, 0.0, 0.0); //Red
    glVertex3f(-10.0, 0.0f, 0.0f);//Para X
    glVertex3f(10.0f, 0.0f, 0.0f);
  glEnd();

  glBegin(GL_LINES);
    glColor3f(0.0, 1.0, 0.0); //Verde
    glVertex3f(0.0, -10.0f, 0.0f);
    glVertex3f(0.0f, 10.0f, 0.0f);//Para Y
  glEnd();

  glBegin(GL_LINES);
    glColor3f(0.0, 0.0, 1.0); //Azul
    glVertex3f(0.0, 0.0f, -10.0f);//Para Z
    glVertex3f(0.0f, 0.0f, 10.0f);
  glEnd();
}

void draw_number_at_position(string number, float x, float y){
  glColor3f(1.0, 1.0, 1.0);
  glPushMatrix();
    glTranslatef(x, y, 0);
    glRasterPos2f(1.5, -1);
    for(unsigned int i = 0; i < number.size(); i++){
      glutBitmapCharacter(GLUT_BITMAP_8_BY_13, number[i]);
    }
  glPopMatrix();
}

void draw_element_at_position(string word, float x, float y){
  glColor3f(1.0, 1.0, 1.0);
  glPushMatrix();
    glTranslatef(x, y, 0);
    glRasterPos2f(3.5, -0.2);
    for(unsigned int i = 0; i < word.size(); i++){
      glutBitmapCharacter(GLUT_BITMAP_HELVETICA_10, word[i]);
    }
    glColor3f(1.0, 0.0, 0.0);
    glutWireTorus(0.1, 5, 25, 25);
  glPopMatrix();
}

void draw_tree(){
  for(unsigned int i = 0; i < tree.size(); i++){
    draw_element_at_position(tree[i]->name, tree[i]->origen.x, tree[i]->origen.y);
    for(unsigned int j = 1; j < tree[i]->center.size(); j++){
      glBegin(GL_LINES);
        glColor3f(1.0, 0.0, 0.0);
        glVertex3f(tree[i]->center[j-1].x, y_razon * tree[i]->center[j-1].y, 0.0);
        glVertex3f(tree[i]->center[j-1].x, y_razon * tree[i]->center[j].y, 0.0);

        glVertex3f(tree[i]->center[j-1].x, y_razon *  tree[i]->center[j].y, 0.0);
        glVertex3f(tree[i]->center[j].x, y_razon *  tree[i]->center[j].y, 0.0);
      glEnd();
      if(bottom_number){
        stringstream ss;
        ss << tree[i]->distances[j];
        draw_number_at_position(ss.str(), tree[i]->center[j].x, y_razon * tree[i]->center[j].y);
      }
    }
  }
}



void keyboard(unsigned char key, int x, int y){
  glutPostRedisplay();
  switch (key) {
    case 'z':
      //position_camera.x += 1.0f;
      position_camera.x += 5.0f;
      break;
    case 'x':
      //position_camera.x += 1.0f;
      position_camera.x -= 5.0f;
      break;
    case 'w':
      position_camera.y += 5.0f;
      position_reference.y += 5.0f;
      break;
    case 's':
      position_camera.y -= 5.0f;
      position_reference.y -= 5.0f;
      break;
    case 'a':
      position_camera.z -= 5.0f;
      position_reference.z -= 5.0f;
      break;
    case 'd':
      position_camera.z += 5.0f;
      position_reference.z += 5.0f;
      break;
    case 'q':
      bottom_number = !bottom_number;
      break;
  }
}

void Idle(void){
  glutPostRedisplay();
}
