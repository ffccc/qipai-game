#ifndef __XLBE_EXCEPTION_H__
#define __XLBE_EXCEPTION_H__

#include <string>
////////////////////////////////////////////////////////////
//namespace xlbe{

////////////////////////////////////////////////////////////
class base_error : public std::exception 
{
public:
    base_error(const std::string& msg):msg_(msg) {} 
    virtual ~base_error() throw() {} 

    virtual const char* what() const throw()    { return msg_.c_str(); } 
    void set(const std::string& msg)            { msg_ = msg; }

private: 
    std::string msg_;
};

class render_error : public base_error
{
public:  
    render_error(const std::string& msg) : base_error(msg) {}
};

class scene_error : public base_error
{
public:  
    scene_error(const std::string& msg) : base_error(msg) {}
};

class root_error : public base_error
{
public:  
    root_error(const std::string& msg) : base_error(msg) {}
};

class resource_error : public base_error
{
public:
    resource_error(const std::string& msg) : base_error(msg) {}
};

///////////////////////////////////////////////////////////
//}
#endif