#include<sstream>
//#include "../common/manager/ImageMng.h"
#include "TmxObj.h"
#include"../../_debug/_DebugConOut.h"

TmxObj::TmxObj()
{
	version_["1.6.0"] = 1;
	version_["1.8.2"] = 1;
	version_["1.8.4"] = 1;
	version_["1.4.1"] = 1;
	version_["1.10.1"] = 1;
}

TmxObj::~TmxObj()
{

}

bool TmxObj::LoadTsx(std::string FileName)
{
	rapidxml::file<> xmlFileName = FileName.c_str();
	tsxDoc_.parse<0>(xmlFileName.data());
	rapidxml::xml_node<>* tileset = tsxDoc_.first_node("tileset");
	
	if (!CheckTiledVersion(tileset))
	{
		return false;
	}
	//std::map<std::string, int>::key_compare comp = version_.key_comp();
	//auto begin = version_.begin(), end = version_.end();
	//for (auto iter = begin; iter != end; iter++)
	//{
	//	if (comp(iter->first, data))
	//	{
	//	}
	//	else
	//	{
	//	}
	//}


	auto node = tsxDoc_.first_node();
	while (node)
	{
		if (node->name_size())
		{
			TRACE("%s→", node->name());
		}
		auto atr = node->first_attribute();
		while (atr)
		{
			if (atr->name_size())
			{
				TRACE("%s:", atr->name());
			}
			TRACE("%s ", atr->value());
			atr = atr->next_attribute();
		}
		TRACE("\n");
		node = node->next_sibling();
	}

	auto imageNode = tileset->first_node("image");
	std::string source = imageNode->first_attribute("source")->value();
	auto keyName = source.substr(source.find_last_of("/") + 1);
	
	auto tilewidth = atoi(tileset->first_attribute("tilewidth")->value());
	auto tileheight = atoi(tileset->first_attribute("tileheight")->value());

	auto columns = atoi(tileset->first_attribute("columns")->value());
	auto rows = atoi(tileset->first_attribute("tilecount")->value()) / columns;

	//lpImageMng.GetID("./Image/map.png", "map.png", { 5,5 }, { 3,3 });
	//f_name=??? key=map.png divsize=16*16 divcnt=208*64
	// 
	//lpImageMng.GetID(source, keyName, { tilewidth,tileheight }, { columns ,rows });

	return true;
}

bool TmxObj::LoadTmx(std::string FileName)
{
	rapidxml::file<> filename = FileName.c_str();
	tmxDoc_.parse<0>(filename.data());
	rapidxml::xml_node<>* mapnode =tmxDoc_.first_node("map");


	if (!CheckTiledVersion(mapnode))
	{
		return false;
	}

	auto tileset = mapnode->first_node("tileset");

	auto pass = FileName.substr(0, FileName.find_last_of("/") + 1);
	std::string source = pass + tileset->first_attribute("source")->value();

	if (!LoadTsx(source))
	{
		return false;
	}

	firstGID_ = atoi(tileset->first_attribute("firstgid")->value());
	layerSize_ = atoi(mapnode->first_attribute("nextlayerid")->value()) - 1;
	worldArea_ = {atoi(mapnode->first_attribute("width")->value()),atoi(mapnode->first_attribute("height")->value()) };
	tileSize_ = {atoi(mapnode->first_attribute("tilewidth")->value()),atoi(mapnode->first_attribute("tileheight")->value()) };

	//auto node = tmxDoc_.first_node();
	//while(node)
	//{
	//	if (node->name_size())
	//	{
	//		TRACE("%s→", node->name());
	//	}
	//	auto atr = node->first_attribute();
	//	while (atr)
	//	{
	//		if (atr->name_size())
	//		{
	//			TRACE("%s:", atr->name());
	//		}
	//		TRACE("%s ", atr->value());
	//		atr = atr->next_attribute();
	//	}
	//	TRACE("\n");
	//	node = node->next_sibling();
	//}
	//auto nodeImage = node->first_node("tileset");
	//std::string source = nodeImage->first_attribute("firstgid")->value();
	//auto keyName=source.substr(source.c_str)
	return 	SetMap();
}

bool TmxObj::LoadTmx(std::string FileName, bool tsxflag)
{
	if (tsxflag == true)
	{
		return false;
	}
	rapidxml::file<> filename = FileName.c_str();
	tmxDoc_.parse<0>(filename.data());
	rapidxml::xml_node<>* mapnode = tmxDoc_.first_node("map");


	if (!CheckTiledVersion(mapnode))
	{
		return false;
	}
	return 	SetMap();
}

bool TmxObj::SetMap(void)
{
	rapidxml::xml_node<>* mapnode = tmxDoc_.first_node("map");
	if (mapnode == nullptr)
	{
		TRACE("mapノード取得エラー");
		return false;
	}

	for (auto LayerNode = mapnode->first_node("layer");
		LayerNode != nullptr;
		LayerNode = LayerNode->next_sibling())
	{

		if (LayerNode == nullptr)
		{
			TRACE("mapノード→layerノード取得エラー");
			return false;
		}

		std::string AttributeName = LayerNode->first_attribute("name")->value();

		if (AttributeName == "Coll")
		{
			continue;
		}
		if (AttributeName == "NorthMagnetColl")
		{
			continue;
		}
		if (AttributeName == "SouthMagnetColl")
		{
			continue;
		}
		if (AttributeName == "ItemColl")
		{
			continue;
		}
		if (AttributeName == "subItemColl")
		{
			continue;
		}

		auto layer = mapData_.try_emplace(LayerNode->first_attribute("name")->value());
		if (layer.second)
		{
			layer.first->second.resize(worldArea_.x * worldArea_.y);
		}

		auto layerData = LayerNode->first_node("data")->first_node();

		std::stringstream dataStream;
		dataStream << layerData->value();

		for (auto& data : layer.first->second)
		{
			std::string numstr;
			std::getline(dataStream, numstr, ',');
			data = atoi(numstr.c_str());
		}

		/*for (const auto& data : layer.first->second)
		{
			TRACE("%d ", data);
		}*/

		TRACE("No Problem\n");

	}
	for (auto CollNode = mapnode->first_node("objectgroup"); CollNode != nullptr; CollNode = CollNode->next_sibling())
	{
		std::string collName = CollNode->first_attribute()->next_attribute()->value();
		if (CollNode == nullptr)
		{
			return true;
		}

		for (auto ObjectNode = CollNode->first_node();
			ObjectNode != nullptr;
			ObjectNode = ObjectNode->next_sibling())
		{
			std::string CollName = ObjectNode->first_attribute("name")->value();
			float CollX = static_cast<float>(atof(ObjectNode->first_attribute("x")->value()));
			float CollY = static_cast<float>(atof(ObjectNode->first_attribute("y")->value()));
			float CollW = static_cast<float>(atof(ObjectNode->first_attribute("width")->value()));
			float CollH = static_cast<float>(atof(ObjectNode->first_attribute("height")->value()));
			
			//ステージ
			if (collName == "stage")
			{
				stageCollList_.push_back(
					{
						{ CollX,CollY},
						{CollW,CollH}
					}
				);
			}
			if (collName == "stageimage")
			{
				stageimageList_.push_back(
					{
						{ CollX,CollY},
						{CollW,CollH}
					}
				);
			}

			//タイトル
			if (collName == "logo")
			{
				titleLogoimageList_.push_back(
					{
						{ CollX,CollY},
						{CollW,CollH}
					}
				);
			}
			if (collName == "bg")
			{
				bgImageList_.push_back(
					{
						{ CollX,CollY},
						{CollW,CollH}
					}
				);
			}

			//セレクト
			if (collName == "stage1")
			{
				stageImageList_.push_back(
					{
						{ CollX,CollY},
						{CollW,CollH}
					}
				);
			}
			//セレクト
			if (collName == "logo")
			{
				logoImageList_.push_back(
					{
						{ CollX,CollY},
						{CollW,CollH}
					}
				);
			}

			//バーサス
			if (collName == "p1window")
			{
				p1windowList_.push_back(
					{
						{ CollX,CollY},
						{CollW,CollH}
					}
				);
			}
			if (collName == "p2window")
			{
				p2windowList_.push_back(
					{
						{ CollX,CollY},
						{CollW,CollH}
					}
				);
			}

			//ゲームシーン
			if (collName == "p1Howto")
			{
				p1HowtoList_.push_back(
					{
						{ CollX,CollY},
						{CollW,CollH}
					}
				);
			}
			if (collName == "p2Howto")
			{
				p2HowtoList_.push_back(
					{
						{ CollX,CollY},
						{CollW,CollH}
					}
				);
			}
			
		}
	}
	return true;
}

const Vector2Int& TmxObj::GetWorldArea(void)
{
	return worldArea_;
}

const Vector2Int& TmxObj::GetTileSize(void)
{
	return tileSize_;
}

const unsigned int TmxObj::GetLayerSize(void)
{
	return layerSize_;
}

const unsigned int TmxObj::GetFirstGID(void)
{
	return firstGID_;
}



const MapData& TmxObj::GetMapData(void)
{
	return mapData_;
}

const int TmxObj::GetMapData(std::string layer, Vector2Int pos)
{
	return GetMapData(layer, pos.x / tileSize_.x, pos.y / tileSize_.y);
}

const int TmxObj::GetMapData(std::string layer, int x, int y)
{
	
	if (mapData_.count(layer) == 0)
	{
		return 0;
	}
	//auto ass = mapData_[layer].size();
	int point = y * static_cast<int>(worldArea_.x) + x;			//何番目の情報か
	if (point >= 0 && point < mapData_[layer].size())
	{
		//auto ds = mapData_[layer][point];
		return mapData_[layer][point];
	}
	else
	{
		return 0;
	}
}


CollList& TmxObj::GetStageCollList(void)
{
	return stageCollList_;
}

CollList& TmxObj::GetStageimageList(void)
{
	return stageimageList_;
}

CollList& TmxObj::GetTitleLogoimageList(void)
{
	return titleLogoimageList_;
}

CollList& TmxObj::GetTitleBgimageList(void)
{
	return bgImageList_;
}

CollList& TmxObj::GetSelectStageList(void)
{
	return stageImageList_;
}

CollList& TmxObj::GetSelectLogoList(void)
{
	return logoImageList_;
}

CollList& TmxObj::Getp1windowList(void)
{
	return p1windowList_;
}

CollList& TmxObj::Getp2windowList(void)
{
	return p2windowList_;
}

CollList& TmxObj::Getp1HowtoList(void)
{
	return p1HowtoList_;
}

CollList& TmxObj::Getp2HowtoList(void)
{
	return p2HowtoList_;
}

bool TmxObj::CheckTiledVersion(rapidxml::xml_node<>* node)
{
	if (node == nullptr)
	{
		return false;
	}

	auto tiledversion = node->first_attribute("tiledversion");
	if (!version_.count(tiledversion->value()))
	{
		TRACE("Tiled非対応");
		return false;
	}
	if (!version_[tiledversion->value()])
	{
		TRACE("Tiled非対応");
		return false;
	}
	TRACE("Tiled対応\n");


	return true;
}
