#include "exception_ptr_wrapper.h"

namespace file_signature {

    exception_ptr_wrapper::exception_ptr_wrapper( )
        : has_exception_{ false }
    { }

    exception_ptr_wrapper::exception_ptr_wrapper( std::exception_ptr ex )
        : exception_{ ex }
        , has_exception_{ true }
    { }

    void exception_ptr_wrapper::set_exception( std::exception_ptr ex )
    {
        exception_ = ex;
        has_exception_ = true;
    }

    void exception_ptr_wrapper::rethrow_exception_if_exists( )
    {
        if ( has_exception_ ) {
            std::rethrow_exception( exception_ );
        }
    }

} // namespace file_signature
