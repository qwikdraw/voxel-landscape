#pragma once

#include "util_inc.hpp"
#include "Time.hpp"
#include "Window.hpp"
#include "Text.hpp"

class	FPSDisplay
{
	int _index;
	double _times[60];
	double _timer;
	Time _clock;
	Text* _text = nullptr;

public:

	FPSDisplay();
	~FPSDisplay();
	void	Render(Window&);
};
