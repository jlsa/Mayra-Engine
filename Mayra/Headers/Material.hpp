//
//  Material.hpp
//  Mayra
//
//  Created by Joel Hoekstra on 14/02/2021.
//

#ifndef Material_h
#define Material_h

namespace Mayra
{
    struct Material
    {
        unsigned int diffuse;
        unsigned int specular;
        unsigned int emission;
        float shininess;
        float transparency;
    };
}

#endif /* Material_h */
