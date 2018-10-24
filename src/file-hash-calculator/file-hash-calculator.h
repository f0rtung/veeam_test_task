#pragma once

#include "../file-chunks-producer/file-chunks-producer-iface.h"
#include "../file-hash-consumer/file-hash-consumer-iface.h"

#include <thread>
#include <vector>

namespace file_signature {

    class file_hash_calculator
    {
    public:
        file_hash_calculator( file_chunks_producer_iface_ptr fc_producer,
                              file_hash_consumer_iface_ptr fh_consumer,
                              std::size_t threads_count );
        void calculate( );

    private:
        void start( );
        void stop( );
        void hash_calculating( );

    private:
        file_chunks_producer_iface_ptr fc_producer_;
        file_hash_consumer_iface_ptr fh_consumer_;
        std::size_t threads_count_;
        std::vector<std::thread> threads_;
    };

} // namespace file_signature
