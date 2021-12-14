/**
    @file ConnectionXTRX.h
    @author Julia Computing
*/

#pragma once

#include <atomic>
#include <mutex>
#include <queue>

#include "ConnectionRegistry.h"
#include "LMS64CProtocol.h"
#include "dataTypes.h"

namespace lime{

class ConnectionXTRX : public IConnection
{
public:
    ConnectionXTRX(const unsigned index);
    ~ConnectionXTRX(void);

    int Open(const unsigned index);
    void Close();
    bool IsOpen();

    // Serial API
    int TransactSPI(const int addr, const uint32_t *writeData, uint32_t *readData, const size_t size) override;
    int WriteLMS7002MSPI(const uint32_t *writeData, size_t size, unsigned periphID = 0) override;
    int ReadLMS7002MSPI(const uint32_t *writeData, uint32_t *readData, size_t size, unsigned periphID = 0) override;

    // Register API
    int WriteRegisters(const uint32_t *addrs, const uint32_t *data, const size_t size) override;
    int ReadRegisters(const uint32_t *addrs, uint32_t *data, const size_t size) override;

    DeviceInfo GetDeviceInfo(void) override;

private:
    int fd;
};

class ConnectionXTRXEntry : public ConnectionRegistryEntry
{
public:
    ConnectionXTRXEntry(void);

    ~ConnectionXTRXEntry(void);

    std::vector<ConnectionHandle> enumerate(const ConnectionHandle &hint);

    IConnection *make(const ConnectionHandle &handle);
};

}
