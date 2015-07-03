#include "camera.h"
#include "math.h"
#include <iostream>

CCamera::CCamera()
{
    //    Position:(0.43,0.79,2.9)
    //    Right Vector(-0.0093,-1,-0.031)
    //    Up Vector(1,-0.0077,-0.051)
    //    View direction(-0.05,0.031,-1)
	//Init with standard OGL values:
	Position = Vector (0.0, 0.0, 0.0);
	ViewDir = Vector(-0.05,0.031,-1);
	RightVector = Vector (-0.0093,-1,-0.031);
	UpVector = Vector (1,-0.0077,-0.051);

	//Only to be sure:
	RotatedX = RotatedY = RotatedZ = 0.0;
}

void CCamera::Move (Vector Direction)
{
	Position = Position + Direction;
}

void CCamera::RotateX (GLfloat Angle)
{
	RotatedX += Angle;
	
	//Rotate viewdir around the right vector:
    ViewDir = ViewDir*cos(Angle*PIdiv180) + UpVector*sin(Angle*PIdiv180);
    ViewDir.normalizeVector();

	//now compute the new UpVector (by cross product)
    UpVector = ViewDir.cross(RightVector); //CrossProduct(&ViewDir, &RightVector)*-1;
    UpVector = UpVector*-1;

}

void CCamera::RotateY (GLfloat Angle)
{
	RotatedY += Angle;
	
	//Rotate viewdir around the up vector:
    ViewDir = ViewDir*cos(Angle*PIdiv180) - RightVector*sin(Angle*PIdiv180);
    ViewDir.normalizeVector();

	//now compute the new RightVector (by cross product)
    RightVector = ViewDir.cross(UpVector); //CrossProduct(&ViewDir, &UpVector);
}

void CCamera::RotateZ (GLfloat Angle)
{
	RotatedZ += Angle;
	
	//Rotate viewdir around the right vector:
    RightVector = RightVector*cos(Angle*PIdiv180) + UpVector*sin(Angle*PIdiv180);//Normalize3dVector();
    RightVector.normalizeVector();

	//now compute the new UpVector (by cross product)
    UpVector = ViewDir.cross(RightVector); //CrossProduct(&ViewDir, &RightVector)*-1;
}

void CCamera::Render( void )
{

	//The point at which the camera looks:
	Vector ViewPoint = Position+ViewDir;

	//as we know the up vector, we can easily use gluLookAt:
	gluLookAt(	Position.getX(), Position.getY(), Position.getZ(),
				ViewPoint.getX(), ViewPoint.getY(), ViewPoint.getZ(),
				UpVector.getX(), UpVector.getY(), UpVector.getZ());

}

void CCamera::MoveForward( GLfloat Distance )
{
	Position = Position + (ViewDir*-Distance);
}

void CCamera::StrafeRight ( GLfloat Distance )
{
	Position = Position + (RightVector*Distance);
}

void CCamera::MoveUpward( GLfloat Distance )
{
	Position = Position + (UpVector*Distance);
}