#include "param_pack.h"
#include "param_tuple_pack.h"

// Change this between
// > ParamTuplePacker
// > ParamPacker
// To alternate the two different versions
using namespace ParamTuplePacker;
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
    return map._map[name](params);
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
        struct {complexType* t;} args; 
        unpackParams(params,&args);
        std::cout << args.t->a << ") In " << args.t->b << std::endl; return true;
    });

    reg_fun("packed",[](ParamPack params) -> double {
        #pragma pack(push, 1)
        struct {int a; int b;} args; 
        #pragma pack(pop)
        unpackParams(params,&args);
        return (double)(args.a + args.b);
    });    
    
    complexType t{25,"tuple"};
    call("print_in",&t);
    return 0;
}


