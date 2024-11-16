#include "graphics/Window.h"
#include "maths/maths.h"
#include "graphics/shader.h"

#include "graphics/buffers/buffer.h"
#include "graphics/buffers/indexBuffer.h"
#include "graphics/buffers/vertexArray.h"

#include "graphics/simpleRenderer2D.h"
#include "graphics/renderable2D.h"

using namespace daedalusCore;
using namespace graphics;
using namespace buffers;
using namespace maths;

int main()
{
	Window window("Test Window", 960, 540);

	glClearColor(0.2f, 0.3f, 0.8f, 1.0f);

	Shader shader("resources/shaders/basic.vert", "resources/shaders/basic.frag");
	shader.enable();

	mat4 ortho = mat4::orthographic(0, 16, 0, 9, -1, 1);
	shader.setUniformMat4("pr_matrix", ortho);

	Renderable2D sprite(maths::vec3(5, 5, 0), maths::vec2(4, 4), maths::vec4(1, 0, 1, 1), shader);
	Renderable2D sprite2(maths::vec3(5, 0, 0), maths::vec2(8, 2), maths::vec4(0, 1, 0, 1), shader);
	SimpleRenderer2D renderer;

	while (!window.closed())
	{
		window.clear();

		double x, y;
		window.getMousePosition(x, y);
		shader.setUniform2f("lightPos", vec2((float)(x * 16.0f / 960.0f), (float)(9.0f - y * 9.0f / 540.0f)));

		renderer.submit(&sprite);
		renderer.submit(&sprite2);
		renderer.flush();

		window.update();
	}

	return 0;
}