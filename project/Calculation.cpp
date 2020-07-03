#include "Calculation.h"
using namespace std;

void *myrealloc(void *ptr, size_t size)
{
    if (ptr)
        return realloc(ptr, size);
    else
        return malloc(size);
}

static size_t write_data2(void *ptr, size_t size, size_t nmemb, void *data)
{
    size_t realsize = size * nmemb;
    struct MemoryStruct *mem = (struct MemoryStruct *)data;
    
    mem->memory = (char *)myrealloc(mem->memory, mem->size + realsize + 1);
    if (mem->memory) {
        memcpy(&(mem->memory[mem->size]), ptr, realsize);
        mem->size += realsize;
        mem->memory[mem->size] = 0;
    }
    return realsize;
}


string format_date(string date) {
    struct tm tm;
    strptime(date.c_str(), "%d-%b-%y", &tm);
    
    char date2[15];
    strftime(date2, sizeof(date2), "%Y-%m-%d", &tm);
    return date2;
}

vector<Stock> ramdom_30(map<string,Stock> stock_group,vector<string> Stock_ID){
    vector<Stock> random_30_stocks;
    srand((unsigned)time(NULL));
    random_shuffle(Stock_ID.begin(), Stock_ID.end());
    // get first 30 stocks
    for (int i = 0; i < 30; i++)
    {
        random_30_stocks.push_back(stock_group.find(Stock_ID[i])->second);
    }
    return random_30_stocks;
}

//calculation functions
vector<double> cal_AAR(vector<Stock> stock_group,int N){
    vector <double> AAR;
    AAR.resize(2*N+1);
    auto itr = stock_group.begin();
    int i = 0;
    while(itr != stock_group.end()){
        AAR = (i / (i + 1.0)) * AAR + (1 / (i + 1.0)) * (itr->get_abnormal_return());//operator
        itr++;
        i ++;
    }
    return AAR;
}

vector<double> cal_CAAR(vector<double> AAR, int N){
    vector<double> CAAR;
    CAAR.resize(2*N+1);
    partial_sum(AAR.begin(), AAR.end(), CAAR.begin());
    return CAAR;
}

vector<double> cal_STD(vector<vector<double>> data) {
    vector<double>std;
    int m = data.size();
    int n = data[0].size();
    for (int i = 0; i < n; i++){
        double ave = 0.0;
        for (int j = 0; j < m; j++){
            ave += data[j][i];
        }
        ave = ave / m;
        double ans = 0.0;
        for (int j = 0; j < m; j++){
            ans += (data[j][i] - ave) * (data[j][i] - ave);
        }
        ans = sqrt(ans / data.size());
        std.push_back(ans);
    }
    return std;
}
