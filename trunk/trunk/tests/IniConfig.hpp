#ifndef XTL__INI_CONFIG_HPP__
#define XTL__INI_CONFIG_HPP__ 1

namespace XTL
{
	class IniConfig
	{
		public:

			class Section
			{
				public:

					explicit Section(const std::string & sectionName);

					bool Exists(const std::string & key)
					{
						return pairs_.count(key) > 0;
					}

					void Set(const std::string & key, const VariantPtr & value)
					{
						pairs_[key] = value;
					}

					VariantPtr Get(const std::string & key) const
					{
						std::map<std::string, VariantPtr>::const_iterator itr = pairs_.find(key);

						if (itr == pairs_.end())
						{
							throw KeyDoesNotExist(sectionName_, key);
						}

						return itr->second;
					}

					const std::string GetString(const std::string & key, const std::string & defaultValue) const
					{
						std::map<std::string, VariantPtr>::iterator itr = pairs_.find(key);

						if (itr == pairs_.end())
						{
							return defaultValue;
						}

						return itr->second.ToString();
					}

					const long long int GetLongLongInt(const std::string & key, const long long int & defaultValue) const
					{
						std::map<std::string, VariantPtr>::iterator itr = pairs_.find(key);

						if (itr == pairs_.end())
						{
							return defaultValue;
						}

						return itr->second.ToLongLongInt();
					}

				private:

					const std::string sectionName_;
					mutable std::map<std::string, VariantPtr> pairs_;
			};

		private:

			AutoPtrMap<std::string, Section> sections_;
	};
}

#endif
