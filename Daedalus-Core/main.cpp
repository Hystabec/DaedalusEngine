#include "graphics/Window.h"
#include "maths/maths.h"
#include "graphics/shader.h"

using namespace daedalusCore;
using namespace graphics;
using namespace maths;

int main()
{
	Window window("Test Window", 960, 540);

	glClearColor(0.2f, 0.3f, 0.8f, 1.0f);

	GLfloat verticies[] =
	{
		0, 0, 0,
		8, 0, 0,
		0, 3, 0,
		0, 3, 0,
		8, 3, 0,
		8, 0, 0
	};

	GLuint vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(verticies), verticies, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	Shader shader("resources/shaders/basic.vert", "resources/shaders/basic.frag");
	shader.enable();

	mat4 ortho = mat4::orthographic(0, 16, 0, 9, -1, 1);
	shader.setUniformMat4("pr_matrix", ortho);
	shader.setUniformMat4("ml_matrix", mat4::translate(vec3(4, 3, 0)));

	shader.setUniform2f("lightPos", vec2(1, 1));
	shader.setUniform4f("colour", vec4(1.0f, 0.0f, 0.0f, 1.0f));

	while (!window.closed())
	{
		window.clear();

		glDrawArrays(GL_TRIANGLES, 0, 6);

		window.update();
	}

	return 0;
}