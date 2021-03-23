//
// Created by Jacob Salvi on 23.03.21.
//

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
           float t = sin(M_PI*v);
           float x1 = t*cos(2*M_PI*u);
           float z1 = t*sin(2*M_PI*u);
           float y1 = cos(M_PI*v);

           float x4 = t*cos(2*M_PI*(u+step));
           float z4 = t*sin(2*M_PI*(u+step));
           float y4 = cos(M_PI*v);

           t = sin(M_PI*(v+step));
           float x2 = t*cos(2*M_PI*u);
           float z2 = t*sin(2*M_PI*u);
           float y2 = cos(M_PI*(v+step));

           float x3 = t*cos(2*M_PI*(u+step));
           float z3 = t*sin(2*M_PI*(u+step));
           float y3 = cos(M_PI*(v+step));

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

   for(int i=0; i<6;++i){
       std::cout<<"colour is: "<<colorBuffer[i]<<std::endl;
   }
}

int sphere::getSize() {
   return sizeof(float)*size*size*18;
}

int sphere::getNumberOfVertices() {
    return size*size*18;
}

