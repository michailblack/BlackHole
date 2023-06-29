#pragma once

#include <vector>

#include "Layer.h"

class LayerStack
{
    using LayerStackBuffer   =   std::vector<Layer*>;
    using LayerStackIterator =   std::vector<Layer*>::iterator;
public:
    LayerStack();
    ~LayerStack();

    void PushLayer(Layer* layer);
    void PushOverlay(Layer* overlay);
    void PopLayer(Layer* layer);
    void PopOverlay(Layer* overlay);

    LayerStackIterator begin() { return m_Layers.begin(); }
    LayerStackIterator end() { return m_Layers.end(); }
private:
    LayerStackBuffer m_Layers;
    unsigned int m_LayerInsertIndex;
};

