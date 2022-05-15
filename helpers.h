#ifndef __HELPERS_H_INCLUDED
#define __HELPERS_H_INCLUDED

#include <iostream>

extern bool verbose_flag;

template<typename T>
void print_verbose(T t)
{
    if(verbose_flag) {
        std::cout << t;
    }
}

template<typename T, typename... Tail>
void print_verbose(T head, Tail... tail)
{
    if(verbose_flag) {
        std::cout << head; 
        print_verbose(tail...); 
    }
}


template <typename T>
void print_vec_verbose(std::vector<T*> vec, const char *sep=", ")
{
    if(verbose_flag) { 
        int size = vec.size();
        for(int i=0; i<size-1; ++i)
            std::cout << *(vec[i]) << sep;
        std::cout << *vec[size-1] << std::endl; 
    }
}


#endif
