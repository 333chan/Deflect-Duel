#pragma once
#include<vector>
#include <unordered_map>
#include <string>
#include"Geometry.h"

#define lpImageMng ImageManager::GetInstance()

class ImageManager
{
public:
	static ImageManager& GetInstance()
	{
		static ImageManager s_Instance;
		return s_Instance;
	}
	/// <summary>
	/// ロード確認
	/// </summary>
	/// <param name="key">名付けたいファイル名</param>
	/// <param name="path">ファイル名</param>
	/// <returns></returns>
	const std::vector<int>& GetID(std::string key);
	const std::vector<int>& GetID(std::string key, std::string path);
	const std::vector<int>& GetID(
		std::string key,
		std::string path,
		Vector2Int divCount,
		Vector2Int divSize
	);

private:
protected:


	ImageManager();
	~ImageManager();

	//map
	std::unordered_map < std::string, std::vector<int>> imageMap_;


};

