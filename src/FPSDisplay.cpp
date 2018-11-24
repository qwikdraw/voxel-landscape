#include "FPSDisplay.hpp"

FPSDisplay::FPSDisplay()
{
	_index = 0;
	std::memset(_times, 0, sizeof(double) * 60);
	_timer = 0;
}

FPSDisplay::~FPSDisplay()
{
	delete _text;
}

void	FPSDisplay::Render(Window& window)
{
	_clock.Step();

	_times[_index] = _clock.Delta();
	_index += 1;
	_index %= 60;

	_timer += _clock.Delta();
	if (_timer > 1)
	{
		double av = 0;

		for (auto t : _times)
			av += t;
		av /= 60;
		av = 1 / av;

		_timer = 0;

		std::stringstream ss;
		ss << std::fixed << std::setprecision(1) << av;
		delete _text;
		_text = new Text(ss.str());
	}
	window.SetRenderMask(0, 0.975, 0.05, 0.025);
	_text->Render(window.GetAspect());
	window.RemoveRenderMask();
}
