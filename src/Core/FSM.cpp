#include "FSM.h"

namespace VendingMachine { namespace Core {

FSM::FSM(const char* name, int initialState) :
    name(name),
    state(initialState)
{

}

FSM::~FSM()
{

}

void FSM::setState(int state)
{

    this->state = state;
}

}}
