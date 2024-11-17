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

#include "utils/timer.h"

using namespace daedalusCore;
using namespace graphics;
using namespace buffers;
using namespace maths;

#define BATCH_RENDERER 1

int main()
{
	Window window("Test Window", 960, 540);

	//glClearColor(0.2f, 0.3f, 0.8f, 1.0f);

	Shader shader("resources/shaders/basic.vert", "resources/shaders/basic.frag");
	shader.enable();

	mat4 ortho = mat4::orthographic(0, 16, 0, 9, -1, 1);
	shader.setUniformMat4("pr_matrix", ortho);

	std::vector<Renderable2D*> sprites;

	srand(time(NULL));

	for (float y = 0; y < 9.0f; y+= 0.05f)
	{
		for (float x = 0; x < 16.0f; x += 0.05f)
		{
			sprites.push_back(new
#if BATCH_RENDERER
				Sprite
#else
				StaticSprite
#endif
				(x, y, 0.04f, 0.04f, maths::vec4(rand() % 1000 / 1000.0f, 0, 1, 1)
#if !BATCH_RENDERER
					,shader
#endif
					));
		}
	}

#if BATCH_RENDERER
	Sprite sprite(5, 5, 4, 4, maths::vec4(1, 0, 1, 1));
	Sprite sprite2(5, 0, 8, 2, maths::vec4(0, 1, 0, 1));
	BatchRenderer2D renderer;
#else
	StaticSprite sprite(5, 5, 4, 4, maths::vec4(1, 0, 1, 1), shader);
	StaticSprite sprite2(5, 0, 8, 2, maths::vec4(0, 1, 0, 1), shader);
	SimpleRenderer2D renderer;
#endif

	utils::Timer time;
	float timer = 0;
	unsigned int frames = 0;

	while (!window.closed())
	{
		window.clear();

		double x, y;
		window.getMousePosition(x, y);
		shader.setUniform2f("lightPos", vec2((float)(x * 16.0f / 960.0f), (float)(9.0f - y * 9.0f / 540.0f)));

#if BATCH_RENDERER
		renderer.begin();
#endif
		for (int i = 0; i < sprites.size(); i++)
		{
			renderer.submit(sprites[i]);
		}

		//renderer.submit(&sprite);
		//renderer.submit(&sprite2);
#if BATCH_RENDERER
		renderer.end();
#endif

		renderer.render();

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