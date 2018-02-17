#include "Compiler.h"

namespace Compiler {

	void readFile(std::string path, std::vector<std::string>& code) {
		std::ifstream in(path);
		
		std::string tmp;
		while(std::getline(in, tmp)) {
			code.push_back(tmp);				
		}
	}

	void splitFile(std::vector<std::string>& code, std::vector<std::string>& tokens) {
		for(int i = 0; i < code.size(); i++) {
			std::string str;
			bool braced = false;
			for(int k = 0 ; k < code[i].size(); k++) {
				char tmp = code[i][k];
				if(tmp == '{') { braced = true; }
				if(tmp == '}') { braced = false; }
				if(braced == true) {
					str.push_back(tmp);
				}else if(braced == false) {
					if(tmp != ';') {
						str.push_back(tmp);
					}else{
						tokens.push_back(str);
						str.clear();
					}
				}
				
			}
			
		}
	};

	void Parse(std::vector<std::string>& tokens) {
		//std::vector<std::string> tokens;
		

		struct variable {
			byte size;
			std::string name;
			int value;
			int sign;
		};

		struct function {
			std::string name;
			std::string return_type;
		};

		std::vector<variable> vars;
		std::vector<function> fns;

		bool hasData = false;

		for(int i = 0 ; i < tokens.size(); i++) {
			if(!tokens[i].compare(0, 6, "u_int8")) {
				hasData = true;

				std::string temp = tokens[i].substr(7, tokens[i].size() - 7);
				std::string name;
				int sign = 0;
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
				sign = 0;

				vars.push_back(temporary);
			}else if(!tokens[i].compare(0, 7, "u_int16")) {
				hasData = true;

				std::string temp = tokens[i].substr(8, tokens[i].size() - 8);
				std::string name;
				int sign = 0;
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
				sign = 0;

				vars.push_back(temporary);
			}else if(!tokens[i].compare(0, 7, "u_int32")) {
				hasData = true;

				std::string temp = tokens[i].substr(8, tokens[i].size() - 8);
				std::string name;
				int sign = 0;
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
				sign = 0;

				vars.push_back(temporary);
			}else if(!tokens[i].compare(0, 2, "fn")) {
				std::string temp = tokens[i].substr(2, tokens[i].size() - 2);
				std::string name;
				std::string return_type;

				int k = 0;

				while(temp[k] == ' ') {k++;}

				temp = temp.erase(0, k);

				for(int j = 0; j < temp.size(); j++) {
					if(temp[j] != ' ') {
						return_type.push_back(temp[j]);
					}else{
						break;
					}
				}

				std::string temp2 = temp.erase(0, return_type.size());

				k = 0;

				while(temp2[k] == ' ') {
					k++;
				}

				temp2 = temp2.erase(0, k);
				
				for(int j = 0; j < temp2.size(); j++) {
					if(temp2[j] != ' ') {
						name.push_back(temp2[j]);
					}else{
						break;
					}
				}

				std::string temp3 = temp2.erase(0, name.size());

				k = 0;

				while(temp3[k] == ' ') {
					k++;
				}

				function temporary;
				temporary.name = name;
				temporary.return_type = return_type;
				fns.push_back(temporary);

				temp3 = temp3.erase(0, k);

				temp3 = temp3.erase(temp3.size() - 1, 1);

				if(temp3[0] == '{') {
					std::vector<std::string> tokens2;
					std::vector<std::string> tmpCode;
					tmpCode.push_back(temp3);

					splitFile(tmpCode, tokens2);
					Parse(tokens2);
				}
				
			}

		}

	}
	
}
