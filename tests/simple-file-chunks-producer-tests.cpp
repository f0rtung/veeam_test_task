#include "../src/file-chunks-producer/simple-file-chunks-producer.h"
#include "stubs/file-reader-stub.h"
#include "utils/utils.h"

#include <atomic>
#include <condition_variable>
#include <map>
#include <thread>
#include <gmock/gmock.h>

using namespace testing;
using namespace file_signature;

class simple_file_chunks_producer_test : public Test
{
public:
    void read_chunk( simple_file_chunks_producer &chunk_producer, std::atomic_size_t &threads_count )
    {
        ++threads_count;
        run_cv_.notify_one( );
        std::unique_lock<std::mutex> lock{ run_guard_ };
        lock.unlock( );

        try {
            while ( true ) {
                chunk_t chunk{ chunk_producer.chunk( ) };
                std::lock_guard<std::mutex> _{ chunks_guard_ };
                chunks_.emplace( chunk.index( ), chunk.buf( ) );
            }
        } catch ( const no_more_chunks_exception &ex ) {

        }
    }

    std::string make_source_file_content( )
    {
        std::string content;
        content.reserve( chunks_.size( ) * chunk_size_ );
        for ( auto &chunk_info : chunks_ ) {
            auto &chunk{ chunk_info.second };
            content.append( std::string( chunk.data( ), chunk.size( ) ) );
        }
        return content;
    }

    const std::size_t chunk_size_{ 1000 };
    std::mutex chunks_guard_;
    std::map<std::size_t, bytes> chunks_;
    std::mutex run_guard_;
    std::condition_variable run_cv_;
    const std::size_t threads_{ 5 };
};

TEST_F(simple_file_chunks_producer_test, TestUnderLoad)
{
    const std::string content{ random_string( 1024  * 1024 * 100 ) };
    auto file_reader_ptr = std::make_unique<file_reader_stub>( content );
    simple_file_chunks_producer chunk_producer{ std::move( file_reader_ptr ), threads_, chunk_size_ };

    std::unique_lock<std::mutex> lock{ run_guard_ };
    std::vector<std::thread> threads;
    std::atomic_size_t threads_count{ 0 };
    for ( std::size_t idx = 0; idx < threads_; ++idx ) {
        threads.emplace_back( std::thread( &simple_file_chunks_producer_test::read_chunk, this,
                                           std::ref( chunk_producer ), std::ref( threads_count ) ) );
    }
    run_cv_.wait( lock, [&threads_count, this] { return threads_ == threads_count; } );
    lock.unlock( );

    for ( auto &t : threads ) {
        t.join( );
    }

    ASSERT_EQ( content, make_source_file_content( ) );
}
