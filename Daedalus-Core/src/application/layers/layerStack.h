#pragma once

#include "layer.h"

namespace daedalusCore { namespace application {

	class LayerStack
	{
	public:
		LayerStack();
		~LayerStack();

		void pushLayer(Layer* layer);
		void popLayer(Layer* layer);
		
		void pushOverlay(Layer* overlay);
		void popOverlay(Layer* overlay);

		std::vector<Layer*>::iterator begin() { return m_layers.begin(); }
		std::vector<Layer*>::iterator end() { return m_layers.end(); }

	private:
		std::vector<Layer*> m_layers;
		unsigned int m_layerInsertIndex = 0;
	};

} }