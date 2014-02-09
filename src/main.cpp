#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>

#include "xbmcclient.h"
#include "xbmcjoystick.h"

int main(int argc, char** argv)
{
	CXBMCClient xbmc("192.168.0.12", 9777);
	xbmc.SendHELO("Joystick", ICON_PNG, "resources/joystick.png");

	CXBMCJoystick js(&xbmc, "/dev/input/js1");

	while(!(js.IsButtonPressed(0) && js.IsButtonPressed(1)))
	{
		usleep(50);
	}

	std::cout << "I'm leaving..." << std::endl;
	return 0;
}
