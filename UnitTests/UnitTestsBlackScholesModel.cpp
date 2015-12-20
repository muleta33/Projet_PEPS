#include "stdafx.h"
#include "CppUnitTest.h"
#include "BlackScholesModel.hpp"
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
			models::BlackScholesModel under_test(fake_model_parser, fake_random_generator);
			PnlMat *past_matrix = pnl_mat_create_from_scalar(9, 5, 5);
			const PnlMat * result = under_test.simulate_asset_paths_from_time(3.75, past_matrix);
			PnlVect * vol = fake_model_parser.get_volatility();
			for (int i = 0; i < under_test.underlying_number(); i++)
			{
				double sigma = GET(vol, i);
				double res = 5 * exp(0.25*(0.05 - 0.5 * sigma * sigma));
				Assert::AreEqual(res, MGET(result, 8, i));
			}
			pnl_mat_free(&past_matrix);
		}


		TEST_METHOD(shift_up_asset_paths_from_time)
		{
			const FakeBlackScholesModelInputParser fake_model_parser;
			const ConstantRandomGeneration fake_random_generator;
			models::BlackScholesModel black_scholes_model(fake_model_parser, fake_random_generator);

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
			models::BlackScholesModel black_scholes_model(fake_model_parser, fake_random_generator);

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