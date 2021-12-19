/********   All Required Header Files ********/
#include <math.h>
#include <limits.h>
#include <float.h>
#include <vector>
#include <iostream>

using namespace std;

// Global variables
double minimum_distance = DBL_MAX; // This stores the minimum distance
pair<double, double> first_point, second_point; // These are the two closest points

// This is the merge step of standard merge sort algorithm, modified to sort the array of 2D coordinates
// The boolean switch "sort_by_x" is to decide if need to sort by X or Y coordinate
void merge(vector<pair<double, double>> &points, int low, int mid, int high, bool sort_by_x = true) {

    // Copy the elements of sorted left and right subarray into two tempurary array
    vector<pair<double, double>> left_points, right_points;
    for (int i = low; i <= mid; i++) {
        left_points.push_back(points[i]);
    }
    for (int i = mid + 1; i <= high; i++) {
        right_points.push_back(points[i]);
    }

    // Merge the two arrays
    int pos = low, i = 0, j = 0;
    while (i < left_points.size() && j < right_points.size()) {
        // Depending on "sort_by_x" switch compare first(stores X) or second(stores Y)
        if (sort_by_x) {
            if (left_points[i].first <= right_points[j].first) {
                points[pos++] = left_points[i++];
            } else {
                points[pos++] = right_points[j++];
            }
        } else {
            if (left_points[i].second <= right_points[j].second) {
                points[pos++] = left_points[i++];
            } else {
                points[pos++] = right_points[j++];
            }
        }
    }

    // Copy rest of the elements
    while (i < left_points.size()) {
        points[pos++] = left_points[i++];
    }
    while (j < right_points.size()) {
        points[pos++] = right_points[j++];
    }
}

// Main merge sort function
void mergeSortFor2DPoints(vector<pair<double, double>> &points, int low, int high, bool sort_by_x = true) {
    if (low < high) {
        int mid = (low + high) / 2;
        mergeSortFor2DPoints(points, low, mid, sort_by_x);
        mergeSortFor2DPoints(points, mid + 1, high, sort_by_x);
        merge(points, low, mid, high, sort_by_x);
    }
}

// This function is to calculate Euclidean distance between two points
double getEuclideanDistance(pair<double, double> point_1, pair<double, double> point_2) {
    return sqrt(pow(point_1.first - point_2.first, 2) + pow(point_1.second - point_2.second, 2));
}

// This is the brute force way to find closest pairs which is called only when there are
// atmost 3 points to check(in the base case of the recursion)
double findMinimumByBruteForce(vector<pair<double, double>> &points) {
    double distance = INT_MAX;
    for (int i = 0; i < points.size() - 1; i++) {
        for (int j = i + 1; j < points.size(); j++) {
            double distance = getEuclideanDistance(points[i], points[j]);

            // Update answers if we have found a new minimum
            if (distance < minimum_distance) {
                minimum_distance = distance;
                first_point = points[i];
                second_point = points[j];
            }
        }
    }
    return distance;
}

// This function is to find closest points crossing the mid or in the (mid - d) to (mid + d) region
// This function takes O(n) time as the inner loop is guarented to iterate maximum 7 times
double closestPointsOnStrip(vector<pair<double, double>> &points_sorted_by_y, double distance) {
    double min_distance = distance;
    for (int i = 0; i < points_sorted_by_y.size() - 1; i++) {

        // For any point only next 7 points need to check at max
        for (int j = i + 1; j < points_sorted_by_y.size() && abs(points_sorted_by_y[j].second - points_sorted_by_y[i].second) < distance; j++) {
            min_distance = min(getEuclideanDistance(points_sorted_by_y[i], points_sorted_by_y[j]), minimum_distance);

            // Update answers if we have found a new minimum
            if (min_distance < minimum_distance) {
                minimum_distance = min_distance;
                first_point = points_sorted_by_y[i];
                second_point = points_sorted_by_y[j];
            }
        }
    }
    return min_distance;
}

// Main recursive definition to find closest pair of points
double findClosestPairOfPoints(vector<pair<double, double>> &points_sorted_by_x, vector<pair<double, double>> &points_sorted_by_y, int low, int high) {

    // Base case: if no of points are less than 4, get solution by brute force
    if (high - low + 1 <= 3) {
        return findMinimumByBruteForce(points_sorted_by_x);
    }

    // Find the mid point from "points_sorted_by_x" array
    double mid = (low + high) / 2;
    pair<double, double> mid_point = points_sorted_by_x[mid];

    // Find the points in the left and right of mid point, which are sorted by Y coordinate
    vector<pair<double, double>> points_sorted_by_y_left, points_sorted_by_y_right;
    for (auto point : points_sorted_by_y) {
        if (point.first <= mid_point.first) {
            points_sorted_by_y_left.push_back(point);
        } else {
            points_sorted_by_y_right.push_back(point);
        }
    }

    // Recursively find closest points in the left and right sub-array
    double min_distance_left = findClosestPairOfPoints(points_sorted_by_x, points_sorted_by_y_left, low, mid);
    double min_distance_right = findClosestPairOfPoints(points_sorted_by_x, points_sorted_by_y_right, mid + 1, high);
    double min_distance = min(min_distance_left, min_distance_right); // Minimum distance from left and right sub-array

    // Find all the points in the range (mid - min_distance) to (mid + min_distance) which
    // are sorted by Y coordinate and store into "points_on_strip" array
    vector<pair<double, double>> points_on_strip;
    for (auto point : points_sorted_by_y) {
        if (abs(point.first - mid_point.first) <= min_distance) {
            points_on_strip.push_back(point);
        }
    }

    // Find the closest pair on the strip and return final minimum
    return min(min_distance, closestPointsOnStrip(points_on_strip, min_distance));
}

// Main function
int main()
{
    int n; // Number of points
    vector<pair<double, double>> points_sorted_by_x, points_sorted_by_y; // To sort and store all points by X and Y coordinate

    // Take inputs
    cout << "How many points are there on the 2D plane? ";
    cin >> n;
    for (int i = 1; i <= n; i++) {
        double x, y;
        cout << "Enter the coordinates of Point " << i << endl;
        cout << "x" << i << " : ";
        cin >> x ;
        cout << "y" << i << " : ";
        cin >> y;
        points_sorted_by_x.push_back(make_pair(x, y));
        points_sorted_by_y.push_back(make_pair(x, y));
    }
    if(n < 2){
        cout << "Please enter the coordinates for atleast 2 points" << endl;
        return 0;
    }

    mergeSortFor2DPoints(points_sorted_by_x, 0, n - 1, true); // Sort by X coordinate
    mergeSortFor2DPoints(points_sorted_by_y, 0, n - 1, false); // Sort by Y coordinate

    // Call the findClosestPairOfPoints function to get the closest points
    findClosestPairOfPoints(points_sorted_by_x, points_sorted_by_y, 0, n - 1);
    cout << "The closest pair of points are (" << first_point.first << ", " << first_point.second << ") and (" << second_point.first << ", " << second_point.second << "). The distance between them is " << minimum_distance << " units." << endl << endl;
}