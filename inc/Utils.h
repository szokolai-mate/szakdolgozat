#pragma once

#include <utility>
#include <array>
#include <atomic>

namespace template_utils
{
template <typename T>
struct extract_inner_base;

template <template <typename> class C, typename T>
struct extract_inner_base<C <T> >{    using type = T; };

/*! This will extract the template parameter of template parameter T
    For example, if you have A with <typename T> and B with <typename K>, and make an A<B<float>>,
    then in A extract_inner<T> will evaluate to K or float in this case.
*/
template <typename T>
using extract_inner = typename extract_inner_base<T>::type;
}

//structs

struct audio_descriptor
{
    unsigned short channels;
    unsigned short sample_rate;

    audio_descriptor() {}
    audio_descriptor(unsigned short _channels, unsigned short _sample_rate)
    {
        channels = _channels;
        sample_rate = _sample_rate;
    }
};

struct audio_file_descriptor
{
    unsigned long data_start;
    unsigned long next_byte;

    audio_file_descriptor() {}
    audio_file_descriptor(unsigned long _data_start, unsigned long _next_byte)
    {
        data_start = _data_start;
        next_byte = _next_byte;
    }
    audio_file_descriptor(unsigned long _data_start)
    {
        data_start = _data_start;
        next_byte = _data_start;
    }
};


//constexpr array maker with templates
namespace constexpr_array{
    
    template<class Function, std::size_t... Indices>
    constexpr static auto make_helper(Function f, std::index_sequence<Indices...>) 
    -> std::array<typename std::result_of<Function(std::size_t)>::type, sizeof...(Indices)> 
    {
        return {{ f(Indices)... }};
    }

    template<int N, class Function>
    constexpr static auto make(Function f)
    -> std::array<typename std::result_of<Function(std::size_t)>::type, N> 
    {
        return make_helper(f, std::make_index_sequence<N>{});
    }
}