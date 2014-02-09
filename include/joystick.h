#ifndef JOYSTICK_H
#define JOYSTICK_H

#include <thread>
#include <linux/joystick.h>
#include <vector>
#include <string>

struct stJSPosition {
	float fTheta, fR, fX, fY;
};

struct stJSState {
	std::vector<short> vButton;
	std::vector<short> vAxis;
};

class CJoystick {
public:
	CJoystick(const std::string& strDevName);
	~CJoystick();

private:
	std::thread m_EventThread;
	bool 		m_bActive;
	int 		m_nFD;
	js_event*	m_pJSEvent;
	stJSState*	m_pJSState;
	uint32_t	m_nVersion;
	uint8_t		m_nAxes;
	uint8_t		m_nButtons;
	std::string m_strName;

public:
	virtual void ButtonPressed(int nButton) = 0;
	virtual void ButtonReleased(int nButton) = 0;
	virtual void AxisMoved(int nAxis, int nValue) = 0;

	static void EventLoop(CJoystick* pJoystick);
	void ReadEvent();

	stJSPosition GetPosition(int nAxis);
	bool IsButtonPressed(int nButton);
};

#endif
