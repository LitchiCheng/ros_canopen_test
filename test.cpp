#include <socketcan_interface/threading.h>
#include <socketcan_interface/socketcan.h>
#include <string>
#include <boost/make_shared.hpp>
#include <socketcan_interface/string.h>

class TopicToSocketCAN
{
  public:
    TopicToSocketCAN(boost::shared_ptr<can::DriverInterface> driver){
        driver_ = driver;
    }
    void setup(){
        state_listener_ = driver_->createStateListener(
        can::StateInterface::StateDelegate(this, &TopicToSocketCAN::stateCallback));
    };

    void sendMsg(const can::Frame & msg){
        bool res = driver_->send(msg);
        if (!res)
        {
            printf("Failed to send message: %s.\r\n", can::tostring(msg, true).c_str());
        }else{
            printf("send msg %s\r\n", can::tostring(msg, true).c_str());
        }
    }

  private:
    boost::shared_ptr<can::DriverInterface> driver_;

    can::StateInterface::StateListener::Ptr state_listener_;

    void stateCallback(const can::State & s){
        std::string err;
        driver_->translateError(s.internal_error, err);
        if (!s.internal_error)
        {
            printf("State: %s, asio: %s\r\n", err.c_str(), s.error_code.message().c_str());
        }
        else
        {
            printf("Error: %s, asio: %s\r\n", err.c_str(), s.error_code.message().c_str());
        }
    }
};

class SocketCANToTopic
{
  public:
    SocketCANToTopic(boost::shared_ptr<can::DriverInterface> driver){
        driver_ = driver;
    }
    void setup(){
        // register handler for frames and state changes.
        frame_listener_ = driver_->createMsgListener(
                can::CommInterface::FrameDelegate(this, &SocketCANToTopic::frameCallback));

        state_listener_ = driver_->createStateListener(
                can::StateInterface::StateDelegate(this, &SocketCANToTopic::stateCallback));
    }

  private:
    boost::shared_ptr<can::DriverInterface> driver_;

    can::CommInterface::FrameListener::Ptr frame_listener_;
    can::StateInterface::StateListener::Ptr state_listener_;

    void frameCallback(const can::Frame& f){
        can::Frame frame = f;  // copy the frame first, cannot call isValid() on const.
        if (!frame.isValid())
        {
            printf("Invalid frame from SocketCAN: id: %#04x, length: %d, is_extended: %d, is_error: %d, is_rtr: %d\r\n",
                        f.id, f.dlc, f.is_extended, f.is_error, f.is_rtr);
            return;
        }
        else
        {
            if (f.is_error)
            {
                // can::tostring cannot be used for dlc > 8 frames. It causes an crash
                // due to usage of boost::array for the data array. The should always work.
                printf("Received frame is error: %s\r\n", can::tostring(f, true).c_str());
            }
        }
        printf("Received frame: %s\r\n", can::tostring(f, true).c_str());
    }

    void stateCallback(const can::State & s){
        std::string err;
        driver_->translateError(s.internal_error, err);
        if (!s.internal_error)
        {
            printf("State: %s, asio: %s\r\n", err.c_str(), s.error_code.message().c_str());
        }
        else
        {
            printf("Error: %s, asio: %s\r\n", err.c_str(), s.error_code.message().c_str());
        }
    };
};

int main(int argc, char const *argv[])
{
    std::string can_device = "can0";

    boost::shared_ptr<can::ThreadedSocketCANInterface> driver = boost::make_shared<can::ThreadedSocketCANInterface>();

    if (!driver->init(can_device, 0))  // initialize device at can_device, 0 for no loopback.
    {
        printf("Failed to initialize can_device at %s\r\n", can_device.c_str());
        return 1;
    }
    else
    {
        printf("Successfully connected to %s.\r\n", can_device.c_str());
    }

    std::string can_device1 = "can1";

    boost::shared_ptr<can::ThreadedSocketCANInterface> driver1 = boost::make_shared<can::ThreadedSocketCANInterface>();

    if (!driver1->init(can_device1, 0))  // initialize device at can_device, 0 for no loopback.
    {
        printf("Failed to initialize can_device at %s\r\n", can_device1.c_str());
        return 1;
    }
    else
    {
        printf("Successfully connected to %s.\r\n", can_device1.c_str());
    }

    TopicToSocketCAN to_socketcan_bridge(driver);
    to_socketcan_bridge.setup();

    SocketCANToTopic to_topic_bridge(driver1);
    to_topic_bridge.setup();

    while (1)
    {
        can::Frame msg;
        msg.dlc = 8;
        msg.id = 0x601;
        msg.is_extended = false;
        for (int i = 0; i < 8; i++)  // always copy all data, regardless of dlc.
        {
            msg.data[i] = i;
        }
        to_socketcan_bridge.sendMsg(msg);
        usleep(100000);
    }

    driver->shutdown();
    driver.reset();
    return 0;
}


