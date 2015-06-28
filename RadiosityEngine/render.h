//
//  render.h
//  RadiosityEngine
//
//  Created by Roshan Gautam on 6/28/15.
//  Copyright (c) 2015 Roshan Gautam. All rights reserved.
//
#ifdef __APPLE__
#include <GLUT/glut.h>
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#else
#include <GL/glut.h>
#include <GL/glu.h>
#include <GL/gl.h>
#endif

#ifndef RadiosityEngine_render_h
#define RadiosityEngine_render_h

double rotate_y = 5;
double rotate_x = 5;


Patch transformToViewPlane(float e, Patch patch) {
    Patch transformed;
    Vector *vertices = patch.getVertices();
    Vector transformedVertices[3];
    float x,y;
    for (int i = 0 ; i < 3; i++) {
        x = (e / ( e + vertices[i].getZ() )) * vertices[i].getY();
        y = (e / ( e + vertices[i].getZ() )) * vertices[i].getX();
        transformedVertices[i].setCoordinates(x, y, 0);
    }
    transformed.setVertices(transformedVertices[0], transformedVertices[1], transformedVertices[2]);
    return transformed;
}

void drawPatch(Patch patch) {
    glBegin(GL_LINE_LOOP);
    glColor3f( 1.0, 0.0, 0.0 );
    for (int i = 0 ; i < 3; i++) {
        glVertex3f(patch.getVertices()[i].getX() / 30, patch.getVertices()[i].getY() / 30, patch.getVertices()[i].getZ() / 30);
    }
    glEnd();
}

void getPatches() {
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    
    // Reset transformations
    glLoadIdentity();
    
    // Rotate when user changes rotate_x and rotate_y
    glRotatef( rotate_x, 1.0, 0.0, 0.0 );
    glRotatef( rotate_y, 0.0, 1.0, 0.0 );
    
    ifstream objectFile("room.obj");
    if (objectFile.is_open()) {
        std::istream_iterator<double> start(objectFile), end;
        std::vector<double> lines(start, end);
        
        Vector *vertices = new Vector[lines.size()];
        Patch *patches = new Patch[lines.size()/9];
        for (int i = 0, j = 0 ; j < lines.size()/3 ; i+=3,j++) {
            vertices[j].setCoordinates(lines.at(i), lines.at(i+1), lines.at(i+2));
        }
        int polyCount;
        for (int i = 0, j = 0; j < lines.size()/9; i+=3, j++) {
            patches[j].setVertices(vertices[i], vertices[i+1], vertices[i+2]);
            polyCount++;
            drawPatch(patches[j]);

        }
//        drawPatch(patches[0]);
    }
    glFlush();
    glutSwapBuffers();
}


void cube()
{
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

    // Reset transformations
    glLoadIdentity();
    
    // Rotate when user changes rotate_x and rotate_y
    glRotatef( rotate_x, 1.0, 0.0, 0.0 );
    glRotatef( rotate_y, 0.0, 1.0, 0.0 );

    //Multi-colored side - FRONT
    glBegin(GL_LINE_LOOP);
    
//    glColor3f( 1.0, 0.0, 0.0 );
    glVertex3f(  0.5, -0.5, -0.5 );      // P1 is red
    glVertex3f(  0.5,  0.5, -0.5 );      // P2 is green
    glVertex3f( -0.5,  0.5, -0.5 );      // P3 is blue
    glVertex3f( -0.5, -0.5, -0.5 );      // P4 is purple
    
    glEnd();
    
    // White side - BACK
    glBegin(GL_LINE_LOOP);
//    glColor3f(   1.0,  1.0, 1.0 );
    glVertex3f(  0.5, -0.5, 0.5 );
    glVertex3f(  0.5,  0.5, 0.5 );
    glVertex3f( -0.5,  0.5, 0.5 );
    glVertex3f( -0.5, -0.5, 0.5 );
    glEnd();
    
    // Purple side - RIGHT
    glBegin(GL_LINE_LOOP);
    glColor3f(  1.0,  0.0,  1.0 );
    glVertex3f( 0.5, -0.5, -0.5 );
    glVertex3f( 0.5,  0.5, -0.5 );
    glVertex3f( 0.5,  0.5,  0.5 );
    glVertex3f( 0.5, -0.5,  0.5 );
    glEnd();
    
    // Green side - LEFT
    glBegin(GL_LINE_LOOP);
    glColor3f(   0.0,  1.0,  0.0 );
    glVertex3f( -0.5, -0.5,  0.5 );
    glVertex3f( -0.5,  0.5,  0.5 );
    glVertex3f( -0.5,  0.5, -0.5 );
    glVertex3f( -0.5, -0.5, -0.5 );
    glEnd();
    
    // Blue side - TOP
    glBegin(GL_LINE_LOOP);
    glColor3f(   0.0,  0.0,  1.0 );
    glVertex3f(  0.5,  0.5,  0.5 );
    glVertex3f(  0.5,  0.5, -0.5 );
    glVertex3f( -0.5,  0.5, -0.5 );
    glVertex3f( -0.5,  0.5,  0.5 );
    glEnd();
    
    // Red side - BOTTOM
    glBegin(GL_LINE_LOOP);
    glColor3f(   1.0,  0.0,  0.0 );
    glVertex3f(  0.5, -0.5, -0.5 );
    glVertex3f(  0.5, -0.5,  0.5 );
    glVertex3f( -0.5, -0.5,  0.5 );
    glVertex3f( -0.5, -0.5, -0.5 );
    glEnd();
    
    glFlush();
    glutSwapBuffers();
}

void specialKeys( int key, int x, int y ) {
    
    //  Right arrow - increase rotation by 5 degree
    if (key == GLUT_KEY_RIGHT)
        rotate_y += 5;
    
    //  Left arrow - decrease rotation by 5 degree
    else if (key == GLUT_KEY_LEFT)
        rotate_y -= 5;
    
    else if (key == GLUT_KEY_UP)
        rotate_x += 5;
    
    else if (key == GLUT_KEY_DOWN)
        rotate_x -= 5;
    
    //  Request display update
    glutPostRedisplay();
    
}

void render(int *argcp, char **argv) {
    glutInit(argcp, argv);
    glutInitWindowSize(2880, 1800);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);
    glutCreateWindow("Radiosity");
    
    glutDisplayFunc(getPatches);
    glutSpecialFunc(specialKeys);
    glClearColor(0, 0, 0, 1);
    glutMainLoop();
}
#endif
