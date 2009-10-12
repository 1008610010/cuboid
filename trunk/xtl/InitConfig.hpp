#ifndef _XTL__INIT_CONFIG_HPP__
#define _XTL__INIT_CONFIG_HPP__ 1

#include "Variant.hpp"
#include "InitConfigParser.hpp"
#include <string>
#include <map>

namespace XTL
{
	class InitConfigSection
	{
		public:

			typedef std::map<std::string, Variant> ParamsMap;

			InitConfigSection(const std::string & name)
				: name_(name), params_() { ;; }

			const std::string Name() const
			{
				return name_;
			}

			void Set(const std::string & key, const long long & value)
			{
				params_[key] = value;
			}

			void Set(const std::string & key, const double & value)
			{
				params_[key] = value;
			}

			void Set(const std::string & key, const std::string & value)
			{
				params_[key] = value;
			}

			const Variant Get(const std::string & key) const
			{
				ParamsMap::const_iterator itr = params_.find(key);
				return itr == params_.end() ? Variant() : itr->second;
			}

			Variant & Get(const std::string & key)
			{
				ParamsMap::iterator itr = params_.find(key);
				if (itr != params_.end())
				{
					return itr->second;
				}
				else
				{
					return params_.insert(std::make_pair(key, Variant())).first->second;
				}
			}

		protected:

			const std::string name_;
			ParamsMap         params_;
	};

	class InitConfig
	{
		public:

			class Error
			{
				public:
					explicit Error(const char * what) : what_(what) { ;; }
					explicit Error(const std::string & what) : what_(what) { ;; }
					const char * What() const { return what_.c_str(); }
				protected:
					const std::string what_;
			};

			typedef std::map<std::string, InitConfigSection *> SectionsMap;

			InitConfig();

			~InitConfig() throw();

			void Clear();

			InitConfig & Set(const std::string & section, const std::string & key, const long long & value);

			InitConfig & Set(const std::string & section, const std::string & key, const double & value);

			InitConfig & Set(const std::string & section, const std::string & key, const std::string & value);

			const InitConfigSection * GetSection(const std::string & name) const;

			InitConfigSection * CreateSection(const std::string & name);

			bool Load(const std::string & filePath);

		protected:

			InitConfig(const InitConfig &);
			InitConfig & operator= (const InitConfig &);

			SectionsMap sections_;
	};
}

#endif
