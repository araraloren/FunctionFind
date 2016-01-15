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
    T_Void,
    T_Type, // for type
    T_Template,
    T_Count, // for args
    T_List, // for args
    T_Name, // for function name
    T_Any, // any
    T_Normal,
    T_Error,
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

    VarType(Type type)
        :type(type)
        ,type_name()
        ,name()
    {}


    VarType(const string& type_name, const string& var_name)
        :type(T_Type)
        ,type_name(type_name)
        ,name(var_name)
    {}

    VarType(const std::string &type_name)
        :type(T_Type)
        ,type_name(type_name)
        ,name()
    {}
};

typedef VarType ArgumentType;

struct ReturnType {
    Type    type; // var type
    string  name; // type name

    VarType()
        :type(T_None)
        ,name()
    {}

    VarType(Type type)
        :type(type)
        ,name()
    {}

    VarType(const std::string &type_name)
        :type(T_Type)
        ,name(type_name)
    {}
};

struct ArgList {
    Type    type;
    union {
        vector<ArgumentType> args;
        size_t count;
    }list;

    ArgList()
        :type(T_None)
        ,list()
    {}

    ArgList(Type type)
        :type(type)
        ,list()
    {}

    ArgList(const vector<ArgumentType>& arglist)
        :type(T_List)
        ,list()
    {
        list.args = arglist;
    }

    ArgList(size_t count)
        :type(T_Count)
        ,list()
    {
        list.count = count;
    }
};

struct FuncName {
    Type    type;
    string  name;

    FuncName()
        :type(T_None)
        ,name()
    {}

    FuncName(Type type)
        :type(type)
        ,name()
    {}

    FuncName(const string& name)
        :type(T_Name)
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

