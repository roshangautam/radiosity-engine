//
//  render.h
//  RadiosityEngine
//
//  Created by Roshan Gautam on 6/28/15.
//  Copyright (c) 2015 Roshan Gautam. All rights reserved.
//

#include <GLUT/glut.h>
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>


#include "camera.h"

CCamera Camera;

Patch *patches;
int polygons;
GLfloat *flux;

#ifndef RadiosityEngine_render_h
#define RadiosityEngine_render_h



void drawPatch(Patch patch, GLfloat flux) {
    Vector *vertices = patch.getVertices();
    for (int i = 0 ; i < 3; i++) {
        glVertex3f(vertices[i].getX()/15, vertices[i].getY()/15, vertices[i].getZ()/15);
    }
}


void reshape(int x, int y)
{
    if (y == 0 || x == 0) return;  //Nothing is visible then, so return
    
    //Set a new projection matrix
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    //Angle of view:40 degrees
    //Near clipping plane distance: 0.5
    //Far clipping plane distance: 20.0
    gluPerspective(40.0,(GLdouble)x/(GLdouble)y,0.5,20.0);
    
    glMatrixMode(GL_MODELVIEW);
    glViewport(0,0,x,y);  //Use the whole window for rendering
}

void Display(void)
{
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();
    
    Camera.Render();
    
    //Draw the "world":
    
    glPushMatrix();

    for (int i = 0 ; i < polygons - 1; i++) {
//        if ((i > 500 && i < 510)) {
//            glBegin(GL_TRIANGLE_STRIP);
//        } else {
//            glBegin(GL_LINE_LOOP);
//        }
        glBegin(GL_LINE_LOOP);
//        glColor3f(flux[i], flux[i], flux[i]);
        drawPatch(patches[i],flux[i]);
        glEnd();
    }

    glPopMatrix();
    
    //finish rendering:
    glFlush();  
    glutSwapBuffers();
    
}

void KeyDown(unsigned char key, int x, int y)
{
    switch (key)
    {
        case 27:		//ESC
            exit(0);
            break;
        case 'a':
            Camera.RotateY(5.0);
            Display();
            break;
        case 'd':
            Camera.RotateY(-5.0);
            Display();
            break;
        case 'w':
            Camera.MoveForward( -0.1 ) ;
            Display();
            break;
        case 's':
            Camera.MoveForward( 0.1 ) ;
            Display();
            break;
        case 'x':
            Camera.RotateX(5.0);
            Display();
            break;
        case 'y':
            Camera.RotateX(-5.0);
            Display();
            break;
        case 'c':
            Camera.StrafeRight(-0.1);
            Display();
            break;
        case 'v':
            Camera.StrafeRight(0.1);
            Display();
            break;
        case 'f':
            Camera.MoveUpward(-0.3);
            Display();
            break;
        case 'r':
            Camera.MoveUpward(0.3);
            Display();
            break;
            
        case 'm':
            Camera.RotateZ(-5.0);
            Display();
            break;
        case 'n':
            Camera.RotateZ(5.0);
            Display();
            break;
        case 'p':
            cout << "Position:";
            Camera.getPosition().print();
            cout << "\nRight Vector";
            Camera.getRightVector().print();
            cout << "\nUp Vector";
            Camera.getUpVector().print();
            cout << "\nView direction";
            Camera.getViewDir().print();
            break;
            
    }
}

int render(int *argcp, char **argv) {

    
    glutInit(argcp, argv);
    glutInitWindowSize(2880, 1800);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
    glutCreateWindow("Camera");
    Camera.Move( Vector(0.43,0.79,2.9 ));
//    Camera.MoveForward( 1.0 );
    glutReshapeFunc(reshape);
    glutDisplayFunc(Display);
	glutKeyboardFunc(KeyDown);
    glutMainLoop();
    return 0;
}
#endif
