#pragma once

#include "interfaces/IConfigPersistence.h"

namespace VendingMachine { namespace Core {

/**
 * \brief A class implemented by a particular HAL layer,
 *        in order to allow access to implementations
 *
 */
class HALFactory
{
public:
    IConfigPersistence* getConfigPersistence();
};

}}
