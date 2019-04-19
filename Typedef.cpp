#include "DXUT.h"

bool Frame::OnFrame()
{
	if (GetNowTime() >= frame_skip)
	{
		if (++current > end)
		{
			current = start;
		}

		frame_skip = delay + GetNowTime();
		return true;
	}
	return false;
}

void Frame::SetFrame(int start, int end, unsigned long delay)
{
	current = Frame::start = start;
	Frame::end = end;
	Frame::delay = delay;
	frame_skip = delay + GetNowTime();
}

void Frame::SetFrame(int start, int end)
{
	Frame::start = start;
	Frame::end = end;
}
