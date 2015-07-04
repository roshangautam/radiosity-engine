//
//  main.cpp
//  LineIntersection
//
//  Created by Roshan Gautam on 6/1/15.
//  Copyright (c) 2015 Roshan Gautam. All rights reserved.
//

#include <iostream>
#include <string>
#include <fstream>
#include <istream>
#include <sstream>
#include <vector>
#include <time.h>
#include "patch.h"
#include "render.h"

using namespace std;

//number of faces on a hemicube
enum Face {TOP_FACE, LEFT_FACE, RIGHT_FACE, FRONT_FACE, BACK_FACE};
int POLY_COUNT = 10000; // we are shooting for these many patches in each object
int n = 8; //resolution of hemicube - default is 8
int cells =  n * ceilf((float)n/2) * 6; // no of cells in each hemicube
Vector *centers = new Vector[cells]; // array of 3dpoints to hold center of each hemicube cell
GLfloat *delA = new GLfloat[cells]; // array of floating point values to hold delA of each hemicube cell
GLfloat **formFactors;
float startX = 0;
float startY = 0;
float startZ = 0;

//Main Program Loop
void loop(int *, char **);
void loadObjectFile();

//function to generate triangles
void generatePolygons(double, double, double, string);

//Functions to calculate form factors
void generateHemicubeCellCenters(bool print);
void determineCellOwnership();
Vector calculateNormal(Vector[]);
bool pointInTriangle(Vector, Vector, Vector, Vector);
Patch transformPatch(Patch, Patch);
void calculateFormFactors();

//function to solve radiosity equation
void progressiveRadiosity();

//It all starts here - behold
int main(int argc,char **argv) {
    cout << "Radiosity Engine - Solid Modeling CS6413 !!! \n";
    loadObjectFile();
    loop(&argc, argv);
    return 0;
}

//load vertices of patches from the generated object file
void loadObjectFile() {
    ifstream objectFile("room.obj");
    if (objectFile.is_open()) {
        std::istream_iterator<double> start(objectFile), end;
        std::vector<double> lines(start, end);
        
        Vector *vertices = new Vector[lines.size()];
        Patch *loadedPatches = new Patch[lines.size()/9];
        for (int i = 0, j = 0 ; j < lines.size()/3 ; i+=3,j++) {
            vertices[j].setCoordinates(lines.at(i), lines.at(i+1), lines.at(i+2));
        }
        int polyCount = 0;
        int i = 0, j = 0;
        for (i = 0, j = 0; j < lines.size()/9; i+=3, j++) {
            loadedPatches[j].setVertices(vertices[i], vertices[i+1], vertices[i+2]);
            polyCount++;
        }
        polygons = polyCount;
        patches = loadedPatches;
    } else {
        cout << "\nMissing object file. Please generate triangles first";
    }

    formFactors = new GLfloat*[polygons];
    flux = new GLfloat[polygons];
    for(int i = 0; i < polygons; ++i) {
        formFactors[i] = new GLfloat[polygons];
    }

}

//main program loop
void loop(int *argcp, char **argv) {
    double length = 0.0, width = 0.0, height = 0.0; // dimensions of the room
    char c = '1';
    while(1) {
        cout << "\nOptions:\n";
        cout << "(1)Calculate Hemicube cell centers and DelA for each cell\n";
        cout << "(2)Generate triangles\n";
        cout << "(3)Draw Mesh\n";
        cout << "(4)Calculate form factors, Solve Radiosity and Render\n";
        cout << "(Q)Quit\n";
        cout << "[Select]:";
        cin >> c;
        if(cin.get() != '\n') { // Look at the next character
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Clear the input stream
            cout << "Invalid Option\n";
        } else {
            switch (c) {
                case '1':
                {
                    cout << "Enter the resolution for hemicube:";
                    cin >> n;
                    generateHemicubeCellCenters(true); // generate hemicube cell centers and print DelA for each cell
                }
                    break;
                case '2':
                {
                    cout << "Enter the height of the room:";
                    cin >> height;
                    cout << "Enter the width of the room:";
                    cin >> width;
                    cout << "Enter the length of the room:";
                    cin >> length;
                    generatePolygons(height, width, length, "room.obj");
                    loadObjectFile();
                }
                    break;
                case '3':
                {
                    render(argcp, argv);
                }
                    break;
                case '4':
                    determineCellOwnership();
                    calculateFormFactors();
                    progressiveRadiosity();
                    render(argcp, argv);
                    break;
                case 'q':
                case 'Q':
                    cout << "\n--- End of Program ---\n";
                    exit(1);
                default:
                    cout << "\nInvalid option\n";
                    break;
            }
        }
    }
}

//this function generates the cell centers for a hemicube of n resolution. The function also calculates delA for each cell as well which will be used to calculate formfactors later
void generateHemicubeCellCenters(bool print) {
    cells =  n * ceilf((float)n/2) * 6;
    centers = new Vector[cells];
    delA = new GLfloat[cells];
    
    Vector** top_buffer = new Vector*[n];
    Vector** front_buffer = new Vector*[n];
    Vector** back_buffer = new Vector*[n];
    Vector** left_buffer = new Vector*[n];
    Vector** right_buffer = new Vector*[n];
    
    for(int i = 0; i < n; ++i) {
        top_buffer[i] = new Vector[n];
        front_buffer[i] = new Vector[n];
        back_buffer[i] = new Vector[n];
        left_buffer[i] = new Vector[n];
        right_buffer[i] = new Vector[n];
    }

    GLfloat checkSum = 0.0;
    int counter = 0;
    int startCounter = 0;
    float delta;
    delta = 2 / (float)n;
    float x, y, z;
    for ( int i = 0 ; i < 5 ; i++) { // for five faces of hemicube
        switch (i) {
            case TOP_FACE:
            {
                x = -1;
                y = 1;
                z = -1;
                startCounter = counter;
                for (int j = 0; j < n; j++) {
                    for (int k=0; k < n; k++) {
                        top_buffer[j][k] = *new Vector(((x+delta) + x) / 2, y, ((z+delta) + z)/2);
                        centers[counter] = top_buffer[j][k];
                        delA[counter] = (delta * delta) / ( PI * centers[counter].getWholeSquare() );
                        counter++;
                        x += delta;
                    }
                    x = -1;
                    z += delta;
                }
                if (print) {
                    cout << "\nTOP FACE\n";
                    for (int l = startCounter; l < counter; l++) {
                        if (l > startCounter && l % n == 0) {
                            cout << "\n\n\n\n\n\n";
                        }
                        cout << setw(10) << setprecision(3) << delA[l];
                        checkSum += delA[l];
                    }
                }
            }
                break;
            case LEFT_FACE:
                x = -1;
                y = 1;
                z = 1;
                startCounter = counter;
                for (int j = 0; j < n/2; j++) {
                    for (int k= 0; k < n; k++) {
                        left_buffer[j][k] = *new Vector(x, ((y-delta) + y) / 2, ((z - delta) + z) / 2);
                        centers[counter] = left_buffer[j][k];
                        delA[counter] = ( centers[counter].getY() * delta * delta ) / (PI * centers[counter].getWholeSquare());
                        counter++;
                        z -= delta;
                    }
                    z = 1;
                    y -= delta;
                }
                if (print) {
                    cout << "\n\nLeft FACE\n\n";
                    for (int l = startCounter; l < counter; l++) {
                        if (l > startCounter && l % n == 0) {
                            cout << "\n\n";
                        }
                        cout << setw(10) << setprecision(3) << delA[l];
                        checkSum += delA[l];
                    }
                }
                break;
            case RIGHT_FACE:
                x = 1;
                y = 1;
                z = 1;
                startCounter = counter;
                for (int j = 0; j < n/2; j++) {
                    for (int k= 0; k < n; k++) {
                        right_buffer[j][k] = *new Vector(x, ((y-delta) + y) / 2, ((z - delta) + z) / 2);
                        centers[counter] = right_buffer[j][k];
                        delA[counter] = ( centers[counter].getY() * delta * delta ) / (PI * centers[counter].getWholeSquare());
                        counter++;
                        z -= delta;
                    }
                    z = 1;
                    y -= delta;
                }
                if (print) {
                    cout << "\n\nRight FACE\n\n";
                    for (int l = startCounter; l < counter; l++) {
                        if (l > startCounter && l % n == 0) {
                            cout << "\n\n";
                        }
                        cout << setw(10) << setprecision(3) << delA[l];
                        checkSum += delA[l];
                    }
                }
                break;
            case FRONT_FACE:
            {
                x = -1;
                y = 1;
                z = 1;
                startCounter = counter;
                for (int j = 0; j < n/2; j++) {
                    for (int k= 0; k < n; k++) {
                        front_buffer[j][k] = *new Vector(((x+delta) + x) / 2, ((y-delta) + y) / 2, z);
                        centers[counter] = front_buffer[j][k];
                        delA[counter] = ( centers[counter].getY() * delta * delta ) / (PI * centers[counter].getWholeSquare());
                        counter++;
                        x += delta;
                    }
                    x = -1;
                    y -= delta;
                }
                if (print) {
                    cout << "\n\nFront FACE\n\n";
                    for (int l = startCounter; l < counter; l++) {
                        if (l > startCounter && l % n == 0) {
                            cout << "\n\n";
                        }
                        cout << setw(10) << setprecision(3) << delA[l];
                        checkSum += delA[l];
                    }
                }
            }
                break;
            case BACK_FACE:
                x = -1;
                y = 1;
                z = -1;
                startCounter = counter;
                for (int j = 0; j < n/2; j++) {
                    for (int k= 0; k < n; k++) {
                        back_buffer[j][k] = *new Vector(((x+delta) + x) / 2, ((y-delta) + y) / 2, z);
                        centers[counter] = back_buffer[j][k];
                        delA[counter] = ( centers[counter].getY() * delta * delta ) / (PI * centers[counter].getWholeSquare());
                        counter++;
                        x += delta;
                    }
                    x = -1;
                    y -= delta;
                }
                if (print) {
                    cout << "\n\nBack FACE\n\n";
                    for (int l = startCounter; l < counter; l++) {
                        if (l > startCounter && l % n == 0) {
                            cout << "\n\n";
                        }
                        cout << setw(10) << setprecision(3) << delA[l];
                        checkSum += delA[l];
                    }
                }
                break;
            default:
                break;
        }
    }
    if (print) {
         cout << "\n" << checkSum << "\n";
    }
}

//this function generates the triangles(patches) for the given object of length, width and height
void generatePolygons(double width, double height, double length, string filename) {
    // WE ASSUME THE BOTTOM LEFT CORNER OF THE ROOM IS (0,0,0)
    ofstream objectFile;
    objectFile.open (filename);
    double factor = (4 * (( width * height) + (length * height) + (width * length))) / POLY_COUNT ;
    double DELTA = floor(sqrt(factor) * 100) / 100;
    cout << DELTA;
    if (objectFile) {
        cout << "File Generated. Now generating polygons\n";
        // FRONT AND BACK
        int i = 1; //polycount
        for (float z = startZ; z <= length; z += length) {
            for (float y = startY; y <= height; y += DELTA) {
                for (float x = startX; x <= width; x+= DELTA) {
                    objectFile << x << " " << y << " " << z << "\n";
                    objectFile << x + DELTA << " " << y << " " << z << "\n";
                    objectFile << x << " " << y + DELTA << " " << z << "\n";
                    i++;
                    objectFile << x + DELTA << " " << y + DELTA << " " << z << "\n";
                    objectFile << x << " " << y + DELTA << " " << z << "\n";
                    objectFile << x + DELTA << " " << y << " " << z << "\n";
                    i++;
                }
            }
        }
        // LEFT AND RIGHT
        for (float x = startX; x <= width; x += width) {
            for (float y = startY; y <= height; y += DELTA) {
                for (float z = startZ; z <= length; z+= DELTA) {
                    objectFile << x << " " << y << " " << z << "\n";
                    objectFile << x << " " << y + DELTA << " " << z << "\n";
                    objectFile << x << " " << y << " " << z + DELTA << "\n";
                    i++;
                    objectFile << x << " " << y + DELTA << " " << z + DELTA << "\n";
                    objectFile << x << " " << y << " " << z + DELTA << "\n";
                    objectFile << x << " " << y + DELTA << " " << z << "\n";
                    i++;
                }
            }
        }
        // TOP AND BOTTTOM
        for (float y = startY; y <= height; y += height) {
            for (float x = startX; x <= width; x += DELTA) {
                for (float z = startZ; z <= length; z+= DELTA) {
                    objectFile << x << " " << y << " " << z << "\n";
                    objectFile << x << " " << y << " " << z + DELTA << "\n";
                    objectFile << x + DELTA  << " " << y << " " << z << "\n";
                    i++;
                    objectFile << x + DELTA << " " << y << " " << z + DELTA << "\n";
                    objectFile << x + DELTA << " " << y << " " << z << "\n";
                    objectFile << x << " " << y << " " << z + DELTA << "\n";
                    i++;
                }
            }
        }
        
        startX = 0;
        startY = height / 2.0;
        startZ = length / 2.0 ;
        int origLength = length;
        int origHeight = height;
        length = 2.0;
        width = 2.0;
        height = 2.0;
        factor = (4 * (( width * height) + (length * height) + (width * length))) / (POLY_COUNT/2) ;
        DELTA = floor(sqrt(factor) * 100) / 100;
        // FRONT AND BACK
        for (float z = startZ; z <= (startZ + length); z += length) {
            for (float y = startY; y < (startY + height); y += DELTA) {
                for (float x = startX; x < ( startX + width); x+= DELTA) {
                    objectFile << x << " " << y << " " << z << "\n";
                    objectFile << x + DELTA << " " << y << " " << z << "\n";
                    objectFile << x << " " << y + DELTA << " " << z << "\n";
                    i++;
                    objectFile << x + DELTA << " " << y + DELTA << " " << z << "\n";
                    objectFile << x << " " << y + DELTA << " " << z << "\n";
                    objectFile << x + DELTA << " " << y << " " << z << "\n";
                    i++;
                }
            }
        }
        // LEFT AND RIGHT
        for (float x = startX; x <= ( startX + width); x += width) {
            for (float y = startY; y < (startY + height); y += DELTA) {
                for (float z = startZ; z < (startZ + length); z+= DELTA) {
                    objectFile << x << " " << y << " " << z << "\n";
                    objectFile << x << " " << y + DELTA << " " << z << "\n";
                    objectFile << x << " " << y << " " << z + DELTA << "\n";
                    i++;
                    objectFile << x << " " << y + DELTA << " " << z + DELTA << "\n";
                    objectFile << x << " " << y << " " << z + DELTA << "\n";
                    objectFile << x << " " << y + DELTA << " " << z << "\n";
                    i++;
                }
            }
        }
        // TOP AND BOTTTOM
        for (float y = startY; y <= (startY + height); y += height) {
            for (float x = startX; x < ( startX + width); x += DELTA) {
                for (float z = startZ; z < (startZ + length); z+= DELTA) {
                    objectFile << x << " " << y << " " << z << "\n";
                    objectFile << x << " " << y << " " << z + DELTA << "\n";
                    objectFile << x + DELTA  << " " << y << " " << z << "\n";
                    i++;
                    objectFile << x + DELTA << " " << y << " " << z + DELTA << "\n";
                    objectFile << x + DELTA << " " << y << " " << z << "\n";
                    objectFile << x << " " << y << " " << z + DELTA << "\n";
                    i++;
                }
            }
        }

        startX = 0;
        startY = (origHeight / 2.0) - 3.0;
        startZ = (origLength / 2.0);
        length = 2.0;
        width = 6.0;
        height = 2.0;
        factor = (4 * (( width * height) + (length * height) + (width * length))) / (POLY_COUNT/2) ;
        DELTA = floor(sqrt(factor) * 100) / 100;
        // FRONT AND BACK
        for (float z = startZ; z <= (startZ + length); z += length) {
            for (float y = startY; y < (startY + height); y += DELTA) {
                for (float x = startX; x < ( startX + width); x+= DELTA) {
                    objectFile << x << " " << y << " " << z << "\n";
                    objectFile << x + DELTA << " " << y << " " << z << "\n";
                    objectFile << x << " " << y + DELTA << " " << z << "\n";
                    i++;
                    objectFile << x + DELTA << " " << y + DELTA << " " << z << "\n";
                    objectFile << x << " " << y + DELTA << " " << z << "\n";
                    objectFile << x + DELTA << " " << y << " " << z << "\n";
                    i++;
                }
            }
        }
        // LEFT AND RIGHT
        for (float x = startX; x <= ( startX + width); x += width) {
            for (float y = startY; y < (startY + height); y += DELTA) {
                for (float z = startZ; z < (startZ + length); z+= DELTA) {
                    objectFile << x << " " << y << " " << z << "\n";
                    objectFile << x << " " << y + DELTA << " " << z << "\n";
                    objectFile << x << " " << y << " " << z + DELTA << "\n";
                    i++;
                    objectFile << x << " " << y + DELTA << " " << z + DELTA << "\n";
                    objectFile << x << " " << y << " " << z + DELTA << "\n";
                    objectFile << x << " " << y + DELTA << " " << z << "\n";
                    i++;
                }
            }
        }
        // TOP AND BOTTTOM
        for (float y = startY; y <= (startY + height); y += height) {
            for (float x = startX; x < ( startX + width); x += DELTA) {
                for (float z = startZ; z < (startZ + length); z+= DELTA) {
                    objectFile << x << " " << y << " " << z << "\n";
                    objectFile << x << " " << y << " " << z + DELTA << "\n";
                    objectFile << x + DELTA  << " " << y << " " << z << "\n";
                    i++;
                    objectFile << x + DELTA << " " << y << " " << z + DELTA << "\n";
                    objectFile << x + DELTA << " " << y << " " << z << "\n";
                    objectFile << x << " " << y << " " << z + DELTA << "\n";
                    i++;
                }
            }
        }
        
        cout << "Polygon count:" << i << " for " << filename << "\n";
        objectFile.close();
        cout << "Done Generating Polygons\n";
    } else {
        cout << "Error Generating object file. Try again\n";
    }
    
}


//this function determines the ownership of each cell of a patch by another patch
void determineCellOwnership() {
    generateHemicubeCellCenters(false);
    float thmin;
    Patch closestPatch;
    int patchIndex = 0;
    for (int j = 0; j < polygons; j++) {
        for (int i = 0; i < cells; i++) {
            thmin = 9999999;
            for (int k = j+1; k < polygons - 1; k++) {
                Patch transformed = transformPatch(patches[j],patches[k]);
                Vector normal = calculateNormal(transformed.getVertices());
                float d = normal.dot(transformed.getVertices()[0]);
                float dot = normal.dot(centers[i]);
                if (dot != 0) {
                    float t = d / dot;
                    if (t > 0 &&
                        t < thmin) {
                        Vector ph;
                        ph.setCoordinates(t * centers[i].getX(), t * centers[i].getY(), t * centers[i].getZ());
                        if (pointInTriangle(ph, transformed.getVertices()[0], transformed.getVertices()[1], transformed.getVertices()[2])) {
                            thmin = t;
                            closestPatch = patches[k];
                            patchIndex = k;
                        }
                    }
                }
            }
            patches[j].setCellData(i, thmin, &closestPatch, patchIndex);
        }
    }
}

//transform the subject patch to the coordinates of current patch
Patch transformPatch(Patch current, Patch subject) {
    current.calcCenter();
    current.calcVectors();
    Vector *vectors = current.getVectors();
    Vector *vertices = subject.getVertices();
    Patch transformed;
    Vector transformedVertices[3];
    for (int i = 0; i < 3; i++) {
        Vector vertex = vertices[i] - current.getCenter();
        float x = vectors[0].dot(vertex);
        float y = vectors[1].dot(vertex);
        float z = vectors[2].dot(vertex);
        transformedVertices[i].setCoordinates(x, y, z);
    }
    transformed.setVertices(transformedVertices[0], transformedVertices[1], transformedVertices[2]);
    return transformed;
}

//calcualte the normal vector of three given points
Vector calculateNormal(Vector givenPoints[]) {
    Vector a = givenPoints[1] - givenPoints[0];
    Vector b = givenPoints[2] - givenPoints[1];
    Vector normal = a.cross(b);
    return normal;
}

//following function determine if the point ph lies inside the traingle defined by v1, v2 and v3
bool pointInTriangle(Vector ph, Vector v1, Vector v2, Vector v3) {

    Vector v21 = v2 - v1;
    Vector vh1 = ph - v1;
    Vector cross1 = v21.cross(vh1);
    
    Vector v32 = v3 - v2;
    Vector vh2 = ph - v2;
    Vector cross2 = v32.cross(vh2);
    
    Vector v13 = v1 - v3;
    Vector vh3 = ph - v3;
    Vector cross3 = v13.cross(vh3);
    
    if (cross1.dot(cross2) >= 0 &&
        cross1.dot(cross3) >= 0 &&
        cross2.dot(cross3) >= 0) {
        return true;
    }
    
    return false;
}

void calculateFormFactors() {
    for (int i = 0; i < polygons; i++) {
        for (int j = i + 1; j < polygons - 1; j++) {
            float sum = 0.0;
            for (int k = 0; k < cells; k++) {
                if(patches[i].getCellData(k).getPatchIndex() == j) {
                    sum += delA[k];
                }
            }
            formFactors[i][j] = sum;
        }
    }

}

void progressiveRadiosity() {
    GLfloat unShot[polygons];
    GLfloat delta[polygons];
    float maxFlux = 1000.0;
    for (int i = 0 ; i < polygons; i++) {
        if ((i > 500 && i < 510)) {
            unShot[i] = delta[i] = flux[i] = 1000.0;  // original light sources
        } else {
            unShot[i] = delta[i] = flux[i] = 0.0;
        }
    }
    for (int k = 0 ; k < 16; k++) {
        for (int i = 0; i < polygons; i++) {
            if (unShot[i] > 0.0) {
                if(patches[i].getArea() == -1.0)
                    patches[i].calcArea();
                for (int j = i ; j < polygons; j++) {
                    if(patches[j].getArea() == -1.0)
                        patches[j].calcArea();
                    delta[j] = 0.75 * formFactors[i][j] * unShot[i] * (patches[i].getArea()/patches[j].getArea());
                    flux[j] += delta[j];
                    if(flux[j] > maxFlux) {
                        maxFlux = flux[j];
                    }
                    unShot[j] += delta[j];
                }
            }
        }
    }
    
    for (int i = 0 ; i < polygons; i++) {
        flux[i] = (flux[i] / maxFlux) * 255;
        cout << flux[i] << "\n";
    }
}