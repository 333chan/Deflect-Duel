#pragma once
#include<list>
#include"Geometry.h"

using Collision = std::pair<Vector2, Vector2>;
using CollList = std::list<Collision>;
using CollisionName = std::pair<std::string,Collision>;
using CollisionNameList = std::list<CollisionName>;