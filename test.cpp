#include <socketcan_interface/threading.h>
#include <socketcan_interface/socketcan.h>
#include <string>
#include <boost/make_shared.hpp>

int main(int argc, char const *argv[])
{
    std::string can_device = "can0";

    boost::shared_ptr<can::ThreadedSocketCANInterface> driver = boost::make_shared<can::ThreadedSocketCANInterface>();

    if (!driver->init(can_device, 0))  // initialize device at can_device, 0 for no loopback.
    {
        printf("Failed to initialize can_device at %s", can_device.c_str());
        return 1;
    }
    else
    {
        printf("Successfully connected to %s.", can_device.c_str());
    }

    // // initialize the bridge both ways.
    // socketcan_bridge::TopicToSocketCAN to_socketcan_bridge(&nh, &nh_param, driver);
    // to_socketcan_bridge.setup();

    // socketcan_bridge::SocketCANToTopic to_topic_bridge(&nh, &nh_param, driver);
    // to_topic_bridge.setup();

    // driver->send()

    driver->shutdown();
    driver.reset();
    return 0;
}


