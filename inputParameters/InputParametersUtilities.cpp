#include "InputParametersUtilities.hpp"
#include <math.h>

namespace input_parameters
{

	void compute_returns(PnlMat * log_returns, const PnlMat * data) {
		for (int j = 0; j < data->n; j++) {
			for (int i = 0; i < data->m - 1; i++) {
				MLET(log_returns, i, j) = log(MGET(data, i + 1, j) / MGET(data, i, j));
			}
		}
	}

	double compute_variance(const PnlVect * data) {
		double mean = compute_mean(data);
		PnlVect * sum_square = pnl_vect_new();
		pnl_vect_clone(sum_square, data);
		pnl_vect_mult_vect_term(sum_square, sum_square);
		double var = compute_mean(sum_square) - mean*mean;
		if (var < PRECISION)
			var = 0.0;
		pnl_vect_free(&sum_square);
		return var;
	}

	double compute_covariance(const PnlVect * data1, const PnlVect * data2) {
		double mean1 = compute_mean(data1);
		double mean2 = compute_mean(data2);
		PnlVect * sum_prod = pnl_vect_new();
		pnl_vect_clone(sum_prod, data1);
		pnl_vect_mult_vect_term(sum_prod, data2);
		double cov = compute_mean(sum_prod) - mean1*mean2;
		pnl_vect_free(&sum_prod);
		return cov;
	}

	double compute_mean(const PnlVect * data) {
		return (pnl_vect_sum(data) / data->size);
	}

	void compute_correlation_matrix(PnlMat * correlation_matrix, const PnlMat * data, const PnlVect * volatilities) {
		pnl_mat_set_diag(correlation_matrix, 1, 0);
		PnlVect * col_data_1 = pnl_vect_create(data->m);
		PnlVect * col_data_2 = pnl_vect_create(data->m);
		double correlation = 0.0;
		for (int i = 0; i < data->n; i++) {
			for (int j = i + 1; j < data->n; j++) {
				pnl_mat_get_col(col_data_1, data, i);
				pnl_mat_get_col(col_data_2, data, j);
				if ((GET(volatilities, i) == 0) || (GET(volatilities, j) == 0))
					correlation = 0.0;
				else
					correlation = (compute_covariance(col_data_1, col_data_2) * 52 * 7) / (GET(volatilities, i)*GET(volatilities, j));
				MLET(correlation_matrix, i, j) = correlation;
				MLET(correlation_matrix, j, i) = correlation;
			}
		}
		PnlVect * eigen = pnl_vect_create(6);
		PnlMat * mat = pnl_mat_create(6,6);
		pnl_mat_eigen(eigen, mat, correlation_matrix, 0);
		pnl_vect_free(&eigen);
		pnl_vect_free(&col_data_1);
		pnl_vect_free(&col_data_2);
	}

	void compute_volatilities(PnlVect * volatilities, const PnlMat * data) {
		PnlVect * col_data = pnl_vect_create(data->m);

		for (int i = 0; i < data->n; i++) {
			pnl_mat_get_col(col_data, data, i);
			LET(volatilities, i) = sqrt(compute_variance(col_data) * 52 * 7);
		}
		pnl_vect_free(&col_data);
	}
}