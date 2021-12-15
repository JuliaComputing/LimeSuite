# Lime Suite

Lime Suite is a collection of software supporting several hardware platforms 
based on the LMS7002M transceiver RFIC, such as LimeSDR family. It contains the
following components:
* **LimeSuite library** that provides C-style API;
* **LimeSuiteGUI** application for accessing low-level chip and board settings, displaying FFT, updating firmware and more;
* **SoapyLMS** plugin for LimeSDR support in SoapySDR;
* **LimeUtil** command line tool for listing LimeSDR devices and updating firmware;
* **LimeQuickTest** application to run some basic tests;
* **LimeSuite API examples** (basicRX, basicTX, singleRX, dualRXTX, gpio_example);
* **Octave plugin** (provides some basic functionality only);

This branch is a WIP fork adding support for Fairwave's XTRX, with [LiteX-based
gateware](https://github.com/enjoy-digital/xtrx_julia/), interfaced over PCIe
using [LitePCIe](https://github.com/JuliaComputing/litepcie). Build
instructions:

```
LITEPCIE_ROOT=/path/to/xtrx_julia/software cmake -DENABLE_XTRX=yes -DCMAKE_BUILD_TYPE=Debug ...
```

## Build Status
- GitHub: ![Cross platform build status](https://github.com/myriadrf/LimeSuite/actions/workflows/cmake.yml/badge.svg)

## Documentation

Find build and install instructions for Lime Suite on the wiki:

* http://wiki.myriadrf.org/Lime_Suite

Information about LimeSDR boards:

* https://wiki.myriadrf.org/LimeSDR

## Help and support

The discourse forum is a good way to find help and discuss topics:

* https://discourse.myriadrf.org/
