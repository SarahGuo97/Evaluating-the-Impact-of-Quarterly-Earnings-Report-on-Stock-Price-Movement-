#include <iostream>
#include <fstream>
#include <stdexcept> // std::runtime_error
#include <sstream> // std::stringstream
#include "Calculation.h"
#include "Stock.h"
#include "Bootstrap.h"
using namespace std;

void print_vector(vector<double>vec){
    auto itr = vec.begin();
    while(itr != vec.end()){
        cout << *itr << " " << std::fixed << ::setprecision(5);
        itr++;
    }
    cout <<endl;
}

int main() {
    
    int x; //Menu
    int N = -1; //Number of days
    Bootstrap test;
    
    while (true) {
        cout << endl;
        cout << "Menu:" << endl;
        cout << "1. Enter N to retrieve historical price data for all stocks." << endl;
        cout << "2. Pull information for one stock from one group." << endl;
        cout << "3. Show AAR, AAR-STD, CAAR and CAAR-STD for one group." << endl;
        cout << "4. Show the Excel or gnuplot graph with CAAR for all 3 groups." << endl;
        cout << "5. Exit your program." << endl;
        
        cin >> x;
        switch(x){
            case 1:{
                while(true){
                    cout << "Enter N:" << endl;
                    cin >> N;
                    if(!cin.fail() && N >= 0 && N <= 100){
                        cout << "N is " << N << endl;
                        test.load_earning_data("SP500 Surprise Ranking.csv");
                        test.load_SPY_data();
                        test.load_price_data(N);
                        test.update_abnormal_return(N);
                        test.bootstrapping(N);
                        break;
                    }else{
                        cout << "Invalid input N. Please enter a positive integer less than 100. " << endl;
                        N = -1;
                        cin.clear();
                        cin.ignore(10000,'\n');
                    }
                }
                break; //optional
            }
            case 2:{
                if(N>=0){
                    while(true){
                        cout << "Enter the ticker:" << endl;
                        string t;
                        cin >> t;
                        if(test.Stock_Beat.count(t) == 1){
                            test.Stock_Beat.at(t).print_info();
                            break;
                        }else if(test.Stock_Meet.count(t) == 1){
                            test.Stock_Meet.at(t).print_info();
                            break;
                        }else if(test.Stock_Miss.count(t) == 1){
                            test.Stock_Miss.at(t).print_info();
                            break;
                        }else{
                            cout <<"The ticker is not in our stock pool. Please try other tickers, such as 'AAPL'." << endl;
                        }
                    }
                }else{
                    cout << "Failed to get data. Have you retrived the data?" << endl;
                }
                break; //optional
            }
                
            case 3:{
                if(N>=0){
                    while(true){
                        cout << "Enter the group: (1 - Beat, 2 - Meet, 3 - Miss)" << endl;
                        int g;
                        cin >> g;
                        int success = 1;
                        switch(g){
                            case 1:{
                                cout <<"********** Group Beat ********** "<<endl;
                                cout << "AAR: ";
                                print_vector(test.Beat_AAR);
                                cout << "AAR-STD: ";
                                print_vector(test.Beat_AAR_STD);
                                cout << "CAAR: ";
                                print_vector(test.Beat_CAAR);
                                cout << "CAAR-STD: ";
                                print_vector(test.Beat_CAAR_STD);
                                break;
                                
                            }
                            case 2:{
                                cout <<"********** Group Meet ********** "<<endl;
                                cout << "AAR: ";
                                print_vector(test.Meet_AAR);
                                cout << "AAR-STD: ";
                                print_vector(test.Meet_AAR_STD);
                                cout << "CAAR: ";
                                print_vector(test.Meet_CAAR);
                                cout << "CAAR-STD: ";
                                print_vector(test.Meet_CAAR_STD);
                                break;
                            }
                            case 3:{
                                cout <<"********** Group Miss ********** "<<endl;
                                cout << "AAR: ";
                                print_vector(test.Miss_AAR);
                                cout << "AAR-STD: ";
                                print_vector(test.Miss_AAR_STD);
                                cout << "CAAR: ";
                                print_vector(test.Miss_CAAR);
                                cout << "CAAR-STD: ";
                                print_vector(test.Miss_CAAR_STD);
                                break;
                            }
                            default:
                                cout << "Invalid input." << endl;
                                cin.clear();
                                cin.ignore(10000,'\n');
                                success = 0;
                        }
                        if(success == 1){
                            break;
                        }
                    }
                }else{
                    cout << "Failed to get data. Have you retrived the data?" << endl;
                }
                break; //optional
            }
            case 4:{
                if(N < 0){
                    cout << "You have not retrieve historical data." << endl;
                }else{
                    test.plotResults(N);
                }
                break; //optional
            }
            case 5:
                exit(0);
            default:
                cout << "Invalid menu input." << endl;
                cin.clear();
                cin.ignore(10000,'\n');
        }
 
    }


    return 0;
     
}

