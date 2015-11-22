#ifndef FFFUNCTION_H
#define FFFUNCTION_H

#include <vector>
#include <string>
#include <ccpublic.h>
#include <ffconfig.h>

NAMESPACE_FF_BEGIN

struct FunctionSignature
{
    std::string ret_type_;

    std::string function_name_;

    std::vector<std::string> args_type_;

    FunctionSignature()
        : ret_type_(),         
          function_name_(),
          args_type_()
    { }

    explicit FunctionSignature(const std::string& ret_type,  \
                        const std::vector<std::string>& args_type,  \
                        const std::string& function_name = std::string())
        : ret_type_(ret_type),
          function_name_(function_name),
          args_type_(args_type)
    { }

    void
    parseSignature(const std::string& signature)
    {
        //add parser signature [type (type)]
        std::string::size_type pos = 0;

        pos = signature.find('(');
        if (std::string::npos == pos) {
            //signature = arg type, .... -> ret type
            pos = signature.rfind("->");

            if (std::string::npos == pos) {
                return;
            } else {
                this->args_type_ = cc::spiltString(signature.substr(0, pos), ',', cc::trim);
                this->ret_type_  = signature.substr(pos + 2);
            }
        } else {
            //signature = ret type(arg type, ...)
            std::string::size_type rpos = 0;

            this->ret_type_ = signature.substr(0, pos);
            if (std::string::npos == (rpos = signature.rfind(')'))) {
                this->args_type_ = cc::spiltString(signature.substr(pos + 1), ',', cc::trim);
            } else {
                this->args_type_ = cc::spiltString(signature.substr(pos + 1, rpos - pos - 1), ',', cc::trim);
            }
        }
    }

    void
    parseRetType(const std::string& ret_type)
    {
        this->ret_type_ = ret_type;
    }

    void parseArgsType(const std::string& args_type)
    {
        //split argstype with ','
        this->args_type_ = cc::spiltString(args_type, ',', cc::trim);
    }

    std::string
    returnType() const
    {
        return this->ret_type_;
    }

    std::string
    functionName() const
    {
        return this->function_name_;
    }

    std::string
    argsType(std::size_t index) const
    {
        return args_type_[index];
    }

    const std::vector<std::string>&
    argsType() const
    {
        return this->args_type_;
    }

    bool
    argsTypeIsEqual(const FunctionSignature& fs) const
    {
        return fs.args_type_ == this->args_type_;
    }

    bool
    retTypeIsEqual(const FunctionSignature& fs) const
    {
        return fs.ret_type_ == this->ret_type_;
    }

    bool
    functionNameIsEqual(const FunctionSignature& fs) const
    {
        return fs.function_name_ == this->function_name_;
    }

    bool
    signatureIsEqual(const FunctionSignature& fs) const
    {
        return argsTypeIsEqual(fs) && retTypeIsEqual(fs);
    }

    bool
    operator ==(const FunctionSignature& fs) const
    {
        return argsTypeIsEqual(fs) && retTypeIsEqual(fs) && functionNameIsEqual(fs);
    }

    bool
    operator !=(const FunctionSignature& fs) const
    {
        return !(this->operator ==(fs));
    }
};

struct Function{

    FunctionSignature fs_;

    unsigned int line_;

    unsigned int colum_;
};

NAMESPACE_FF_END

#endif // FFFUNCTION_H

