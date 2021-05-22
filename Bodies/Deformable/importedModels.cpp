#include "importedModels.h"

importedModels::importedModels(const char * path, glm::mat4 mod) : modelMatrix(mod){
    std::cout<<"loading object "<<path<<std::endl;
    //needed variables to store the object information
    std::vector<glm::vec3> verts;
    std::vector<glm::vec3> normals;
    std::vector<glm::vec2> uvs;
    std::vector<int> vert_ids;
    std::vector<glm::ivec3> indices;

    std::fstream file;
    file.open(path, std::ios::in);
    if(!file.is_open()){
        std::cout<<"File couldn't be loaded"<<std::endl;
        return;
    }

    //the file opened successfully
    //read the content
    std::string line;
    //read file line by line
    while(getline(file, line)){
        //std::cout<<line<<std::endl;
        std::string s;
        std::istringstream ss(line);
        std::vector<std::string> arguments;
        //divide line based on spaces
        while(std::getline(ss, s, ' ')){
            arguments.push_back(s);
        }
        // what kind of information are we reading?
        // i am not going to make an enumerator to
        // switch on strings
        if(arguments[0]=="v"){
            //push vert information in the array
            verts.push_back(glm::vec3(std::stof(arguments[1]),
                                      std::stof(arguments[2]),
                                      std::stof(arguments[3])));
        }else if(arguments[0]=="vn"){
            normals.push_back(glm::vec3(std::stof(arguments[1]),
                                        std::stof(arguments[2]),
                                        std::stof(arguments[3])));
        }else if(arguments[0]=="vt"){
            uvs.push_back(glm::vec2(std::stof(arguments[1]),
                                    1.0f-std::stof(arguments[2])));
        }else if(arguments[0]=="f"){
            for(int j = 1; j<=3;++j){
                std::vector<int> index;
                std::string tmp;
                std::istringstream iStream(arguments[j]);
                //divide line based on "/"
                while(std::getline(iStream, tmp, '/')){
                    index.push_back(std::stoi(tmp));
                }
                // need to adjust since we start from 0
                indices.push_back(glm::ivec3(index[0]-1,
                                             index[1]-1,
                                             index[2]-1));
            }
            //if mesh is composed of quads I will probably cry
            if(arguments.size()==5){
                for(int j =3; j>=3 && j<=5;++j){
                    int it = j;
                    if(it==5){
                        it=1;
                    }
                    std::vector<int> index;
                    std::string tmp;
                    std::istringstream iStream(arguments[it]);
                    //divide line based on "/"
                    while(std::getline(iStream, tmp, '/')){
                        index.push_back(std::stoi(tmp));
                    }
                    indices.push_back(glm::ivec3(index[0]-1,
                                                 index[1]-1,
                                                 index[2]-1));
                }
            }
        }
    }

    int numVerts = 0;
    // something about vertices duplicates
    for(int i=0; i<indices.size();i+=3){
        // compute tangent and bitangent
        glm::vec3 v1 = verts[indices[i+0].x];
        glm::vec3 v2 = verts[indices[i+1].x];
        glm::vec3 v3 = verts[indices[i+2].x];

        glm::vec3 w1 = verts[indices[i+0].y];
        glm::vec3 w2 = verts[indices[i+1].y];
        glm::vec3 w3 = verts[indices[i+2].y];

        float x1 = v2[0] - v1[0];
        float x2 = v3[0] - v1[0];
        float y1 = v2[1] - v1[1];
        float y2 = v3[1] - v1[1];
        float z1 = v2[2] - v1[2];
        float z2 = v3[2] - v1[2];

        float s1 = w2[0] - w1[0];
        float s2 = w3[0] - w1[0];
        float t1 = w2[1] - w1[1];
        float t2 = w3[1] - w1[1];

        float r = 1.0f/(s1*s2-s2*t1);

        glm::vec3 face_tangent((t2*x1 - t1*x2)*r,
                               (t2*y1 - t1*y2)*r,
                               (t2*z1 - t1*z2)*r);
        glm::vec3 face_bitangent = -glm::vec3((s1*x2 - s2*x1)*r,
                                              (s1*y2 - s2*y1)*r,
                                              (s1*z2 - s2*z1)*r);

        //add triangle
        for(int j=0; j<3;++j){
            glm::ivec3 P = indices[i+j];

            glm::vec3 normal = normals[P.y];

            //idk, I am scared
            glm::vec3 tangent(1,0,0);
            tangent = glm::dot(normal, face_tangent)*normal;
            tangent = face_tangent -tangent;
            tangent = glm::normalize(tangent);

            glm::vec3 bitangent;
            bitangent = glm::cross(normal, tangent);

            float handedness = glm::dot(bitangent, face_bitangent) < 0.0f ? -1.0f : 1.0f;

            // set bitangent
            bitangent = glm::cross(normal, tangent);
            bitangent = handedness*bitangent;
            bitangent = glm::normalize(bitangent);

            //pushing stuff
            linearVerts.push_back(verts[P.x][0]);
            linearVerts.push_back(verts[P.x][1]);
            linearVerts.push_back(verts[P.x][2]);

            linearNormals.push_back(normals[P.z][0]);
            linearNormals.push_back(normals[P.z][1]);
            linearNormals.push_back(normals[P.z][2]);

            linearTangents.push_back(tangent[0]);
            linearTangents.push_back(tangent[1]);
            linearTangents.push_back(tangent[2]);

            linearBitangents.push_back(bitangent[0]);
            linearBitangents.push_back(bitangent[1]);
            linearBitangents.push_back(bitangent[2]);

            linearUVs.push_back(uvs[P.y][0]);
            linearUVs.push_back(uvs[P.y][1]);
            tris.push_back(numVerts++);
        }
    }

    //we need to return
    // linearVerts linearNormals linearUvs, linear Tangents, linearBitangents and tris
    std::cout<<"object should have been loaded correctly, maybe"<<std::endl;

    generateMSS();
}

void importedModels::render(glm::mat4 pm, glm::mat4 vm, GLuint pi, GLuint t, GLuint ti) {
    // trying to render
    GLuint v;
    glGenBuffers(1, &v);
    glBindBuffer(GL_ARRAY_BUFFER, v);
    glBufferData(GL_ARRAY_BUFFER, linearVerts.size()*sizeof(float), &linearVerts[0], GL_STATIC_DRAW);

    GLuint uv;
    glGenBuffers(1, &uv);
    glBindBuffer(GL_ARRAY_BUFFER, uv);
    glBufferData(GL_ARRAY_BUFFER, linearUVs.size()*sizeof(float), &linearUVs[0], GL_STATIC_DRAW);

    GLuint ind;
    glGenBuffers(1, &ind);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ind);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, tris.size()*sizeof(unsigned int), &tris[0], GL_STATIC_DRAW);

    GLuint n;
    glGenBuffers(1, &n);
    glBindBuffer(GL_ARRAY_BUFFER, n);
    glBufferData(GL_ARRAY_BUFFER, linearNormals.size(), &linearNormals[0], GL_STATIC_DRAW);

    glUseProgram(pi);
    GLuint matrixID = glGetUniformLocation(pi, "MVP");
    glm::mat4 MVP = pm * vm * modelMatrix;
    glUniformMatrix4fv(matrixID, 1, GL_FALSE, &MVP[0][0]);

    // Bind our texture in Texture Unit 0
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, t);
    // Set our "myTextureSampler" sampler to use Texture Unit 0
    glUniform1i(ti, 0);

    // 1rst attribute buffer : vertices
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, v);
    glVertexAttribPointer(
            0,                  // attribute. No particular reason for 0, but must match the layout in the shader.
            3,                  // size
            GL_FLOAT,           // type
            GL_FALSE,           // normalized?
            0,                  // stride
            (void*)0            // array buffer offset
    );

    // 2nd attribute buffer : UVs
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, uv);
    glVertexAttribPointer(
            1,                                // attribute. No particular reason for 1, but must match the layout in the shader.
            2,                                // size : U+V => 2
            GL_FLOAT,                         // type
            GL_FALSE,                         // normalized?
            0,                                // stride
            (void*)0                          // array buffer offset
    );

    //i do not know for which law of the universe nullptr fixes the problem
    //but somehow it does. I swear that nothing makes less sense that this
    glDrawElements(GL_TRIANGLES, tris.size(), GL_UNSIGNED_INT, nullptr);

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
}

void importedModels::meshToTethrahedon() {

    //igl::copyleft::tetgen::tetrahedralize();

}

void importedModels::generateMSS() {
    //create particles
    for(int i =0; i<linearVerts.size()-2; i+=3){
        glm::vec3 tmp(linearVerts[i], linearVerts[i+1], linearVerts[i+2]);
        //vert number i/3
        int id = i/3;
        particle * toAdd= new particle(tmp, 1);
        bool alreadyIn = false;
        auto it = p.begin();
        // Iterate over the map using Iterator till end.
        while (it != p.end()) {
            if(tmp==it->first->getPosition()){
                it->second.push_back(id);
                alreadyIn=true;
            }
            it++;
        }
        if(!alreadyIn){
            particles.push_back(toAdd);
            p[toAdd] = std::vector<int>{id};
        }
    }
    //create springs
    auto it = p.begin();
    // Iterate over the map using Iterator till end.
    while (it != p.end()) {
        auto j = std::next(it, 1);
        while(j!=p.end()){
            particle * p1=it->first;
            particle * p2=j->first;
            spring * toAdd = new spring(glm::length(p1->getPosition()-p2->getPosition()), 40, 0.90f, p1, p2);
            springs.push_back(toAdd);
            j++;
        }
        it++;
    }
}

void importedModels::updateBuffer() {
   auto it = p.begin();
   while(it!=p.end()){
       for(int i=0; i<it->second.size();++i){
           linearVerts[3*it->second[i]] = it->first->getPosition().x;
           linearVerts[3*it->second[i]+1] = it->first->getPosition().y;
           linearVerts[3*it->second[i]+2] = it->first->getPosition().z;
       }
       it++;
   }
}

//second order Runge Kutta integrator
//half the time -> 1/4 the error
void importedModels::rungeKutta(float timeDelta){
    std::vector<glm::vec3> a1;
    std::vector<glm::vec3> a2;
    //gravity
    for(int i =0;i< particles.size();++i){
        a1.push_back(particles[i]->getVelocity());
        particles[i]->setForce(importedModels::wind+glm::vec3(0.0f,importedModels::gravity, 0.0f));
    }

    //iterating through the springs to add the force due to
    //springs elongation/compression
    for(int i=0; i<springs.size(); ++i){
        //update both particles
        springs[i]->updateParticlesForce();
    }

    for(int i=0; i<particles.size();++i){
        particles[i]->addForce(particles[i]->getCollisionForce());
        particles[i]->addForce(particles[i]->getFrictionForce());
        a2.push_back(particles[i]->getForce()/particles[i]->getMass());
    }

    //second evaluation of the forces
    for(int i =0;i< particles.size();++i){
        particles[i]->setForce(importedModels::wind+glm::vec3(0.0f,importedModels::gravity, 0.0f)+particles[i]->getCollisionForce() + particles[i]->getFrictionForce());
        particles[i]->resetCollisionForce();
        particles[i]->resetFrictionForce();
    }

    for(int i=0; i<springs.size(); ++i){
        //indices of the particles
        int part1 = springs[i]->getPart1Id();
        int part2 = springs[i]->getPart2Id();
        springs[i]->rungeKuttaHelper((timeDelta/2.0f)*a1[part1], (timeDelta/2.0f)*a1[part2],
                                     (timeDelta/2.0f)*a2[part1], (timeDelta/2.0f)*a2[part2]);
    }

    for(int i=0; i<particles.size(); ++i){
        if(!vectorContains(specialParticles, particles[i])){
            glm::vec3 vel = particles[i]->getVelocity();

            //updating position
            glm::vec3 pos = particles[i]->getPosition();
            pos+=timeDelta*(vel+(timeDelta/2.0f)*a2[particles[i]->getId()]);
            particles[i]->setPosition(pos);

            //updating velocity
            vel+=timeDelta*(particles[i]->getForce()/particles[i]->getMass());
            particles[i]->setVelocity(vel);
        }
    }
}
