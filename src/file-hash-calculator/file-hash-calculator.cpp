#include "file-hash-calculator.h"
#include "../common/hash-functions.h"

#include <iostream>

namespace file_signature {

    file_hash_calculator::file_hash_calculator( file_chunks_producer_iface_ptr fc_producer,
                                                file_hash_consumer_ptr fh_consumer,
                                                std::size_t threads_count )
        : fc_producer_{ std::move( fc_producer ) }
        , fh_consumer_{ std::move( fh_consumer ) }
        , threads_count_{ threads_count }
    { }

    void file_hash_calculator::calculate( )
    {
        start( );
        stop( );
    }

    void file_hash_calculator::start( )
    {
        fh_consumer_->start( );
        for ( std::size_t idx = 0; idx < threads_count_; ++idx ) {
            threads_.emplace_back( std::thread( &file_hash_calculator::hash_calculating, this ) );
        }
    }

    void file_hash_calculator::stop( )
    {
        for ( auto &t : threads_ ) {
            if ( t.joinable( ) ) {
                t.join( );
            }
        }
        fh_consumer_->stop( );
    }

    void file_hash_calculator::hash_calculating( )
    {
        try {
            while ( true ) {
                chunk_t chunk{ fc_producer_->chunk( ) };
                hash_t hash{ crc_32( chunk.buf( ) ), chunk.index( ) };
                fh_consumer_->add_hash( std::move( hash ) );
            }
        } catch ( std::exception &ex ) {
            std::cout << ex.what( ) << std::endl;
        }
    }

} // namespace file_signature
