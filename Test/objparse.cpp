#include <fstream>
#include <sstream>
#include "objparse.h"
#include <iostream>

using namespace std;

inline void dealv(const string &buf, struct FileInfo &info, stringstream& ss)
{
	string data;
	ss.clear();
	ss.str(buf);

	//omit 'v' of string begin
	getline(ss, data, ' ');	
	while (getline(ss, data, ' '))
	{
		if (data.size() != 0)
		{
			info.pos.push_back(stof(data));
		}
	}
	return;
}

inline void dealvt(const string &buf, struct FileInfo& info, stringstream& ss)
{
	string data;
	ss.clear();
	ss.str(buf);

	//omit 'vt' of string begin
	getline(ss, data, ' ');

	int cnt = 0;
	while (getline(ss, data, ' '))
	{
		if (data.size() != 0) {
			info.uv.push_back(stof(data));
			++cnt;
		}
	}
	//
	if (cnt == 2)
		info.uv.push_back(0.f);
	return;
}

inline void dealvn(const string &buf, struct FileInfo& info, stringstream& ss)
{
	string data;
	ss.clear();
	ss.str(buf);

	//omit 'vn' of string begin
	getline(ss, data, ' ');

	while (getline(ss, data, ' '))
	{
		if (data.size() != 0) {
			info.normal.push_back(stof(data));
		}
	}
	return;
}

inline void dealf(const string &buf, struct FileInfo& info, stringstream& ss)
{
	string data;
	ss.clear();
	ss.str(buf);

	//omit 'f' of the string begin
	getline(ss, data, ' ');
	while (getline(ss, data, ' '))
	{
		stringstream tmp(data);
		string val;
		getline(tmp, val, '/');
		info.face.push_back(stof(val));
		getline(tmp, val, '/');
		info.face.push_back(stof(val));
		getline(tmp, val, '/');
		info.face.push_back(stof(val));
	}
	return;
}

void read_obj(fstream &file,  struct FileInfo&info)
{
	string buf;
	stringstream ss;

	while (!file.eof())
	{
		getline(file, buf);
		//vt
		if (buf[0] == 'v' && buf[1] == 't')
			dealvt(buf, info, ss);
		//vn
		else if (buf[0] == 'v' && buf[1] == 'n')
			dealvn(buf, info, ss);
		//v
		else if (buf[0] == 'v')
			dealv(buf, info, ss);
		//f
		else if (buf[0] == 'f')
			dealf(buf, info, ss);
		else if (buf[0] == 'm')
		{
			int len = buf.size();
			if (len > 6 && buf.substr(0, 6) == "mtllib")
			{
				int idx = 7;
				while (idx < len && buf[idx] == ' ')
					++idx;
				info.mtl = buf.substr(idx, len - idx);
			}
		}
	}
}


void parse_mtl(struct FileInfo& info)
{
	fstream file(info.mtl, ios::in);
	if (!file.is_open())
	{
		cout << "fail to open file: " <<info.mtl<< endl;
		exit(-1);
	}

	string buf;
	stringstream ss;

	while (!file.eof())
	{
		getline(file, buf);
		if (buf[0] == 'm')
		{
			int len = buf.size();
			if (len > 6 && buf.substr(0, 6) == "map_Kd")
			{
				int idx = 7;
				while (idx < len && buf[idx] == ' ')
					++idx;
				info.bmp = buf.substr(idx, len - idx);
				break;
			}
		}
	}
}

//parse obj file to get vertex, uv and normal arraies
void parse_obj(string filename, vector<float> &vertex, vector<string>& text_file)
{
	fstream file(filename, ios::in);
	if (!file.is_open())
	{
		cout << "fail to open file: " <<filename<< endl;
		exit(-1);
	}
	struct FileInfo info;
	read_obj(file, info);

	int fCnt = info.face.size() / 9;
	
	int vIdx = 0;
	for (int fIdx = 0; fIdx < fCnt; ++fIdx)
	{
		for (int j = 0; j < 3; ++j)
		{
			vertex.push_back(info.pos[(info.face[fIdx * 9 + j * 3] - 1) * 3]);
			vertex.push_back(info.pos[(info.face[fIdx * 9 + j * 3] - 1) * 3 + 1]);
			vertex.push_back(info.pos[(info.face[fIdx * 9 + j * 3] - 1) * 3 + 2]);

			vertex.push_back(info.normal[(info.face[fIdx * 9 + j * 3 + 2] - 1) * 3]);
			vertex.push_back(info.normal[(info.face[fIdx * 9 + j * 3 + 2] - 1) * 3 + 1]);
			vertex.push_back(info.normal[(info.face[fIdx * 9 + j * 3 + 2] - 1) * 3 + 2]);
			
			vertex.push_back(info.uv[(info.face[fIdx * 9 + j * 3 + 1] - 1) * 3]);
			vertex.push_back(info.uv[(info.face[fIdx * 9 + j * 3 + 1] - 1) * 3 + 1]);
		}
	}
	
	parse_mtl(info);
	text_file.emplace_back(info.bmp);

	return ;
}

