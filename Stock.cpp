//
//  Stock.cpp
//  final_project
//
//  Created by 张嘉昕 on 5/3/20.
//  Copyright © 2020 NYU. All rights reserved.
//

#include "Stock.h"

//iterate through trading day list to find calculate the start date and end date
void Stock:: get_Date(int N,vector<string> Date_list){
    //change the date formate to dd-mm-yy
    auto itr = find(Date_list.begin(), Date_list.end(), day_zero);
    start_date = (*(itr - N - 1)); //-1, because calcuating daily return will get rid of the first price data.
    end_date = (*(itr + N));
}


void Stock::print_info(){
    cout << "Ticker ID: " << get_ticker_id()<<endl;
    cout << "Group: " << group <<endl;
    cout << "Earning announcement date: " << day_zero <<endl;
    cout << "Estimated earning: " << est_earning <<endl;
    cout << "Actual earning: " << actual_earning <<endl;
    cout << "Surprise " << surprise << endl;
    cout << "Surprise (%)" << surprise_pct <<endl;
    cout <<endl;

}

void Ticker::cal_daily_return(){
    
    for(int i = 1; i < price.size(); i++) {
        daily_return.push_back((price[i] - price[i-1])/price[i-1]);
    }
   
}

void Stock::cal_abnormal_return(Benchmark SPY, vector<string> date_list, int N){
    
    //calculate abonormal return
    abnormal_return.resize(2*N+1);
    auto itr_d = find(date_list.begin(),date_list.end(),start_date);
    int begin_index = distance(date_list.begin(), itr_d);
    auto itr_d2 = find(date_list.begin(), date_list.end(), end_date);
    int end_index = distance(date_list.begin(), itr_d2);
    vector<double>spyreturn = SPY.get_return();
    spyreturn.assign(spyreturn.begin()+begin_index, spyreturn.begin()+end_index); //The range used is [first,last)
    abnormal_return = daily_return - spyreturn;
}
