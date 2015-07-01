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

#include "patch.h"
#include "render.h"

using namespace std;

enum Face {TOP_FACE, LEFT_FACE, RIGHT_FACE, FRONT_FACE, BACK_FACE, NA};

int POLY_COUNT = 10000;
int n = 8; //no of cells in hemicube - default is 8
int cells =  n * ceilf((float)n/2) * 6;
Vector *centers = new Vector[cells];
GLfloat *delA = new GLfloat[cells];


//Main Program Loop
void loop(int *, char **);
void loadObjectFile();

//Functions to calculate form factors
void generateHemicubeCellCenters(bool print);
void determineCellOwnership();
Vector calculateNormal(Vector[]);
bool pointInTriangle(Vector, Vector, Vector, Vector);
Patch transformPatch(Patch, Patch);

//function to generate triangles
void generatePolygons(double, double, double, string);

int main(int argc,char **argv) {
    cout << "Radiosity Engine - Solid Modeling CS6413 !!! \n";
    loadObjectFile();
    loop(&argc, argv);
    return 0;
}

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
        noOfPolygons = polyCount;
        patches = loadedPatches;
    } else {
        cout << "\nMissing object file. Please generate triangles first";
    }
}

void loop(int *argcp, char **argv) {
    double length = 0.0, width = 0.0, height = 0.0; // dimensions of the room
    char c = '1';
    while(1) {
        cout << "\nOptions:\n";
        cout << "(1)Calculate Hemicube cell centers and DelA for each cell\n";
        cout << "(2)Generate triangles\n";
        cout << "(3)Draw Mesh\n";
        cout << "(4)Calculate form factors\n";
        cout << "(5)Calculate Radiosity Equation";
        cout << "(6)Render\n";
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

Patch transformPatch(Patch current, Patch subject) {
    current.calcCenter();
    current.calcVectors();
    Vector *vectors = current.getVectors();
    Vector *vertices = subject.getVertices();
    Patch transformed;
    Vector transformedVertices[3];
    for (int i =0; i<3; i++) {
        Vector vertex = vertices[i] - current.getCenter();
        float x = vectors[0].dot(vertex);
        float y = vectors[1].dot(vertex);
        float z = vectors[2].dot(vertex);
        transformedVertices[i].setCoordinates(x, y, z);
    }
    transformed.setVertices(transformedVertices[0], transformedVertices[1], transformedVertices[2]);
    return transformed;
}

void determineCellOwnership() {
    generateHemicubeCellCenters(false);
    float thmin;
    Patch closestPatch;
    for (int j = 0; j < noOfPolygons; j++) {
        for (int i = 0; i < cells; i++) {
            thmin = 9999999;
            for (int k = j+1; k < noOfPolygons - 1; k++) {
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
                        }
                    }
                }
            }
            patches[j].setCellData(i, thmin, &closestPatch);
        }
    }
}

void generateHemicubeCellCenters(bool print) {
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
                            cout << "\n\n";
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
                    cout << "\nLeft FACE\n";
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
                    cout << "\nRight FACE\n";
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
                    cout << "\nFront FACE\n";
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
                    cout << "\nBack FACE\n";
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


void generatePolygons(double width, double height, double length, string filename) {
    // WE ASSUME THE BOTTOM LEFT CORNER OF THE ROOM IS (0,0,0)
    ofstream objectFile;
    objectFile.open (filename);
    double factor = (4 * (( width * height) + (length * height) + (width * length))) / POLY_COUNT ;
    double DELTA = floor(sqrt(factor) * 100) / 100;
    if (objectFile) {
        cout << "File Generated. Now generating polygons\n";
        // FRONT AND BACK
        int i = 1; //polycount
        for (float z = 0; z <= length; z += length) {
            for (float y = 0; y < height; y += DELTA) {
                for (float x =0; x < width; x+= DELTA) {
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
        for (float x = 0; x <= width; x += width) {
            for (float y = 0; y < height; y += DELTA) {
                for (float z =0; z < length; z+= DELTA) {
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
        for (float y = 0; y <= height; y += height) {
            for (float x = 0; x < width; x += DELTA) {
                for (float z =0; z < length; z+= DELTA) {
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

Vector calculateNormal(Vector givenPoints[]) {
    Vector a = givenPoints[1] - givenPoints[0];
    Vector b = givenPoints[2] - givenPoints[1];
    Vector normal = a.cross(b);
    return normal;
}


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