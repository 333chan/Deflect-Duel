#pragma once
#include<string>
#include<map>
#include<vector>
#include<list>
#include"../common/Geometry.h"
#include"rapidxml.hpp"
#include"rapidxml_utils.hpp"
#include"../common/Collision2D.h"

//stringはname・intはcsvのデータ・
using MapData = std::map <std::string, std::vector<int>>;

class TmxObj
{
public:
	TmxObj();
	~TmxObj();
	bool LoadTsx(std::string FileName);
	bool LoadTmx(std::string FileName);
	
	/// <summary>
/// tmx読み込みtsxなし
/// </summary>
/// <param name="FileName">ファイル名</param>
/// <param name="tsxflag">tsxを使用するか　使用するtrueしないfalse</param>
/// <returns></returns>
	bool LoadTmx(std::string FileName,bool tsxflag);
	bool SetMap(void);

	const Vector2Int& GetWorldArea(void);
	const Vector2Int& GetTileSize(void);
	const unsigned int GetLayerSize(void);
	const unsigned int GetFirstGID(void);

	const MapData& GetMapData(void);
	const int GetMapData(std::string layer, Vector2Int pos);	//posは座標
	const int GetMapData(std::string layer, int x, int y);	//x,yは升目
	
	CollisionNameList& GetAllCollList(void);
	CollisionNameList& GetTitleCollList(void);
	CollisionNameList& GetInventoryCollList(void);
	CollisionNameList& GetOptionCollList(void);
	CollisionNameList& GetGameoverCollList(void);
	CollisionNameList& GetResultCollList(void);
	CollisionNameList& GetTipsCollList(void);
	CollisionNameList& GetCameraCollList(void);
	CollisionNameList& GetSelectCollList(void);
	CollisionNameList& GetConnectedCollList(void);
	CollisionNameList& GetOpeningCollList(void);
	CollisionNameList& GetExitCollList(void);

private:
	bool CheckTiledVersion(rapidxml::xml_node<>* node);

	std::map<std::string, int> version_;

	//TMX
	rapidxml::xml_document<> tmxDoc_;
	unsigned int firstGID_;		//GID
	unsigned int layerSize_;	//nextlayerID-1
	Vector2Int worldArea_;			//width
	Vector2Int tileSize_;			//tilesize
	MapData mapData_;

	//TSX
	rapidxml::xml_document<> tsxDoc_;

	//当たり判定
	CollisionNameList collListAll_;
	CollisionNameList titleCollList_;
	CollisionNameList inventoryCollList_;
	CollisionNameList optionCollList_;
	CollisionNameList gameoverCollList_;
	CollisionNameList resultCollList_;
	CollisionNameList tipsCollList_;
	CollisionNameList cameraCollList_;
	CollisionNameList SelectCollList_;
	CollisionNameList ConnectedCollList_;
	CollisionNameList OpeningCollList_;
	CollisionNameList ExitCollList_;
};

