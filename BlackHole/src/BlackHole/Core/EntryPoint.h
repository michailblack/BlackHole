#pragma once
#include "BlackHole/Core/Application.h"
#include "BlackHole/Core/Base.h"
#include "BlackHole/Core/Filesystem.h"

int main(int argc, char** argv)
{
    Log::Init();
    Filesystem::Init();

    auto* app = CreateApplication();
    app->Run();
    delete app;
}