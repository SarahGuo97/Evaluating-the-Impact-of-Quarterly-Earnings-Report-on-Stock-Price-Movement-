#ifndef Bootstrap_h
#define Bootstrap_h

#include <stdio.h>
#include <iostream>
#include <fstream>
#include <stdexcept> // std::runtime_error
#include <sstream> // std::stringstream
#include <thread>
#include <cstdlib>
#include <mutex>
#include "Calculation.h"


using namespace std;
class Bootstrap{
private:
    vector<vector<double>> Beat_AAR_matrix, Meet_AAR_matrix,Miss_AAR_matrix;//matix with 30 rows (30 loop results) and 2N+1 columns (numeber of days)
   
    vector<vector<double>> Beat_CAAR_matrix,Meet_CAAR_matrix,Miss_CAAR_matrix; //matix with 30 rows (30 loop results) and 2N+1 columns (numeber of days)
    
    //CAAR matrix of three groups, used to plot the graph
    vector<vector<double>> CAAR_combined;
    
    //Build a big matrix for all the results
    vector<vector<double>> Beat_result;
    vector<vector<double>> Meet_result;
    vector<vector<double>> Miss_result;
    vector<vector<vector<double>>> Result_Matrix;
    
public:
    Benchmark SPY;
    //three maps that store three groups of stocks
    map<string,Stock> Stock_Beat;
    map<string,Stock> Stock_Meet;
    map<string,Stock> Stock_Miss;
    
    mutex mut;
    
    vector<string> ticker_list;
    vector<string> date_list;
    
    vector<double> Beat_AAR, Meet_AAR, Miss_AAR;
    vector<double> Beat_AAR_STD, Meet_AAR_STD, Miss_AAR_STD;
    vector<double> Beat_CAAR, Meet_CAAR, Miss_CAAR;
    vector<double> Beat_CAAR_STD, Meet_CAAR_STD, Miss_CAAR_STD;
    
    string sCookies = "";
    string sCrumb = "";
    
    void load_earning_data(string filename); //read csv, set Stock maps and ticker_list
    void load_SPY_data(); //set date_list and benchmark object
    void load_price_data(int N); //set price list for each stock in Stock_Map, use libcurl
    void update_abnormal_return(int N); //update daily return and abnormal return for all stocks
    
    void bootstrapping(int N); //get AAR, AAR_STD, CAAR, CAAR_STD
    
    //libcurl
    void get_price_data(map<string,Stock> stock_map,string group,int N);
    void get_SPY_data(string start_date,string end_date);
    
    //set and get
    vector<vector<double>> get_CAAR_combined(){return CAAR_combined;}
    vector<vector<vector<double>>> get_Result_Matrix(){return Result_Matrix;}
    //gnuplot
    void plotResults(int dataSize);
};






#endif /* Bootstrap_hpp */
