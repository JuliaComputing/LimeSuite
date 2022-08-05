/**
    @file ConnectionXTRX.cpp
    @author Julia Computing
    @brief Implementation of Fairwaves XTRX connection over LitePCIEs.
*/

#include <linux/spi/spidev.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <fstream>

#include "Logger.h"
#include "ConnectionXTRX.h"
#include "SystemResources.h"
#include "liblitepcie.h"

using namespace std;
using namespace lime;

/** @brief Initializes port type and object necessary to communicate to LitePCIe device.
*/
ConnectionXTRX::ConnectionXTRX(const unsigned index) : fd(-1)
{
    if (Open(index) < 0)
        lime::error("Failed to open LitePCIe device");
}

/** @brief Closes connection to chip and deallocates used memory.
*/
ConnectionXTRX::~ConnectionXTRX()
{
    Close();
}

/** @brief Tries to open connected LitePCIe device and find communication endpoints.
    @return Returns 0-Success, other-EndPoints not found or device didn't connect.
*/
int ConnectionXTRX::Open(const unsigned index)
{
    Close();

    char litepcie_device[1024];
    snprintf(litepcie_device, sizeof(litepcie_device), "/dev/litepcie%d", index);
    fd = open(litepcie_device, O_RDWR);

    return IsOpen() ? 0 : -1;
}

/** @brief Closes communication to device.
*/
void ConnectionXTRX::Close()
{
    if (fd != -1)
        close(fd);
    fd = -1;
}

/** @brief Returns connection status
    @return 1-connection open, 0-connection closed.
*/
bool ConnectionXTRX::IsOpen()
{
    if (fd < 0)
        return false;
    return true;
}

int ConnectionXTRX::TransactSPI(const int addr, const uint32_t *writeData, uint32_t *readData, const size_t size)
{
    if (!this->IsOpen())
    {
        ReportError(ENOTCONN, "connection is not open");
        return -1;
    }

    //perform spi writes when there is no read data
    if (readData == nullptr) switch(addr)
    {
        case 0x10: return this->WriteLMS7002MSPI(writeData, size);
    }

    if (readData != nullptr && addr == 0x10) {
        auto val = this->ReadLMS7002MSPI(writeData, readData, size);
        printf("[TransactSPI] Asked for 0x%x, got 0x%x\n", addr, val);
        return val;
    }

    return ReportError(ENOTSUP, "unknown spi address");
}

/***********************************************************************
 * LMS7002M SPI access
 **********************************************************************/

#define LITEPCIE_SPI_CS_HIGH (0 << 0)
#define LITEPCIE_SPI_CS_LOW  (1 << 0)
#define LITEPCIE_SPI_START   (1 << 0)
#define LITEPCIE_SPI_DONE    (1 << 0)
#define LITEPCIE_SPI_LENGTH  (1 << 8)

int ConnectionXTRX::WriteLMS7002MSPI(const uint32_t *data, size_t size, unsigned /*periphID*/)
{
    //printf("[WriteLMS7002MSPI] Writing %d words\n", size);
    //for (int i = 0; i < size; i++) {
    //    // top bit is used to indicate write
    //    printf("[WriteLMS7002MSPI] Writing addr:0x%x data:0x%x\n", (data[i] >> 16) & 0x7fff, data[i] & 0xffff);
    //}
    for (unsigned i = 0; i < size; i++)
    {
        litepcie_writel(fd, CSR_LMS7002M_SPI_MOSI_ADDR, data[i]);
        litepcie_writel(fd, CSR_LMS7002M_SPI_CONTROL_ADDR, 32*LITEPCIE_SPI_LENGTH | LITEPCIE_SPI_START);
        while ((litepcie_readl(fd, CSR_LMS7002M_SPI_STATUS_ADDR) & LITEPCIE_SPI_DONE) == 0);
    }

    uint32_t data_out[size];
    
    int ret = ReadLMS7002MSPI(data, data_out, size);
    //if (ret != 0) {
    //    printf("[WriteLMS7002MSPI] read-back failed with code %d\n", ret);
    //    return 1;
    //}

    //for (unsigned i = 0; i < size; i++){
    //    if (data[i] != data_out[i])
    //        printf("[WriteLMS7002MSPI] Read back failed at word %d, expected 0x%x, got 0x%x\n", i, data[i], data_out[i]);
    //}

    return 0;
}

int ConnectionXTRX::ReadLMS7002MSPI(const uint32_t *data_in, uint32_t *data_out, size_t size, unsigned /*periphID*/)
{
    //printf("[ReadLMS7002MSPI] Reading %d words\n", size);
    for (unsigned i = 0; i < size; i++)
    {
        litepcie_writel(fd, CSR_LMS7002M_SPI_MOSI_ADDR, data_in[i]);
        litepcie_writel(fd, CSR_LMS7002M_SPI_CONTROL_ADDR, 32*LITEPCIE_SPI_LENGTH | LITEPCIE_SPI_START);
        while ((litepcie_readl(fd, CSR_LMS7002M_SPI_STATUS_ADDR) & LITEPCIE_SPI_DONE) == 0);
        data_out[i] = litepcie_readl(fd, CSR_LMS7002M_SPI_MISO_ADDR) & 0xffff;
    }
    //for (unsigned i = 0; i < size; i++)
    //    printf("[ReadLMS7002MSPI] Read addr:0x%x data:0x%x\n", data_in[i] >> 16, data_out[i]);

    return 0;
}

int ConnectionXTRX::WriteRegisters(const uint32_t *addrs, const uint32_t *vals, const size_t size)
{
    for (unsigned i = 0; i < size; i++) {
        if (addrs[i] < CSR_BASE || addrs[i] >= CSR_BASE+0x10000L)
            return ReportError(EINVAL, "attempt to write invalid register 0x%x", addrs[i]);
        litepcie_writel(fd, addrs[i], vals[i]);
    }

    return 0;
}

int ConnectionXTRX::ReadRegisters(const uint32_t *addrs, uint32_t *vals, const size_t size)
{
    for (unsigned i = 0; i < size; i++) {
        if (addrs[i] < CSR_BASE || addrs[i] >= CSR_BASE+0x10000L)
            return ReportError(EINVAL, "attempt to read invalid register 0x%x", addrs[i]);
        vals[i] = litepcie_readl(fd, addrs[i]);
    }

    return 0;
}

int ConnectionXTRX::DeviceReset(int ind)
{
    litepcie_writel(fd, CSR_LMS7002M_CONTROL_ADDR, 1*(1 << CSR_LMS7002M_CONTROL_RESET_OFFSET));
    litepcie_writel(fd, CSR_LMS7002M_CONTROL_ADDR, 0*(1 << CSR_LMS7002M_CONTROL_RESET_OFFSET));
    return 0;
}

DeviceInfo ConnectionXTRX::GetDeviceInfo(void)
{
    DeviceInfo info;
    info.deviceName = "Fairwaves-XTRX";
    info.protocolVersion = "0";
    info.firmwareVersion = "0";
    info.expansionName = "EXP_BOARD_UNKNOWN";

    uint32_t addrs[256] = {0};
    for (unsigned i = 0; i < 256; i ++)
        addrs[i] = CSR_IDENTIFIER_MEM_BASE + 4 * i;
    uint32_t vals[256] = {0};
    ReadRegisters(addrs, vals, 256);
    char id[256] = {0};
    for (unsigned i = 0; i < 256; i ++)
        id[i] = vals[i];

    info.gatewareTargetBoard = id;
    info.gatewareVersion = "";
    info.gatewareRevision = "";
    info.hardwareVersion = "";
    return info;
}
