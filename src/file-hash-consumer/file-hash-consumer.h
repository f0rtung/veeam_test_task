#pragma once

#include "hash.h"
#include "../file-writer/text-file-writer.h"
#include "../common/exception_ptr_wrapper.h"

#include <atomic>
#include <chrono>
#include <condition_variable>
#include <map>
#include <mutex>
#include <string>
#include <thread>
#include <vector>

namespace file_signature {

    namespace ch = std::chrono;

    class file_hash_consumer
    {
        using hashes_map_t = std::map<std::size_t, std::string>;
        using hashes_map_cit_t = hashes_map_t::const_iterator;

    public:
        file_hash_consumer( file_writer_iface_ptr file_writer, ch::microseconds write_interval = ch::microseconds( 100 ) );
        void start( );
        void stop( );
        void add_hash( hash_t &&hash );

    private:
        void hash_processing( );
        void move_from_hashes_buffer( );
        hashes_map_cit_t find_end_hashes_index_sequence( );
        void write_hashes( hashes_map_cit_t begin, hashes_map_cit_t end );

    private:
        file_writer_iface_ptr file_writer_;
        std::mutex hashes_buffer_guard_;
        std::vector<hash_t> hashes_buffer_;
        hashes_map_t hashes_;
        std::size_t curr_hash_idx_;
        std::mutex processing_thread_m_;
        std::thread processing_thread_;
        std::condition_variable processing_thread_cv_;
        exception_ptr_wrapper processing_exception_;
        std::atomic_bool active_;
        const ch::microseconds write_interval_;
    };

    using file_hash_consumer_ptr = std::unique_ptr<file_hash_consumer>;

} // namespace file_signature
