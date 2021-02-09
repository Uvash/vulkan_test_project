#pragma once
#include "stdafx.h"
#include "WindowManager.h"

class DiplomApp
{

private:
	WindowManager whindow;

	void init();
	void mainLoop();
	void cleanup();
public:
	void run();
};