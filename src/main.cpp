#include <unordered_map>
#include <string>
#include <iostream>
#include <fstream>

int main(int argc, char const *argv[])
{
	// params
		// argv[1] is NAT file
		// argv[2] is FLOW file

	std::unordered_map<std::string,std::string> lookupTable;
	std::string line;

	// open NAT file and add all elements from NAT file into the hash table
	std::ifstream natFile(argv[1]);

	while(getline(natFile,line)){
		unsigned int pos = line.find(',');
		std::string in = line.substr(0,pos);
		std::string out = line.substr(pos+1);
		lookupTable[in] = out;
	}

	natFile.close();

	//convert Flows to Output
		// priority if multiple matches
		// 1) return exact match for IP + Port
		// 2) return IP match + wildcard port
		// 3) return wildcard IP + port
		// 4) not found

	std::ofstream outputFile("OUTPUT");
	std::ifstream flowFile(argv[2]);

	while(getline(flowFile,line)){
		// split line ip and port
		unsigned int pos = line.find(":");
		std::string ip = line.substr(0,pos);
		std::string port = line.substr(pos+1);

		std::string result;

		if(lookupTable.find(ip+":"+port) != lookupTable.end()){
			// exact ip and exact port match
			result = line + " -> " + lookupTable[ip+":"+port];
		} else if (lookupTable.find(ip+":*") != lookupTable.end()){
			// exact ip and widlcard port match 
			result = line + " -> " + lookupTable[ip+":*"];
		} else if (lookupTable.find("*:"+port) != lookupTable.end()){
			// wildcard ip and exact port match
			result = line + " -> " + lookupTable["*:"+port];
		} else {
			// no match
			result = "No nat match for " + ip + ":" + port;
		}

		outputFile << result << std::endl;
	}

	outputFile.close();
	flowFile.close();

	return 0;
}