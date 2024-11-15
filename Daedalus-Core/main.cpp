#include "graphics/Window.h"
#include "maths/maths.h"
#include "graphics/shader.h"

#include "graphics/buffers/buffer.h"
#include "graphics/buffers/indexBuffer.h"
#include "graphics/buffers/vertexArray.h"

using namespace daedalusCore;
using namespace graphics;
using namespace buffers;
using namespace maths;

int main()
{
	Window window("Test Window", 960, 540);

	glClearColor(0.2f, 0.3f, 0.8f, 1.0f);

	/*GLfloat verticies[] =
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
	glEnableVertexAttribArray(0);*/

	GLfloat verticies[] =
	{
		0, 0, 0,
		0, 3, 0,
		8, 3, 0,
		8, 0, 0
	};

	GLushort indices[] = { 0, 1, 2, 2, 3, 0 };

	GLfloat colour1[] =
	{
		1, 0, 1, 1,
		1, 0, 1, 1,
		1, 0, 1, 1,
		1, 0, 1, 1
	};

	GLfloat colour2[] =
	{
		1, 1, 0, 1,
		1, 1, 0, 1,
		1, 1, 0, 1,
		1, 1, 0, 1
	};

	VertexArray vao1, vao2;
	IndexBuffer ibo(indices, 6);

	vao1.addBuffer(new Buffer(verticies, 4 * 3, 3), 0);
	vao1.addBuffer(new Buffer(colour1, 4 * 4, 4), 1);

	vao2.addBuffer(new Buffer(verticies, 4 * 3, 3), 0);
	vao2.addBuffer(new Buffer(colour2, 4 * 4, 4), 1);

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

		//glDrawArrays(GL_TRIANGLES, 0, 6);

		vao1.bind();
		ibo.bind();
		shader.setUniformMat4("ml_matrix", mat4::translate(vec3(4, 3, 0)));
		glDrawElements(GL_TRIANGLES, ibo.getCount(), GL_UNSIGNED_SHORT, 0);
		ibo.unbind();
		vao1.unbind();

		vao2.bind();
		ibo.bind();
		shader.setUniformMat4("ml_matrix", mat4::translate(vec3(0, 0, 0)));
		glDrawElements(GL_TRIANGLES, ibo.getCount(), GL_UNSIGNED_SHORT, 0);
		ibo.unbind();
		vao2.unbind();

		window.update();
	}

	return 0;
}