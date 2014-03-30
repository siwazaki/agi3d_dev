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

Config::Config()
{
  this->xRotation = 0;
  this->yRotation = 0;
}

bool Config::rotateX()
{
  return this->xRotation;
}

bool Config::rotateY()
{
  return this->yRotation;
}

void Config::rotateX(bool rotation)
{
  this->xRotation = rotation;
}

void Config::rotateY(bool rotation)
{
  this->yRotation = rotation;
}