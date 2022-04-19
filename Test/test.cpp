#include <glad/glad.h> 
#include <GLFW/glfw3.h>
#include <glm/glm/glm.hpp>
#include <glm/glm/gtc/matrix_transform.hpp>
#include <glm/glm/gtc/type_ptr.hpp>
#include <iostream>
#include <vector>

#include "window.h"
#include "shader.h"
#include "objparse.h"
#include "camera.h"
#include "ModelViewerCamera.h"
#include "texture.h"
#include "global.h"


glm::vec3 lightDirection = glm::vec3(0.1, -.81f, -.61f);
glm::vec3 lightPos = glm::vec3(2.f, 6.f, 7.f);

glm::vec3 cube_pos = glm::vec3(0.0f, 0.0f, 0.0f);
SCamera Camera;

unsigned int shaderProgram;

int ProduceWall(unsigned int VAO, unsigned int VBO)
{
	vector<float> vertices = {
		//back
		-1.0f, 0.8f, -1.0f,		0.f, 0.f, 1.f,
		1.0f,  0.f,  -1.0f,		0.f, 0.f, 1.f,
		-1.0f, 0.f, -1.0f,		0.f, 0.f, 1.f,

		-1.0f, 0.8f, -1.0f,		0.f, 0.f, 1.f,
		1.0f, 0.8f, -1.0f,		0.f, 0.f, 1.f,
		1.0f, 0.f,	-1.0f,		0.f, 0.f, 1.f,

		//left
		-1.f, 0.8f, -1.f,		1.f, 0.f, 0.f,
		-1.f, 0.f,	1.f,		1.f, 0.f, 0.f,
		-1.f, 0.f,	-1.f,		1.f, 0.f, 0.f,

		-1.f, 0.8f, -1.f,		1.f, 0.f, 0.f,
		-1.f, 0.8f, 1.f,		1.f, 0.f, 0.f,
		-1.f, 0.f,  1.f,		1.f, 0.f, 0.f,

		//right
		1.f, 0.8f, -1.f,		-1.f, 0.f, 0.f,
		1.f, 0.f,	1.f,		-1.f, 0.f, 0.f,
		1.f, 0.f,	-1.f,		-1.f, 0.f, 0.f,

		1.f, 0.8f, -1.f,		-1.f, 0.f, 0.f,
		1.f, 0.8f, 1.f,			-1.f, 0.f, 0.f,
		1.f, 0.f,  1.f,			-1.f, 0.f, 0.f,
	};

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &vertices[0], GL_STATIC_DRAW);

	//position
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	//normal
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	return 18;
}

int ProduceObjectByObj(const string &filename, unsigned int VAO, unsigned int VBO, vector<string>& text_file)
{
	vector<float> vertices;
	parse_obj(filename, vertices, text_file);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &vertices[0], GL_STATIC_DRAW);

	//position
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	//normal
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	//texture
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	return vertices.size() / 8;

}


int ProduceMagicCube(unsigned VAO, unsigned int VBO, vector<string>& text_file)
{
	vector<float> vertices = {
		//back face
		//pos					//normal
		-0.5f, -0.5f, -0.5f,  	0.0f, 0.0f, -1.0f,		0.25f,	2.0f / 3.0f,
		0.5f, -0.5f, -0.5f,  	0.0f, 0.0f, -1.0f,		0.5f,	2.0f / 3.0f,
		0.5f,  0.5f, -0.5f,  	0.0f, 0.0f, -1.0f,		0.5f,	1.0f,

		0.5f,  0.5f, -0.5f,  	0.0f, 0.0f, -1.0f,		0.5f,	1.0f,
		-0.5f,  0.5f, -0.5f,  	0.0f, 0.0f, -1.0f,		0.25f,	1.0f,
		-0.5f, -0.5f, -0.5f,  	0.0f, 0.0f, -1.0f,		0.25f,  2.0f / 3.0f,

		//front face
		-0.5f, -0.5f,  0.5f,  	0.0f, 0.0f,  1.0f,		0.25f,	1.0f / 3.0f,
		0.5f, -0.5f,  0.5f,  	0.0f, 0.0f,  1.0f,		0.5f,	1.0f / 3.0f,
		0.5f,  0.5f,  0.5f,  	0.0f, 0.0f,  1.0f,		0.5f,	0.f,
		0.5f,  0.5f,  0.5f,  	0.0f, 0.0f,  1.0f,		0.5f,	0.f,
		-0.5f,  0.5f,  0.5f,  	0.0f, 0.0f,  1.0f,		0.25f,	0.f,
		-0.5f, -0.5f,  0.5f,  	0.0f, 0.0f,  1.0f,		0.25f,	1.0f / 3.0f,

		//left face
		-0.5f,  0.5f,  0.5f,  	-1.0f, 0.0f, 0.0f,		0.f,	1.0f / 3.0f,
		-0.5f,  0.5f, -0.5f,  	-1.0f, 0.0f, 0.0f,		0.f,	2.0f / 3.0f,
		-0.5f, -0.5f, -0.5f,  	-1.0f, 0.0f, 0.0f,		0.25f,	2.0f / 3.0f,
		-0.5f, -0.5f, -0.5f,  	-1.0f, 0.0f, 0.0f,		0.25f,	2.0f / 3.0f,
		-0.5f, -0.5f,  0.5f,  	-1.0f, 0.0f, 0.0f,		0.25f,  1.0f / 3.0f,
		-0.5f,  0.5f,  0.5f,  	-1.0f, 0.0f, 0.0f,		0.f,	1.0f / 3.0f,

		//right face
		0.5f,  0.5f,  0.5f,  	1.0f,  0.0f, 0.0f,		0.75f,	1.0f / 3.0f,
		0.5f,  0.5f, -0.5f,  	1.0f,  0.0f, 0.0f,		0.75f,	2.0f / 3.0f,
		0.5f, -0.5f, -0.5f, 	1.0f,  0.0f, 0.0f,		0.5f,	2.0f / 3.0f,
		0.5f, -0.5f, -0.5f,  	1.0f,  0.0f, 0.0f,		0.5f,	2.0f / 3.0f,
		0.5f, -0.5f,  0.5f,  	1.0f,  0.0f, 0.0f,		0.5f,	1.0f / 3.0f,
		0.5f,  0.5f,  0.5f,  	1.0f,  0.0f, 0.0f,		0.75f,	1.0f / 3.0f,

		//bottom face
		-0.5f, -0.5f, -0.5f,  	0.0f, -1.0f, 0.0f,		0.25f,	2.0f / 3.0f,
		0.5f, -0.5f, -0.5f,  	0.0f, -1.0f, 0.0f,		0.5f,	2.0f / 3.0f,
		0.5f, -0.5f,  0.5f,  	0.0f, -1.0f, 0.0f,		0.5f,	1.0f / 3.0f,
		0.5f, -0.5f,  0.5f,  	0.0f, -1.0f, 0.0f,		0.5f,	1.0f / 3.0f,
		-0.5f, -0.5f,  0.5f,  	0.0f, -1.0f, 0.0f,		0.25f,	1.0f / 3.0f,	
		-0.5f, -0.5f, -0.5f,  	0.0f, -1.0f, 0.0f,		0.25f,	2.0f / 3.0f,

		//top face
		-0.5f,  0.5f, -0.5f,  	0.0f,  1.0f, 0.0f,		1.0f,	2.0f / 3.0f,
		0.5f,  0.5f, -0.5f,  	0.0f,  1.0f, 0.0f,		0.75f,	2.0f / 3.0f,
		0.5f,  0.5f,  0.5f,  	0.0f,  1.0f, 0.0f,		0.75f,	1.0f / 3.0f,
		0.5f,  0.5f,  0.5f,  	0.0f,  1.0f, 0.0f,		0.75f,	1.0f / 3.0f,
		-0.5f,  0.5f,  0.5f,  	0.0f,  1.0f, 0.0f,		1.0f,	1.0f / 3.0f,
		-0.5f,  0.5f, -0.5f, 	0.0f,  1.0f, 0.0f,		1.0f,	2.0f / 3.0f,
	};


	cout << vertices.size() << endl;
	text_file.emplace_back("magiccube.bmp");

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &vertices[0], GL_STATIC_DRAW);

	//position
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	//normal
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	//texture
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	return 36;

}

int ProduceFloor(unsigned int VAO, unsigned int VBO,  vector<string> &text_file)
{
	vector<float> vertices = {
		-1.f, 1.0f, 0.f,	0.f, 1.f, 0.f,	0.f, 1.f,
		-0.5f, -1.0f, 0.f,  0.f, 1.f, 0.f,	1.f, 0.f,
		-1.f, -1.0f, 0.f,	0.f, 1.f, 0.f,	0.f, 0.f,
		
		-1.f, 1.0f, 0.f,	0.f, 1.f, 0.f,	0.f, 1.f,
		-0.5f, 1.0f, 0.f,	0.f, 1.f, 0.f,	1.f, 1.f,
		-0.5f, -1.0f, 0.f,	0.f, 1.f, 0.f,	1.f, 0.f,
	};

	text_file.emplace_back("wall.bmp");

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &vertices[0], GL_STATIC_DRAW);

	//position
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	//normal
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	//texture
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	return 6;
}

/*global*/
int ProduceGlobal(unsigned int VAO, unsigned int VBO, vector<string>& text_file)
{
	vector<float> vertices;
	vector<unsigned int> indices;
	CreateGlobal(vertices, indices);
	text_file.emplace_back("R.bmp");

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &vertices[0], GL_STATIC_DRAW);

	GLuint element_buffer_object;
	glGenBuffers(1, &element_buffer_object);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, element_buffer_object);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(float), &indices[0], GL_STATIC_DRAW);

	//position
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	//normal
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(0 * sizeof(float)));
	glEnableVertexAttribArray(1);
	//texture
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(2);

	return indices.size();
}




int ProduceGuitar(unsigned int VAO, unsigned int VBO, vector<string>& text_file)
{
	vector<float> vertices;
	parse_obj("nmah-2018_0136_01-feliciano_guitar-150k_watertight_model.obj", vertices, text_file);
	
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &vertices[0], GL_STATIC_DRAW);

	//position
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	//normal
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	//texture
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);
	
	return vertices.size() / 8;
	
}

void setViewProjection()
{
	glm::mat4 view = glm::mat4(1.f);
	view = glm::lookAt(Camera.Position, Camera.Position + Camera.Front, Camera.Up);
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "view"), 1, GL_FALSE, glm::value_ptr(view));

	glm::mat4 projection = glm::mat4(1.f);
	projection = glm::perspective(glm::radians(45.f), 800.f / 600.f, 1.f, 10.f);
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
}


void transformGlobal( )
{
	glm::mat4 model = glm::mat4(1.f);
	model = glm::translate(model, glm::vec3(-0.7f, 0.32f, -0.83f));
	model = glm::rotate(model, (float)glfwGetTime() / 2, glm::vec3(0.f, 1.f, 0.f));
	model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
}


void transformGuitar()
{
	glm::mat4 model = glm::mat4(1.f);
	model = glm::translate(model, glm::vec3(0.3f, 0.3f, -0.93f));
	model = glm::scale(model, glm::vec3(0.45f, 0.45f, 0.45f));

	model = glm::rotate(model, glm::radians(-90.f), glm::vec3(1.f, 0.f, 0.f));
	model = glm::rotate(model, glm::radians(-90.f), glm::vec3(0.f, 0.f, 1.f));
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));

}




void processKeyboard(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
	{
		lightDirection = Camera.Front;
		lightPos = Camera.Position;
	}

	bool cam_changed = false;
	float x = 0.f, y = 0.f;
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
	{
		x = 1.f;
		y = 0.f;
		cam_changed = true;
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
	{
		x = -1.f;
		y = 0.f;
		cam_changed = true;
	}

	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
	{
		x = 0.f;
		y = 1.f;
		cam_changed = true;
	}

	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
	{
		x = 0.f;
		y = -1.f;
		cam_changed = true;
	}

	if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS)
	{
		cam_dist += 0.002f;
		cam_changed = true;
	}

	if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
	{
		cam_dist -= 0.002f;
		cam_changed = true;
	}

	if (cam_changed)
	{
		MoveAndOrientCamera(Camera, glm::vec3(0, 0, 0), cam_dist, x, y);
	}

}


int main(int argc, char** argv)
{
	GLFWwindow* window = CreateGLFWWindow(1920, 1080, "Test");

	gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

	shaderProgram = LoadShader("mvp.vert", "col.frag");

	InitCamera(Camera, 90.0f, 0);
	MoveAndOrientCamera(Camera, glm::vec3(0, 0, 0), cam_dist, 0.f, 0.f);


	unsigned int VAO[7];
	glGenVertexArrays(7, VAO);
	unsigned int VBO[7];
	glGenBuffers(7, VBO);

	vector<GLuint> texture;
	vector<string> text_file;

	//global
	int VertexSize0 = ProduceGlobal(VAO[0], VBO[0], text_file);

	///*guitar*/
	int VertexSize1 = ProduceGuitar(VAO[1], VBO[1], text_file);

	// The floor
	int VertexSize2 = ProduceFloor(VAO[2], VBO[2], text_file);

	//the desk

	int VertexSize3 = ProduceObjectByObj("Desk.obj", VAO[3], VBO[3], text_file);

	//chair
	int VertexSize4 = ProduceObjectByObj("chair_01.obj", VAO[4], VBO[4], text_file);

	//magic cube
	int VertexSize5 = ProduceMagicCube(VAO[5], VBO[5], text_file);

	int VertexSize6 = ProduceWall(VAO[6], VBO[6]);

	texture = setup_texture(text_file);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glEnable(GL_DEPTH_TEST);
	glUseProgram(shaderProgram);


	while (!glfwWindowShouldClose(window))
	{
		processKeyboard(window);

		glClearColor(.0f, .0f, .0f, 1.f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		//light
		glUniform3f(glGetUniformLocation(shaderProgram, "lightDirection"), lightDirection.x, lightDirection.y, lightDirection.z);
		glUniform3f(glGetUniformLocation(shaderProgram, "lightPos"), lightPos.x, lightPos.y, lightPos.z);
		glUniform3f(glGetUniformLocation(shaderProgram, "lightColour"), 1.f, 1.f, 1.f);
		glUniform3f(glGetUniformLocation(shaderProgram, "camPos"), Camera.Position.x, Camera.Position.y, Camera.Position.z);


		setViewProjection();
		/* 3D transform*/

		//global
		transformGlobal();
		glBindTexture(GL_TEXTURE_2D, texture[0]);
		glUseProgram(shaderProgram);
		glBindVertexArray(VAO[0]);
		glDrawElements(GL_TRIANGLES, VertexSize0, GL_UNSIGNED_INT, 0);


		//guitar
		transformGuitar();
		glBindTexture(GL_TEXTURE_2D, texture[1]);
		glUseProgram(shaderProgram);
		glBindVertexArray(VAO[1]);
		glDrawArrays(GL_TRIANGLES, 0, VertexSize1);

		//floor
		for(int i = 0; i < 4; ++i)
		{
			glm::mat4 model = glm::mat4(1.f);
			model = glm::translate(model, glm::vec3(  (float)(0.5 * i), 0.0f, 0.0f) );
			model = glm::rotate(model, glm::radians(-90.f), glm::vec3(1.f, 0.f, 0.f));

			glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
			glBindTexture(GL_TEXTURE_2D, texture[2]);
			glUseProgram(shaderProgram);
			glBindVertexArray(VAO[2]);
			glDrawArrays(GL_TRIANGLES, 0, VertexSize2);

		}

		//desk
		glm::mat4 model = glm::mat4(1.f);
		model = glm::translate(model, glm::vec3(-0.58f, 0.22f, -0.8f));
		model = glm::scale(model, glm::vec3(0.005f, 0.003f, 0.005f));
		glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));

		glBindTexture(GL_TEXTURE_2D, texture[3]);
		glUseProgram(shaderProgram);
		glBindVertexArray(VAO[3]);
		glDrawArrays(GL_TRIANGLES, 0, VertexSize3);

		//chair
		model = glm::mat4(1.f);
		model = glm::translate(model, glm::vec3(-0.56f, 0.0f, -0.4f));
		model = glm::rotate(model, glm::radians(180.f), glm::vec3(0.f, 1.f, 0.f));
		model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.4f));
		glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));

		glBindTexture(GL_TEXTURE_2D, texture[4]);
		glUseProgram(shaderProgram);
		glBindVertexArray(VAO[4]);
		glDrawArrays(GL_TRIANGLES, 0, VertexSize4);

		//magic cube
		model = glm::mat4(1.f);
		model = glm::translate(model, glm::vec3(-0.35f, 0.27f, -0.8f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));

		glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));

		glBindTexture(GL_TEXTURE_2D, texture[5]);
		glUseProgram(shaderProgram);
		glBindVertexArray(VAO[5]);
		glDrawArrays(GL_TRIANGLES, 0, VertexSize5);

		//wall
		model = glm::mat4(1.f);

		glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));

		glBindTexture(GL_TEXTURE_2D, texture[5]);
		glUseProgram(shaderProgram);
		glBindVertexArray(VAO[6]);
		glDrawArrays(GL_TRIANGLES, 0, VertexSize6);


		glBindVertexArray(0);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	return 0;
}
