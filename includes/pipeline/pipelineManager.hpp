#pragma once

#include <memory>
namespace Volly {
    class PipelineManager {
        public:

        class PipelineManagerImpl;

        PipelineManager(std::unique_ptr<PipelineManagerImpl> impl);
        ~PipelineManager();

        private:

        std::unique_ptr<PipelineManagerImpl> impl;
    };
}
