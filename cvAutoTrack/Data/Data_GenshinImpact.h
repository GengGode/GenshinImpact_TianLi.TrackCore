#pragma once
#include "DataFacility.h"
#include "DataTypeBase.h"
#include "Data_GenshinImpact.h"
#include <atomic>

class Data_GenshinImpact// : public DataFacility<Direction>
{
public:
	Transform		    _transform;
	Position		    _position;
	Direction			_direction;
	Rotation			_rotation;
	Star				_star;
	StarJson			_starJson;
	UID					_uid;
	InfoLoadPicture		_infoLoadPicture;
	InfoLoadVideo		_infoLoadVideo;
};
typedef Data_GenshinImpact Datas;

//using Datas = std::shared_ptr<Data_GenshinImpact>;
//using Datas = Data_GenshinImpact;

//struct Transform 
//struct Position 
//struct Direction 
//struct Rotation 
//struct Star 
//struct StarJson
//struct UID 
//struct InfoLoadPicture 
//struct InfoLoadVideo 

//#include <vector>
//#include <any>

//class Data_GenshinImpact// : public DataFacility<Direction>
//{
//	std::vector<std::any> _values;
//public:
//
//	template<typename T>
//	void get(T t,int id = 0)
//	{
//		_values[id] = t;
//	}
//	template<typename T, typename...Ts>
//	void get(T t,int id=0, Ts... ts)
//	{
//		_values[id] = t;
//		get(Ts..., id + 1);
//	}
//
//	template<typename T>
//	void set(T& t, int id = 0)
//	{
//		t = _values.at(id);
//	}
//	template<typename T, typename...Ts>
//	void set(T& t, int id = 0, Ts&... ts)
//	{
//		t = _values.at(id);
//		set(ts..., id + 1);
//	}
//};