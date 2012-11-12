#ifndef XTL__INI_CONFIG_HPP__
#define XTL__INI_CONFIG_HPP__ 1

#include <map>
#include <string>

#include "../lib/xtl/utils/AutoPtrMap.hpp"

namespace XTL
{
	class IniConfig
	{
		public:

			class Error : public XTL::Exception
			{
				public:

					class SectionDoesNotExist;
					class KeyWasNotFound;
			};

			class Section
			{
				public:

					explicit Section(const std::string & sectionName);

					bool Contains(const std::string & key) const;

					void Set(const std::string & key, const VariantPtr & value);

					/**
					 * @throw XTL::IniConfig::Error::KeyWasNotFound
					 */
					VariantPtr Get(const std::string & key) const;

					/**
					 * Если заданный ключ не найден, метод возвращает defaultValue
					 */
					const long long int GetLongLongInt(const std::string & key, const long long int & defaultValue) const;

					const double GetDouble(const std::string & key, const double & defaultValue) const;

					const std::string GetString(const std::string & key, const std::string & defaultValue) const;

				private:

					const std::string sectionName_;
					mutable std::map<std::string, VariantPtr> pairs_;
			};

			Section & CreateSection(const std::string & sectionName);

			void Set(const std::string & sectionName, const std::string & key, VariantPtr value);

			/**
			 * @throw XTL::IniConfig::Error::SectionDoesNotExist
			 */
			const Section & GetSectionRequired(const std::string & sectionName) const;

			const Section & GetSectionOptional(const std::string & sectionName) const;

		private:


			AutoPtrMap<std::string, Section> sections_;
	};


	class IniConfig::Error::SectionDoesNotExist : public IniConfig::Error
	{
		public:

			explicit SectionDoesNotExist(const std::string & sectionName);

			virtual ~SectionDoesNotExist() throw();

			virtual const std::string What() const;

			const std::string & SectionName() const;

		private:

			const std::string sectionName_;
	};


	class IniConfig::Error::KeyWasNotFound : public IniConfig::Error
	{
		public:

			KeyWasNotFound(const std::string & sectionName, const std::string & key);

			virtual ~KeyWasNotFound() throw();

			virtual const std::string What() const;

			const std::string & SectionName() const;

			const std::string & Key() const;

		private:

			const std::string sectionName_;
			const std::string key_;
	};
}

#endif
