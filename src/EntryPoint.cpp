#include "BlackHole/Core/Application.h"

int main(int argc, char** argv)
{
    Log::Init();
    Filesystem::Init();

    auto* app = new Application(WindowProps());
    app->Run();
    delete app;
}