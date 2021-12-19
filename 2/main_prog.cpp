/********   All Required Header Files ********/
#include <vector>
#include <climits>
#include <numeric>
#include <iostream>

using namespace std;

// Max-Heapify method to maintain heap property
void maxHeapify(vector<int> &heap, int index, int heapSize)
{
    // Find largest by comparing with left and right child
    int largest = index;
    int left = 2 * index + 1;
    int right = 2 * index + 2;
    if (left < heapSize && heap[left] > heap[largest]) {
        largest = left;
    }
    if (right < heapSize && heap[right] > heap[largest]) {
        largest = right;
    }

    // Recursively call maxHeapify if root is not the largest
    if (largest != index) {
        swap(heap[index], heap[largest]);
        maxHeapify(heap, largest, heapSize);
    }
}

// Main method to do Heapsort
void heapSort(vector<int> &heap)
{
    // Build-Heap: call maxHeapify for first (n/2) elements
    for (int i = (heap.size() / 2) - 1; i >= 0; i--) {
        maxHeapify(heap, i, heap.size());
    }

    // Heapsort: sorting array in non-decreasing order
    for (int i = heap.size() - 1; i >= 0; i--) {
        swap(heap[0], heap[i]);
        maxHeapify(heap, 0, i);
    }
}

// Greedy solution for the problem
void solveGreedy(vector<int> &efforts, int maximumEffort) {

    // Stores the breakup for the total effort
    string effortsString = "";

    // Stores the total effort
    int totalEffort = 0;

    // Sort the efforts in non-decreasing order
    heapSort(efforts);

    // Find solution starting from minimum effort
    for (int effort : efforts) {
        if (effort <= maximumEffort) {
            effortsString = effortsString + " + " + to_string(effort);
            totalEffort = totalEffort + effort;
            maximumEffort = maximumEffort - effort;
        } else {
            break;
        }
    }
    cout << "Minimum effort = " << totalEffort << (effortsString.empty() ?  "" : (" = " + effortsString.substr(3))) << endl;
}

// Dynamic programming solution for the problem
void solveDP(vector<int> &points, vector<int> &efforts, int maximumEffort, int numberOfDishes) {

    // Total possible points
    int totalPoints = accumulate(points.begin(), points.end(), 0);

    // 2D vector used for tabulation, initially all have INFINITY
    vector<vector<int>> table(numberOfDishes + 1, vector<int>(totalPoints + 1, INT_MAX));

    // Initialize the base case for table[i][0]
    for (int i = 0; i < (numberOfDishes + 1); i++) {
        table[i][0] = 0;
    }

    // Bottom-Up tabulation
    for (int i = 1; i < (numberOfDishes + 1); i++) {
        for (int j = 1; j < (totalPoints + 1); j++) {
            if (points[i - 1] > j) {
                table[i][j] = table[i - 1][j];
            } else {

                // Additional check to avoid overflow
                if (table[i - 1][j - points[i - 1]] == INT_MAX || (efforts[i - 1] + table[i - 1][j - points[i - 1]]) > maximumEffort) {
                    table[i][j] = min(table[i - 1][j], INT_MAX);
                } else {
                    table[i][j] = min(table[i - 1][j], (efforts[i - 1] + table[i - 1][j - points[i - 1]]));
                }
            }
        }
    }

    // Stores solution for maximum points and effort
    int maximumPoints, minimumEffort;
    
    // Stores the breakup for maximum points and effort
    string pointsString = "", effortsString = "";

    // Find maximum points and effort from last row of the table
    for (int j = totalPoints; j >= 0; j--) {
        if (table[numberOfDishes][j] <= maximumEffort) {
            maximumPoints = j;
            minimumEffort = table[numberOfDishes][j];
            break;
        }
    }

    // Find the breakup for maximum points and effort
    for (int i = numberOfDishes, j = maximumPoints; i > 0 && j > 0; ) {
        if (points[i - 1] > j) {
            i = i - 1;
        } else if (table[i][j] == efforts[i - 1] + table[i - 1][j - points[i - 1]]) {
            pointsString = " + " + to_string(points[i - 1]) + pointsString;
            effortsString = " + " + to_string(efforts[i - 1]) + effortsString;
            j = j - points[i - 1];
            i = i - 1;
        } else {
            i = i - 1;
        }
    }
    cout << "Maximum points = " << maximumPoints << (pointsString.empty() ?  "" : (" = " + pointsString.substr(3))) << endl;
    cout << "Minimum effort = " << minimumEffort << (effortsString.empty() ?  "" : (" = " + effortsString.substr(3))) << endl;
}

int main()
{
    int n, m;
    cout << "n = ";
    cin >> n;
    cout << "M = ";
    cin >> m;
    vector<int> efforts, points;

    // Read inputs for Greedy solution and call solveGreedy
    cout << "+++ Efforts : ";
    for (int i = 1; i <= n; i++) {
        int effort;
        cin >> effort;
        efforts.push_back(effort);
    }
    cout << "+++ Part 1 (Greedy)" << endl;
    solveGreedy(efforts, m);

    // Read inputs for Dynamic programming solution and call solveDP
    efforts.clear();
    cout << "+++ Points : ";
    for (int i = 1; i <= n; i++) {
        int point;
        cin >> point;
        points.push_back(point);
    }
    cout << "+++ Efforts : ";
    for (int i = 1; i <= n; i++) {
        int effort;
        cin >> effort;
        efforts.push_back(effort);
    }
    cout << "+++ Part 2 (DP)" << endl;
    solveDP(points, efforts, m, n);
}