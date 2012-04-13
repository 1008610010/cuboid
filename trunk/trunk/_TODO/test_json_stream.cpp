#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <string>
#include <vector>

#include "xtl/Json.hpp"
#include "xtl/JsonParser.hpp"
#include "xtl/JsonStream.hpp"
#include "xtl/HttpDownloader.hpp"

namespace XTL
{
	class JsonFileOutputter
	{
		public:
	
			JsonFileOutputter(FILE * file)
				: file_(file) { ;; }

			void Write(char ch)
			{
				fprintf(file_, "%c", ch);
			}

			void Write(const char * ptr)
			{
				fprintf(file_, ptr);
			}
		
			void Write(const char * ptr, size_t size)
			{
				fwrite(ptr, 1, size, file_);
			}
		
		protected:
	
			FILE * file_;
	};

	class JsonStringOutputter
	{
		public:
	
			JsonStringOutputter(std::string & s)
				: s_(s) { ;; }

			void Write(char ch)
			{
				s_.append(1, ch);
			}

			void Write(const char * ptr)
			{
				s_.append(ptr);
			}
		
			void Write(const char * ptr, size_t size)
			{
				s_.append(ptr, size);
			}
		
		protected:
	
			std::string & s_;
	};
}

typedef std::vector<std::pair<unsigned int, unsigned int> > ContainerOfPosts;

void Execute(const ContainerOfPosts & posts)
{
	XTL::HttpDownloader downloader;
	
	std::string postData = "posts=";
	XTL::JsonStringOutputter outputter(postData);
	XTL::JsonStream<XTL::JsonStringOutputter, XTL::JsonNullSpacer> stream(outputter);
	
	stream << json::array();

	const ContainerOfPosts::const_iterator end(posts.end());
	for (ContainerOfPosts::const_iterator itr = posts.begin(); itr != end; ++itr)
	{
		stream << json::plain_array() << itr->first << itr->second << json::end();
	}
	
	stream << json::end();
	
	fprintf(stderr, "%s\n", postData.c_str());
	
	downloader.Post(
		"http://www.lj-3-m.bulyon.local/tools/endpoints/rating_allow_posts.bml",
		postData,
		"fake_ipclass=sup_dtc"
	);
	
	if (downloader.ResponseCode() == 200)
	{
		fprintf(stdout, "%s\n", downloader.Content().c_str());

		XTL::JsonValue * json = 0;
		try
		{
			XTL::JsonParser parser(downloader.Content().c_str());
			json = parser.Release();
		}
		catch (const XTL::JsonParser::Error & e)
		{
			fprintf(stderr, "JsonParser::Error\n");
		}
		
		XTL::JsonObject obj = XTL::JsonVariableRef(json).ToObject();
		if (obj.Exists("allow"))
		{
			XTL::JsonArray arr = obj["allow"].ToArray();
		
			for (XTL::JsonArray::Iterator itr(arr.Begin()); !itr.AtEnd(); ++itr)
			{
				printf("%lld\n", (*itr).AsInteger(-1));
			}
		}
		else if (obj.Exists("error"))
		{
			printf("%s\n", obj["error"].AsString().c_str());
		}

		delete json;
	}
	
}

int main(int argc, const char * argv[])
{
	ContainerOfPosts posts;
	posts.push_back(std::make_pair(1, 1));
	posts.push_back(std::make_pair(2, 1530));
	posts.push_back(std::make_pair(2, 1640));

	Execute(posts);


/*
	JsonFileOutputter outputter(stdout);
	XTL::JsonStream<JsonFileOutputter, XTL::JsonSpacer> stream(outputter);
	
	stream
		<< json::object()
			<< "\"fir/st\"\\m" << json::array() << json::end()
			<< "second" << json::object()
				<< "a" << json::object()
					<< "int" << -1u
					<< "3" << -1234567890123456789.1
					<< "5" << -1u << json::end()
			<< json::end()
			<< "third" << json::array() << true << false << true
		<< json::finish();
		
	
	fprintf(stdout, "\n");
*/
	return 0;
}

