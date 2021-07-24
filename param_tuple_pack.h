#include <cstring>
#include <any>
#include <memory>

namespace ParamTuplePacker
{
    using ParamBuf = std::unique_ptr<unsigned char[]>;
    using ParamBufBase = ParamBuf::element_type;
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
        auto buf = std::make_unique<ParamBufBase[]>(ts_size);
        std::memcpy(buf.get(),&pack,ts_size);
        return {std::move(buf),ts_size};
    }

    template <typename TypeStruct,bool Strict = false>
    bool unpackParams(ParamPack& params,TypeStruct* ts) {
        if constexpr(Strict) { if(sizeof(TypeStruct) != params.size) return false; } 
        else { if(sizeof(TypeStruct) < params.size) return false; }

        auto buf = params.buf.get(); 
        std::memcpy(ts,buf,params.size);
        return true;
    }

} // namespace ParamPacker
