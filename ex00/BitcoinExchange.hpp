#pragma once

#include <string>
#include <map>
#include <fstream>
#include <iostream>
#include <sstream>

std::map<std::string, double>	buildDataMap();
void	printMap(std::map<std::string, double> _data);