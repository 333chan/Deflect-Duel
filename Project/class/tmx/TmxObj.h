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
	

	//ステージの判定
	CollList& GetStageCollList(void);

	//ステージの画像
	CollList& GetStageimageList(void);

	//タイトル
	CollList& GetTitleLogoimageList(void);
	CollList& GetTitleBgimageList(void);

	CollList& GetSelectStageList(void);
	CollList& GetSelectLogoList(void);

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
	CollList stageCollList_;

	//ステージ画像の大きさ
	CollList stageimageList_;

	CollList bgImageList_;			//背景

	CollList titleLogoimageList_;	//タイトルロゴ

	//ステージセレクト
	CollList stageImageList_;	//ステージ1画像
	CollList logoImageList_;	//セレクトロゴ画像
};

