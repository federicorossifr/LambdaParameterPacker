#include "param_pack.h"



using Funct = std::function<std::any(ParamPacker::ParamPack params)>;

typedef struct {
    std::unordered_map<std::string,Funct> _map;
} FunMap;

FunMap map;

void reg_fun(std::string key, Funct f) {
    map._map.insert({key,f});
}

template <typename ...Ts>
std::any call(std::string name,Ts&&... pack) {
    ParamPacker::ParamPack params = ParamPacker::packParams(pack...);
    return map._map[name](params);
}

struct complexType {
    int a;
    std::string b;
};

// Test main
int main() {
    reg_fun("print_in",[](ParamPacker::ParamPack params) -> bool {
        #pragma pack(push, 1)
        struct {complexType* t;} args; 
        #pragma pack(pop)
        ParamPacker::unpackParams(params,&args);
        std::cout << args.t->a << ") In " << args.t->b << std::endl; return true;
    });

    reg_fun("sum",[](ParamPacker::ParamPack params) -> double {
        #pragma pack(push, 1)
        struct {int a; int b;} args; 
        #pragma pack(pop)
        ParamPacker::unpackParams(params,&args);
        return (double)(args.a + args.b);
    });    
    
    complexType t{25,"hello"};
    call("print_in",&t);
    return 0;
}


