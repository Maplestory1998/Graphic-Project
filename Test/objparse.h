#pragma once
#ifndef OBJ_PARSE
#define	OBJ_PARSE

#include <string>
#include <vector>
#include "texture.h"

using namespace std;

struct FileInfo {
	vector<float> pos;
	vector<float> uv;
	vector<float> normal;
	vector<float> face;
	string mtl;
	string bmp;
};


void parse_mtl(struct FileInfo& info);

inline void dealv(const string &buf, struct FileInfo& info, stringstream& ss);

inline void dealvt(const string &buf, struct FileInfo& info, stringstream& ss);

inline void dealf(const string &buf, struct FileInfo& info, stringstream& ss);

inline void dealvn(const string &buf, struct FileInfo& info, stringstream& ss);

void read_obj(fstream& file, struct FileInfo& info);

//parse obj file to get vertex, uv and normal arraie
void parse_obj(string filename, vector<float>& vertex, vector<string>& text_file);



#endif // !OBJ_PARSE
