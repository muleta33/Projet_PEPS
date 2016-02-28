#include "pnl/pnl_vector.h"
#include "EurostralMutualFundInputParser.hpp"
#include "Product.hpp"

namespace products {

	class EurostralMutualFund : public Product {
	public:
		EurostralMutualFund();
		EurostralMutualFund(const input_parsers::EurostralMutualFundInputParser &parser);
		EurostralMutualFund(double maturity, int underlying_number, double guaranteed_capital_percentage,
			double indexes_return_percentage, PnlVect * underlying_coefficients, double initial_capital, PnlVect * currencies);
		EurostralMutualFund(const EurostralMutualFund & eurostralMutualFund);

		EurostralMutualFund& operator=(const EurostralMutualFund & eurostralMutualFund);

		~EurostralMutualFund();

		double get_payoff(const PnlMat * const underlying_paths) const;

	private:
		double guaranteed_capital_percentage_;
		double indexes_return_percentage_;
		double initial_capital_;
		PnlVect * underlying_coefficients_;

		double compute_finale_performance(const PnlMat * const underlying_paths) const;
	};

}