#include "file-hash-consumer.h"

namespace file_signature {

    file_hash_consumer::file_hash_consumer( file_writer_iface_ptr file_writer, ch::microseconds write_interval )
        : file_writer_{ std::move( file_writer ) }
        , curr_hash_idx_{ 0 }
        , active_{ true }
        , write_interval_{ write_interval }
    { }

    void file_hash_consumer::start( )
    {
        processing_thread_ = std::thread( &file_hash_consumer::hash_processing, this );
    }

    void file_hash_consumer::stop( )
    {
        active_ = false;
        processing_thread_cv_.notify_one( );
        if ( processing_thread_.joinable( ) ) {
            processing_thread_.join( );
        }
    }

    void file_hash_consumer::add_hash( hash_t &&hash )
    {
        processing_exception_.rethrow_exception_if_exists( );
        std::lock_guard<std::mutex> _{ hashes_buffer_guard_ };
        hashes_buffer_.emplace_back( std::move( hash ) );
    }

    void file_hash_consumer::hash_processing( )
    {
        try {
            std::unique_lock<std::mutex> lock{ processing_thread_m_ };
            while( active_ ) {
                processing_thread_cv_.wait_for( lock, write_interval_ );
                if ( active_ ) {
                    move_from_hashes_buffer( );
                    const auto cend{ find_end_hashes_index_sequence( ) };
                    write_hashes( hashes_.cbegin( ), cend );
                    hashes_.erase( hashes_.cbegin( ), cend );
                }
            }
            move_from_hashes_buffer( );
            write_hashes( hashes_.cbegin( ), hashes_.cend( ) );
        } catch ( const std::exception &/* ex */ ) {
            processing_exception_.set_exception( std::current_exception( ) );
        }

    }

    void file_hash_consumer::move_from_hashes_buffer( )
    {
        std::vector<hash_t> tmp_hashes_buffer;
        {
            std::lock_guard<std::mutex> _{ hashes_buffer_guard_ };
            tmp_hashes_buffer.swap( hashes_buffer_ );
        }

        for ( const auto& hash : tmp_hashes_buffer ) {
            hashes_.emplace( hash.index( ), hash.hash( ) );
        }
    }

    file_hash_consumer::hashes_map_cit_t file_hash_consumer::find_end_hashes_index_sequence( )
    {
        hashes_map_cit_t pos{ hashes_.cbegin( ) };

        if ( hashes_.empty( ) ) {
            return pos;
        }

        if ( pos->first != curr_hash_idx_ ) {
            return pos;
        }

        for ( ++pos, ++curr_hash_idx_; pos != hashes_.cend( ); ++pos, ++curr_hash_idx_ ) {
            if ( pos->first != curr_hash_idx_ ) {
                break;
            }
        }
        return pos;
    }

    void file_hash_consumer::write_hashes( hashes_map_cit_t begin, hashes_map_cit_t end )
    {
        if ( begin != end ) {
            for ( ; begin != end; ++begin ) {
                file_writer_->write( begin->second );
            }
            file_writer_->flush( );
        }
    }

} // namespace file_signature
