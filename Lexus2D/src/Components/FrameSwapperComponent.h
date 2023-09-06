#ifndef FRAMESWAPPERCOMPONENT_H
#define FRAMESWAPPERCOMPONENT_H

#include <SDL.h>

struct FrameSwapperComponent
{
	int framesCount;
	int currentFrameIndex;
	double frameTime;
	bool shouldLoop;
	int startTime;
	bool isRunning;

	FrameSwapperComponent(int i_frameCount = 1, double i_frameTime = 1.0, bool i_shouldLoop = true, bool i_isRunning = true) {
		framesCount = i_frameCount;
		currentFrameIndex = 0;
		frameTime = i_frameTime;
		shouldLoop = i_shouldLoop;
		isRunning = i_isRunning;
		startTime = SDL_GetTicks();
	}
};

#endif 
