#pragma once

#include "type-defs.h"

#include <string>

namespace file_signature {

    std::string crc_32( const bytes &buf );

} // namespace file_signature
