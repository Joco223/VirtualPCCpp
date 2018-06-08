#include <vector>
#include <sstream>
#include <iostream>
#include <fstream>

typedef unsigned char byte;

int main(int argc, char** argv) {

    std::vector<byte> hdd;
    std::vector<byte> data;

    std::ifstream HDD;
    HDD.open(argv[1], std::ios::binary);
	std::cout << "Loading HDD..." << '\n';

    hdd.resize(268435456);

    char* buffer = new char[268435456];
	HDD.read(buffer, 268435456);

	for(int i = 0; i < 268435456; i++) {
		hdd[i] = buffer[i];
	}

	std::cout << '\n' << "Finished loading the HDD..." << '\n' << '\n';

	HDD.close();
	delete buffer;

    std::ifstream DATA;
    DATA.open(argv[2]);
    std::string line2;
    std::cout << "Loading data..." << '\n';
    std::string buf2;
    std::stringstream ss2(line2);

    if (DATA.is_open()){
        while (getline (DATA,line2)) {
            data.push_back(std::stoi(line2));
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

    std::cout << '\n' << "Saving the HDD..." << '\n';

    file.open(argv[1], std::ios::binary);

    file.write((char*)hdd.data(), 268435456);

    file.close();

    std::cout << "Finished..." << '\n';

    return 0;
}
