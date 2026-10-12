#ifndef MICROSD_H
#define MICROSD_H

#include "../GedeeldeBus/GedeeldeBus.h"

#if ((DATA_LAYER_CONFIG) & DATA_TYPE_MICROSD) || REDIRECT_SERIAL == REDIRECT_SERIAL_TO_MICROSD

  #include <SD.h>
  #if defined(ARDUINO_ARCH_ESP32)
    #include <vfs_api.h>
  #endif

struct DataMicroSD : HardwareResourceTypeSPI, SDFileSystemClass {
  DataMicroSD(GedeeldeBusNode* parent, HardwareResourcePin np_CS, HardwareResourcePin np_SCK, HardwareResourcePin np_MISO, HardwareResourcePin np_MOSI, HardwareResourceToegang Extender = HardwareResourceToegang::GEDEELD)
    : HardwareResourceTypeSPI(parent, GedeeldeBusComponent::DATA_MICROSD, np_CS, np_SCK, np_MISO, np_MOSI, Extender)
#if defined(ARDUINO_ARCH_ESP32)
    , SDFileSystemClass(FSImplPtr(new VFSImpl()))
#endif
    {}

  bool Activeren() override;
};

#endif

#endif // MICROSD_H
