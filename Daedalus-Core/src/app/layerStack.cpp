#include "ddpch.h"
#include "layerStack.h"

namespace daedalusCore { namespace app {

	layerStack::layerStack()
	{
		m_layerInsert = m_layers.begin();
	}

	layerStack::~layerStack()
	{
		for (Layer* layer : m_layers)
			delete layer;
	}

	void layerStack::PushLayer(Layer* layer)
	{
		m_layerInsert = m_layers.emplace(m_layerInsert, layer);
	}

	void layerStack::PopLayer(Layer* layer)
	{
		auto it = std::find(m_layers.begin(), m_layers.end(), layer);
		if (it != m_layers.end())
		{
			m_layers.erase(it);
			m_layerInsert--;
		}
	}

	void layerStack::PushOverlay(Layer* overlay)
	{
		m_layers.emplace_back(overlay);
	}

	void layerStack::PopOverlay(Layer* overlay)
	{
		auto it = std::find(m_layers.begin(), m_layers.end(), overlay);
		if (it != m_layers.end())
			m_layers.erase(it);
	}

} }