#pragma once

namespace VendingMachine { namespace Core {

// enough length for acserver.lan.london.hackspace.org.uk + 1 + 4 extra for luck.
static const int SERVERNAME_LENGTH = (36 + 1 + 4);
static const int SECRET_KEY_LENGTH = 8 + 1;

static const int CONFIG_VERSION = 1;

struct Config_1
{
    int toolId; /**< ID of tool on ACServer */
    bool inService; /**< Is this machine in service? */

    char serverName[SERVERNAME_LENGTH]; /**< Hostname of ACServer */
    char syslogserver[SERVERNAME_LENGTH]; /**< Hostname of Syslog Server */
    char secret[SECRET_KEY_LENGTH]; /**< API key to talk to the acserver */
};

// \typedef typedef to map current config version to Config
typedef Config_1 Config;

}}