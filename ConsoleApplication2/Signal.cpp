#include "stdafx.h"
#include "Signal.h"

/**
*  @brief output class signal
*  @param an ostream object
*  @param class signal
*  @return an ostream object
*/
std::ostream & operator<<(std::ostream & os, const Signal & signal)
{
	os << "Signal name: " << signal.Name() <<
		"\tSignal startbit: " << static_cast<int>(signal.StartBit()) <<
		"\tSignal size: " << static_cast<int>(signal.SignalSize()) <<
		"\tSignal factor: " << signal.Factor() <<
		"\tSignal offset: " << signal.Offset() <<
		"\tSignal minimum: " << signal.Minimum() <<
		"\tSignal maximum: " << signal.Maximum() <<
		"\tSignal unit: " << signal.Unit() <<
		"\tSignal receivers: ";
	for (auto iter = signal.Receivers().begin(); iter < signal.Receivers().end(); iter++)
	{
		os << *iter << ' ';
	}
	return os;
}