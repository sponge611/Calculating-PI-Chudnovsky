#include <iostream>
#include <string>
#include <assert.h>
#include <thread>
#include <stdexcept>
#include <cmath>
#include <time.h>
#include <vector>
#include <mutex>

#define constA 13591409
#define constB 545140134
#define constC 640320
using namespace std;
const auto processor_count = thread::hardware_concurrency();
mutex mut; // Prevent race condition

double pi = 0;

double factorial(int k){
    if(0 == k || 1 == k) return 1;
    double result = 1;
    while(k>=1){
        result *= k;
        k--;
    }
    return result;
}

void Chudnovsky(int i){
    // Use Chundnovsky Series to calculate pi;
    double denominator = 0;
    double numerator = 0;
    double sum = 0;
    double sign = i%2==0?1:-1;
    denominator = sign*factorial(6*i)*(constA+constB*i);
    double fac = factorial(i);
    numerator = factorial(3*i)*fac*fac*fac*pow(constC,3*i+1.5);
    sum = denominator/numerator;
    mut.lock();
    pi += sum;
    mut.unlock();
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
    int precision = digits/14+1; // If 1 item in Chundnovsky Series is calculated, 14 
    vector<thread> threads;
    for(int i=0;i<processor_count && i<precision;i++){
        threads.push_back(thread(&Chudnovsky,i));
    }
    for(auto& t:threads){
        t.join();
    }
    cout.precision(digits+1);
    pi = 1/(12*pi);
    cout << pi << endl;
    cout.precision(10);
    cout << "Total Execution Time : "<< (double)clock() / CLOCKS_PER_SEC << "Sec";
    return 0;
}