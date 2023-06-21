#ifndef DEBUG_HPP
#define DEBUG_HPP

#include <iostream>
#include "../../thirdparty/glm/glm.hpp"

class Debug{

public:
    static void Log(glm::vec3 v)
    {
        std::cout<<"( "<< v.x <<" , "<< v.y << " , "<<v.z<<" )\n";
    }

    static void Log(glm::vec2 v)
    {
        std::cout<<"( "<< v.x <<" , "<< v.y << " )\n";
    }

    template < typename T> 
    static void Log(T t)
    {
        std::cout<<t<<'\n';
    }
};

#endif