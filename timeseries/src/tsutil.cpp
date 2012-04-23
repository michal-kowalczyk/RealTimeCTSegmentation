#include <vector>
#include <utility>
#include <cmath>
#include "tsutil.h"

namespace TimeSeries {
	
	double dist(double x, double y) {
		return sqrt(pow((x - y), 2));
	}
	
	double dtw(const std::vector<double>& t1, const std::vector<double>& t2) {
		int m = t1.size();
		int n = t2.size();

		// create cost matrix
		double cost[m][n];

		cost[0][0] = dist(t1[0], t2[0]);

		// calculate first row
		for(int i = 1; i < m; i++)
			cost[i][0] = cost[i-1][0] + dist(t1[i], t2[0]);
		// calculate first column
		for(int j = 1; j < n; j++)
			cost[0][j] = cost[0][j-1] + dist(t1[0], t2[j]);
		// fill matrix
		for(int i = 1; i < m; i++)
			for(int j = 1; j < n; j++)
				cost[i][j] = std::min(cost[i-1][j], std::min(cost[i][j-1], cost[i-1][j-1])) 
					+ dist(t1[i],t2[j]);
	
		return cost[m-1][n-1];
	}

	int lcs(const std::vector<double>& t1, const std::vector<double>& t2) {
	
		int m = t1.size() + 1;
		int n = t2.size() + 1;

		// create cost matrix
		int cost[m][n];

		cost[0][0] = 0;

		// first row
		for(int i = 1; i < m; i++)
			cost[i][0] = 0;
		// first column
		for(int j = 1; j < n; j++)
			cost[0][j] = 0;


		for(int i = 1; i < m; i++) {
			for(int j = 1; j < n; j++) {
				 if(std::abs(t1[i-1] - t2[j-1]) <= 1e-10) {
					cost[i][j] = 1 + cost[i-1][j-1];
				} else {
					cost[i][j] = std::max(cost[i][j-1], cost[i-1][j]);
				}
			}
		}
	
		return cost[m-1][n-1];
	}
	
	int twlcs(const std::vector<double>& t1, const std::vector<double>& t2) {
	
		int m = t1.size() + 1;
		int n = t2.size() + 1;

		// create cost matrix
		int cost[m][n];

		cost[0][0] = 0;

		// first row
		for(int i = 1; i < m; i++)
			cost[i][0] = 0;
		// first column
		for(int j = 1; j < n; j++)
			cost[0][j] = 0;


		for(int i = 1; i < m; i++) {
			for(int j = 1; j < n; j++) {
				 if(std::abs(t1[i-1] - t2[j-1]) <= 1e-10) {
					cost[i][j] = 1 + std::max(cost[i][j-1],
						        std::max(cost[i-1][j], 
								cost[i-1][j-1]));
				} else {
					cost[i][j] = std::max(cost[i][j-1], 
							cost[i-1][j]);
				}
			}
		}
		
		return cost[m-1][n-1];
	}
}
