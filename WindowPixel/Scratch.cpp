#include "Scratch.h"
#include "CustomRunner.h"

Scratch::Scratch()
{

}
Scratch::~Scratch()
{
}

void Scratch::APP(CustomRunner& customRunner)
{

	int x = 0;
	int y = 0;

	customRunner.RenderWeirdGradient(x, y);

}


