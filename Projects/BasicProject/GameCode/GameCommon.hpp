#ifndef GAMECOMMON_HPP
#define GAMECOMMON_HPP

namespace Henry
{

enum LoadPreference{ DATA_ONLY = 0 , ZIP_ONLY , PREFER_DATA , PREFER_ZIP };
extern LoadPreference g_loadPreference;

};

#endif