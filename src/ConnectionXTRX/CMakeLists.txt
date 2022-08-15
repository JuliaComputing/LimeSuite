########################################################################
## Support for Fairwaves XTRX over PCIe using LitePCIe connection
########################################################################
set(THIS_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/ConnectionXTRX)

set(CONNECTION_XTRX_SOURCES
    ${THIS_SOURCE_DIR}/ConnectionXTRXEntry.cpp
    ${THIS_SOURCE_DIR}/ConnectionXTRX.cpp
)

########################################################################
## Feature registration
########################################################################
include(FeatureSummary)
include(CMakeDependentOption)
cmake_dependent_option(ENABLE_XTRX "Enable XTRX over PCIe connection" OFF "ENABLE_LIBRARY" OFF)
add_feature_info(ConnectionXTRX ENABLE_XTRX "Fairwaves XTRX over PCIe Connection support")
if (NOT ENABLE_XTRX)
    return()
endif()

########################################################################
## LitePCIe discovery
########################################################################
find_path(LITEPCIE_KERNEL_INCLUDE_DIR litepcie.h
          REQUIRED
          HINTS
            "${LITEPCIE_KERNEL_MODULE_DIR}")
find_path(LITEPCIE_USER_INCLUDE_DIR liblitepcie.h
          REQUIRED
          HINTS
            "${LITEPCIE_INSTALL_PREFIX}"
          PATH_SUFFIXES
            include/liblitepcie)
set(LITEPCIE_INCLUDE_DIR ${LITEPCIE_KERNEL_INCLUDE_DIR} ${LITEPCIE_USER_INCLUDE_DIR})
find_library(LITEPCIE_LIBRARY litepcie
             REQUIRED
             HINTS
               "${LITEPCIE_INSTALL_PREFIX}"
             PATH_SUFFIXES
               lib)

########################################################################
## CUDA
########################################################################
find_package(CUDAToolkit)

########################################################################
## Add to library
########################################################################
target_include_directories(LimeSuite PUBLIC ${LITEPCIE_INCLUDE_DIR})
target_sources(LimeSuite PRIVATE ${CONNECTION_XTRX_SOURCES})
target_link_libraries(LimeSuite ${LITEPCIE_LIBRARY} crypt CUDA::cuda_driver)
