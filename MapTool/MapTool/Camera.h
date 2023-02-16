#pragma once
class Camera
{
	static Camera* m_inst;
	int m_xpos = 0, m_ypos = 0;

	Camera();
	~Camera();
public:
	static Camera* GetInst() {
		if (m_inst == nullptr)
			m_inst = new Camera;
		return m_inst;
	}

	static void DestroyInst() {
		if (m_inst != nullptr)
			delete m_inst;
		m_inst = nullptr;
	}

	int GetXPos() const { return m_xpos; }
	int GetYPos() const { return m_ypos; }

	void UpdateXPos(int _xpos) { m_xpos += _xpos; }
	void UpdateYPos(int _ypos) { m_ypos += _ypos; }
};

