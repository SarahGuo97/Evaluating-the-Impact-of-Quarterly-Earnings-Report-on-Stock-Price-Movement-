#include "Bootstrap.h"

using namespace std;

void Bootstrap:: load_earning_data(string filename){
    ifstream ip(filename);
    if(!ip.is_open()) std::cout << "ERROR: File Open" << '\n';
    
    int index = 0;
    while(ip.good()){
        vector<string> earning_info;
        string substr;
        //first 6 columns ends with comma
        for(int i = 0; i < 6; i++){
            getline(ip, substr, ',');
            earning_info.push_back(substr);
        }
        getline(ip, substr,'\n');  //the last one ends with"\n"
        earning_info.push_back(substr);
        
        //build map for stock objects and set ticker_list
        if(index!=0){
            //format day_zero to YYYY-MM-DD
            string day_zero = earning_info[1];
            if (earning_info[1].length() < 9){
                day_zero = "0"+ day_zero;
            }
            day_zero = format_date(day_zero);
            
            //build new stock object
            Stock new_stock(earning_info[0],day_zero,earning_info[3],earning_info[4],earning_info[5],earning_info[6]);
            
            //divide into 3 groups with different group name
            if(index >= 1 && index<=160){
                new_stock.set_group("Beat Group");
                Stock_Beat.insert(pair<string, Stock>(earning_info[0],new_stock));
            }else if (index >160 && index <=320){
                new_stock.set_group("Meet Group");
                Stock_Meet.insert(pair<string, Stock>(earning_info[0],new_stock));
            }else{
                new_stock.set_group("Miss Group");
                Stock_Miss.insert(pair<string, Stock>(earning_info[0],new_stock));
            }
            //build ticker_list
            ticker_list.push_back(earning_info[0]);
        }
        index++;
    }
    ip.close();
    
    cout << "Load earning data successfull." << endl;
}

void Bootstrap::load_SPY_data(){
    get_SPY_data("2019-01-01","2020-05-01");
    
}

void Bootstrap:: load_price_data(int N){
    
    vector<thread> threads;
    threads.push_back(thread(&Bootstrap::get_price_data,this,Stock_Beat,"Beat",N));
    threads.push_back(thread(&Bootstrap::get_price_data,this,Stock_Meet,"Meet",N));
    threads.push_back(thread(&Bootstrap::get_price_data,this,Stock_Miss,"Miss",N));
    
    for (auto& t : threads)
        t.join();
    
    cout << "Load price data successfull." << endl;
}

void Bootstrap::update_abnormal_return(int N){
    //calculate SPY daily return
    SPY.cal_daily_return();
    //calculate daily return, then calculate abnormal return
    auto itr = Stock_Beat.begin();
    while(itr != Stock_Beat.end()){
        itr->second.cal_daily_return();
        itr->second.cal_abnormal_return(SPY, date_list,N);
        itr++;
    }
    
    auto itr_2 = Stock_Meet.begin();
    while(itr_2 != Stock_Meet.end()){
        itr_2->second.cal_daily_return();
        itr_2->second.cal_abnormal_return(SPY, date_list,N);
        itr_2++;
    }
    
    auto itr_3 = Stock_Miss.begin();
    while(itr_3 != Stock_Miss.end()){
        itr_3->second.cal_daily_return();
        itr_3->second.cal_abnormal_return(SPY, date_list,N);
        itr_3++;
    }

    cout << "Calcualte abnormal return successfull." << endl;
}

void Bootstrap::bootstrapping(int N){
    //get up the stock ID for each group
    vector<string> Stock_Beat_ID;
    vector<string> Stock_Meet_ID;
    vector<string> Stock_Miss_ID;
    auto itr_1 = Stock_Beat.begin();
    while(itr_1 != Stock_Beat.end()){
        Stock_Beat_ID.push_back(itr_1->first);
        itr_1++;
    }
    auto itr_2 = Stock_Meet.begin();
    while(itr_2 != Stock_Meet.end()){
        Stock_Meet_ID.push_back(itr_2->first);
        itr_2++;
    }
    auto itr_3 = Stock_Miss.begin();
    while(itr_3 != Stock_Miss.end()){
        Stock_Miss_ID.push_back(itr_3->first);
        itr_3++;
    }
    
    Beat_AAR.resize(2*N+1);
    Meet_AAR.resize(2*N+1);
    Miss_AAR.resize(2*N+1);
    Beat_CAAR.resize(2*N+1);
    Meet_CAAR.resize(2*N+1);
    Miss_CAAR.resize(2*N+1);
    
    //bootstrap 30 times
    for(int i = 0; i < 30; i++){
        vector<Stock> random_30_Beat = ramdom_30(Stock_Beat,Stock_Beat_ID);
        vector<double> AAR_Beat_temp = cal_AAR(random_30_Beat,N);
        //Beat_AAR += Beat_AAR;
        Beat_AAR = (i / (i + 1.0)) * Beat_AAR + (1 / (i + 1.0)) * (AAR_Beat_temp);
        Beat_AAR_matrix.push_back(AAR_Beat_temp);
        Beat_CAAR_matrix.push_back(cal_CAAR(AAR_Beat_temp,N));
        
        
        vector<Stock> random_30_Meet = ramdom_30(Stock_Meet,Stock_Meet_ID);
        vector<double> AAR_Meet_temp = cal_AAR(random_30_Meet,N);
        //Meet_AAR += AAR_Meet_temp;
        Meet_AAR = (i / (i + 1.0)) * Meet_AAR + (1 / (i + 1.0)) * (AAR_Meet_temp);
        Meet_AAR_matrix.push_back(AAR_Meet_temp);
        Meet_CAAR_matrix.push_back(cal_CAAR(AAR_Meet_temp,N));
        
        vector<Stock> random_30_Miss = ramdom_30(Stock_Miss,Stock_Miss_ID);
        vector<double> AAR_Miss_temp = cal_AAR(random_30_Miss,N);
        //Miss_AAR += AAR_Miss_temp;
        Miss_AAR = (i / (i + 1.0)) * Miss_AAR + (1 / (i + 1.0)) * (AAR_Miss_temp);
        Miss_AAR_matrix.push_back(AAR_Miss_temp);
        Miss_CAAR_matrix.push_back(cal_CAAR(AAR_Miss_temp,N));
    }
    
    Beat_CAAR = cal_CAAR(Beat_AAR,N);
    Meet_CAAR = cal_CAAR(Meet_AAR,N);
    Miss_CAAR = cal_CAAR(Miss_AAR,N);
    
    //calculate std
    Beat_AAR_STD = cal_STD(Beat_AAR_matrix);
    Meet_AAR_STD = cal_STD(Meet_AAR_matrix);
    Miss_AAR_STD = cal_STD(Miss_AAR_matrix);
    
    Beat_CAAR_STD = cal_STD(Beat_CAAR_matrix);
    Meet_CAAR_STD = cal_STD(Meet_CAAR_matrix);
    Miss_CAAR_STD = cal_STD(Miss_CAAR_matrix);
    
    //build a CAAR matrix for plot
    CAAR_combined.push_back(Beat_CAAR);
    CAAR_combined.push_back(Meet_CAAR);
    CAAR_combined.push_back(Miss_CAAR);
    
    //Build the overall result matrix
    Beat_result = {Beat_AAR, Beat_AAR_STD,Beat_CAAR,Beat_CAAR_STD};
    Meet_result = {Meet_AAR,Meet_AAR_STD,Meet_CAAR,Meet_CAAR_STD};
    Miss_result = {Miss_AAR,Miss_AAR_STD,Miss_CAAR,Miss_CAAR_STD};
    
    Result_Matrix = {Beat_result,Meet_result,Miss_result};
    
    cout << "Bootstrap finished" <<endl;
}


//simple_libcurl

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


void Bootstrap:: get_price_data(map<string,Stock> stock_map,string group,int N) {
    
    CURL* handle;
    
    CURLcode result;
    // set up the program environment that libcurl needs
    curl_global_init(CURL_GLOBAL_ALL);
    
    // curl_easy_init() returns a CURL easy handle
    handle = curl_easy_init();
    
    // if everything's all right with the easy handle...
    if (handle)
    {
        string url_common = "https://eodhistoricaldata.com/api/eod/";
        
        string api_token = "5ba84ea974ab42.45160048";
        
        auto itr = stock_map.begin();
        for (; itr != stock_map.end(); itr++)
        {
            vector<double>price;
            struct MemoryStruct data;
            data.memory = NULL;
            data.size = 0;
            
            string symbol = itr -> first;
            //load price data
            itr->second.get_Date(N, date_list);
            string start_date = itr->second.get_start_date();
            string end_date = itr->second.get_end_date();
            
            //change starting date format
            if (symbol.back() == '\r')
                symbol.pop_back();
            string url_request = url_common + symbol + ".US?" + "from=" + start_date + "&to=" + end_date + "&api_token=" + api_token + "&period=d";
            curl_easy_setopt(handle, CURLOPT_URL, url_request.c_str());
            
            //adding a user agent
            curl_easy_setopt(handle, CURLOPT_USERAGENT, "Mozilla/5.0 (Windows NT 10.0; Win64; x64; rv:74.0) Gecko/20100101 Firefox/74.0");
            curl_easy_setopt(handle, CURLOPT_SSL_VERIFYPEER, 0);
            curl_easy_setopt(handle, CURLOPT_SSL_VERIFYHOST, 0);
            
            result = curl_easy_perform(handle);
            
            
            // check for errors
            if (result != CURLE_OK) {
                fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(result));
                break;
            }
            
            curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, write_data2);
            curl_easy_setopt(handle, CURLOPT_WRITEDATA, (void*)&data);
            
            // perform, then store the expected code in result
            result = curl_easy_perform(handle);
            
            if (result != CURLE_OK)
            {
                // if errors have occured, tell us what is wrong with result
                fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(result));
                break;
            }
            
            cout << symbol<< endl;
            stringstream sData;
            sData.str(data.memory);
            //cout<<data.memory<<endl;
            string sValue, sDate;
            double dValue = 0;
            string line;
            
            
            for(int i = 0; i <= 2*N+2; i ++){
                getline(sData, line);
                size_t found = line.find('-');
                if (found != std::string::npos)
                {
                    sDate = line.substr(0, line.find_first_of(','));
                    line.erase(line.find_last_of(','));
                    sValue = line.substr(line.find_last_of(',') + 1);
                    dValue = strtod(sValue.c_str(), NULL);
                    price.push_back(dValue); //string to double
                }
            }
            
            free(data.memory);
            data.size = 0;
            
            itr->second.set_price(price);
        
            if(price.size() == 0){
                stock_map.erase(itr); //delete the stock with no price data
            }
        }
    }
    else
    {
        fprintf(stderr, "Curl init failed!\n");
    }
    
    lock_guard<mutex>guard(mut);
    
    // cleanup since you've used curl_easy_init
    curl_easy_cleanup(handle);
    
    // release resources acquired by curl_global_init()
    curl_global_cleanup();
    
    //update the stock maps
    if(group == "Beat"){
        Stock_Beat = stock_map;
    }else if(group == "Meet"){
        Stock_Meet = stock_map;
    }else{
        Stock_Miss = stock_map;
    }
}

void Bootstrap::get_SPY_data(string start_date,string end_date) {
    vector<double>price;
    // file pointer to create file that store the data
    //FILE* fp;
    
    // declaration of an object CURL
    CURL* handle;
    
    CURLcode result;
    
    // set up the program environment that libcurl needs
    curl_global_init(CURL_GLOBAL_ALL);
    
    // curl_easy_init() returns a CURL easy handle
    handle = curl_easy_init();
    
    // if everything's all right with the easy handle...
    if (handle)
    {
        string url_common = "https://eodhistoricaldata.com/api/eod/";
        
        string api_token = "5ba84ea974ab42.45160048";
        
        struct MemoryStruct data;
        data.memory = NULL;
        data.size = 0;
        
        string symbol = "SPY";
        
        if (symbol.back() == '\r')
            symbol.pop_back();
        string url_request = url_common + symbol + ".US?" + "from=" + start_date + "&to=" + end_date + "&api_token=" + api_token + "&period=d";
        curl_easy_setopt(handle, CURLOPT_URL, url_request.c_str());
        
        //adding a user agent
        curl_easy_setopt(handle, CURLOPT_USERAGENT, "Mozilla/5.0 (Windows NT 10.0; Win64; x64; rv:74.0) Gecko/20100101 Firefox/74.0");
        curl_easy_setopt(handle, CURLOPT_SSL_VERIFYPEER, 0);
        curl_easy_setopt(handle, CURLOPT_SSL_VERIFYHOST, 0);
        
        result = curl_easy_perform(handle);
        
        
        // check for errors
        if (result != CURLE_OK) {
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(result));
        }
        
        curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, write_data2);
        curl_easy_setopt(handle, CURLOPT_WRITEDATA, (void*)&data);
        
        // perform, then store the expected code in result
        result = curl_easy_perform(handle);
        
        if (result != CURLE_OK)
        {
            // if errors have occured, tell us what is wrong with result
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(result));
        }
        
        stringstream sData;
        sData.str(data.memory);
        string sValue, sDate;
        double dValue = 0;
        string line;
        cout << symbol << endl;;
        while (getline(sData, line)) {
            size_t found = line.find('-');
            if (found != std::string::npos)
            {
                sDate = line.substr(0, line.find_first_of(','));
                line.erase(line.find_last_of(','));
                sValue = line.substr(line.find_last_of(',') + 1);
                dValue = strtod(sValue.c_str(), NULL);
                price.push_back(dValue);
                date_list.push_back(sDate);  
            }
        }
        SPY.set_price(price);
        free(data.memory);
        data.size = 0;
    }
    else
    {
        fprintf(stderr, "Curl init failed!\n");
    }
    
    // cleanup since you've used curl_easy_init
    curl_easy_cleanup(handle);
    
    // release resources acquired by curl_global_init()
    curl_global_cleanup();
    
}

