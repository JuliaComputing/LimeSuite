/**
    @file ConnectionCTRXEntry.cpp
    @author Julia Computing
    @brief Implementation of XTRX board connection.
*/
#ifdef __unix__
#include <unistd.h>
#endif
#include "ConnectionXTRX.h"
using namespace lime;
#include <fstream>


void __loadConnectionXTRXEntry(void)
{
    static ConnectionXTRXEntry SPIEntry;
}


ConnectionXTRXEntry::ConnectionXTRXEntry(void):
    ConnectionRegistryEntry("XTRX")
{
}

ConnectionXTRXEntry::~ConnectionXTRXEntry(void)
{
}

std::vector<ConnectionHandle> ConnectionXTRXEntry::enumerate(const ConnectionHandle &hint)
{
    std::vector<ConnectionHandle> handles;
    ConnectionHandle handle;
    handle.media = "PCIe";

    if (access("/dev/litepcie0", F_OK ) != -1)
    {
        handle.name = "Fairwaves XTRX (over LitePCIe)";
        handle.index = 0;
        handles.push_back(handle);
    }

    return handles;
}

IConnection *ConnectionXTRXEntry::make(const ConnectionHandle &handle)
{
    return new ConnectionXTRX(handle.index);
}
