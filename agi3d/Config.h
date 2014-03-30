#ifndef CONFIG_H
#define	CONFIG_H

#include <memory>
#include <mutex>
#include <string>

/**
 *
 * アプリケーションの設定値を保持するシングルトンクラス。
 *
 */
class Config {
public:
  virtual ~Config(){};
  static Config& instance();
  
private:
  static std::unique_ptr<Config> m_instance;
  static std::once_flag m_onceFlag;
  Config(void){};
  Config(const Config& src);
  Config& operator=(const Config& rhs);
};
#endif	/* CONFIG_H */