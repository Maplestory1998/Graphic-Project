#pragma once
#ifndef GLOBAL_H
#define GLOBAL_H

#include <math.h>
#include <vector>


#define _USE_MATH_DEFINES 
#define DEG2RAD(n)	(n*(M_PI/180))

void CreateGlobal(std::vector<float>& vertices, std::vector<unsigned int>& indices);


#endif