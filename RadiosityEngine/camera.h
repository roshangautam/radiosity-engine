#include <GLUT/glut.h>
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <stdio.h>
#include "vector.h"
#define PI 3.1415926535897932384626433832795
#define PIdiv180 (PI/180.0)

class CCamera
{
private:

	Vector ViewDir;
	Vector RightVector;
	Vector UpVector;
	Vector Position;

	GLfloat RotatedX, RotatedY, RotatedZ;	
	
public:
	CCamera();				//inits the values (Position: (0|0|0) Target: (0|0|-1) )
	void Render ( void );	//executes some glRotates and a glTranslate command
							//Note: call glLoadIdentity before using Render
    Vector getPosition() { return Position; };
    Vector getRightVector() { return RightVector; };
    Vector getUpVector() { return UpVector; };
    Vector getViewDir() { return ViewDir; };
    
	void Move ( Vector Direction );
	void RotateX ( GLfloat Angle );
	void RotateY ( GLfloat Angle );
	void RotateZ ( GLfloat Angle );

	void MoveForward ( GLfloat Distance );
	void MoveUpward ( GLfloat Distance );
	void StrafeRight ( GLfloat Distance );


};


