#include <vector>
#include <unordered_map>
#include <string>
#include <functional>
#include <numeric>
#include <algorithm>
#include <iostream>
#include <memory>
#include <cassert>
#include <cstring>
#include <any>

namespace ParamPacker
{
    using ParamBuf = unsigned char*;

    struct ParamPack {
        ParamBuf buf;
        size_t size;
    };


    template<typename T, typename... Args>
    constexpr std::size_t ts_length()
    {
        if constexpr (sizeof...(Args) >= 1) 
            return sizeof(T) + ts_length<Args...>();
        return sizeof(T); 
    }


    template <typename T, typename ...Ts>
    ParamBuf _packParams(ParamBuf col,T el,Ts... pack) {
        std::memcpy(col,&el,sizeof(T));
        col+= sizeof(el);
        if constexpr(sizeof...(Ts) >=1) return _packParams(col,pack...);
        return col;

    }

    template <typename ...Ts>
    ParamPack packParams(Ts... args) {
        std::size_t ts_size = ts_length<Ts...>();
        ParamBuf buf = new unsigned char[ts_size];
        _packParams<Ts...>(buf,args...);
        return {buf,ts_size};
    }

    template <typename TypeStruct,bool Strict = false>
    bool unpackParams(ParamPack& params,TypeStruct* ts) {
        if constexpr(Strict)
            if(sizeof(TypeStruct) != params.size) return false;
        else {
            if(sizeof(TypeStruct) < params.size) return false;
        }
        std::memcpy(ts,params.buf,params.size);
        return true;
    }

} // namespace ParamPacker
