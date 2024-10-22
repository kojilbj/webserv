#include "Protocol.hpp"

using namespace Wbsv;

Protocol::~Protocol()
{
	std::vector<PhaseHandler*>::iterator it;
	for (it = ph.begin(); it != ph.end(); it++)
	{
		delete *it;
	}
}
