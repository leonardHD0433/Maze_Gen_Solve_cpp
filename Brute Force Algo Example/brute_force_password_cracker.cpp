/***************************************************************************************
Reference 1
*    Title: Brute Force Password Cracker
*    Author: CyanCoding
*    Date: 2021
*    Code version: -
*    Availability: https://github.com/CyanCoding/Brute-Force-Password-Cracker/blob/master/C%2B%2B/Brute%20Force%20Password%20Cracker.cpp

***************************************************************************************/

#include <iostream>
#include <string>
#include <iomanip>
#include <thread>
#include <mutex>
#include <fstream>
#include <condition_variable>
using namespace std;

mutex mtx;
condition_variable cv;
bool stop = false;
long long amount = 0;
string password;
clock_t start;

const char Alphabet[62] = {
    'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U','V', 'W', 'X', 'Y', 'Z'
};

string separateWithCommas(long long num) {
    string s = to_string(num);
    int thousands = s.length() - 3;

    while (thousands > 0) {
        s.insert(thousands, ",");
        thousands -= 3;
    }

    return s;
}

void crack(unsigned int length, string current, int startIdx, int endIdx, long long &localAmount) {
    if (length == 0 && stop == false) {
        localAmount++;
        if (localAmount % 10000000 == 0) {
            lock_guard<mutex> lock(mtx);
            cout << '\r' << separateWithCommas(amount + localAmount) << " - " << current << " - " << separateWithCommas((amount + localAmount) / ((float)(clock() - start) / CLOCKS_PER_SEC)) << " p/sec";
            cout.flush();
        }
        if (current == password) {
            stop = true;
            cv.notify_all();
        }
        return;
    }
    if (stop == false) {
        for (int i = startIdx; i < endIdx; i++) {
            crack(length - 1, current + Alphabet[i], 0, 62, localAmount);
        }
    }
}

void threadFunction(unsigned int length, int startIdx, int endIdx) {
    long long localAmount = 0;
    crack(length, "", startIdx, endIdx, localAmount);
    lock_guard<mutex> lock(mtx);
    amount += localAmount;
}

int main() {
    // Greet the user
    cout << "Welcome to CyanCoding's Brute Force Password Cracker!" << endl << endl;
    cout << "What do you want your password to be? > ";
    cin >> password;

    cout << "\rAttempting to crack " << password << "..." << endl;

    start = clock();

    const int numThreads = 30; // Number of threads
    thread threads[numThreads];

    while (stop == false) {
        static unsigned int pwLength = 1;

        // Divide the search space among threads
        int range = 62 / numThreads;
        for (int i = 0; i < numThreads; i++) {
            int startIdx = i * range;
            int endIdx = (i == numThreads - 1) ? 62 : startIdx + range;
            threads[i] = thread(threadFunction, pwLength, startIdx, endIdx);
        }

        // Join threads
        for (int i = 0; i < numThreads; i++) {
            threads[i].join();
        }

        pwLength++;
        if (stop == true) {
            break;
        }
    }

    cout << "Threads Used: " << numThreads << endl << "\rCyanCoding's C++ BFPC cracked the password \"" << password << "\" in " <<
        separateWithCommas(amount) << " attempts and " << setprecision(2) << fixed <<
        (float)(clock() - start) / CLOCKS_PER_SEC << " seconds." << endl << endl <<
        "That's about " << setprecision(0) <<
        separateWithCommas(amount / ((float)(clock() - start) / CLOCKS_PER_SEC)) <<
        " passwords per second!" << endl << endl;

    // Save to file
    ofstream outFile("results.txt", ios::app); // Open in append mode
    if (outFile.is_open()) {
        outFile << "Threads Used: " << numThreads << endl << "Password: " << password << endl << "Attempts: " <<
            separateWithCommas(amount) << " attempts" << endl << "Time: " << setprecision(2) << fixed <<
            (float)(clock() - start) / CLOCKS_PER_SEC << " seconds." << endl <<
            "P/S: " << setprecision(0) <<
            separateWithCommas(amount / ((float)(clock() - start) / CLOCKS_PER_SEC)) <<
            " passwords per second!" << endl << endl;
        outFile.close();
    } else {
        cerr << "Unable to open file for writing." << endl;
    }

    return 0;
}