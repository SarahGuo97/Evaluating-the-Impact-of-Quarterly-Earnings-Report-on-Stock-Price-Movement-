#ifndef simple_curl_h
#define simple_curl_h

#include <stdio.h>
#include <string>
#include <iostream>
#include <sstream>
#include <vector>
#include <locale>
#include <iomanip>
#include <numeric>
#include "Stock.h"
#include "curl/curl.h"

// Store the functions for libcurl and calculaiton

using namespace std;
struct MemoryStruct {
    char *memory;
    size_t size;
};

void *myrealloc(void *ptr, size_t size);
//string getTimeinSeconds(string Time);

string format_date(string Time);
vector<Stock> ramdom_30(map<string,Stock> stock_group,vector<string> Stock_ID);

//calculation
vector<double> cal_AAR(vector<Stock> stock_group,int N);
vector<double> cal_CAAR(vector<double> AAR, int N);
vector<double> cal_STD(vector<vector<double>> data); //input matix with 30 rows (30 loop results) and 2N+1 columns (numeber of days)

#endif /* simple_curl_h */
