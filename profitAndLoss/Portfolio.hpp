#include <cmath>
#include "pnl/pnl_vector.h"

class Portfolio
{
public:
	PnlVect *deltas_;
	double risk_free_investment_;

	Portfolio() { deltas_ = pnl_vect_create_from_zero(0); };
	Portfolio(const PnlVect *deltas, double riskFreeRateInvestment);
	~Portfolio() { pnl_vect_free(&deltas_); deltas_ = nullptr; };


	void initialisation(double optionPrice, const PnlVect *deltas, const PnlVect *prices);

	void rebalancing(const PnlVect *deltas, const PnlVect *prices, double interestRate, double timeStep);

	double compute_value(const PnlVect *prices);

	double compute_final_value(const PnlVect *prices, double interestRate, double timeStep);
};
