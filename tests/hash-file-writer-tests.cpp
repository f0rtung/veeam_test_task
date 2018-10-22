#include "../src/file-hash-consumer/file-hash-consumer.h"
#include "stubs/file-writer-stub.h"

#include <chrono>
#include <vector>
#include <thread>
#include <gmock/gmock.h>

using namespace testing;
using namespace file_signature;

class hash_file_writer_test : public Test
{
public:
    std::string make_sample( )
    {
        std::string sample;
        for ( std::size_t idx = 0; idx < max_hashes_count_; ++idx ) {
            sample.append( std::to_string( idx ) + " " );
        }
        return sample;
    }

    void add_hashes( file_hash_consumer &writer, std::size_t start_pos, std::size_t step, std::atomic_size_t &threads_count )
    {
        ++threads_count;
        run_cv_.notify_one( );
        std::unique_lock<std::mutex> lock{ run_guard_ };
        lock.unlock( );
        for ( std::size_t idx = start_pos; idx < max_hashes_count_; idx += step ) {
            hash_t hash{ std::to_string( idx ) + " ", idx };
            writer.add_hash( std::move( hash ) );
        }
    }

    std::mutex run_guard_;
    std::condition_variable run_cv_;
    const std::size_t max_hashes_count_{ 1000000 };
    const std::size_t threads_{ 7 }; // should be less than 10 for test
};

TEST_F(hash_file_writer_test, TestUnderLoad)
{
    auto file_writer_ptr = std::make_unique<file_writer_stub>( true );
    auto *raw_file_writer_ptr = static_cast<file_writer_stub *>( file_writer_ptr.get( ) );
    file_hash_consumer hash_writer( std::move( file_writer_ptr ), std::chrono::microseconds( 100 ) );
    hash_writer.start( );
    std::unique_lock<std::mutex> lock{ run_guard_ };
    std::vector<std::thread> threads;
    std::atomic_size_t threads_count{ 0 };
    for ( std::size_t idx = 0; idx < threads_; ++idx ) {
        threads.emplace_back( std::thread( &hash_file_writer_test::add_hashes, this,
                                           std::ref( hash_writer ), idx, threads_, std::ref( threads_count ) ) );
    }
    run_cv_.wait( lock, [&threads_count, this] { return threads_ == threads_count; } );
    lock.unlock( );

    for ( auto &t : threads ) {
        t.join( );
    }

    hash_writer.stop( );

    ASSERT_EQ( make_sample( ), raw_file_writer_ptr->content( ) );
}

TEST_F(hash_file_writer_test, TestCatchExceptionOnWriteOrStop)
{
    auto file_writer_ptr = std::make_unique<file_writer_stub>( false );
    file_hash_consumer hash_writer( std::move( file_writer_ptr ), std::chrono::microseconds( 0 ) );
    hash_writer.start( );
    try {
        hash_writer.add_hash( hash_t{ "test", 0 } );
        std::this_thread::sleep_for( std::chrono::milliseconds( 1 ) );
        hash_writer.add_hash( hash_t{ "test", 1 } );
        FAIL( );
    } catch ( const std::exception &ex ) {
        ASSERT_THAT( ex.what( ), ::testing::StartsWith( "Can not write" ) );
    }
    hash_writer.stop( );
}
