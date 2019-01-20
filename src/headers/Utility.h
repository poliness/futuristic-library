#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <ctype.h>
#include <list>
#include <cstdio>
#include <time.h>
#include <chrono>
#include <map>
#include <algorithm>



using namespace std;

void sysMsg(std::string s);

void clear();
void error(std::string s);
void printEndlEndl(std::string s);
void pause();

void toUpper(std::string& str);
std::string Time_T_ToStamp(time_t tt);

std::string stampToNormalDate(long timestamp);

bool isInteger(const std::string & s);