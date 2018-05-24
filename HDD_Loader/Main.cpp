#include <vector>
#include <sstream>
#include <iostream>
#include <fstream>

typedef unsigned char byte;

int main(int argc, char** argv) {

    std::vector<byte> hdd;
    std::vector<byte> data;

    std::ifstream HDD;
    HDD.open(argv[1]);
	std::string line;
	std::cout << "Loading HDD..." << '\n';
	std::string buf;
	std::stringstream ss(line);

    hdd.resize(16384 * 16384);

    int j = 0;
	int i = 0;

	if (HDD.is_open()){
		while (getline (HDD,line)) {
			ss.str(line);
			while (ss >> buf) {
				hdd[j * 16384 + i] = std::stoi(buf);
                i++;
			}
            i = 0;
            j++;
		}

	}

    HDD.close();

    std::cout << "Finished loading HDD..." << '\n';

    std::ifstream DATA;
    DATA.open(argv[2]);
    std::string line2;
    std::cout << "Loading data..." << '\n';
    std::string buf2;
    std::stringstream ss2(line2);

    if (DATA.is_open()){
		while (getline (DATA,line2)) {
			ss2.str(line2);
			while (ss2 >> buf2) {
				data.push_back(std::stoi(buf2));
			}
		}

	}

    DATA.close();

    std::cout << "Finished loading the data..." << '\n';

    std::cout << "Copying data..." << '\n';

    for(int i = 0; i < data.size(); i++){
        hdd[i + std::stoi(argv[3])] = data[i];
    }

    std::cout << "Finished copying the data..." << '\n';

    std::ofstream file;;

    file.open(argv[1], std::ios::out | std::ios::trunc);
    file.close();

    std::cout << '\n' << "Saving the HDD...";

    file.open(argv[1]);

    for (int i = 0; i < 16384; i++) {
        for (int j = 0; j < 16384; j++) {
            file << (std::to_string((int)hdd[i * 16384 + j]));
            file << " ";
        }
        file << '\n';
    }

    file.close();

    std::cout << "Finished..." << '\n';

    return 0;
}
