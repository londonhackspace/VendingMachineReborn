#include "interfaces/ICardReader.h"
#include "interfaces/IPollable.h"
#include "interfaces/ICore.h"
#include "interfaces/IConfigPersistence.h"

namespace VendingMachine { namespace Core {

ICardReader::~ICardReader() {}
IPollable::~IPollable() {}
ICore::~ICore() {}
IConfigPersistence::~IConfigPersistence() {}

}}
