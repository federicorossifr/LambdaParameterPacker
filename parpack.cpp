#include "param_pack.h"
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

// Test main
int main() {
    reg_fun("print_in",[](ParamPack params) -> bool {
        #pragma pack(push, 1)
        struct {char a; int c;} args; 
        #pragma pack(pop)
        unpackParams(params,&args);
        std::cout << args.c << ") In " << args.a << std::endl; return true;
    });

    reg_fun("sum",[](ParamPack params) -> double {
        #pragma pack(push, 1)
        struct {int a; int b;} args; 
        #pragma pack(pop)
        unpackParams(params,&args);
        return (double)(args.a + args.b);
    });    

    double a = std::any_cast<double>(call("sum",2,20));
    std::cout << a << std::endl;
    return 0;
}

