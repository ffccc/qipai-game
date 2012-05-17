#ifndef __XLB_CONFIG_OPTION_H__
#define __XLB_CONFIG_OPTION_H__

#include <string>
#include <map>

/////////////////////////////////////////////////////////////
//namespace xlbe {

/////////////////////////////////////////////////////////////
struct Config_Option
{
    std::string name;
    std::string value;
};

typedef std::map<std::string, Config_Option> Config_Option_Map;
//////////////////////////////////////////////////////////////
//}

#endif 