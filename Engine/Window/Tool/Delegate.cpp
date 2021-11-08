#include "Delegate.h"

Delegate::delenode::~delenode()
{
	while (useages.size() != 0)
	{
		(*useages.begin())->RemoveNode(this);
	}
}
