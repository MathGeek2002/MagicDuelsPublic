#ifndef VERTEX_HPP
#define VERTEX_HPP

#include <GL/glew.h>
#include "../../thirdparty/glm/glm.hpp"
#include "../../thirdparty/glm/gtc/matrix_transform.hpp"
#include "../../thirdparty/glm/gtc/type_ptr.hpp"

struct Vertex{
    glm::vec3 position;
    glm::vec3 color;
    glm::vec3 normal;
    glm::vec2 texcoord;
    static void ConfigureAttributes()
    {
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE, sizeof(Vertex),(void*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE, sizeof(Vertex),(void*)offsetof(Vertex,color));
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2,3,GL_FLOAT,GL_FALSE, sizeof(Vertex),(void*)offsetof(Vertex,normal));
        glEnableVertexAttribArray(3);
        glVertexAttribPointer(3,2,GL_FLOAT,GL_FALSE, sizeof(Vertex),(void*)offsetof(Vertex,texcoord));
    }
};

#endif