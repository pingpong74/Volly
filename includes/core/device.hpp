#pragma once

#include <memory>

namespace Volly {

    class Instance;

    class Device {
        public:
        class DeviceImpl;

        Device(std::unique_ptr<DeviceImpl> devImpl);
        ~Device();

        private:

        std::unique_ptr<DeviceImpl> impl;
    };
}
