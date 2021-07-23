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

namespace ParamTuplePacker
{
    using ParamBuf = unsigned char*;
    
    template <typename... Ts>
    using TuplePack = std::tuple<Ts...>;
    
    struct ParamPack {
        ParamBuf buf;
        size_t size;
    };


    template <typename ...Ts>
    ParamPack packParams(Ts... args) {
        TuplePack<Ts...> pack{args...};
        size_t ts_size = sizeof(pack); 

        ParamBuf buf = new unsigned char[ts_size];
        std::memcpy(buf,&pack,ts_size);
        return {buf,ts_size};
    }

    template <typename TypeStruct,bool Strict = false>
    bool unpackParams(ParamPack& params,TypeStruct* ts) {

        if constexpr(Strict)
            if(sizeof(TypeStruct) != params.size) return false;
        else {
            if(sizeof(TypeStruct) < params.size) return false;
        }
        ParamBuf buf = params.buf; 
        std::memcpy(ts,buf,params.size);
        return true;
    }

} // namespace ParamPacker
