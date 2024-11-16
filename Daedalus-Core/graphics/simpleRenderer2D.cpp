#include "simpleRenderer2D.h"

namespace daedalusCore { namespace graphics {

		void SimpleRenderer2D::submit(const Renderable2D* renderable)
		{
			m_renderQueue.push_back(renderable);
		}

		void SimpleRenderer2D::flush()
		{
			while (!m_renderQueue.empty())
			{
				const Renderable2D* renderalbe = m_renderQueue.front();
				
				renderalbe->getVAO()->bind();
				renderalbe->getIBO()->bind();

				renderalbe->getShader().setUniformMat4("ml_matrix", maths::mat4::translate(renderalbe->getPosition()));
				glDrawElements(GL_TRIANGLES, renderalbe->getIBO()->getCount(), GL_UNSIGNED_SHORT, 0);

				renderalbe->getIBO()->unbind();
				renderalbe->getVAO()->unbind();

				m_renderQueue.pop_front();
			}
		}

} }