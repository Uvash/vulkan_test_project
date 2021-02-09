#include "stdafx.h"
#include "DiplomApp.h"


void DiplomApp::init()
{
	whindow.windowManagerInit();
}

void DiplomApp::mainLoop()
{
	while (!whindow.shoudClose())
	{
		whindow.pollEvents();
	}
}
void DiplomApp::cleanup()
{

}

void DiplomApp::run()
{
	init();
	mainLoop();
	cleanup();
}

