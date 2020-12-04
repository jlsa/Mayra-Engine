//
//  application.hpp
//  Mayra
//
//  Created by Joel Hoekstra on 04/12/2020.
//

#ifndef application_h
#define application_h

#include "gui.hpp"
#include "window.hpp"

namespace Mayra
{
    class Application
    {
        Mayra::Window* _window;
        Mayra::Gui* _gui;
        Mayra::WindowProps* _props;
        
    public:
        Application(Mayra::WindowProps* props);
        int Initialize();
        void Run();
        void Terminate();
        void HandleInput(Mayra::Window* window);
        int GetShader(const char* vertShaderSource, const char* fragShaderSource);
    };
}
#endif /* application_h */
