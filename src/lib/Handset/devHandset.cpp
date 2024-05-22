#include "targets.h"

#ifdef TARGET_TX

#include "CRSF.h"
#include "CRSFHandset.h"
#include "POWERMGNT.h"
#include "devHandset.h"

#if defined(PLATFORM_ESP32)
#include "AutoDetect.h"
#endif

Handset *handset;

static void initialize()
{
#if defined(PLATFORM_ESP32)
    if (GPIO_PIN_RCSIGNAL_RX == GPIO_PIN_RCSIGNAL_TX)
    {
        handset = new AutoDetect();
        return;
    }
#endif
    handset = new CRSFHandset();
}

static int start()
{
    handset->Begin();
#if defined(DEBUG_TX_FREERUN)
    if (!handset->connect())
    {
        ERRLN("CRSF::connected has not been initialised");
    }
#endif
    return DURATION_IMMEDIATELY;
}

static int timeout()
{
    handset->handleInput();
    return DURATION_IMMEDIATELY;
}

static int event(bool timeout_expired)
{
    // An event should be generated every time the TX power changes
    CRSF::LinkStatistics.uplink_TX_Power = powerToCrsfPower(PowerLevelContainer::currPower());
    return DURATION_IGNORE;
}

device_t Handset_device = {
    .initialize = initialize,
    .start = start,
    .event = event,
    .timeout = timeout};
#endif
