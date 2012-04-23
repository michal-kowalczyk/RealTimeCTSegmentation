#include <vector>

#ifndef TSUTIL_H
#define TSUTIL_H
/**
 * @author philipp
 * @date 2010
*/
namespace TimeSeries {
	
	/**
	 * @brief Calculates the DTW Distance of two timeseries.
	 * @param[in] t1 a timeseries
	 * @param[in] t2 a timeseries
	 * @return DTW Distance
	 */
	double dtw(const std::vector<double>& t1, const std::vector<double>& t2);
	
	/**
	 * @brief Calculates the Longest Common Subsequence of two timeseries.
	 * @param[in] t1 a timeseries
	 * @param[in] t2 a timeseries
	 * @return LCS
	 */
	int lcs(const std::vector<double>& t1, const std::vector<double>& t2);
	
	/**
	 * @brief Calculates the Time Warped Longes Common Subsequence of two timeseries.
	 * @param[in] t1 a timeseries
	 * @param[in] t2 a timeseries
	 * @return Time-Warped LCS.
	 */
	int twlcs(const std::vector<double>& t1, const std::vector<double>& t2);

}

#endif
