#include "Compiler.h"

namespace Compiler {

	void readFile(std::string path, std::vector<std::string>& code) {
		std::ifstream in(path);
		
		std::string tmp;
		while(std::getline(in, tmp)) {
			code.push_back(tmp);				
		}
	}

	void Compile(std::vector<std::string>& code, std::vector<std::string>& assembly_code) {
		std::vector<std::string> tokens;
		for(int i = 0; i < code.size(); i++) {
			std::string str;
			for(int k = 0 ; k < code[i].size(); k++) {
				char tmp = code[i][k];
				if(tmp != ';') {
					str.push_back(tmp);
				}else{
					tokens.push_back(str);
					str.clear();
				}
			}
			
		}

		struct variable {
			byte size;
			std::string name;
			int value;
		};

		std::vector<variable> vars;

		bool hasData = false;

		for(int i = 0 ; i < tokens.size(); i++) {
			if(!tokens[i].compare(0, 6, "u_int8")) {
				hasData = true;

				std::string temp = tokens[i].substr(7, tokens[i].size() - 7);
				std::string name;
				int size = 1;
				int value = 0;

				int k = 0;

				while(temp[k] == ' ') {
					k++;
				}

				temp = temp.erase(0, k);

				for(int j = 0; j < temp.size(); j++) {
					if(temp[j] != ' ') {
						name.push_back(temp[j]);
					}else{
						break;
					}
				}

				std::string temp2 = temp.erase(0, name.size());

				k = 0;

				while(temp2[k] == ' ') {
					k++;
				}

				temp2 = temp2.erase(0, k + 1);

				k = 0;

				while(temp2[k] == ' ') {
					k++;
				}

				temp2 = temp2.erase(0, k);

				value = std::stoi(temp2);

				variable temporary;
				temporary.name = name;
				temporary.size = size;
				temporary.value = value;

				vars.push_back(temporary);
			}else if(!tokens[i].compare(0, 7, "u_int16")) {
				hasData = true;

				std::string temp = tokens[i].substr(8, tokens[i].size() - 8);
				std::string name;
				int size = 2;
				int value = 0;

				int k = 0;

				while(temp[k] == ' ') {
					k++;
				}

				temp = temp.erase(0, k);

				for(int j = 0; j < temp.size(); j++) {
					if(temp[j] != ' ') {
						name.push_back(temp[j]);
					}else{
						break;
					}
				}

				std::string temp2 = temp.erase(0, name.size());

				k = 0;

				while(temp2[k] == ' ') {
					k++;
				}

				temp2 = temp2.erase(0, k + 1);

				k = 0;

				while(temp2[k] == ' ') {
					k++;
				}

				temp2 = temp2.erase(0, k);

				value = std::stoi(temp2);

				variable temporary;
				temporary.name = name;
				temporary.size = size;
				temporary.value = value;

				vars.push_back(temporary);
			}else if(!tokens[i].compare(0, 7, "u_int32")) {
				hasData = true;

				std::string temp = tokens[i].substr(8, tokens[i].size() - 8);
				std::string name;
				int size = 4;
				int value = 0;

				int k = 0;

				while(temp[k] == ' ') {
					k++;
				}

				temp = temp.erase(0, k);

				for(int j = 0; j < temp.size(); j++) {
					if(temp[j] != ' ') {
						name.push_back(temp[j]);
					}else{
						break;
					}
				}

				std::string temp2 = temp.erase(0, name.size());

				k = 0;

				while(temp2[k] == ' ') {
					k++;
				}

				temp2 = temp2.erase(0, k + 1);

				k = 0;

				while(temp2[k] == ' ') {
					k++;
				}

				temp2 = temp2.erase(0, k);

				value = std::stoi(temp2);

				variable temporary;
				temporary.name = name;
				temporary.size = size;
				temporary.value = value;

				vars.push_back(temporary);
			}
		}

		std::cout << vars[0].name << " " << vars[0].value << '\n';
		std::cout << vars[1].name << " " << vars[1].value << '\n';
		std::cout << vars[2].name << " " << vars[2].value << '\n';

	}
	
}
