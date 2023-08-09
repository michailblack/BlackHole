#include "bhpch.h"
#include "BlackHole/Core/LayerStack.h"

LayerStack::~LayerStack()
{
	for (const Layer* layer : m_Layers)
		delete layer;
}

void LayerStack::PushLayer(Layer* layer)
{
	m_Layers.emplace(m_Layers.begin() + m_LayerInsertIndex++, layer);
}

void LayerStack::PushOverlay(Layer* overlay)
{
	m_Layers.emplace_back(overlay);
}

void LayerStack::PopLayer(Layer* layer)
{
	const auto it = std::ranges::find(m_Layers, layer);
    if (it != m_Layers.end())
	{
		m_Layers.erase(it);
		--m_LayerInsertIndex;
	}
}

void LayerStack::PopOverlay(Layer* overlay)
{
	const auto it = std::ranges::find(m_Layers, overlay);
    if (it != m_Layers.end())
		m_Layers.erase(it);
}
