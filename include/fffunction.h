#ifndef FFFUNCTION_H
#define FFFUNCTION_H

#include <vector>
#include <string>
#include <ccpublic.h>
#include <ffconfig.h>

NAMESPACE_FF_BEGIN

struct FunctionSignature
{
    /**
     * @brief m_ret
     */
    std::string m_ret;

    /**
     * @brief m_name
     */
    std::string m_name;

    /**
     * @brief m_args
     */
    std::vector<std::string> m_args;

    /**
     * @brief FunctionSignature
     */
    FunctionSignature()
        : m_ret(),
          m_name(),
          m_args()
    { }

    /**
     * @brief FunctionSignature
     * @param ret_type
     * @param args_type
     * @param function_name
     */
    explicit FunctionSignature(const std::string& ret_type,  \
                        const std::vector<std::string>& args_type,  \
                        const std::string& function_name = std::string())
        : m_ret(ret_type),
          m_name(function_name),
          m_args(args_type)
    { }

    /**
     * @brief parseSignature
     * @param signature
     */
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
                this->m_args = cc::spiltString(signature.substr(0, pos), ',', cc::trim);
                this->m_ret  = signature.substr(pos + 2);
            }
        } else {
            //signature = ret type(arg type, ...)
            std::string::size_type rpos = 0;

            this->m_ret = signature.substr(0, pos);
            if (std::string::npos == (rpos = signature.rfind(')'))) {
                this->m_args = cc::spiltString(signature.substr(pos + 1), ',', cc::trim);
            } else {
                this->m_args = cc::spiltString(signature.substr(pos + 1, rpos - pos - 1), ',', cc::trim);
            }
        }
    }

    /**
     * @brief parseRetType
     * @param ret_type
     */
    void
    parseRetType(const std::string& ret_type)
    {
        this->m_ret = ret_type;
    }

    /**
     * @brief parseArgsType
     * @param args_type
     */
    void
    parseArgsType(const std::string& args_type)
    {
        //split argstype with ','
        this->m_args = cc::spiltString(args_type, ',', cc::trim);
    }

    /**
     * @brief argsTypeIsEqual
     * @param fs
     * @return
     */
    bool
    argsTypeIsEqual(const FunctionSignature& fs) const
    {
        return fs.m_args == this->m_args;
    }

    /**
     * @brief retTypeIsEqual
     * @param fs
     * @return
     */
    bool
    retTypeIsEqual(const FunctionSignature& fs) const
    {
        return fs.m_ret == this->m_ret;
    }

    /**
     * @brief functionNameIsEqual
     * @param fs
     * @return
     */
    bool
    functionNameIsEqual(const FunctionSignature& fs) const
    {
        return fs.m_name == this->m_name;
    }

    /**
     * @brief signatureIsEqual
     * @param fs
     * @return
     */
    bool
    signatureIsEqual(const FunctionSignature& fs) const
    {
        return argsTypeIsEqual(fs) && retTypeIsEqual(fs);
    }

    /**
     * @brief operator ==
     * @param fs
     * @return
     */
    bool
    operator ==(const FunctionSignature& fs) const
    {
        return argsTypeIsEqual(fs) && retTypeIsEqual(fs) && functionNameIsEqual(fs);
    }

    /**
     * @brief operator !=
     * @param fs
     * @return
     */
    bool
    operator !=(const FunctionSignature& fs) const
    {
        return !(this->operator ==(fs));
    }
};

struct Function{
    /**
     * @brief m_fs
     */
    FunctionSignature m_fs;
    /**
     * @brief m_file source name
     */
    std::string m_file;
    /**
     * @brief m_row m_col m_off
     * @ m_off -> offset of whole file
     */
    unsigned int m_row;
    unsigned int m_col;
    unsigned int m_off;
};

NAMESPACE_FF_END

#endif // FFFUNCTION_H

