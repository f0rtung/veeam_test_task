#pragma once

#include <atomic>
#include <exception>

namespace file_signature {

    class exception_ptr_wrapper
    {
    public:
        exception_ptr_wrapper( );
        exception_ptr_wrapper( std::exception_ptr ex );

        void set_exception( std::exception_ptr ex );
        void rethrow_exception_if_exists( );

    private:
        std::exception_ptr exception_;
        std::atomic_bool has_exception_;
    };

} // namespace file_signature
