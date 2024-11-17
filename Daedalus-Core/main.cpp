#include "graphics/Window.h"
#include "maths/maths.h"
#include "graphics/shader.h"

#include "graphics/buffers/buffer.h"
#include "graphics/buffers/indexBuffer.h"
#include "graphics/buffers/vertexArray.h"

#include "graphics/simpleRenderer2D.h"
#include "graphics/batchRenderer2D.h"
#include "graphics/renderable2D.h"
#include "graphics/staticSprite.h"
#include "graphics/sprite.h"

#include "graphics/layers/tileLayer.h"

#include "utils/timer.h"

using namespace daedalusCore;
using namespace graphics;
using namespace buffers;
using namespace maths;


int main()
{
	Window window("Test Window", 960, 540);
	//glClearColor(0.2f, 0.3f, 0.8f, 1.0f);

	mat4 ortho = mat4::orthographic(0, 16, 0, 9, -1, 1);

	Shader* shader = new Shader("resources/shaders/basic.vert", "resources/shaders/basic.frag");
	shader->enable();
	shader->setUniform2f("lightPos", vec2(4.0f, 1.5f));

	TileLayer layer(shader);

	for (float y = -9.0f; y < 9.0f; y += 0.1f)
	{
		for (float x = -16.0f; x < 16.0f; x += 0.1f)
		{
			layer.add(new Sprite(x, y, 0.09f, 0.09f, maths::vec4(rand() % 1000 / 1000.0f, 0, 1, 1)));
		}
	}

	Shader* shader2 = new Shader("resources/shaders/basic.vert", "resources/shaders/basic.frag");
	TileLayer layer2(shader2);
	layer2.add(new Sprite(-2, -2, 4, 4, maths::vec4(0, 0, 1, 1)));

	utils::Timer time;
	float timer = 0;
	unsigned int frames = 0;

	while (!window.closed())
	{
		window.clear();

		double x, y;
		window.getMousePosition(x, y);
		shader->enable();
		shader->setUniform2f("lightPos", vec2((float)(x * 32.0f / 960.0f - 16.0f), (float)(9.0f - y * 18.0f / 540.0f)));
		shader2->enable();
		shader2->setUniform2f("lightPos", vec2((float)(x * 32.0f / 960.0f - 16.0f), (float)(9.0f - y * 18.0f / 540.0f)));

		layer.render();
		layer2.render();

		window.update();
		
		frames++;
		if (time.elapsed() - timer > 1.0f)
		{
			timer += 1.0f;
			printf("%d fps\n", frames);
			frames = 0;
		}
	}

	return 0;
}