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
	/// ���[�h�m�F
	/// </summary>
	/// <param name="key">���t�������t�@�C����</param>
	/// <param name="path">�t�@�C����</param>
	/// <returns></returns>
	const int& GetID(std::string key, std::string path);
	const int& GetID(std::string key);

	//��x�̂ݍĐ�
	void PlaySoundOneTime(std::string name);
	//�Đ��J�E���g���Z�b�g
	void ResetCount(std::string name);

	//���ׂẲ����X�g�b�v
	void AllStopSound();


private:

	std::unordered_map< std:: string, int>count_;

protected:


	SoundManager();
	~SoundManager();

	//map
	std::unordered_map < std::string, int> soundMap_;


};

