#include <iostream>
#include <string>
#include <iostream>
#include <fcntl.h>
#include <math.h>
#include <unistd.h>

#include "joystick.h"

////////////////////////////////////////////////////////////////////////////////
CJoystick::CJoystick(const std::string& strDevName) :
	m_bActive(false),
	m_nFD(0)
{
	m_pJSEvent = new js_event();
	m_pJSState = new stJSState();
	m_nFD = open(strDevName.c_str(), O_RDONLY | O_NONBLOCK);

	if(m_nFD > 0) {
		char cName[255];
		ioctl(m_nFD, JSIOCGNAME(256), cName);
		m_strName= cName;
		ioctl(m_nFD, JSIOCGVERSION, &m_nVersion);
		ioctl(m_nFD, JSIOCGAXES, &m_nAxes);
		ioctl(m_nFD, JSIOCGBUTTONS, &m_nButtons);

		std::cout << "   Name: " << m_strName << std::endl;
		std::cout << "Version: " << m_nVersion << std::endl;
		std::cout << "   Axes: " << (int)m_nAxes << std::endl;
		std::cout << "Buttons: " << (int)m_nButtons << std::endl;

		m_pJSState->vAxis.reserve(m_nAxes);
		m_pJSState->vButton.reserve(m_nButtons);
		m_pJSState->vButton[1]=0;
		m_bActive = true;

		m_EventThread = std::thread(EventLoop, this);
	}
}

////////////////////////////////////////////////////////////////////////////////
CJoystick::~CJoystick()
{
	if(m_nFD > 0) {
		m_bActive = false;
		m_EventThread.join();
		close(m_nFD);
	}

	delete m_pJSState;
	delete m_pJSEvent;
	m_nFD = 0;
}

////////////////////////////////////////////////////////////////////////////////
void CJoystick::EventLoop(CJoystick* pJoystick)
{
	while(pJoystick->m_bActive)
		pJoystick->ReadEvent();
}

////////////////////////////////////////////////////////////////////////////////
void CJoystick::ReadEvent()
{
	int nBytes = read(m_nFD, m_pJSEvent, sizeof(js_event));

	if(nBytes > 0) {
		if(m_pJSEvent->type & JS_EVENT_INIT)
		{
			std::cout << "joystick sends init events" << std::endl;
			return;
		}

		m_pJSEvent->type &= ~JS_EVENT_INIT;
		if(m_pJSEvent->type & JS_EVENT_BUTTON)
		{
			m_pJSState->vButton[m_pJSEvent->number] = m_pJSEvent->value;

			if(m_pJSEvent->value)
				ButtonPressed(m_pJSEvent->number);
			else
				ButtonReleased(m_pJSEvent->number);
		}
		if(m_pJSEvent->type & JS_EVENT_AXIS)
		{
			m_pJSState->vAxis[m_pJSEvent->number] = m_pJSEvent->value;
			AxisMoved(m_pJSEvent->number, m_pJSEvent->value);
		}
	}
}

////////////////////////////////////////////////////////////////////////////////
stJSPosition CJoystick::GetPosition(int nAxis)
{
	stJSPosition pos;

	if(nAxis >= 0 && nAxis < m_nAxes) {
		int nXAxis = nAxis * 2;
		int nYAxis = nAxis * 2 + 1;

		float fX0 = m_pJSState->vAxis[nXAxis] / 32767.0f;
		float fY0 = m_pJSState->vAxis[nYAxis] / 32767.0f;
		float fX = fX0 * sqrt(1 - pow(fY0, 2) / 2.0f);
		float fY = fY0 * sqrt(1 - pow(fX0, 2) / 2.0f);

		pos.fX = fX0;
		pos.fY = fY0;
		pos.fTheta = atan2(fY, fX);
		pos.fR = sqrt(pow(fY, 2) + pow(fX, 2));
	} else {
		pos.fTheta = pos.fR = pos.fX = pos.fY = 0;
	}

	return pos;
}

////////////////////////////////////////////////////////////////////////////////
bool CJoystick::IsButtonPressed(int nButton) {
	return nButton >= 0 && nButton < m_nButtons ? m_pJSState->vButton[nButton] : false;
}