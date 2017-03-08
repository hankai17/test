#include <iostream>
#include <string>

#include "LuaReader.h"

int main() {
	LuaScript script("config.lua");
	std::string elfIp = script.get<std::string>("elfogo.ip");
	std::string elfPort = script.get<std::string>("elfogo.port");
	std::vector<std::string> cameraVec = script.getTableKeys("kamera");
	
	std::cout<<"Elfogo IP: "<< elfIp << std::endl;
	std::cout<<"Elfogo Port: " << elfPort <<std::endl;  
	
	for(int i=0; i<cameraVec.size();i++ ) {
		std::string camIp = script.get<std::string>("kamera." + cameraVec[i] + ".ip");
		std::string camPort = script.get<std::string>("kamera." + cameraVec[i] + ".port");
		std::cout << "Kamera name: " << cameraVec[i] << " Kamera ip: " << camIp << " Kamera port: " << camPort << std::endl;
	}
} 
