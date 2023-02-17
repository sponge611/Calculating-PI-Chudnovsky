#include <iostream>
#include <string>
#include <assert.h>
#include <thread>
#include <stdexcept>
#include <cmath>
#include <time.h>
#include <vector>
#include <mutex>
#include <boost/multiprecision/cpp_int.hpp>
#include <math.h> 

// Use boost::multiprecision::int1024_t to extend the limitation of double type 
// (double can only represent 3.141592653589793560087173318606801331043243408203125)

using namespace std;

boost::multiprecision::int1024_t pi = 0;

boost::multiprecision::int1024_t factorial(int k){
    if(0 == k || 1 == k) return 1;
    boost::multiprecision::int1024_t result = 1;
    while(k>=1){
        result *= k;
        k--;
    }
    return result;
}

void Chudnovsky(boost::multiprecision::int1024_t precision){
    // Use Chundnovsky Series to calculate pi;
    boost::multiprecision::int1024_t i = 1;
    boost::multiprecision::int1024_t a_i = precision;
    boost::multiprecision::int1024_t a_sum = precision;
    boost::multiprecision::int1024_t b_sum = 0;
    boost::multiprecision::int1024_t constC = 640320;
    boost::multiprecision::int1024_t constC_over_24 = constC*constC*constC / 24;
    while(true){
        a_i *= -(6*i-5)*(2*i-1)*(6*i-1);
        boost::multiprecision::int1024_t tmp = a_i / (i*i*i*constC_over_24);
        if(a_i*(i*i*i*constC_over_24) < 1){
            a_i = tmp-1;
        }
        else{
            a_i = tmp;
        }
        a_sum += a_i;
        b_sum += i * a_i;
        i++;
        if(a_i == 0)
            break;
    }
    boost::multiprecision::int1024_t total = 13591409*a_sum + 545140134*b_sum;
    pi = (426880*boost::multiprecision::sqrt(10005*precision*precision)*precision) / total;
}

int main(int argc, char *argv[]){
    int digits = 0;
    try{
        string argv1(argv[1]);
        if (argc != 3 || argv1 != "-p") throw invalid_argument("Incorrect command");
    }
    catch (exception &err){
        std::cerr << "The command should be ./pi -p [Non-negative Integer]\n";
        return 1;
    }
    try{
        digits = stoi(argv[2]);
        if(digits < 0) throw invalid_argument("Received negative number");
    }
    catch (exception &err){
        std::cerr << "You must input a number >= 0.\n";
        return 1;
    }
    vector<thread> threads;
    boost::multiprecision::int1024_t precision = 10;
    precision = boost::multiprecision::pow(precision, digits);
    for(int i=0; i < 1;i++){
        threads.push_back(thread(&Chudnovsky,precision));
    }
    for(auto& t:threads){
        t.join();
    }
    cout << 3 << "." <<  pi%precision << endl;
    cout.precision(10);
    cout << "Total Execution Time : "<< (double)clock() / CLOCKS_PER_SEC << "Sec";
    return 0;
}