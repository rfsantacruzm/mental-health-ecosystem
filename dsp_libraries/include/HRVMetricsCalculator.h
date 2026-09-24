/**
 * @file HRVMetricsCalculator.h
 * @brief Time-domain Heart Rate Variability (HRV) metrics calculation module.
 * @details Processes RR intervals (inter-beat intervals) to extract autonomic biomarkers (SDNN, RMSSD, pNN50).
 * @author Ramon F. Santacruz
 */

#ifndef BIO_SIGNAL_DSP_HRV_METRICS_CALCULATOR_H
#define BIO_SIGNAL_DSP_HRV_METRICS_CALCULATOR_H

#include <vector>
#include <cstddef>

namespace BioSignalDSP {

/**
 * @brief Data structure encapsulating calculated physiological HRV metrics.
 */
struct HRVMetrics {
    double mean_rr_ms{0.0};     ///< Average RR interval duration in milliseconds.
    double sdnn_ms{0.0};        ///< Standard deviation of NN/RR intervals (SDNN).
    double rmssd_ms{0.0};       ///< Root mean square of successive differences (RMSSD).
    double pnn50_percent{0.0};  ///< Percentage of successive RR interval differences > 50 ms.
};

/**
 * @brief Class for time-domain Heart Rate Variability (HRV) signal processing.
 */
class HRVMetricsCalculator {
public:
    /**
     * @brief Default constructor.
     */
    HRVMetricsCalculator() = default;

    /**
     * @brief Constructor initialized with a set of RR intervals.
     * @param rr_intervals Vector containing inter-beat intervals in milliseconds.
     */
    explicit HRVMetricsCalculator(std::vector<double> rr_intervals);

    /**
     * @brief Appends a single RR interval to the internal buffer.
     * @param interval_ms Duration between consecutive beats in milliseconds.
     */
    void addRRInterval(double interval_ms);

    /**
     * @brief Replaces the current buffer with a new set of RR intervals.
     * @param intervals Vector containing RR intervals in milliseconds.
     */
    void setRRIntervals(const std::vector<double>& intervals);

    /**
     * @brief Clears all stored RR intervals from the buffer.
     */
    void clear();

    /**
     * @brief Calculates the mean duration of RR intervals.
     * @return Mean RR interval in milliseconds.
     */
    double calculateMeanRR() const;

    /**
     * @brief Calculates SDNN (Standard Deviation of NN intervals).
     * @details Reflects global autonomic variability over the observation window.
     * @return SDNN in milliseconds.
     */
    double calculateSDNN() const;

    /**
     * @brief Calculates RMSSD (Root Mean Square of Successive Differences).
     * @details Primary biomarker for parasympathetic (vagal) tone and stress monitoring.
     * @return RMSSD in milliseconds.
     */
    double calculateRMSSD() const;

    /**
     * @brief Calculates pNN50 (Percentage of adjacent RR intervals differing by > 50 ms).
     * @details Complementary indicator of parasympathetic activity and sudden beat-to-beat changes.
     * @return Percentage value between 0.0 and 100.0.
     */
    double calculatePNN50() const;

    /**
     * @brief Calculates all time-domain HRV metrics in a single pass.
     * @return Struct containing all calculated HRV metrics.
     */
    HRVMetrics calculateAllMetrics() const;

    /**
     * @brief Retrieves the total number of RR intervals currently stored in the buffer.
     * @return Sample count.
     */
    std::size_t getSampleCount() const;

private:
    std::vector<double> rr_intervals_;
};

} // namespace BioSignalDSP

#endif // BIO_SIGNAL_DSP_HRV_METRICS_CALCULATOR_H