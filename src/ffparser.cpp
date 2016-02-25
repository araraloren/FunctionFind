#include "ffparser.h"

ff::Signature ff::SignatureParser::parse(const std::string &str)
{
    string::size_type pos = 0;

    if ((pos = str.rfind("->")) == string::npos) {
        return parse1(str);
    }

    return parse2(str, pos);
}

ff::Signature ff::SignatureParser::parse1(const std::string &str)
{
    Signature sign;

    string::size_type rpos, lpos;

    if ((rpos = str.rfind(')')) == string::npos) {
        return sign;
    }

    if ((lpos = str.find('(')) == string::npos) {
        return sign;
    }

    sign.arg_list = parseArgument(str, lpos, rpos);

    {// parse return and name
        string rn = str.substr(0, lpos);

        string::size_type space_pos = 0;

        if ((space_pos = rn.rfind(' ')) == string::npos) {
            sign.func_name      = rn.empty() ? FuncName(ff::T_Any) : FuncName(rn);
        } else {
            string rnt = cc::trim(rn.substr(0, space_pos));

            sign.return_type    = rnt.empty() ? ReturnType(ff::T_Any) : ReturnType(rnt);

            string fn = cc::trim(rn.substr(space_pos));

            sign.func_name      = fn.empty() ? FuncName(ff::T_Any) : FuncName(fn);
        }

        if (sign.return_type.name == string("void")) {
            sign.return_type.type = ff::T_Void;
        }
    }

    return sign;
}

ff::Signature ff::SignatureParser::parse2(const std::string &str, std::string::size_type pos)
{
    Signature sign;

    string::size_type rpos, lpos;

    if ((rpos = str.rfind(')', pos)) == string::npos) {
        return sign;
    }

    if ((lpos = str.find('(')) == string::npos) {
        return sign;
    }

    sign.arg_list = parseArgument(str, lpos, rpos);

    {// return type
        string rt = cc::trim(str.substr(pos + 2));

        sign.return_type = rt.empty() ? ReturnType(ff::T_Any) : ReturnType(rt);
    }
    {// function name
        string fn = cc::trim(str.substr(0, lpos));

        sign.func_name = fn.empty() ? FuncName(ff::T_Any) : FuncName(fn);
    }

    return sign;
}

ff::ArgList ff::SignatureParser::parseArgument(const std::string &str, std::string::size_type l, std::string::size_type r)
{
    if (l + 1 >= r) return ArgList(ff::T_Void);

    string astr = str.substr(l + 1, r - l - 1);

    {// (void)
        if (cc::trim(astr) == string("void")) {
            return ArgList(ff::T_Void);
        }
    }
    { // (/count)
        string::size_type slash_pos = 0;

        if ((slash_pos = astr.rfind('/', r)) != string::npos) {
            string count_str = str.substr(slash_pos + 1);

            size_t count = std::atoi(count_str.c_str());

            return ArgList(count);
        }
    }
    {// (type name, ...)
        vector<string> al = cc::splitString(astr, ',', cc::trim);

        string::size_type space_pos = 0;

        ArgList arglist(ff::T_List);

        for (vector<string>::iterator it = al.begin();  \
             it != al.end();it ++) {
            if (it->empty()) {
                arglist.arglist.push_back(ArgumentType(T_Any));
                continue;
            }
            if ((space_pos = it->find(' ')) == string::npos) {
                arglist.arglist.push_back(ArgumentType(it->substr(0, space_pos),  \
                                                cc::trim(it->substr(space_pos))));
            } else {
                arglist.arglist.push_back(ArgumentType(*it));
            }
        }
    }

    return ArgList(T_Error);
}

