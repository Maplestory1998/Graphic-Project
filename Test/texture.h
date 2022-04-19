#pragma once

#ifndef TEXTURE_H
#define TEXTURE_H


#include <glad/glad.h> 
#include <iostream>
#include <vector>
#include <string>
#include "bitmap.h"

std::vector<GLuint> setup_texture(std::vector<std::string>& filename);
GLuint setup_mipmaps(const char* filename[], int n);

#endif // !TEXTURE_H