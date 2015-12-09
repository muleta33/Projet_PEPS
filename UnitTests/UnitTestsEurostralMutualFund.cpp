#include "stdafx.h"
#include "CppUnitTest.h"
#include "pnl/pnl_matrix.h"
#include "EurostralMutualFund.hpp"
#include "FakeEurostralMutualFundInputParser.hpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTests
{
	TEST_CLASS(TestEurostralMutualFund)
	{
	public:

		TEST_METHOD(get_payoff_simple)
		{
			const FakeEurostralMutualFundInputParser fake_fund_parser;
			products::EurostralMutualFund fund(fake_fund_parser);
			PnlMat * underlying_paths = pnl_mat_create_from_scalar(3, 3, 100);
			MLET(underlying_paths, 2, 0) = 110;
			MLET(underlying_paths, 2, 1) = 110;
			MLET(underlying_paths, 2, 2) = 110;
			double computed_payoff = fund.get_payoff(underlying_paths);
			// Calculé à la main (mettre calcul pour précision) : à vérifier si problème
			double reference_payoff = 94.75;
			Assert::AreEqual(reference_payoff, computed_payoff);

			pnl_mat_free(&underlying_paths);
		}

		TEST_METHOD(get_payoff_complete)
		{
			const FakeEurostralMutualFundInputParser fake_fund_parser;
			products::EurostralMutualFund fund(fake_fund_parser);
			PnlMat * underlying_paths = pnl_mat_create_from_scalar(3, 3, 100);
			MLET(underlying_paths, 1, 0) = 120;
			MLET(underlying_paths, 1, 1) = 110;
			MLET(underlying_paths, 1, 2) = 105;
			MLET(underlying_paths, 2, 0) = 110;
			MLET(underlying_paths, 2, 1) = 110;
			MLET(underlying_paths, 2, 2) = 110;
			double computed_payoff = fund.get_payoff(underlying_paths);
			// Calculé à la main (mettre calcul pour précision) : à vérifier si problème
			double reference_payoff = (0.14 + 0.1) / 2 * fake_fund_parser.get_indexes_return_percentage()
				+ fake_fund_parser.get_guaranteed_capital_percentage();
			reference_payoff *= fake_fund_parser.get_initial_capital();
			Assert::AreEqual(reference_payoff, computed_payoff);

			pnl_mat_free(&underlying_paths);
		}

	};
}