// Local Headers
#include <Mayra.hpp>

int main()
{
    Mayra::WindowProps* props = new Mayra::WindowProps("Mayra::Engine");

    Mayra::Application* app = new Mayra::Application(props);
    if (app->Initialize() == EXIT_SUCCESS) {
        app->Run();
        app->Terminate();
        return EXIT_SUCCESS;
    } else {
        return EXIT_FAILURE;
    }
}
