#include "stdafx.h"
#include "CppUnitTest.h"
#include "BlackScholesModelRiskNeutral.hpp"
#include "PNLRandomGeneration.hpp"
#include "BlackScholesModelInputParser.hpp"
#include "FakeBlackScholesModelInputParser.hpp"
#include "ConstantRandomGeneration.hpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTests
{		
	TEST_CLASS(TestBlackScholesModel)
	{
	public:
		
		TEST_METHOD(simulate_asset_paths_from_time)
		{
			const FakeBlackScholesModelInputParser fake_model_parser;
			const ConstantRandomGeneration fake_random_generator;
			models::BlackScholesModelRiskNeutral under_test(fake_model_parser, fake_random_generator);
			PnlMat *past_matrix = pnl_mat_create_from_scalar(7, 5, 5);
			const PnlMat * result = under_test.simulate_asset_paths_from_time(2.75, past_matrix);
			PnlVect * vol = fake_model_parser.get_volatility();
			for (int i = 0; i < under_test.underlying_number(); i++)
			{
				double sigma = GET(vol, i);
				double res = 5 * exp(1.25*(0.05 - 0.5 * sigma * sigma));
				Assert::AreEqual(res, MGET(result, 8, i), 0.0001);
			}
			pnl_mat_free(&past_matrix);
		}


		TEST_METHOD(simulate_asset_paths_from_time_border_case)
		{
			const FakeBlackScholesModelInputParser fake_model_parser;
			const ConstantRandomGeneration fake_random_generator;
			models::BlackScholesModelRiskNeutral under_test(fake_model_parser, fake_random_generator);
			PnlMat *past_matrix = pnl_mat_create_from_scalar(9, 5, 5);
			const PnlMat * result = under_test.simulate_asset_paths_from_time(4, past_matrix);
			PnlVect * vol = fake_model_parser.get_volatility();
			for (int i = 0; i < under_test.underlying_number(); i++)
			{
				Assert::AreEqual(5, MGET(result, 8, i), 0.0001);
			}
			pnl_mat_free(&past_matrix);
		}


		TEST_METHOD(simulate_asset_paths_from_zero)
		{
			const FakeBlackScholesModelInputParser fake_model_parser;
			const ConstantRandomGeneration fake_random_generator;
			models::BlackScholesModelRiskNeutral under_test(fake_model_parser, fake_random_generator);
			PnlVect *spot = pnl_vect_create_from_scalar(5, 10);
			const PnlMat * result = under_test.simulate_asset_paths_from_start(spot);
			PnlVect * vol = fake_model_parser.get_volatility();
			for (int i = 0; i < under_test.underlying_number(); i++)
			{
				double sigma = GET(vol, i);
				double res = 10 * exp(4*(0.05 - 0.5 * sigma * sigma));
				Assert::AreEqual(res, MGET(result, 8, i), 0.0001);
			}
			pnl_vect_free(&spot);
		}

		TEST_METHOD(simulate_asset_paths_from_zero_from_time_coherence)
		{
			const FakeBlackScholesModelInputParser fake_model_parser;
			const ConstantRandomGeneration fake_random_generator;
			models::BlackScholesModelRiskNeutral under_test(fake_model_parser, fake_random_generator);
			PnlVect *spot = pnl_vect_create_from_scalar(5, 15);
			const PnlMat * result_from_zero = under_test.simulate_asset_paths_from_start(spot);
			PnlMat *past_matrix = pnl_mat_create_from_scalar(1, 5, 15);
			const PnlMat * result_from_time = under_test.simulate_asset_paths_from_time(0, past_matrix);
			PnlVect * vol = fake_model_parser.get_volatility();
			for (int i = 0; i < under_test.underlying_number(); i++)
			{
				Assert::AreEqual(MGET(result_from_zero, 4, i), MGET(result_from_time, 4, i), 0.0001);
			}
			pnl_vect_free(&spot);
			pnl_mat_free(&past_matrix);
		}


		TEST_METHOD(shift_up_asset_paths_from_time)
		{
			const FakeBlackScholesModelInputParser fake_model_parser;
			const ConstantRandomGeneration fake_random_generator;
			models::BlackScholesModelRiskNeutral black_scholes_model(fake_model_parser, fake_random_generator);

			PnlMat * past_values = pnl_mat_create_from_scalar(2, 5, 5);
			PnlMat * shifted_up_asset_path = pnl_mat_new();

			double shift = 0.01;
			double from_time = 0.5;
			PnlMat * asset_path = pnl_mat_create_from_scalar(9, 5, 5);
			black_scholes_model.get_shifted_asset_paths(asset_path, 4, shift, from_time, 2, shifted_up_asset_path);

			for (int j = 0; j < black_scholes_model.underlying_number() - 1; ++j)
			{
				for (int i = 0; i < 9; ++i)
					Assert::AreEqual(MGET(asset_path, i, j), MGET(shifted_up_asset_path, i, j));
			}
			Assert::AreEqual(MGET(asset_path, 0, 4), MGET(shifted_up_asset_path, 0, 4));
			Assert::AreEqual(MGET(asset_path, 1, 4), MGET(shifted_up_asset_path, 1, 4));
			for (int i = 2; i < 9; ++i)
				Assert::AreEqual(MGET(asset_path, i, 4) * (1 + shift), MGET(shifted_up_asset_path, i, 4));

			pnl_mat_free(&past_values);
			pnl_mat_free(&shifted_up_asset_path);
		}


		TEST_METHOD(shift_down_asset_paths_from_start)
		{
			const FakeBlackScholesModelInputParser fake_model_parser;
			const ConstantRandomGeneration fake_random_generator;
			models::BlackScholesModelRiskNeutral black_scholes_model(fake_model_parser, fake_random_generator);

			PnlMat * asset_path = pnl_mat_create_from_scalar(9, 5, 5);
			PnlMat * shifted_down_asset_path = pnl_mat_new();

			double shift = 0.05;
			double from_time = 0.;
			black_scholes_model.get_shifted_asset_paths(asset_path, 0, -shift, from_time, 1, shifted_down_asset_path);

			for (int j = 1; j < black_scholes_model.underlying_number(); ++j)
			{
				for (int i = 0; i < 9; ++i)
					Assert::AreEqual(MGET(asset_path, i, j), MGET(shifted_down_asset_path, i, j));
			}
			Assert::AreEqual(MGET(asset_path, 0, 0), MGET(shifted_down_asset_path, 0, 0));
			for (int i = 1; i < 9; ++i)
				Assert::AreEqual(MGET(asset_path, i ,0) * (1 - shift), MGET(shifted_down_asset_path, i, 0));

			pnl_mat_free(&asset_path);
			pnl_mat_free(&shifted_down_asset_path);
		}

	};
}