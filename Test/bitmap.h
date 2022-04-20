#pragma once

#ifndef BITMAP_H
#define BITMAP_H

#include "glad/glad.h"
#include <stdio.h>
#include <windows.h>
#include <wingdi.h>


GLuint loadbitmap(const char* filename, unsigned char*& pixelBuffer, BITMAPINFOHEADER* infoHeader, BITMAPFILEHEADER* fileHeader);

#endif // !BITMAP_H