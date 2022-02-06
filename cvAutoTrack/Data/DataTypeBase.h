#pragma once

struct Transform {
	bool result;
	double x, y, a;
};
/* 角色位置 */
struct Position {
	bool result;
	double x, y;
};
/* 角色朝向 */
struct Direction {
	bool result;
	double a;
};
/* 视角朝向 */
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