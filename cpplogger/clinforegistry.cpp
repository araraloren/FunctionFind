#include "clinforegistry.h"

cl::ClInfoRegistry::~ClInfoRegistry()
{
    for(std::vector<cl::ClInfoPtr>::iterator it = m_registry.begin();
            it != m_registry.end();it ++){
        cl::ClInfoRegistry::s_free(*it);
    }
}

size_t
cl::ClInfoRegistry::registerInfo(const cl::ClInfoPtr &info)
{
    m_registry.push_back(info);

    return m_registry.size();
}

cl::ClInfoPtr
cl::ClInfoRegistry::operator[](size_t id)
{
    if(id <= m_registry.size()){
        return m_registry[id - 1];
    }
    else{
        return nullptr;
    }
}

cl::ClInfoPtr
cl::ClInfoRegistry::operator[](std::string name)
{
    for(std::vector<cl::ClInfoPtr>::iterator it = m_registry.begin();
            it != m_registry.end();it ++){

        cl::ClInfoPtr& cip = *it;

        if(cip && cip->m_name == name){
            return cip;
        }
    }

    return nullptr;
}

std::string
cl::ClInfoRegistry::getLogName(size_t id)
{
    if(id <= m_registry.size()){
        return m_registry[id - 1]->m_name;
    }
    else{
		return string("");
    }
}

size_t
cl::ClInfoRegistry::getLogId(std::string name)
{
	size_t id = 1;

    for(std::vector<cl::ClInfoPtr>::iterator it = m_registry.begin();
            it != m_registry.end();it ++, id ++){

        cl::ClInfoPtr& cip = *it;

        if(cip && cip->m_name == name){
            return id;
        }
    }

    return 0;
}

void
cl::ClInfoRegistry::checkInfo()
{
    for(std::vector<cl::ClInfoPtr>::iterator it = m_registry.begin();
            it != m_registry.end();it ++){

        cl::ClInfoPtr& cip = *it;

        if(cip && cip->isWork()){
            cip->check();
        }
    }
}

void
cl::ClInfoRegistry::setFreeFunc(cl::freeMemoryFunc *value)
{
    s_free = value;
}


cl::freeMemoryFunc* cl::ClInfoRegistry::s_free = cl::defaultFreeFunc;

void
cl::defaultFreeFunc(ClInfoPtr ptr)
{
    delete ptr;
}
