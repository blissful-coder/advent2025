#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <map>
#include <set>

#define ll long long


bool isSilly(ll num){
    // std::cout<<"\nChecking Silly:"<<num;
    std::string temp = std::to_string(num);
    int len = temp.length();
    if (len%2==0 && len>0){
        std::string firstHalf = temp.substr(0,len/2);
        std::string secondHalf = temp.substr(len/2);
        if (firstHalf==secondHalf){
            return true;
        }
    }
    return false;
}

bool isSilly2(ll num){
    // std::cout<<"\nChecking Silly2:"<<num;
    std::string temp = std::to_string(num);
    int len = temp.length();
    
    // Try different pattern lengths, from 1 to half the string length
    for (int patternLen = 1; patternLen <= len/2; patternLen++) {
        // Only consider if the string length is divisible by the pattern length
        if (len % patternLen == 0) {
            std::string pattern = temp.substr(0, patternLen);
            bool isRepeated = true;
            
            // Check if the entire string consists of repetitions of this pattern
            for (int i = patternLen; i < len; i += patternLen) {
                if (temp.substr(i, patternLen) != pattern) {
                    isRepeated = false;
                    break;
                }
            }
            
            // If the pattern repeats throughout the string and repeats at least twice
            if (isRepeated && len / patternLen >= 2) {
                return true;
            }
        }
    }
    return false;
}

// Part 1 solution
ll solvePart1(ll first, ll last) {
    std::cout<<"\nSearching between:"<<first<<" "<<last;
    ll sum = 0;
    for (ll i = first; i<=last; i++){
        if (isSilly(i)){
            std::cout<<"\nFound:"<<i;
            sum = sum + i;
        }
    }
    return sum;
}

// Part 2 solution
ll solvePart2(ll first, ll last) {
    std::cout<<"\nSearching between:"<<first<<" "<<last;
    ll sum = 0;
    for (ll i = first; i<=last; i++){
        if (isSilly2(i)){
            std::cout<<"\nFound:"<<i;
            sum = sum + i;
        }
    }
    return sum;
}

int main() {
    // Read input into a vector to process it twice
    std::vector<std::pair<ll, ll>> ranges;
    ll a, b;
    char dash;
    
    while(std::cin >> a >> dash >> b) {
        if(dash == '-') {
            ranges.push_back({a, b});
        }
    }
    
    // Solve Part 1
    ll p1 = 0;
    for(const auto& range : ranges) {
        p1 += solvePart1(range.first, range.second);
    }
    std::cout << "\nPart 1 Answer: " << p1;
    
    // Solve Part 2
    ll p2 = 0;
    for(const auto& range : ranges) {
        p2 += solvePart2(range.first, range.second);
    }
    std::cout << "\nPart 2 Answer: " << p2;
    
    return 0;
}
