#pragma once

#include "../../src/file-writer/file-writer-iface.h"

#include <string>

class file_writer_stub
    : public file_signature::file_writer_iface
{
public:
    explicit file_writer_stub( bool can_write );

    void write( const std::string &content ) override;
    void flush( ) override;

    const std::string &content( ) const noexcept;

private:
    std::string content_;
    std::string buffer_;
    bool can_write_;
};
