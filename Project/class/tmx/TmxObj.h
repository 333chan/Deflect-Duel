#pragma once
#include<string>
#include<map>
#include<vector>
#include<list>
#include"../common/Geometry.h"
#include"rapidxml.hpp"
#include"rapidxml_utils.hpp"
#include"../common/Collision2D.h"

//string��name�Eint��csv�̃f�[�^�E
using MapData = std::map <std::string, std::vector<int>>;

class TmxObj
{
public:
	TmxObj();
	~TmxObj();
	bool LoadTsx(std::string FileName);
	bool LoadTmx(std::string FileName);
	
	/// <summary>
/// tmx�ǂݍ���tsx�Ȃ�
/// </summary>
/// <param name="FileName">�t�@�C����</param>
/// <param name="tsxflag">tsx���g�p���邩�@�g�p����true���Ȃ�false</param>
/// <returns></returns>
	bool LoadTmx(std::string FileName,bool tsxflag);
	bool SetMap(void);

	const Vector2Int& GetWorldArea(void);
	const Vector2Int& GetTileSize(void);
	const unsigned int GetLayerSize(void);
	const unsigned int GetFirstGID(void);

	const MapData& GetMapData(void);
	const int GetMapData(std::string layer, Vector2Int pos);	//pos�͍��W
	const int GetMapData(std::string layer, int x, int y);	//x,y�͏���
	

	CollList& GetStageCollList(void);

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

	//�����蔻��
	CollList stageCollList_;
};

