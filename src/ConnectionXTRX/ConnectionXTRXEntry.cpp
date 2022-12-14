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

    char litepcie_device_string[1024];
    for(int i = 0; i <= 10; i++) {
        if (hint.index != -1 and hint.index != i) continue;
        ConnectionHandle handle;
        handle.media = "PCIe";
        snprintf(litepcie_device_string, sizeof(litepcie_device_string), "/dev/litepcie%d", i);
        if (access(litepcie_device_string, F_OK ) != -1)
        {
            handle.name = "Fairwaves XTRX (over LitePCIe)";
            handle.index = i;
            handles.push_back(handle);
        }
    }

    return handles;
}

IConnection *ConnectionXTRXEntry::make(const ConnectionHandle &handle)
{
    return new ConnectionXTRX(handle.index);
}
