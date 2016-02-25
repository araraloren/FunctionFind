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

/*
 * T_Error
 * T_Any
 * T_Type
*/
typedef VarType ArgumentType;

/*
 * T_Error
 * T_Any
 * T_Void
 * T_Type
*/
struct ReturnType {
    Type    type; // var type
    string  name; // type name

    ReturnType()
        :type(T_None)
        ,name()
    {}

    ReturnType(Type type)
        :type(type)
        ,name()
    {}

    ReturnType(const std::string &type_name)
        :type(T_Type)
        ,name(type_name)
    {}
};

/*
 * T_Error
 * T_List
 * T_Count
 * T_Void
*/
struct ArgList {
    Type    type;
    vector<ArgumentType> arglist;
    size_t count;

    ArgList()
        :type(T_None)
        ,arglist()
        ,count(0)
    {}

    ArgList(Type type)
        :type(type)
        ,arglist()
        ,count(0)
    {}

    ArgList(const vector<ArgumentType>& arglist)
        :type(T_List)
        ,arglist(arglist)
        ,count(0)
    {}

    ArgList(size_t count)
        :type(T_Count)
        ,arglist()
        ,count(count)
    {}
};

/*
 * T_Name
 * T_Any
*/
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

    bool isValid() const
    {
        return return_type.type != T_Error && arg_list.type != T_Error;
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

