#ifndef AGICONFIG_H
#define	AGICONFIG_H

#include <memory>
#include <mutex>

/**
 *
 * アプリケーションの設定値を保持するシングルトンクラス。
 *
 */
class AGIConfig {
public:
  virtual ~AGIConfig(){};
  static AGIConfig& instance();
  
private:
  static std::unique_ptr<AGIConfig> m_instance;
  static std::once_flag m_onceFlag;
  AGIConfig(void){};
  AGIConfig(const AGIConfig& src);
  AGIConfig& operator=(const AGIConfig& rhs);
};
#endif	/* AGICONFIG_H */