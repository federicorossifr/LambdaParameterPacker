#include "param_pack.h"
#include "param_tuple_pack.h"

using namespace ParamTuplePacker;
using Funct = std::function<std::any(ParamPack params)>;

typedef struct {
    std::unordered_map<std::string,Funct> _map;
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
int main() {
    reg_fun("print_in",[](ParamPack params) -> bool {
        struct {complexType* t;} args; 
        unpackParams(params,&args);
        std::cout << args.t->a << ") In " << args.t->b << std::endl; return true;
    });

    reg_fun("sum",[](ParamPack params) -> double {
        #pragma pack(push, 1)
        struct {int a; int b;} args; 
        #pragma pack(pop)
        unpackParams(params,&args);
        return (double)(args.a + args.b);
    });    
    
    complexType t{25,"hello"};
    call("print_in",&t);
    return 0;
}


