//
//  Stock.hp
//  final_project
//
//  Created by 张嘉昕 on 5/3/20.
//  Copyright © 2020 NYU. All rights reserved.
//

#ifndef Stock_h
#define Stock_h

#include <stdio.h>
#include <string>
#include <iostream>
#include <sstream>
#include <vector>
#include <map>
#include <locale>
#include <iomanip>
#include <valarray>     
#include "OperationOverloader.h"

using namespace std;

//base class
class Ticker{
protected:
    string ticker_id;
    string start_date;
    string end_date;
    vector<double> price;  //length(2N+2),extra one day before the start date. to calculate the daily return.
    vector<double> daily_return; //length(2N+1)
    
public:
    //calculate daily return
    void cal_daily_return();
    
    //get and set functions
    vector<double> get_price(){ return price; }
    vector<double> get_return(){ return daily_return; }
    string get_ticker_id(){ return ticker_id; }
    string get_start_date(){ return start_date;}
    string get_end_date(){ return end_date;}
    void set_price(vector<double> price_){price = price_;}
    void set_start_date(string start_){start_date = start_;}
    void set_end_date(string end_){end_date = end_;}
};

//child classes
class Benchmark: public Ticker{
private:
    string ticker_id = "SPY";
public:
    string get_ticker_id(){ return ticker_id; }

};


class Stock: public Ticker{
private:
    vector<double> abnormal_return;
    string day_zero;
    string est_earning;
    string actual_earning;
    string surprise;
    string surprise_pct;
    string group;
    
public:
    //constructor
    Stock(string ticker_id_,string day_zero_, string est_e_, string act_e_, string surprise_, string surprise_pct_){
        ticker_id = ticker_id_;
        day_zero = day_zero_;
        est_earning = est_e_;
        actual_earning = act_e_;
        surprise = surprise_;
        surprise_pct = surprise_pct_;
    }
    
    void print_info(); 
    void get_Date(int N,vector<string> Date_list); //calculte start and end date
    void cal_abnormal_return(Benchmark SPY, vector<string> date_list,int N);
    
    
    //get and set functions
    void set_id(string id){ticker_id = id;}
    void set_surprise_pct(double s_){surprise_pct = s_;}
    void set_day_zero(string date_){day_zero = date_;}
    void set_group(string group_){group = group_;}

    string get_surprise_pct(){return surprise_pct;}
    string get_day_zero(){return day_zero;}
    vector<double> get_abnormal_return(){return abnormal_return;} //length(2N+1)
    
    
};



#endif /* Stock_hpp */
