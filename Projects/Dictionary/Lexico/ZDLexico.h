#pragma once

#include <vector>
#include <string>
#include <iostream>
#include <fstream>

namespace Dico
{
	//! @brief This class juste wrap a std::vector<string> representing the lexico data base.
	class Lexico
	{
	public:
		//! @brief defaut constructor
		Lexico() = default;

		//! @brief read the lexico data base from a given path file 
		//! @param intputFile 
		//! @return true if succes, false otherwise
		bool read(std::string intputFile)
		{
			bool result = true;
			std::ifstream file(intputFile);
			if (file.is_open())
			{
				std::string line;
				while (std::getline(file, line))
				{
					m_words.push_back(line.c_str());
				}
				file.close();
			}
			else
			{
				result = false;
			}

			return result;
		};

		//! @brief get the vector of words of the lexico data base
		//! @return 
		const std::vector<std::string>& getWords()
		{
			return m_words;
		}

	private:
		std::vector<std::string> m_words;
	};
}
