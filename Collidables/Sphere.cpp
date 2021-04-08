#include "Sphere.h"
#include <cmath>
#include <iostream>

//number of vertices:
//size^2*18
sphere::sphere(int size, glm::mat4 model, glm::vec3 colour) : collidable(colour, model), size(size){
   vertexBuffer = new float[size*size*18];
   colorBuffer = new float [size*size*18];

   int vertPos =0;
   int colPos = 0;
   float step = (float)1/size;
   float u = 0.0f;
   for(int i =0; i<size; ++i){
       float v = 0.0f;
       for(int j=0; j<size; ++j){
           double t = sin(M_PI*v);
           double x1 = t*cos(2*M_PI*u);
           double z1 = t*sin(2*M_PI*u);
           double y1 = cos(M_PI*v);

           double x4 = t*cos(2*M_PI*(u+step));
           double z4 = t*sin(2*M_PI*(u+step));
           double y4 = cos(M_PI*v);

           t = sin(M_PI*(v+step));
           double x2 = t*cos(2*M_PI*u);
           double z2 = t*sin(2*M_PI*u);
           double y2 = cos(M_PI*(v+step));

           double x3 = t*cos(2*M_PI*(u+step));
           double z3 = t*sin(2*M_PI*(u+step));
           double y3 = cos(M_PI*(v+step));

           vertexBuffer[vertPos++]=x1;
           vertexBuffer[vertPos++]=y1;
           vertexBuffer[vertPos++]=z1;

           vertexBuffer[vertPos++]=x3;
           vertexBuffer[vertPos++]=y3;
           vertexBuffer[vertPos++]=z3;

           vertexBuffer[vertPos++]=x2;
           vertexBuffer[vertPos++]=y2;
           vertexBuffer[vertPos++]=z2;

           vertexBuffer[vertPos++]=x1;
           vertexBuffer[vertPos++]=y1;
           vertexBuffer[vertPos++]=z1;

           vertexBuffer[vertPos++]=x4;
           vertexBuffer[vertPos++]=y4;
           vertexBuffer[vertPos++]=z4;

           vertexBuffer[vertPos++]=x3;
           vertexBuffer[vertPos++]=y3;
           vertexBuffer[vertPos++]=z3;

           for(int k = 0; k < 6; k++){
               colorBuffer[colPos++]=colour[0];
               colorBuffer[colPos++]=colour[1];
               colorBuffer[colPos++]=colour[2];
           }
           v+=step;
       }
       u+=step;
   }

   //unit sphere
   for(int i=0; i<size*size*18;++i){
       vertexBuffer[i]=vertexBuffer[i]/2.0f;
   }

   setGLuint();
}

int sphere::getSize() {
   return sizeof(float)*size*size*18;
}

int sphere::getNumberOfVertices() {
    return size*size*18;
}

