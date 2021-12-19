/********   All Required Header Files ********/
#include <math.h>
#include <random>
#include <climits>
#include <numeric>
#include <iostream>

using namespace std;

// Initialize uniform distribution to generate random number
std::random_device                  rand_dev;
std::mt19937                        generator(rand_dev());
std::uniform_int_distribution<int>  distribution(0, INT_MAX);

/*
- It's the fast modulo exponentiation method to calcualte (a^b mod n)
- Default value of n is largest "Long" value, so that it can be used to calculate a^b by avoiding overflow
- Time complexity: O(logb)
*/
long long modular_higher_power(long long a, long long b, long long n = LLONG_MAX)
{
    long long result = 1;
    a = a % n;
    if (a == 0) {
        return 0;
    }
    while (b > 0)
    {
        if (b % 2 == 1) {
            result = (result * a) % n;
        }
        b = b / 2;
        a = (a * a) % n;
    }
    return result;
}

/*
- This method is to find if a^b = n for given "b" and "n" using "binary search"
- We know that "b" will be in range of (2, logn), so we call this method roughly "logn" times
- Time complexity: O(logn)
*/
bool higher_power(long long b, long long n) {
    long long left = 2, right = n;
    while (left <= right) {
        long long mid = left + (right - left) / 2;

        // Calculate mid^b using modular_higher_power method(faster than pow() and avoids overflow)
        long long result = modular_higher_power(mid, b);
        if (result == n) {
            return true;
        }
        if (result < n) {
            left = mid + 1;
        } else {
            right = mid - 1;
        }
    }
    return false;
}

/*
- This method is to find "k" and "t" such that x = 2^k.t and returns "t"(odd number)
- Time complexity: O(logx)
*/
long long two_factorize(long long x) {

    // "k" will be in a range of (1, logx)
    for (long long k = 1; k <= (long long)ceil(log2(x)); k++) {

        // Calculate 2^k
        long long power_of_two = modular_higher_power(2, k);
        long long t = x / power_of_two;

        // Check if 2^k perfectly divides "x" and "t" is odd
        if (x % power_of_two == 0 && t % 2 == 1) {
            return t;
        }
    }
    return 0;
}

/*
- It's the main Miller-Rabin primality testing method
- Returns "false" if "n" is composite and "true" if its probably prime
- Time compexity: polynotial to logn
*/
bool Miller_Rabin_test(long long n) {

    // Prime power checking: for "b" in range (2, logn), check if a^b = n
    // If "yes" then return "composite"
    for (long long b = 2; b <= (long long)ceil(log2(n)); b++) {
        if (higher_power(b, n)) {
            return false;
        }
    }

    // "a" is an uniformly generated random number between (1, n-1)
    long long a = distribution(generator) % (n - 1) + 1;

    // Check if a^(n-1) mod n = 1, if "not" then return "composite"
    long long fermat_result = modular_higher_power(a, n - 1, n);
    if (fermat_result != 1) {
        return false;
    }

    // Find "k" and "t" such that n-1 = 2^k.t where "t" is an odd number
    long long t = two_factorize(n - 1);

    // If a^t mod n = 1 then "n" is probably prime
    long long previous = modular_higher_power(a, t, n), current = 0;
    if (previous == 1) {
        return true;
    }
    t = t * 2;

    // Keep checking if a^t mod n = 1 and increase "t" as (t*2) at each step
    while (true) {
        current = modular_higher_power(a, t, n);

        // If a^t mod n = 1 in current step
        if (current == 1) {

            // If the previous step result was not (n-1) then "n" is composite else probably prime
            if (previous != n - 1) {
                return false;
            } else {
                return true;
            }
        }

        // Update previous
        previous = current;
        t = t * 2;
    }
    return true;
}

int main()
{
    long long n, r;
    cout << "n = ";
    cin >> n;
    cout << "r = ";
    cin >> r;
    string answer = "prime";

    // If "n"(>=3) is even then it's surely composite 
    if (n >= 3 && n % 2 == 0) {
        answer = "composite";
    } else {
        // Conduct Miller-Rabin test "r" times
        // Given number "n" is composite if atleast one of the experiment returns "false"(means composite)
        // otherwise it's probably prime
        for (long long i = 1; i <= r; i++) {
            if (!Miller_Rabin_test(n)) {
                answer = "composite";
                break;
            }
        }
    }

    cout << n << " is a " << answer << " number." << endl;
}