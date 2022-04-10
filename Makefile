ARCH ?= arm64
CROSS_COMPILE ?= aarch64-linux-gnu-
SOC ?= IMX8QM

kbuild:
	$(CXX) test.cpp -o test -I /home/.conan/data/aarch64/boost/1.67.0/scouchi/shared/package/8c2ffdb236c63cee13119da8304029707ac36735/include -I /home/dar/project/ARM/Ctest/ros_canopen_test/socketcan_interface/include -L /home/.conan/data/aarch64/boost/1.67.0/scouchi/shared/package/8c2ffdb236c63cee13119da8304029707ac36735/lib -Wall -pthread -lboost_system -lboost_thread -O2
clean:
	rm test -rf