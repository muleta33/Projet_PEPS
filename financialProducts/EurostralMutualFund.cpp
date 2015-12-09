#include "EurostralMutualFund.hpp"
#include "pnl/pnl_matrix.h"
#include <vector>
#include <algorithm>

using namespace products;


EurostralMutualFund::EurostralMutualFund() :
	Product(),
	guaranteed_capital_percentage_(0),
	indexes_return_percentage_(0),
	underlying_coefficients_(nullptr),
	initial_capital_(0)
{
}


EurostralMutualFund::EurostralMutualFund(const input_parsers::EurostralMutualFundInputParser &parser)
{
	maturity_ = parser.get_maturity();
	underlying_number_ = parser.get_underlying_number();
	guaranteed_capital_percentage_ = parser.get_guaranteed_capital_percentage();
	indexes_return_percentage_ = parser.get_indexes_return_percentage();
	underlying_coefficients_ = parser.get_underlying_coefficients();
	initial_capital_ = parser.get_initial_capital();
}


EurostralMutualFund::EurostralMutualFund(double maturity, int underlying_number, double guaranteed_capital_percentage,
	double indexes_return_percentage, PnlVect * underlying_coefficients, double initial_capital) :
	Product(maturity, underlying_number),
	guaranteed_capital_percentage_(guaranteed_capital_percentage),
	indexes_return_percentage_(indexes_return_percentage),
	initial_capital_(initial_capital)
{
	underlying_coefficients_ = pnl_vect_copy(underlying_coefficients);
}


EurostralMutualFund::EurostralMutualFund(const EurostralMutualFund & eurostralMutualFund) :
	Product(eurostralMutualFund.maturity_, eurostralMutualFund.underlying_number_),
	guaranteed_capital_percentage_(eurostralMutualFund.guaranteed_capital_percentage_),
	indexes_return_percentage_(eurostralMutualFund.indexes_return_percentage_),
	initial_capital_(eurostralMutualFund.initial_capital_)
{
	underlying_coefficients_ = pnl_vect_copy(eurostralMutualFund.underlying_coefficients_);
}


EurostralMutualFund& EurostralMutualFund::operator=(const EurostralMutualFund & eurostralMutualFund)
{
	if (this != &eurostralMutualFund)
	{
		maturity_ = eurostralMutualFund.maturity_;
		underlying_number_ = eurostralMutualFund.underlying_number_;
		guaranteed_capital_percentage_ = eurostralMutualFund.guaranteed_capital_percentage_;
		indexes_return_percentage_ = eurostralMutualFund.indexes_return_percentage_;
		initial_capital_ = eurostralMutualFund.initial_capital_;
		underlying_coefficients_ = pnl_vect_copy(eurostralMutualFund.underlying_coefficients_);
	}
	return *this;
}


EurostralMutualFund::~EurostralMutualFund()
{
	pnl_vect_free(&underlying_coefficients_);
}


double EurostralMutualFund::compute_finale_performance(const PnlMat * const underlying_paths) const
{
	int number_of_values = underlying_paths->m;
	double finale_performance = 0;

	for (int i = 1; i < number_of_values; ++i)
	{
		std::vector<double> indexes_performances = std::vector<double>(underlying_number_);
		for (int j = 0; j < underlying_number_; ++j)
		{
			double initial_value = MGET(underlying_paths, 0, j);
			indexes_performances[j] = (MGET(underlying_paths, i, j) - initial_value) / initial_value;
		}

		std::sort(indexes_performances.begin(), indexes_performances.end());
		double basket_performance = 0;
		for (int j = 0; j < underlying_number_; ++j)
			basket_performance += indexes_performances[j] * GET(underlying_coefficients_, underlying_number_ - 1 - j);
		finale_performance += basket_performance;
	}
	finale_performance /= (number_of_values - 1);
	return finale_performance;
}


double EurostralMutualFund::get_payoff(const PnlMat * const underlying_paths) const
{
	double finale_performance = compute_finale_performance(underlying_paths);
	double yield_rate = guaranteed_capital_percentage_;
	if (finale_performance > 0)
		yield_rate += indexes_return_percentage_ * finale_performance;
	double payoff = initial_capital_ * yield_rate;
	return payoff;
}
