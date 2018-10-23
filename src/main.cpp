#include "file-reader/binary-file-reader.h"
#include "file-writer/text-file-writer.h"
#include "file-chunks-producer/simple-file-chunks-producer.h"
#include "file-chunks-producer/async-file-chunks-producer.h"
#include "file-hash-consumer/file-hash-consumer.h"
#include "file-hash-consumer/one-write-file-hash-consumer.h"
#include "file-hash-calculator/file-hash-calculator.h"

#include <algorithm>
#include <iostream>
#include <boost/program_options.hpp>
#include <thread>

boost::program_options::variables_map parse_command_line( int argc, const char* const *argv )
{
    namespace po = boost::program_options;
    po::variables_map vm;
    po::options_description desc{ "Options" };

    try {
        desc.add_options( )
        ( "help,h",
          "Help message" )
        ( "input_file,i", po::value<std::string>( )->required( ),
          "File for which you want to make a hash calculation" )
        ( "out_file,o", po::value<std::string>( )->default_value( "result.txt" ),
          "Result file" )
        ( "chunk_size,c", po::value<std::size_t>( )->default_value( 1024 * 1024 ),
          "Size of one chunk (in bytes)" );
        po::store( po::parse_command_line( argc, argv, desc ), vm );
        po::notify(vm);
    } catch ( const po::required_option &ex ) {
        if ( vm.count( "help" ) ) {
            std::cout << desc << "\n";
        } else {
            throw ex;
        }
    }
    return vm;
}

int main( int argc, char **argv )
{
    try {
        const auto params = parse_command_line( argc, argv );

        if ( params.count( "help" ) ) {
            return 0;
        }

        const std::string input_file{ params["input_file"].as<std::string>( ) };
        const std::string out_file{ params["out_file"].as<std::string>( ) };
        const std::size_t chunk_size{ params["chunk_size"].as<std::size_t>( ) };
        const std::size_t hc = std::thread::hardware_concurrency( ) > 0 ?
                               std::thread::hardware_concurrency( ) : 4;

        using namespace file_signature;

        file_reader_iface_ptr f_reader{ std::make_unique<binary_file_reader>( input_file ) };
        const std::size_t chunks_count{ f_reader->size( ) / chunk_size + 1 };
        const std::size_t threads_count = std::min( hc, chunks_count );

        file_chunks_producer_iface_ptr fc_produser{
            std::make_unique<simple_file_chunks_producer>( std::move( f_reader ), threads_count, chunk_size )
        };

//        file_chunks_producer_iface_ptr fc_produser{
//            std::make_unique<async_file_chunks_producer>( std::move( f_reader ), chunk_size )
//        };

        file_writer_iface_ptr f_writer{ std::make_unique<text_file_writer>( out_file ) };

//        file_hash_consumer_iface_ptr fh_consumer{ std::make_unique<file_hash_consumer>( std::move( f_writer ) ) };

        file_hash_consumer_iface_ptr fh_consumer{ std::make_unique<one_write_file_hash_consumer>( std::move( f_writer ), chunks_count ) };

        file_hash_calculator fh_calculator{ std::move( fc_produser ), std::move( fh_consumer ), threads_count };
        fh_calculator.calculate( );

    } catch ( const std::exception &ex ) {
        std::cout << ex.what( ) << std::endl;
        return 1;
    }
    return 0;
}
