#pragma once
#include "Layer.h"

class LayerStack
{
    using LayerStackBuffer          =   std::vector<Layer*>;
    using LayerStackIterator        =   std::vector<Layer*>::iterator;
    using LayerStackReverseIterator =   std::vector<Layer*>::reverse_iterator;
public:
    LayerStack() = default;
    ~LayerStack();

    void PushLayer(Layer* layer);
    void PushOverlay(Layer* overlay);
    void PopLayer(Layer* layer);
    void PopOverlay(Layer* overlay);

    LayerStackIterator begin() { return m_Layers.begin(); }
    LayerStackIterator end() { return m_Layers.end(); }

    LayerStackReverseIterator rbegin() { return m_Layers.rbegin(); }
    LayerStackReverseIterator rend() { return m_Layers.rend(); }
private:
    LayerStackBuffer m_Layers;
    unsigned int m_LayerInsertIndex = 0;
};

