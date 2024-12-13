#pragma once

#include "layer.h"

namespace daedalusCore { namespace app {

	class DD_API layerStack
	{
	private:
		std::vector<Layer*> m_layers;
		std::vector<Layer*>::iterator m_layerInsert;

	public:
		layerStack();
		~layerStack();

		void PushLayer(Layer* layer);
		void PopLayer(Layer* layer);
		
		void PushOverlay(Layer* overlay);
		void PopOverlay(Layer* overlay);

		std::vector<Layer*>::iterator begin() { return m_layers.begin(); }
		std::vector<Layer*>::iterator end() { return m_layers.end(); }
	};

} }