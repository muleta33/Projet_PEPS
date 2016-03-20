#pragma once
#include "pnl/pnl_vector.h"
#include "pnl/pnl_matrix.h"

namespace input_parameters
{
	const double PRECISION = 1.0e-10;

	extern void compute_returns(PnlMat * log_returns, const PnlMat * data);

	extern double compute_variance(const PnlVect * data);

	extern double compute_covariance(const PnlVect * data1, const PnlVect * data2);

	extern double compute_mean(const PnlVect * data);

	extern void compute_correlation_matrix(PnlMat * correlation_matrix, const PnlMat * data, const PnlVect * volatilities);

	extern void compute_volatilities(PnlVect * volatilities, const PnlMat * data);
}