#include "HRVMetricsCalculator.h"
#include <numeric>   // std::accumulate
#include <cmath>     // std::sqrt, std::abs
#include <algorithm> // std::for_each

namespace BioSignalDSP {

HRVMetricsCalculator::HRVMetricsCalculator(std::vector<double> rr_intervals)
    : rr_intervals_(std::move(rr_intervals)) {}

void HRVMetricsCalculator::addRRInterval(double interval_ms) {
    if (interval_ms > 0.0) {
        rr_intervals_.push_back(interval_ms);
    }
}

void HRVMetricsCalculator::setRRIntervals(const std::vector<double>& intervals) {
    rr_intervals_ = intervals;
}

void HRVMetricsCalculator::clear() {
    rr_intervals_.clear();
}

std::size_t HRVMetricsCalculator::getSampleCount() const {
    return rr_intervals_.size();
}

double HRVMetricsCalculator::calculateMeanRR() const {
    if (rr_intervals_.empty()) {
        return 0.0;
    }
    double sum = std::accumulate(rr_intervals_.begin(), rr_intervals_.end(), 0.0);
    return sum / static_cast<double>(rr_intervals_.size());
}

double HRVMetricsCalculator::calculateSDNN() const {
    const std::size_t n = rr_intervals_.size();
    if (n < 2) {
        return 0.0;
    }

    const double mean_rr = calculateMeanRR();
    double variance_sum = 0.0;

    for (const double rr : rr_intervals_) {
        const double diff = rr - mean_rr;
        variance_sum += diff * diff;
    }

    return std::sqrt(variance_sum / static_cast<double>(n - 1));
}

double HRVMetricsCalculator::calculateRMSSD() const {
    const std::size_t n = rr_intervals_.size();
    if (n < 2) {
        return 0.0;
    }

    double successive_diff_sum = 0.0;
    for (std::size_t i = 0; i < n - 1; ++i) {
        const double diff = rr_intervals_[i + 1] - rr_intervals_[i];
        successive_diff_sum += diff * diff;
    }

    return std::sqrt(successive_diff_sum / static_cast<double>(n - 1));
}

double HRVMetricsCalculator::calculatePNN50() const {
    const std::size_t n = rr_intervals_.size();
    if (n < 2) {
        return 0.0;
    }

    std::size_t count_nn50 = 0;
    for (std::size_t i = 0; i < n - 1; ++i) {
        const double diff = std::abs(rr_intervals_[i + 1] - rr_intervals_[i]);
        if (diff > 50.0) {
            ++count_nn50;
        }
    }

    return (static_cast<double>(count_nn50) / static_cast<double>(n - 1)) * 100.0;
}

HRVMetrics HRVMetricsCalculator::calculateAllMetrics() const {
    HRVMetrics metrics{};
    
    if (rr_intervals_.size() < 2) {
        return metrics;
    }

    metrics.mean_rr_ms = calculateMeanRR();
    metrics.sdnn_ms = calculateSDNN();
    metrics.rmssd_ms = calculateRMSSD();
    metrics.pnn50_percent = calculatePNN50();

    return metrics;
}

} // namespace BioSignalDSP