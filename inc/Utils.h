#pragma once

#include <utility>
#include <array>
#include <atomic>


/*! \~english Namespace for template metaprogramming utilities.
    \~hungarian Sablon metaprogramozáshoz használt eszközöket tartalmazó névtér.
*/
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