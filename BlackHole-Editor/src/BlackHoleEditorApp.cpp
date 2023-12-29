#include "BlackHole/Core/EntryPoint.h"

#include "BlackHole-Editor/EditorLayer.h"

class BlackHoleEditor : public Application
{
public:
    BlackHoleEditor(const ApplicationSpecification& spec)
        : Application(spec)
    {
        PushLayer(new EditorLayer());
    }
};

Application* CreateApplication()
{
    ApplicationSpecification spec;
    spec.Name = "BlackHole Editor";
    return new BlackHoleEditor(spec);
}