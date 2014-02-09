#ifndef XBMCJOYSTICK_H
#define XBMCJOYSTICK_H

#include <string>
#include <time.h>

#include "joystick.h"
#include "xbmcclient.h"

class CXBMCJoystick : public CJoystick
{
public:
	CXBMCJoystick(CXBMCClient* pXBMC, const std::string strDevName);
	~CXBMCJoystick();

protected:
	CXBMCClient* m_pXBMC;
	int m_nLastButton;
	int m_nLastAxis;
	time_t m_tLastButtonPressed;
	time_t m_tLastAxisMoved;

public:
	void ButtonPressed(int nButton) override;
	void ButtonReleased(int nButton) override;
	void AxisMoved(int nAxis, int nValue) override;
};

#endif
