#include <time.h>

#include "xbmcjoystick.h"

#define BUTTON_REPEAT_TIME 0.350

////////////////////////////////////////////////////////////////////////////////
CXBMCJoystick::CXBMCJoystick(CXBMCClient* pXBMC, const std::string strDevName) :
	CJoystick(strDevName)
{
	m_nLastButton = 0;
	m_nLastAxis = 0;
	m_tLastButtonPressed = time(NULL);
	m_tLastAxisMoved = time(NULL);
	m_pXBMC = pXBMC;
}

////////////////////////////////////////////////////////////////////////////////
CXBMCJoystick::~CXBMCJoystick()
{
	m_pXBMC = nullptr;
}

////////////////////////////////////////////////////////////////////////////////
void CXBMCJoystick::ButtonPressed(int nButton)
{
	float fSeconds = difftime(time(NULL), m_tLastButtonPressed);
	if(nButton == m_nLastButton && fSeconds < BUTTON_REPEAT_TIME)
		return;

	m_nLastButton = nButton;
	m_tLastButtonPressed = time(NULL);

	if(m_pXBMC) {
		switch(nButton) {
			case 0:
				m_pXBMC->SendButton("escape", "KB", BTN_USE_NAME|BTN_NO_REPEAT);
				break;
			case 1:
				m_pXBMC->SendButton("backspace", "KB", BTN_USE_NAME|BTN_NO_REPEAT);
				break;
			case 2:
				m_pXBMC->SendButton("enter", "KB", BTN_USE_NAME|BTN_NO_REPEAT);
				break;
			case 3:
				m_pXBMC->SendButton("p", "KB", BTN_USE_NAME|BTN_NO_REPEAT);
				break;
			case 4:
				m_pXBMC->SendButton("c", "KB", BTN_USE_NAME|BTN_NO_REPEAT);
				break;
			case 5:
				m_pXBMC->SendButton("space", "KB", BTN_USE_NAME|BTN_NO_REPEAT);
				break;
			default:
				std::cerr << "unknown button pressed : " << nButton << std::endl;
		}
	}
}

////////////////////////////////////////////////////////////////////////////////
void CXBMCJoystick::ButtonReleased(int nButton)
{
	std::cout << "button released : " << nButton << std::endl;
}

////////////////////////////////////////////////////////////////////////////////
void CXBMCJoystick::AxisMoved(int nAxis, int nValue)
{
	float fSeconds = difftime(time(NULL), m_tLastAxisMoved);
	if(nAxis == m_nLastAxis && fSeconds < BUTTON_REPEAT_TIME)
		return;

	m_nLastAxis = nAxis;
	m_tLastAxisMoved = time(NULL);

	if(m_pXBMC) {
		switch(nAxis) {
			case 5:	// left/right in analog mode
				if(nValue < 0)
					m_pXBMC->SendButton("left", "KB", BTN_USE_NAME|BTN_NO_REPEAT);
				else
					m_pXBMC->SendButton("right", "KB", BTN_USE_NAME|BTN_NO_REPEAT);
				break;
			case 6: // up/down in analog mode
				if(nValue < 0)
					m_pXBMC->SendButton("up", "KB", BTN_USE_NAME|BTN_NO_REPEAT);
				else
					m_pXBMC->SendButton("down", "KB", BTN_USE_NAME|BTN_NO_REPEAT);
				break;
			default:
				std::cout << "unknown axis moved : " << nAxis << std::endl;
		}
	}
	std::cout << "axis moved : " << nAxis << std::endl;
}
