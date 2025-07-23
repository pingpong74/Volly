#pragma once

#include <memory>

namespace Volly {

    class Buffer;
    class BufferID{
        public:

        BufferID(std::shared_ptr<Buffer> impl);

        private:
        std::shared_ptr<Buffer> impl;
        bool IsMapped = false;
    };

    struct ImageID {

    };
}
