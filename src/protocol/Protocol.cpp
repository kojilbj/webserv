#include "Protocol.hpp"

using namespace Wbsv;

extern Event* ev;

Protocol::~Protocol()
{
	std::vector<PhaseHandler*>::iterator it;
	for (it = ph.begin(); it != ph.end(); it++)
	{
		delete *it;
	}
	delete upstream;
}
