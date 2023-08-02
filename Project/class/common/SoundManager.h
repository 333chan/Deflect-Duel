#pragma once
#include <unordered_map>
#include <string>

#define lpSoundMng SoundManager::GetInstance()

class SoundManager
{
public:
	static SoundManager& GetInstance()
	{
		static SoundManager s_Instance;
		return s_Instance;
	}
	/// <summary>
	/// ロード確認
	/// </summary>
	/// <param name="key">名付けたいファイル名</param>
	/// <param name="path">ファイル名</param>
	/// <returns></returns>
	const int& GetID(std::string key, std::string path);
	const int& GetID(std::string key);

	//一度のみ再生
	void PlaySoundOneTime(std::string name);
	//再生カウントリセット
	void ResetCount(std::string name);

	//すべての音声ストップ
	void AllStopSound();


private:

	std::unordered_map< std:: string, int>count_;

protected:


	SoundManager();
	~SoundManager();

	//map
	std::unordered_map < std::string, int> soundMap_;


};

