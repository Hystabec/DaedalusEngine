#include "graphics/Window.h"
#include "maths/maths.h"
#include "graphics/shader.h"
#include "utils/timer.h"

#include "graphics/buffers/buffer.h"
#include "graphics/buffers/indexBuffer.h"
#include "graphics/buffers/vertexArray.h"

#include "graphics/simpleRenderer2D.h"
#include "graphics/batchRenderer2D.h"
#include "graphics/renderable2D.h"
#include "graphics/staticSprite.h"
#include "graphics/sprite.h"

#include "graphics/layers/tileLayer.h"

#include "graphics/layers/group.h"


#include "graphics/texture.h"

using namespace daedalusCore;
using namespace graphics;
using namespace buffers;
using namespace maths;

#define TEST_50K_SPRITES 0

int main()
{
	Window window("Test Window", 960, 540);
	//glClearColor(0.2f, 0.3f, 0.8f, 1.0f);

	mat4 ortho = mat4::orthographic(0, 16, 0, 9, -1, 1);

	Shader* shader = new Shader("resources/shaders/basic.vert", "resources/shaders/basic.frag");
	shader->enable();
	shader->setUniform2f("lightPos", vec2(4.0f, 1.5f));

	TileLayer layer(shader);

	//Texture* texture = new Texture("resources/testImage.png");
	//Texture* texture2 = new Texture("resources/testImage2.png");

	Texture* textures[] =
	{
		new Texture("resources/testImage.png"),
		new Texture("resources/testImage2.png"),
		new Texture("resources/testImage3.png")
	};

#if TEST_50K_SPRITES
	for (float y = -9.0f; y < 9.0f; y += 0.1f)
	{
		for (float x = -16.0f; x < 16.0f; x += 0.1f)
		{
			layer.add(new Sprite(x, y, 0.09f, 0.09f, vec4(rand() % 1000 / 1000.0f, 0, 1, 1)));
		}
	}
#else
	for (float y = -9.0f; y < 9.0f; y++)
	{
		for (float x = -16.0f; x < 16.0f; x++)
		{
			//layer.add(new Sprite(x, y, 0.9f, 0.9f, maths::vec4(rand() % 1000 / 1000, 0, 1, 1)));
			layer.add(new Sprite(x, y, 0.9f, 0.9f, textures[rand() % 3]));
		}
	}
#endif

	GLint texID[] =
	{
		0,1,2,3,4,5,6,7,8,9
	};

	shader->enable();
	shader->setUniform1iv("textures", texID, 10);
	shader->setUniformMat4("pr_matrix", maths::mat4::orthographic(-16.0f, 16.0f, -9.0f, 9.0f, -1.0f, 1.0f));

	utils::modTimer time;
	float timer = 0;
	unsigned int frames = 0;

	while (!window.closed())
	{
		window.clear();

		double x, y;
		window.getMousePosition(x, y);
		shader->enable();
		shader->setUniform2f("lightPos", vec2((float)(x * 32.0f / 960.0f - 16.0f), (float)(9.0f - y * 18.0f / 540.0f)));

		layer.render();

		window.update();
		
		frames++;
		if (time.getSeconds() - timer > 1.0f)
		{
			timer += 1.0f;
			printf("%d fps\n", frames);
			frames = 0;
		}
	}

	//delete texture;
	//delete texture2;
	
	for (int i = 0; i < 3; i++)
		delete textures[i];

	return 0;
}


