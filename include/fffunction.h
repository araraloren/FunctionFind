#ifndef FFFUNCTION_H
#define FFFUNCTION_H

#include <vector>
#include <string>
#include <ccpublic.h>
#include <ffconfig.h>

using std::string;
using std::vector;

NAMESPACE_FF_BEGIN

enum Type {
    T_None,
    T_Any,
    T_Void,
    T_Template,
    T_Type,
    T_Normal,
};

struct VarType {
    Type    type; // var type
    string  type_name; // type name
    string  name; // var name

    VarType()
        :type(T_None)
        ,type_name()
        ,name()
    {}

    VarType(Type type, const string& type_name, const string& var_name)
        :type(type)
        ,type_name(type_name)
        ,name(var_name)
    {}

    VarType(Type type, const std::string &type_name)
        :type(type)
        ,type_name(type_name)
        ,name()
    {}
};

typedef VarType ReturnType;
typedef VarType ArgumentType;

struct ArgList {
    Type    type;
    vector<ArgumentType> list;

    ArgList()
        :type(T_None)
        ,list()
    {}

    ArgList(Type type, const vector<ArgumentType>& arglist)
        :type(type)
        ,list(arglist)
    {}
};

struct FuncName {
    Type    type;
    string  name;

    FuncName()
        :type(T_None)
        ,name()
    {}

    FuncName(Type type, const string& name)
        :type(type)
        ,name(name)
    {}
};

struct Signature {
    ReturnType  return_type;
    ArgList     arg_list;
    FuncName    func_name;

    Signature()
        :return_type()
        ,arg_list()
        ,func_name()
    {}

    Signature(const ReturnType& return_type, const ArgList& arg_list, const FuncName& func_name)
        :return_type(return_type)
        ,arg_list(arg_list)
        ,func_name(func_name)
    {}

    bool isNone() const
    {
        return return_type.type == T_None && arg_list.type == T_None && func_name.type == T_None;
    }
};

struct Function : public Signature {
    string orignal;

    unsigned int row;
    unsigned int col;
    unsigned int off;

    Function()
        :Signature()
        ,orignal()
        ,row(0)
        ,col(0)
        ,off(0)
    {}

    Function(const ReturnType& return_type, const ArgList& arg_list, const FuncName& func_name)
         :Signature(return_type, arg_list, func_name)
         ,orignal()
         ,row(0)
         ,col(0)
         ,off(0)
     {}
};

NAMESPACE_FF_END

#endif // FFFUNCTION_H

