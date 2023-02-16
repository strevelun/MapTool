#pragma once
class FileManager
{
private:
	static FileManager* m_inst;

	FileManager();
	~FileManager();

public:
	static FileManager* GetInst() {
		if (m_inst == nullptr)
			m_inst = new FileManager;
		return m_inst;
	}

	static void DestroyInst() {
		if (m_inst != nullptr)
			delete m_inst;
		m_inst = nullptr;
	}

	void SaveMap();
	void LoadMap();
};

