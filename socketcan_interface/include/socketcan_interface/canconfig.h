#ifndef _CAN_CONFIG_H_
#define _CAN_CONFIG_H_
#include <iostream>

class CanConfig
{
public:
    CanConfig(){};
    ~CanConfig(){};
    //dev:can0,can1
    //baudrate:250000,500000
    void config(std::string dev, std::string baudrate){
        auto can_up_cmd = "ifconfig " + dev + " down";
        system(can_up_cmd.c_str());
        system("sleep 0.5");
        std::string can_set_cmd = "";
        if(baudrate == "20000")
            can_set_cmd = b_20k;
        else if(baudrate == "50000")
            can_set_cmd = b_50k;
        else if(baudrate == "100000")
            can_set_cmd = b_100k;
        else if(baudrate == "125000")
            can_set_cmd = b_125k;
        else if(baudrate == "250000")
            can_set_cmd = b_250k;
        else if(baudrate == "500000")
            can_set_cmd = b_500k;
        else if(baudrate == "1000000")
            can_set_cmd = b_1000k;
        else
            can_set_cmd = b_250k;
        system(can_set_cmd.c_str());
        system("sleep 0.5");
        auto can_set_tx_cmd = "ip link set " + dev + " txqueuelen " + "1000";
        system(can_set_tx_cmd.c_str());
        system("sleep 0.1");
        can_up_cmd = "ifconfig " + dev + " up";
        system(can_up_cmd.c_str());
    }
private:
    const std::string b_20k = "ip link set can0 type can tq 625 prop-seg 34 phase-seg1 35 phase-seg2 10 sjw 4 restart-ms 50";
    const std::string b_50k = "ip link set can0 type can tq 250 prop-seg 34 phase-seg1 35 phase-seg2 10 sjw 4 restart-ms 50";
    const std::string b_100k = "ip link set can0 type can tq 125 prop-seg 34 phase-seg1 35 phase-seg2 10 sjw 4 restart-ms 50";
    const std::string b_125k = "ip link set can0 type can tq 100 prop-seg 34 phase-seg1 35 phase-seg2 10 sjw 8 restart-ms 50";
    const std::string b_250k = "ip link set can0 type can tq 50 prop-seg 34 phase-seg1 35 phase-seg2 10 sjw 8 restart-ms 50";
    const std::string b_500k = "ip link set can0 type can tq 25 prop-seg 34 phase-seg1 35 phase-seg2 10 sjw 8 restart-ms 50";
    const std::string b_1000k = "ip link set can0 type can tq 25 prop-seg 14 phase-seg1 15 phase-seg2 10 sjw 8 restart-ms 50";
};

#endif