#include <pnl/pnl_matrix.h>

namespace products {

	class Product
	{
	protected:
		double maturity_;
		int underlying_number_;

	public:
		Product() :
			maturity_(0),
			underlying_number_(0)
		{};

		Product(double maturity, int underlying_number) :
			maturity_(maturity),
			underlying_number_(underlying_number)
		{};

		virtual double get_payoff(const PnlMat * const underlying_paths) const = 0;
		virtual ~Product() {};
	};
}