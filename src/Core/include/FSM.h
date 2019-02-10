#pragma once

namespace VendingMachine { namespace Core {

/**
 * \brief Common logic for an FSM
 *
 * Encapsulates the logic required to implement a
 * finite state machine (or a Flying Spaghetti Monster)
 * 
 * Main use is to allow consistent logging of state transitions
 */
class FSM
{
public:
    /**
     * \brief Constructor for FSM
     *
     * \param[in] name null-terminated string describing FSM name, for logging
     * \param[in] initialState Initial state value for FSM
     */
    FSM(const char* name, int initialState);
    ~FSM();

    int getState() const { return state; }
    void setState(int state);

    const char* getName() const { return name; }

private:
    const char* name;
    int state;
};

}}
