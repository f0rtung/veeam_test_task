#pragma once

#include "../../src/file-reader/file-reader-iface.h"

#include <string>

using namespace file_signature;

class file_reader_stub
    : public file_reader_iface
{
public:
    explicit file_reader_stub( const std::string &content );

    std::size_t read( byte *buf, std::size_t size ) override;
    std::size_t size( ) const noexcept override;
    bool eof( ) const override;

    const std::string &content( ) const noexcept;

private:
    std::string content_;
    std::string::const_iterator read_pos_;
};
