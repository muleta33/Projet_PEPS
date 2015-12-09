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

		TEST_METHOD(get_payoff_with_only_one_positive_performance)
		{
			const FakeEurostralMutualFundInputParser fake_fund_parser;
			products::EurostralMutualFund fund(fake_fund_parser);
			PnlMat * underlying_paths = pnl_mat_create_from_scalar(3, 3, 100);
			MLET(underlying_paths, 2, 0) = 110;
			MLET(underlying_paths, 2, 1) = 110;
			MLET(underlying_paths, 2, 2) = 110;
			double computed_payoff = fund.get_payoff(underlying_paths);
			double reference_payoff = 0.1 / 2 * fake_fund_parser.get_indexes_return_percentage()
				+ fake_fund_parser.get_guaranteed_capital_percentage();
			reference_payoff *= fake_fund_parser.get_initial_capital();
			Assert::AreEqual(reference_payoff, computed_payoff);

			pnl_mat_free(&underlying_paths);
		}


		TEST_METHOD(get_payoff_with_saveral_positive_performances)
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
			double reference_payoff = (0.14 + 0.1) / 2 * fake_fund_parser.get_indexes_return_percentage()
				+ fake_fund_parser.get_guaranteed_capital_percentage();
			reference_payoff *= fake_fund_parser.get_initial_capital();
			Assert::AreEqual(reference_payoff, computed_payoff);

			pnl_mat_free(&underlying_paths);
		}


		TEST_METHOD(get_payoff_with_only_one_negative_performance)
		{
			const FakeEurostralMutualFundInputParser fake_fund_parser;
			products::EurostralMutualFund fund(fake_fund_parser);
			PnlMat * underlying_paths = pnl_mat_create_from_scalar(3, 3, 100);
			MLET(underlying_paths, 2, 0) = 90;
			MLET(underlying_paths, 2, 1) = 90;
			MLET(underlying_paths, 2, 2) = 90;
			double computed_payoff = fund.get_payoff(underlying_paths);
			double reference_payoff = fake_fund_parser.get_guaranteed_capital_percentage() * fake_fund_parser.get_initial_capital();
			Assert::AreEqual(reference_payoff, computed_payoff);

			pnl_mat_free(&underlying_paths);
		}


		TEST_METHOD(get_payoff_with_positive_finale_performance)
		{
			const FakeEurostralMutualFundInputParser fake_fund_parser;
			products::EurostralMutualFund fund(fake_fund_parser);
			PnlMat * underlying_paths = pnl_mat_create_from_scalar(3, 3, 100);
			MLET(underlying_paths, 1, 0) = 120;
			MLET(underlying_paths, 1, 1) = 110;
			MLET(underlying_paths, 1, 2) = 105;
			MLET(underlying_paths, 2, 0) = 90;
			MLET(underlying_paths, 2, 1) = 90;
			MLET(underlying_paths, 2, 2) = 80;
			double computed_payoff = fund.get_payoff(underlying_paths);
			double reference_payoff = (0.14 - 0.12) / 2 * fake_fund_parser.get_indexes_return_percentage()
				+ fake_fund_parser.get_guaranteed_capital_percentage();
			reference_payoff *= fake_fund_parser.get_initial_capital();
			Assert::AreEqual(reference_payoff, computed_payoff);

			pnl_mat_free(&underlying_paths);
		}


		TEST_METHOD(get_payoff_with_negative_finale_performance)
		{
			const FakeEurostralMutualFundInputParser fake_fund_parser;
			products::EurostralMutualFund fund(fake_fund_parser);
			PnlMat * underlying_paths = pnl_mat_create_from_scalar(3, 3, 100);
			MLET(underlying_paths, 1, 0) = 120;
			MLET(underlying_paths, 1, 1) = 110;
			MLET(underlying_paths, 1, 2) = 100;
			MLET(underlying_paths, 2, 0) = 85;
			MLET(underlying_paths, 2, 1) = 90;
			MLET(underlying_paths, 2, 2) = 80;
			double computed_payoff = fund.get_payoff(underlying_paths);
			double reference_payoff = fake_fund_parser.get_guaranteed_capital_percentage() * fake_fund_parser.get_initial_capital();
			Assert::AreEqual(reference_payoff, computed_payoff);

			pnl_mat_free(&underlying_paths);
		}

	};
}