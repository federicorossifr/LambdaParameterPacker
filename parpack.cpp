#include "param_pack.h"
#include "param_tuple_pack.h"

// Change this between
// > ParamTuplePacker
// > ParamPacker
// To alternate the two different versions
using namespace ParamPacker;
using Funct = std::function<std::any(ParamPack params)>;

typedef struct {
    std::unordered_map<std::string,Funct> _map;
    /* OTHER FIELDS ..... */
} FunMap;

FunMap map;

void reg_fun(std::string key, Funct f) {
    map._map.insert({key,f});
}

template <typename ...Ts>
std::any call(std::string name,Ts&&... pack) {
    ParamPack params = packParams(pack...);
    std::any ret =  map._map[name](params);
    delete[] params.buf; // ToDo switch to smart pointers to avoid explicit delete[]
    return ret;
}

struct complexType {
    int a;
    std::string b;
};

// Test main
// If using the plain packed struct you need to use the packed-style declaration as in "packed"
// If you use the tuple-based approach no need to do it
int main() {
    reg_fun("tuple",[](ParamPack params) -> bool {
        std::tuple<complexType*,int> args; 
        if(!unpackParams(params,&args)) return false;
        std::cout << std::get<0>(args)->a << std::endl; 
        std::cout << std::get<0>(args)->b << std::endl; 
        std::cout << std::get<1>(args) << std::endl; 
        return true;
    });

    reg_fun("packed",[](ParamPack params) -> double {
        #pragma pack(push, 1)
        struct {complexType* a; int b;} args; 
        #pragma pack(pop)
        if(!unpackParams(params,&args)) return false;
        std::cout << args.a->a << std::endl; 
        std::cout << args.a->b << std::endl; 
        std::cout << args.b << std::endl; 
        return true;
        
    });    
    
    // Always pass complex types as pointers (bonus: already aligned type)
    complexType t{25,"tuple"};

    // Call and parameter passing are transparent to packing method
    call("packed",&t,55);
    return 0;
}


