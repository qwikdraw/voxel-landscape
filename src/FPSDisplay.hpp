#pragma once

#include "voxel.hpp"
#include "Time.hpp"

class	FPSDisplay
{
	int _index;
	double _times[60];
	double _timer;
	Time _clock;	

public:

	FPSDisplay(void);
	void	Render(void);
};
