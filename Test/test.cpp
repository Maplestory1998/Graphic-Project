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
#include "FlyThroughCamera.h"

//Directional Light
glm::vec3 dLightDirection = glm::vec3(0.0f, -1.f, 0.0f);
glm::vec3 dLightColour = glm::vec3(1.f, 1.f, 1.f);

//Positional Light
glm::vec3 pLightDirection = glm::vec3(0.0f, -1.f, 0.0f);
glm::vec3 pLightPos = glm::vec3(0.0f,  0.5f, 0.0f);
glm::vec3 pLightColour = glm::vec3(1.f, 1.f, 1.f);

//Spot Light
glm::vec3 sLightDirection = glm::vec3(0.0f, -1.f, 0.0f);
glm::vec3 sLightPos = glm::vec3(0.0f, 0.5f, 0.0f);
glm::vec3 sLightColour = glm::vec3(1.f, 0.f, 0.f);

// origin
glm::vec3 cube_pos = glm::vec3(0.0f, 0.0f, 0.0f);



//ModelViewerCamera
SCamera Camera1;
//FlyThroughCamera
SCamera Camera2;
enum CameraType
{
	ModelViewer,
	FlyThrough
};

CameraType Camera = ModelViewer;

//used for FlyThrought camera
bool firstMouse = true;
float prevMouseX;
float prevMouseY;

unsigned int shaderProgram;

//Produce 4 WAlls, return numbers of vertices
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


//Produce object by parsing obj file, return numbers of vertices
//NB: It is used for producing guitar, chair and desk;
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
		//pos					//normal				//uv
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

//proce one piece floor, full floor is created  procedurally
int ProduceFloor(unsigned int VAO, unsigned int VBO,  vector<string> &text_file)
{
	vector<float> vertices = {
		-1.f, 1.0f, 0.f,	0.f, 0.f, 1.f,	0.f, 1.f,
		-0.5f, -1.0f, 0.f,  0.f, 0.f, 1.f,	1.f, 0.f,
		-1.f, -1.0f, 0.f,	0.f, 0.f, 1.f,	0.f, 0.f,
		
		-1.f, 1.0f, 0.f,	0.f, 0.f, 1.f,	0.f, 1.f,
		-0.5f, 1.0f, 0.f,	0.f, 0.f, 1.f,	1.f, 1.f,
		-0.5f, -1.0f, 0.f,	0.f, 0.f, 1.f,	1.f, 0.f,
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

	//Create Global vertex data procedurally
	GetGlobalData(vertices, indices);
	text_file.emplace_back("R.bmp");

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &vertices[0], GL_STATIC_DRAW);


	
	GLuint EBO;
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
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

//View and Projection transform
void setViewProjection(SCamera &Camera)
{
	glm::mat4 view = glm::mat4(1.f);
	view = glm::lookAt(Camera.Position, Camera.Position + Camera.Front, Camera.Up);
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "view"), 1, GL_FALSE, glm::value_ptr(view));

	glm::mat4 projection = glm::mat4(1.f);
	projection = glm::perspective(glm::radians(45.f), 800.f / 600.f, 1.f, 10.f);
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
}


inline void transformGlobal( )
{
	glm::mat4 model = glm::mat4(1.f);
	model = glm::translate(model, glm::vec3(-0.7f, 0.32f, -0.83f));
	model = glm::rotate(model, (float)glfwGetTime() / 2, glm::vec3(0.f, 1.f, 0.f));
	model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
}


inline void transformGuitar()
{
	glm::mat4 model = glm::mat4(1.f);
	model = glm::translate(model, glm::vec3(0.3f, 0.3f, -0.93f));
	model = glm::scale(model, glm::vec3(0.45f, 0.45f, 0.45f));

	model = glm::rotate(model, glm::radians(-90.f), glm::vec3(1.f, 0.f, 0.f));
	model = glm::rotate(model, glm::radians(-90.f), glm::vec3(0.f, 0.f, 1.f));
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));

}

inline void transformChair()
{
	glm::mat4 model = glm::mat4(1.f);
	model = glm::translate(model, glm::vec3(-0.56f, 0.0f, -0.4f));
	model = glm::rotate(model, glm::radians(180.f), glm::vec3(0.f, 1.f, 0.f));
	model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.4f));

	glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
}

inline void transformDesk()
{
	glm::mat4 model = glm::mat4(1.f);
	model = glm::translate(model, glm::vec3(-0.58f, 0.22f, -0.8f));
	model = glm::scale(model, glm::vec3(0.005f, 0.003f, 0.005f));
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
}

inline void transformMagicCube()
{
	glm::mat4 model = glm::mat4(1.f);
	model = glm::translate(model, glm::vec3(-0.35f, 0.27f, -0.8f));
	model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
}


void processKeyboard(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	
	//Change Camera by pressing  C;
	if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS)
	{
		Camera = Camera == ModelViewer ? FlyThrough : ModelViewer;
	}

	if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
	{
		pLightColour.x = pLightColour.x >= 1.0f? 0.f : pLightColour.x + 0.01f;
	}
	if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
	{
		pLightColour.y = pLightColour.y >= 1.0f ? 0.f : pLightColour.y + 0.01f;
	}
	if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS)
	{
		pLightColour.z = pLightColour.z >= 1.0f ? 0.f : pLightColour.z + 0.01f;
	}

	//change the position of Spot Light into Camera Position;
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
	{
		sLightDirection = Camera == ModelViewer? Camera1.Front : Camera2.Front;
		sLightPos = Camera == ModelViewer ?  Camera1.Position : Camera2.Position;
	}

	if (Camera == ModelViewer)
	{
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
			y = -1.f;
			cam_changed = true;
		}

		if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
		{
			x = 0.f;
			y = 1.f;
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
			MoveAndOrientCamera(Camera1, glm::vec3(0, 0, 0), cam_dist, x, y);
		}
	}
	else {

		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)			MoveCamera(Camera2, SCamera::FORWARD);
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)			MoveCamera(Camera2, SCamera::BACKWARD);
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)			MoveCamera(Camera2, SCamera::LEFT);
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)			MoveCamera(Camera2, SCamera::RIGHT);
	}
}

void processMouse(GLFWwindow* window, double x, double y)
{
	if (Camera == ModelViewer)
		return;

	if (firstMouse)
	{
		prevMouseX = x;
		prevMouseY = y;
		firstMouse = false;
	}

	float dX = x - prevMouseX;
	float dY = prevMouseY - y;

	prevMouseX = x;
	prevMouseY = y;

	OrientCamera(Camera2, dX, dY);
}



int main(int argc, char** argv)
{
	GLFWwindow* window = CreateGLFWWindow(1920, 1080, "Test");
	
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPosCallback(window, processMouse);

	gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

	shaderProgram = LoadShader("mvp.vert", "test.frag");

	//Init ModelViewer Camera
	InitCamera(Camera1, 90.0f, 0);
	MoveAndOrientCamera(Camera1, glm::vec3(0, 0, 0), cam_dist, 0.f, 0.f);

	//Init FlyThrough Camera
	InitCamera(Camera2, 90.f, 0);
	OrientCamera(Camera2, 0.f, 0.f);

	unsigned int VAO[7];
	glGenVertexArrays(7, VAO);
	unsigned int VBO[7];
	glGenBuffers(7, VBO);

	vector<GLuint> texture;
	//array of BMP file
	vector<string> text_file;

	//global
	int VertexSize0 = ProduceGlobal(VAO[0], VBO[0], text_file);

	///*guitar*/
	int VertexSize1 = ProduceObjectByObj("nmah-2018_0136_01-feliciano_guitar-150k_watertight_model.obj", VAO[1], VBO[1], text_file);

	// The floor
	int VertexSize2 = ProduceFloor(VAO[2], VBO[2], text_file);

	//the desk
	int VertexSize3 = ProduceObjectByObj("Desk.obj", VAO[3], VBO[3], text_file);

	//chair
	int VertexSize4 = ProduceObjectByObj("chair_01.obj", VAO[4], VBO[4], text_file);

	//magic cube
	int VertexSize5 = ProduceMagicCube(VAO[5], VBO[5], text_file);

	//Wall
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

		if(Camera == ModelViewer)
			glUniform3f(glGetUniformLocation(shaderProgram, "camPos"), Camera1.Position.x, Camera1.Position.y, Camera1.Position.z);
		else
			glUniform3f(glGetUniformLocation(shaderProgram, "camPos"), Camera2.Position.x, Camera2.Position.y, Camera2.Position.z);

		//Directional Light
		glUniform3f(glGetUniformLocation(shaderProgram, "dLightDirection"), dLightDirection.x, dLightDirection.y, dLightDirection.z);
		glUniform3f(glGetUniformLocation(shaderProgram, "dLightColour"), 1.f, 1.f, 1.f);

		//Positional Light
		glUniform3f(glGetUniformLocation(shaderProgram, "pLightDirection"), pLightDirection.x, pLightDirection.y, pLightDirection.z);
		glUniform3f(glGetUniformLocation(shaderProgram, "pLightPos"), pLightPos.x, pLightPos.y, pLightPos.z);
		glUniform3f(glGetUniformLocation(shaderProgram, "pLightColour"), pLightColour.x, pLightColour.y, pLightColour.z);

		//Spot Light
		glUniform3f(glGetUniformLocation(shaderProgram, "sLightDirection"), sLightDirection.x, sLightDirection.y, sLightDirection.z);
		glUniform3f(glGetUniformLocation(shaderProgram, "sLightPos"), sLightPos.x, sLightPos.y, sLightPos.z);
		glUniform3f(glGetUniformLocation(shaderProgram, "sLightColour"), sLightColour.x, sLightColour.y, sLightColour.z);

		/* 3D transform*/
		if (Camera == ModelViewer)
			setViewProjection(Camera1);
		else
			setViewProjection(Camera2);

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
		transformDesk();
		glBindTexture(GL_TEXTURE_2D, texture[3]);
		glUseProgram(shaderProgram);
		glBindVertexArray(VAO[3]);
		glDrawArrays(GL_TRIANGLES, 0, VertexSize3);

		//chair
		transformChair();
		glBindTexture(GL_TEXTURE_2D, texture[4]);
		glUseProgram(shaderProgram);
		glBindVertexArray(VAO[4]);
		glDrawArrays(GL_TRIANGLES, 0, VertexSize4);

		//magic cube
		transformMagicCube();
		glBindTexture(GL_TEXTURE_2D, texture[5]);
		glUseProgram(shaderProgram);
		glBindVertexArray(VAO[5]);
		glDrawArrays(GL_TRIANGLES, 0, VertexSize5);

		//wall
		glm::mat4 model = glm::mat4(1.f);
		glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
		glBindTexture(GL_TEXTURE_2D, texture[5]);
		glUseProgram(shaderProgram);
		glBindVertexArray(VAO[6]);
		glDrawArrays(GL_TRIANGLES, 0, VertexSize6);

		//CreateGlobal(VAO[0], texture[0], VertexSize0);
		//CreateGuitar(VAO[1], texture[1], VertexSize1);
		//CreateFloor(VAO[2], texture[2], VertexSize2);
		//CreateDesk(VAO[3], texture[3], VertexSize3);
		//CreateChair(VAO[4], texture[4], VertexSize4);
		//CreateMagicCube(VAO[5], texture[5], VertexSize5);
		//CreateWall(VAO[6], texture[5], VertexSize6);

		glBindVertexArray(0);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	return 0;
}
