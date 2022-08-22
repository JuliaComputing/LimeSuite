/**
@file	SoapyLMS7.h
@brief	Header for Soapy SDR + IConnection bindings.
@author Lime Microsystems (www.limemicro.com)
*/

#include <SoapySDR/Device.hpp>
#include <ConnectionRegistry.h>
#include <mutex>
#include <chrono>
#include <map>
#include <set>
#include "Streamer.h"
#include "liblitepcie.h"

#define BYTES_PER_SAMPLE 2 // TODO validate this 


enum class TargetDevice { CPU, GPU };

namespace lime
{
    class LIME_API LMS7_Device;
}
struct IConnectionStream;

class SoapyLMS7 : public SoapySDR::Device
{
public:
    SoapyLMS7(const lime::ConnectionHandle &handle, const SoapySDR::Kwargs &args);

    ~SoapyLMS7(void);

    /*******************************************************************
     * Identification API
     ******************************************************************/

    std::string getDriverKey(void) const;

    std::string getHardwareKey(void) const;

    SoapySDR::Kwargs getHardwareInfo(void) const;

    /*******************************************************************
     * Channels API
     ******************************************************************/

    size_t getNumChannels(const int direction) const;

    bool getFullDuplex(const int direction, const size_t channel) const;

    /*******************************************************************
     * Stream API
     ******************************************************************/
    std::vector<std::string> getStreamFormats(const int direction, const size_t channel) const;

    std::string getNativeStreamFormat(const int direction, const size_t channel, double &fullScale) const;

    SoapySDR::ArgInfoList getStreamArgsInfo(const int direction, const size_t channel) const;

    SoapySDR::Stream *setupStream(const int direction,
                                  const std::string &format,
                                  const std::vector<size_t> &channels,
                                  const SoapySDR::Kwargs &);
    void closeStream(SoapySDR::Stream *stream) override;
    int activateStream(SoapySDR::Stream *stream, const int flags,
                       const long long timeNs, const size_t numElems) override;
    int deactivateStream(SoapySDR::Stream *stream, const int flags,
                         const long long timeNs) override;

    size_t getStreamMTU(SoapySDR::Stream *stream) const override;
    size_t getNumDirectAccessBuffers(SoapySDR::Stream *stream) override;
    int getDirectAccessBufferAddrs(SoapySDR::Stream *stream,
                                   const size_t handle, void **buffs) override;
    int acquireReadBuffer(SoapySDR::Stream *stream, size_t &handl,
                          const void **buffs, int &flags, long long &timeNs,
                          const long timeoutUs) override;
    void releaseReadBuffer(SoapySDR::Stream *stream, size_t handle) override;
    int acquireWriteBuffer(SoapySDR::Stream *stream, size_t &handle,
                           void **buffs, const long timeoutUs) override;
    void releaseWriteBuffer(SoapySDR::Stream *stream, size_t handle,
                            const size_t numElems, int &flags,
                            const long long timeNs = 0) override;

    int readStream(
        SoapySDR::Stream *stream,
        void * const *buffs,
        const size_t numElems,
        int &flags,
        long long &timeNs,
        const long timeoutUs = 100000 );


    int writeStream(
            SoapySDR::Stream *stream,
            const void * const *buffs,
            const size_t numElems,
            int &flags,
            const long long timeNs = 0,
            const long timeoutUs = 100000);

    /*******************************************************************
     * Antenna API
     ******************************************************************/

    std::vector<std::string> listAntennas(const int direction, const size_t channel) const;

    void setAntenna(const int direction, const size_t channel, const std::string &name);

    std::string getAntenna(const int direction, const size_t channel) const;

    /*******************************************************************
     * Frontend corrections API
     ******************************************************************/

    bool hasDCOffsetMode(const int direction, const size_t channel) const;

    void setDCOffsetMode(const int direction, const size_t channel, const bool automatic);

    bool getDCOffsetMode(const int direction, const size_t channel) const;

    bool hasDCOffset(const int direction, const size_t channel) const;

    void setDCOffset(const int direction, const size_t channel, const std::complex<double> &offset);

    std::complex<double> getDCOffset(const int direction, const size_t channel) const;

    bool hasIQBalance(const int direction, const size_t channel) const;

    void setIQBalance(const int direction, const size_t channel, const std::complex<double> &balance);

    std::complex<double> getIQBalance(const int direction, const size_t channel) const;

    /*******************************************************************
     * Gain API
     ******************************************************************/

    std::vector<std::string> listGains(const int direction, const size_t channel) const;

    void setGain(const int direction, const size_t channel, const double value) override;

    double getGain(const int direction, const size_t channel) const;

    void setGain(const int direction, const size_t channel, const std::string &name, const double value);

    double getGain(const int direction, const size_t channel, const std::string &name) const;

    SoapySDR::Range getGainRange(const int direction, const size_t channel) const;

    SoapySDR::Range getGainRange(const int direction, const size_t channel, const std::string &name) const;

    /*******************************************************************
     * Frequency API
     ******************************************************************/

    SoapySDR::ArgInfoList getFrequencyArgsInfo(const int direction, const size_t channel) const;

    void setFrequency(const int direction, const size_t channel, const double frequency, const SoapySDR::Kwargs &args = SoapySDR::Kwargs());

    void setFrequency(const int direction, const size_t channel, const std::string &name, const double frequency, const SoapySDR::Kwargs &args = SoapySDR::Kwargs());

    double getFrequency(const int direction, const size_t channel, const std::string &name) const;

    double getFrequency(const int direction, const size_t channel) const override;

    std::vector<std::string> listFrequencies(const int direction, const size_t channel) const;

    SoapySDR::RangeList getFrequencyRange(const int direction, const size_t channel) const;

    SoapySDR::RangeList getFrequencyRange(const int direction, const size_t channel, const std::string &name) const;

    /*******************************************************************
     * Sample Rate API
     ******************************************************************/

    std::map<size_t, int> _interps;
    std::map<size_t, int> _decims;

    void setSampleRate(const int direction, const size_t channel, const double rate);

    double getSampleRate(const int direction, const size_t channel) const;

    std::vector<double> listSampleRates(const int direction, const size_t channel) const;

    SoapySDR::RangeList getSampleRateRange(const int direction, const size_t channel) const;

    /*******************************************************************
     * Bandwidth API
     ******************************************************************/

    void setBandwidth(const int direction, const size_t channel, const double bw);

    double getBandwidth(const int direction, const size_t channel) const;

    SoapySDR::RangeList getBandwidthRange(const int direction, const size_t channel) const;

    /*******************************************************************
     * Clocking API
     ******************************************************************/

    double getMasterClockRate(void) const;

    /*******************************************************************
     * Time API
     ******************************************************************/

    bool hasHardwareTime(const std::string &what = "") const;

    long long getHardwareTime(const std::string &what = "") const;

    void setHardwareTime(const long long timeNs, const std::string &what = "");

    /*******************************************************************
     * Sensor API
     ******************************************************************/

    std::vector<std::string> listSensors(void) const;

    SoapySDR::ArgInfo getSensorInfo(const std::string &name) const;

    std::string readSensor(const std::string &name) const;

    std::vector<std::string> listSensors(const int direction, const size_t channel) const;

    SoapySDR::ArgInfo getSensorInfo(const int direction, const size_t channel, const std::string &name) const;

    std::string readSensor(const int direction, const size_t channel, const std::string &name) const;

    /*******************************************************************
     * Register API
     ******************************************************************/

    std::vector<std::string> listRegisterInterfaces(void) const;

    void writeRegister(const std::string &name, const unsigned addr, const unsigned value);

    unsigned readRegister(const std::string &name, const unsigned addr) const;

    void writeRegister(const unsigned addr, const unsigned value);

    unsigned readRegister(const unsigned addr) const;

    /*******************************************************************
     * Settings API
     ******************************************************************/

    SoapySDR::ArgInfoList getSettingInfo(void) const;

    void writeSetting(const std::string &key, const std::string &value);

    SoapySDR::ArgInfoList getSettingInfo(const int direction, const size_t channel) const;

    void writeSetting(const int direction, const size_t channel, const std::string &key, const std::string &value);

    std::string readSetting(const std::string &key) const;

    std::string readSetting(const int direction, const size_t channel, const std::string &key) const;

    /*******************************************************************
     * GPIO API
     ******************************************************************/

    std::vector<std::string> listGPIOBanks(void) const;

    void writeGPIO(const std::string &bank, const unsigned value);

    unsigned readGPIO(const std::string &bank) const;

    void writeGPIODir(const std::string &bank, const unsigned dir);

    unsigned readGPIODir(const std::string &bank) const;

private:

    SoapySDR::Stream *const TX_STREAM = (SoapySDR::Stream *)0x1;
    SoapySDR::Stream *const RX_STREAM = (SoapySDR::Stream *)0x2;

    struct litepcie_ioctl_mmap_dma_info _dma_mmap_info;
    TargetDevice _dma_target;
    void *_dma_buf;

    struct Stream {
        Stream() : opened(false) {}

        bool opened;
        void *buf;
        struct pollfd fds;
        int64_t hw_count, sw_count, user_count;

        int32_t remainderHandle;
        size_t remainderSamps;
        size_t remainderOffset;
        int8_t* remainderBuff;
        std::string format;
    };

    struct RXStream: Stream {
        uint32_t vga_gain;
        uint32_t lna_gain;
        uint8_t amp_gain;
        double samplerate;
        uint32_t bandwidth;
        uint64_t frequency;

        bool overflow;
    };

    struct TXStream: Stream {
        uint32_t vga_gain;
        uint8_t amp_gain;
        double samplerate;
        uint32_t bandwidth;
        uint64_t frequency;
        bool bias;

        bool underflow;

        bool burst_end;
        int32_t burst_samps;
    } ;

    RXStream _rx_stream;
    TXStream _tx_stream;

    int _fd;

    // register protection
    std::mutex _mutex;

    struct Channel{
        Channel():freq(-1),bw(-1),rf_bw(-1),cal_bw(-1),gfir_bw(-1),tst_dc(0){};
        double freq;
        double bw;
        double rf_bw;
        double cal_bw;
        double gfir_bw;
        int tst_dc;
    };

    int setBBLPF(bool direction, size_t channel, double bw);

    const SoapySDR::Kwargs _deviceArgs; //!< stash of constructor arguments
    const std::string _moduleName;
    lime::LMS7_Device * lms7Device;
    double sampleRate[2]; //sampleRate[direction]
    int oversampling;
    std::set<std::pair<int, size_t>> _channelsToCal;
    mutable std::recursive_mutex _accessMutex;
    std::vector<Channel> mChannels[2]; //mChannels[direction]
    std::set<SoapySDR::Stream *> activeStreams;
};
