#define _USE_MATH_DEFINES
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "global.h"

using namespace std;

const int num_segments = 128;


void GetGlobalData(vector<float> &vertices, vector<unsigned int> &indices)
{
	float x_pos = 0.f;
	float step = 180.f /(float) num_segments;

	//x : jing   y : wei s


	for (int x = 0; x <= num_segments; ++x)
	{
		float y_pos = 0.f;
		for (int y = 0; y <= num_segments; ++y)
		{
			//float x_pos = (float)x / float(num_segments);
			//float y_pos = (float)y / float(num_segments);

			float xP = cos(DEG2RAD(2 * x_pos)) * sin(DEG2RAD(y_pos));
			float yP = cos(DEG2RAD(y_pos));
			float zP = sin(DEG2RAD(2 * x_pos)) * sin(DEG2RAD(y_pos));

			//texture
			float u = 1.0f -(float)x / (float)num_segments;
			float v = 1.0f -(float)y / (float)num_segments;

			vertices.emplace_back(xP);
			vertices.emplace_back(yP);
			vertices.emplace_back(zP);
			

			vertices.emplace_back(u);
			vertices.emplace_back(v);

			y_pos += step;
		}
		x_pos += step;
	}

	for (int i = 0; i < num_segments; ++i)
	{
		for (int j = 0; j < num_segments; ++j)
		{
			indices.emplace_back(i * (num_segments + 1) + j);
			indices.emplace_back((i + 1) * (num_segments + 1) + j);
			indices.emplace_back((i + 1) * (num_segments + 1) + j + 1);
			indices.emplace_back(i * (num_segments + 1) + j);
			indices.emplace_back((i + 1) * (num_segments + 1) + j + 1);
			indices.emplace_back(i * (num_segments + 1) + j + 1);
		}
	}
	return;


}


