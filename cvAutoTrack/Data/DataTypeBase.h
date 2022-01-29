#pragma once

struct Transform {
	bool result;
	double x, y, a;
};
struct Position {
	bool result;
	double x, y;
};
struct Direction {
	bool result;
	double a;
};
struct Rotation {
	bool result;
	double a2;
};
struct Star {
	bool result;
	double x, y;
	bool isEnd;
};
struct StarJson {
	bool result;
	char* jsonBuff;
};
struct UID {
	bool result;
	int uid;
};
struct InfoLoadPicture {
	bool result;
	char* path;
	int uid;
	double x, y, a;
};
struct InfoLoadVideo {
	bool result;
	char* path;
	char* pathOutFile;
};