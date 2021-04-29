
#include "deformableSphere.h"
#include "../../utilities/helperFunctions.h"

//sorting based on y
bool sortHelperY(particle *a, particle *b) {
    return a->getPosition().y > b->getPosition().y;
}

//sorting based on counterclockwise angle
bool sortHelperAngle(particle *a, particle *b) {
    //a and b should be at the same y, unless I somehow did something wrong
    glm::vec3 center(0, a->getPosition().y, 0);
    float angle1 = atan2(a->getPosition().z - center.z, a->getPosition().x - center.x);
    float angle2 = atan2(b->getPosition().z - center.z, b->getPosition().x - center.x);
    return angle1 < angle2;
}

deformableSphere::deformableSphere(glm::mat4 mod, glm::vec3 color, glm::vec3 lPos) : deformableObjects(-1.0f, mod,
                                                                                                       lPos), size(5) {
    //I fear that a too detailed sphere might
    //burn my cpu, so for now it will have size of 10
    vertexBuffer = new float[size * size * 18];
    colorBuffer = new float[size * size * 18];

    //helper needed to avoid pushing multiple times
    //the same vertex in the particles
    std::vector<particle *> helper;

    int vertPos = 0;
    int colPos = 0;
    int id = 0;
    float step = (float) 1 / size;
    float u = 0.0f;
    for (int i = 0; i < size; ++i) {
        float v = 0.0f;
        for (int j = 0; j < size; ++j) {
            double t = sin(M_PI * v);
            double x1 = t * cos(2 * M_PI * u);
            double z1 = t * sin(2 * M_PI * u);
            double y1 = cos(M_PI * v);

            double x4 = t * cos(2 * M_PI * (u + step));
            double z4 = t * sin(2 * M_PI * (u + step));
            double y4 = cos(M_PI * v);

            t = sin(M_PI * (v + step));
            double x2 = t * cos(2 * M_PI * u);
            double z2 = t * sin(2 * M_PI * u);
            double y2 = cos(M_PI * (v + step));

            double x3 = t * cos(2 * M_PI * (u + step));
            double z3 = t * sin(2 * M_PI * (u + step));
            double y3 = cos(M_PI * (v + step));

            vertexBuffer[vertPos++] = x1;
            vertexBuffer[vertPos++] = y1;
            vertexBuffer[vertPos++] = z1;

            vertexBuffer[vertPos++] = x3;
            vertexBuffer[vertPos++] = y3;
            vertexBuffer[vertPos++] = z3;

            vertexBuffer[vertPos++] = x2;
            vertexBuffer[vertPos++] = y2;
            vertexBuffer[vertPos++] = z2;

            vertexBuffer[vertPos++] = x1;
            vertexBuffer[vertPos++] = y1;
            vertexBuffer[vertPos++] = z1;

            vertexBuffer[vertPos++] = x4;
            vertexBuffer[vertPos++] = y4;
            vertexBuffer[vertPos++] = z4;

            vertexBuffer[vertPos++] = x3;
            vertexBuffer[vertPos++] = y3;
            vertexBuffer[vertPos++] = z3;

            //add particles
            particle *p1 = new particle(glm::vec3(x1 / 2.0f, y1 / 2.0f, z1 / 2.0f), 1.0f);
            particle *p2 = new particle(glm::vec3(x2 / 2.0f, y2 / 2.0f, z2 / 2.0f), 1.0f);
            particle *p3 = new particle(glm::vec3(x3 / 2.0f, y3 / 2.0f, z3 / 2.0f), 1.0f);
            particle *p4 = new particle(glm::vec3(x4 / 2.0f, y4 / 2.0f, z4 / 2.0f), 1.0f);

            //set id
            if (id == 0) {
                specialParticles.push_back(p1);
            }
            p1->setId(id++);
            p2->setId(id++);
            p3->setId(id++);
            p4->setId(id++);

            helper.push_back(p1);
            helper.push_back(p2);
            helper.push_back(p3);
            helper.push_back(p4);

            for (int k = 0; k < 6; k++) {
                colorBuffer[colPos++] = color[0];
                colorBuffer[colPos++] = color[1];
                colorBuffer[colPos++] = color[2];
            }
            v += step;
        }
        u += step;
    }

    //clean the helper
    int it = 0;
    while (it < helper.size()) {
        int internal = it + 1;
        while (internal < helper.size()) {
            glm::vec3 curr = helper[it]->getPosition();
            //due to the conversion from double to float
            //I added the second part to find duplicates
            //that wouldn't be found other wise
            if (helper[internal]->getPosition() == curr ||
                glm::length(curr - helper[internal]->getPosition()) < 0.0001) {
                helper.erase(helper.begin() + internal);
            } else {
                internal++;
            }
        }
        it++;
    }

    // now that we successfully removed all duplicates,
    // we can sort the particle by y and counter clock wise rotation
    std::sort(helper.begin(), helper.end(), sortHelperY);
    //it bugged me to see the approximation
    helper[helper.size() - 1]->setPosition(glm::vec3(0, 0.5, 0));

    //sorting based on the counter clockwise order
    //the first and last particle are ok
    for (int i = 0; i < size - 1; ++i) {
        std::sort(helper.begin() + 1 + i * size, helper.begin() + 1 + (i + 1) * size, sortHelperAngle);
    }

    std::cout << "helper has size: " << helper.size() << std::endl;
    //set the id in a meaningful way
    //and finally set the particle vector
    for (int i = 0; i < helper.size(); ++i) {
        helper[i]->setId(i);
    }

    //springs
    for (int i = 0; i < particles.size(); ++i) {
        for (int j = i + 1; j < particles.size(); ++j) {
            float dist = glm::length(particles[i]->getPosition() - particles[j]->getPosition());
            springs.push_back(new spring(dist, 100, .95, particles[i], particles[j]));
        }
    }

    std::cout<<"number of springs: "<<springs.size()<<std::endl;

    //unit sphere
    for (int i = 0; i < size * size * 18; ++i) {
        vertexBuffer[i] = vertexBuffer[i] / 2.0f;
    }

    //normals are the same as the vertex
    normalBuffer = vertexBuffer;

    setGLuint();

    //temporary stuff to avoid crashes
    std::vector<particle *> name;
    name.push_back(new particle(glm::vec3(0, 0, 0), 1));
    name.push_back(new particle(glm::vec3(0, 1, 0), 1));
    name.push_back(new particle(glm::vec3(0, 1, 1), 1));
    name.push_back(new particle(glm::vec3(0, 0, 1), 1));
    bvh = new sphereBVH(name, 2);
}

void deformableSphere::setGLuint() {
    GLuint deformableSphereVertex;
    glGenBuffers(1, &deformableSphereVertex);
    glBindBuffer(GL_ARRAY_BUFFER, deformableSphereVertex);
    glBufferData(GL_ARRAY_BUFFER, deformableSphere::getSize(), deformableSphere::getVertexBuffer(), GL_DYNAMIC_DRAW);

    GLuint deformableSphereColor;
    glGenBuffers(1, &deformableSphereColor);
    glBindBuffer(GL_ARRAY_BUFFER, deformableSphereColor);
    glBufferData(GL_ARRAY_BUFFER, deformableSphere::getSize(), deformableSphere::getColorBuffer(), GL_STATIC_DRAW);


    GLuint deformableSphereNormal;
    glGenBuffers(1, &deformableSphereNormal);
    glBindBuffer(GL_ARRAY_BUFFER, deformableSphereNormal);
    glBufferData(GL_ARRAY_BUFFER, deformableSphere::getSize(), deformableSphere::getNormalBuffer(), GL_STATIC_DRAW);

    deformableSphere::setVertex(deformableSphereVertex);
    deformableSphere::setColour(deformableSphereColor);
    deformableSphere::setNormal(deformableSphereNormal);
}

int deformableSphere::getSize() {
    return sizeof(float) * size * size * 18;
}

int deformableSphere::getNumberOfVertices() {
    return size * size * 18;
}

void deformableSphere::render(glm::mat4 ProjectionMatrix, glm::mat4 ViewMatrix, GLuint programID) {
    deformableObjects::render(ProjectionMatrix, ViewMatrix, programID);
}

helperStruct deformableSphere::isHovered(glm::vec3 origin, glm::vec3 direction) {
//return struct
    helperStruct toReturn;
    toReturn.isMouseOver = false;

    //i don't even know
    glm::mat4 whatEven = getModelMatrix();

    //sphere center
    glm::vec3 center(whatEven[3][0], whatEven[3][1], whatEven[3][2]);

    //transformation into local coordinate system
    glm::mat4 M_ = glm::inverse(whatEven);
    glm::vec4 orHelper = M_ * glm::vec4(origin, 1.0f);
    glm::vec3 origin_(orHelper.x, orHelper.y, orHelper.z);
    glm::vec4 dirHelper = M_ * glm::vec4(direction, 0.0f);
    glm::vec3 d_(dirHelper.x, dirHelper.y, dirHelper.z);
    d_ = glm::normalize(d_);

    float t = -glm::dot(origin_, d_);

    float d2 = glm::dot(origin_, origin_) - t * t;

    if (d2 > 1.0f) {
        //no intersection
        return toReturn;
    } else {
        //intersection
        float dt = sqrt(1.0 - d2);
        float t1 = t - dt;

        //intersected point on the sphere
        glm::vec3 p_ = origin_ + t1 * d_;
        float distance = 10000.0f;
        particle *closest;

        for (auto i : particles) {
            if (glm::length(i->getPosition() - p_) <= distance) {
                closest = i;
                distance = glm::length(i->getPosition() - p_);
            }
        }

        //savings the intersection data in the struct
        toReturn.isMouseOver = true;
        toReturn.point = closest;
        return toReturn;
    }
}

void deformableSphere::integrate(float timeDelta) {
    deformableSphere::rungeKutta(timeDelta);
}

sphereBVH *deformableSphere::getBvh() const {
    return bvh;
}

void deformableSphere::rungeKutta(float timeDelta) {

    std::vector<glm::vec3> a1;
    std::vector<glm::vec3> a2;
    //gravity
    for (int i = 0; i < particles.size(); ++i) {
        a1.push_back(particles[i]->getVelocity());
        particles[i]->setForce(deformableSphere::wind + glm::vec3(0.0f, deformableSphere::gravity, 0.0f));
    }

    //iterating through the springs to add the force due to
    //springs elongation/compression
    for (int i = 0; i < springs.size(); ++i) {
        //update both particles
        springs[i]->updateParticlesForce();
    }

    for (int i = 0; i < particles.size(); ++i) {
        particles[i]->addForce(particles[i]->getCollisionForce());
        a2.push_back(particles[i]->getForce() / particles[i]->getMass());
    }

    //second evaluation of the forces
    for (int i = 0; i < particles.size(); ++i) {
        particles[i]->setForce(deformableSphere::wind + glm::vec3(0.0f, deformableSphere::gravity, 0.0f) +
                               particles[i]->getCollisionForce());
        particles[i]->resetCollisionForce();
    }

    for (int i = 0; i < springs.size(); ++i) {
        //indices of the particles
        int part1 = springs[i]->getPart1Id();
        int part2 = springs[i]->getPart2Id();
        springs[i]->rungeKuttaHelper((timeDelta / 2.0f) * a1[part1], (timeDelta / 2.0f) * a1[part2],
                                     (timeDelta / 2.0f) * a2[part1], (timeDelta / 2.0f) * a2[part2]);
    }

    for (int i = 0; i < particles.size(); ++i) {
        if (!vectorContains(specialParticles, particles[i])) {
            glm::vec3 vel = particles[i]->getVelocity();

            //updating position
            glm::vec3 pos = particles[i]->getPosition();
            pos += timeDelta * (vel + (timeDelta / 2.0f) * a2[particles[i]->getId()]);
            particles[i]->setPosition(pos);

            //updating velocity
            vel += timeDelta * (particles[i]->getForce() / particles[i]->getMass());
            particles[i]->setVelocity(vel);
        }
    }

}

void deformableSphere::updateBuffer() {
    int pos = 0;
    for (int i = 0; i < particles.size(); i += 4) {
        glm::vec3 p0 = particles[i]->getPosition();
        glm::vec3 p1 = particles[i + 1]->getPosition();
        glm::vec3 p2 = particles[i + 2]->getPosition();
        glm::vec3 p3 = particles[i + 3]->getPosition();

        vertexBuffer[pos++] = p0[0];
        vertexBuffer[pos++] = p0[1];
        vertexBuffer[pos++] = p0[2];

        vertexBuffer[pos++] = p2[0];
        vertexBuffer[pos++] = p2[1];
        vertexBuffer[pos++] = p2[2];

        vertexBuffer[pos++] = p1[0];
        vertexBuffer[pos++] = p1[1];
        vertexBuffer[pos++] = p1[2];

        vertexBuffer[pos++] = p0[0];
        vertexBuffer[pos++] = p0[1];
        vertexBuffer[pos++] = p0[2];

        vertexBuffer[pos++] = p3[0];
        vertexBuffer[pos++] = p3[1];
        vertexBuffer[pos++] = p3[2];

        vertexBuffer[pos++] = p2[0];
        vertexBuffer[pos++] = p2[1];
        vertexBuffer[pos++] = p2[2];
    }
}

void deformableSphere::reset() {
    int vertPos = 0;
    int id = 0;
    float step = (float) 1 / size;
    float u = 0.0f;
    for (int i = 0; i < size; ++i) {
        float v = 0.0f;
        for (int j = 0; j < size; ++j) {
            double t = sin(M_PI * v);
            double x1 = t * cos(2 * M_PI * u) / 2.0f;
            double z1 = t * sin(2 * M_PI * u) / 2.0f;
            double y1 = cos(M_PI * v) / 2.0f;

            double x4 = t * cos(2 * M_PI * (u + step)) / 2.0f;
            double z4 = t * sin(2 * M_PI * (u + step)) / 2.0f;
            double y4 = cos(M_PI * v) / 2.0f;

            t = sin(M_PI * (v + step));
            double x2 = t * cos(2 * M_PI * u) / 2.0f;
            double z2 = t * sin(2 * M_PI * u) / 2.0f;
            double y2 = cos(M_PI * (v + step)) / 2.0f;

            double x3 = t * cos(2 * M_PI * (u + step)) / 2.0f;
            double z3 = t * sin(2 * M_PI * (u + step)) / 2.0f;
            double y3 = cos(M_PI * (v + step)) / 2.0f;

            vertexBuffer[vertPos++] = x1;
            vertexBuffer[vertPos++] = y1;
            vertexBuffer[vertPos++] = z1;

            //reset particles
            particles[id]->setPosition(glm::vec3(x1, y1, z1));
            particles[id]->setForce(glm::vec3(0, 0, 0));
            particles[id++]->setVelocity(glm::vec3(0, 0, 0));

            particles[id]->setPosition(glm::vec3(x2, y2, z2));
            particles[id]->setForce(glm::vec3(0, 0, 0));
            particles[id++]->setVelocity(glm::vec3(0, 0, 0));

            particles[id]->setPosition(glm::vec3(x3, y3, z3));
            particles[id]->setForce(glm::vec3(0, 0, 0));
            particles[id++]->setVelocity(glm::vec3(0, 0, 0));

            particles[id]->setPosition(glm::vec3(x4, y4, z4));
            particles[id]->setForce(glm::vec3(0, 0, 0));
            particles[id++]->setVelocity(glm::vec3(0, 0, 0));


            vertexBuffer[vertPos++] = x3;
            vertexBuffer[vertPos++] = y3;
            vertexBuffer[vertPos++] = z3;

            vertexBuffer[vertPos++] = x2;
            vertexBuffer[vertPos++] = y2;
            vertexBuffer[vertPos++] = z2;

            vertexBuffer[vertPos++] = x1;
            vertexBuffer[vertPos++] = y1;
            vertexBuffer[vertPos++] = z1;

            vertexBuffer[vertPos++] = x4;
            vertexBuffer[vertPos++] = y4;
            vertexBuffer[vertPos++] = z4;

            vertexBuffer[vertPos++] = x3;
            vertexBuffer[vertPos++] = y3;
            vertexBuffer[vertPos++] = z3;
            v += step;
        }
        u += step;
    }
}
