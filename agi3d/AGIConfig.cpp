#include "AGIConfig.h"

std::unique_ptr<AGIConfig> AGIConfig::m_instance;
std::once_flag AGIConfig::m_onceFlag;

AGIConfig& AGIConfig::instance()
{
  std::call_once(m_onceFlag,
                 [] {
                   m_instance.reset(new AGIConfig);
                 });
  return *m_instance.get();
  
}