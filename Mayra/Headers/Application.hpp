//
//  application.hpp
//  Mayra
//
//  Created by Joel Hoekstra on 04/12/2020.
//

#ifndef application_h
#define application_h

#include <Gui.hpp>
#include <Window.hpp>
#include <OrthographicCamera.hpp>

namespace Mayra
{
    class Application
    {
    public:
        Application(Mayra::WindowProps* props);
        ~Application();
        int Initialize();
        void Run();
        void Terminate();

        OrthographicCamera _camera;
    private:
        Mayra::Window* _window;
        Mayra::Gui* _gui;
        Mayra::WindowProps* _props;
    };
}
#endif /* application_h */
