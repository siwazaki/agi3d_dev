#include "Config.h"

std::unique_ptr<Config> Config::m_instance;
std::once_flag Config::m_onceFlag;

Config& Config::instance()
{
  std::call_once(m_onceFlag,
                 [] {
                   m_instance.reset(new Config);
                 });
  return *m_instance.get();
}