//
//  Light.hpp
//  Mayra
//
//  Created by Joel Hoekstra on 14/02/2021.
//

#ifndef Light_h
#define Light_h

namespace Mayra
{
    struct DirectionalLight
    {
        glm::vec3 direction;

        glm::vec3 ambient;
        glm::vec3 diffuse;
        glm::vec3 specular;
    };

    struct PointLight
    {
        glm::vec3 position;

        glm::vec3 ambient;
        glm::vec3 diffuse;
        glm::vec3 specular;

        float constant;
        float linear;
        float quadratic;
    };

    struct SpotLight
    {
        glm::vec3 position;
        glm::vec3 direction;
        float cutOff;
        float outerCutOff;

        glm::vec3 ambient;
        glm::vec3 diffuse;
        glm::vec3 specular;
        unsigned int diffuseImage;

        float constant;
        float linear;
        float quadratic;
    };
}

#endif /* Light_h */
