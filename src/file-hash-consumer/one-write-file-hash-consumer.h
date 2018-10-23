#pragma once

#include "file-hash-consumer-iface.h"
#include "../file-writer/file-writer-iface.h"

#include <vector>

namespace file_signature {

    class one_write_file_hash_consumer
        : public file_hash_consumer_iface
    {
    public:
        one_write_file_hash_consumer( file_writer_iface_ptr file_writer, std::size_t chunks_count );
        void start( ) override;
        void stop( ) override;
        void add_hash( hash_t &&hash ) override;

    private:
        file_writer_iface_ptr file_writer_;
        std::vector<std::string> hashes_;
    };

    using one_write_file_hash_consumer_ptr = std::unique_ptr<one_write_file_hash_consumer>;

} // namespace file_signature
