#include "one-write-file-hash-consumer.h"

namespace file_signature {

    one_write_file_hash_consumer::one_write_file_hash_consumer( file_writer_iface_ptr file_writer,
                                                                std::size_t chunks_count )
        : file_writer_{ std::move( file_writer ) }
        , hashes_( chunks_count )
    { }

    void one_write_file_hash_consumer::start( )
    {

    }

    void one_write_file_hash_consumer::stop( )
    {
        for ( const auto &hash : hashes_ ) {
            file_writer_->write( hash );
        }
        file_writer_->flush( );
    }

    void one_write_file_hash_consumer::add_hash( hash_t &&hash )
    {
        hashes_[hash.index( )] = std::move( hash.hash( ) );
    }

} // namespace file_signature
