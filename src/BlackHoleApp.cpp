#include "bhpch.h"

int main()
{
    Log::Init();

    Application::Init();
    Application::Get().Run();
}