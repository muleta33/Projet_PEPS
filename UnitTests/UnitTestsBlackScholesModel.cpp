#include "stdafx.h"
#include "CppUnitTest.h"
#include "BlackScholesModelRiskNeutral.hpp"
#include "BlackScholesModelRoutine.hpp"
#include "PNLRandomGeneration.hpp"
#include "BlackScholesModelInputParameters.hpp"
#include "BlackScholesModelUtilities.hpp"
#include "FakeBlackScholesModelInputParameters.hpp"
#include "ConstantRandomGeneration.hpp"
#include "SameSeedRandomGeneration.hpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTests
{		
	TEST_CLASS(TestBlackScholesModel)
	{
	public:
		//TEST MODIFIE
		TEST_METHOD(add_one_simulation_to_generated_asset_paths)
		{
			int underlying_number = 5;
			const ConstantRandomGeneration fake_random_generator;
			PnlVect *vol = pnl_vect_create_from_scalar(2*underlying_number, 0.2);
			PnlVect *trend = pnl_vect_create_from_scalar(2*underlying_number, 0.05);
			models::BlackScholesModelRoutine test_routine(underlying_number, 12, 4, trend, vol, 0.1, fake_random_generator);
			PnlMat *path_matrix = pnl_mat_create_from_scalar(7, 2*underlying_number, 0);
			PnlVect *last_values = pnl_vect_create_from_scalar(2*underlying_number, 10);
			test_routine.add_one_simulation_to_generated_asset_paths(6, 0.17, last_values, path_matrix);
			for (int i = 0; i < 2*underlying_number; i++)
			{
				for (int j = 0; j < 6; j++)
				{
					Assert::AreEqual(0, MGET(path_matrix, j, i), 0.0001);
				}
				double sigma = GET(vol, i);
				double res = 10 * exp(0.17*(0.05 - 0.5 * sigma * sigma));
				Assert::AreEqual(res, MGET(path_matrix, 6, i), 0.0001);
			}
			pnl_vect_free(&vol);
			pnl_vect_free(&trend);
			pnl_vect_free(&last_values);
			pnl_mat_free(&path_matrix);
		}

		TEST_METHOD(fill_remainder_of_generated_asset_paths)
		{
			int underlying_number = 5;
			const ConstantRandomGeneration fake_random_generator;
			PnlVect *vol = pnl_vect_create_from_scalar(2*underlying_number, 0.2);
			PnlVect *trend = pnl_vect_create_from_scalar(2*underlying_number, 0.05);
			models::BlackScholesModelRoutine test_routine(underlying_number, 16, 4, trend, vol, 0.1, fake_random_generator);
			PnlMat *path_matrix = pnl_mat_create_from_scalar(13, 2*underlying_number, 10);
			test_routine.fill_remainder_of_generated_asset_paths(1, path_matrix);
			for (int i = 0; i < 2*underlying_number; i++)
			{
				Assert::AreEqual(10, MGET(path_matrix, 0, i), 0.0001);
				double sigma = GET(vol, i);
				double res = 10;
				double factor = exp(0.25*(0.05 - 0.5 * sigma * sigma));
				for (int j = 1; j < 13; j++)
				{
					res *= factor;
					Assert::AreEqual(res, MGET(path_matrix, j, i), 0.0001);
				}
			}
			pnl_vect_free(&vol);
			pnl_vect_free(&trend);
			pnl_mat_free(&path_matrix);
		}

		TEST_METHOD(get_generated_foreign_asset_paths)
		{
			const FakeBlackScholesModelInputParameters fake_model_input_parameters;
			const ConstantRandomGeneration fake_random_generator;
			models::BlackScholesModelRiskNeutral under_test(fake_model_input_parameters, fake_random_generator);
			
			int underlying_number = 3;

			PnlMat * generated_asset_paths = pnl_mat_create_from_scalar(8, 6, 1);
			PnlMat * generated_foreign_asset_paths = pnl_mat_new();
			double timestep = 0.5;
			double interest_rate = 0.05;

			models::get_generated_foreign_asset_paths(generated_asset_paths, generated_foreign_asset_paths, fake_model_input_parameters.get_foreign_interest_rates(), 
				timestep);

			for (int i = 0; i < 8; i++)
			{
				for (int j = 0; j < 3; j++)
				{
					double res = exp(interest_rate*i*timestep);
					Assert::AreEqual(res, MGET(generated_foreign_asset_paths, i, j), 0.0001);
				}
			}
			pnl_mat_free(&generated_foreign_asset_paths);
			pnl_mat_free(&generated_asset_paths);
		}

		TEST_METHOD(simulate_asset_paths_from_time)
		{
			const FakeBlackScholesModelInputParameters fake_model_input_parameters;
			const ConstantRandomGeneration fake_random_generator;
			models::BlackScholesModelRiskNeutral under_test(fake_model_input_parameters, fake_random_generator);
			PnlMat *past_matrix = pnl_mat_create_from_scalar(7, 6, 5);
			for (int i = 0; i < 6; i++){
				for (int j = 3; j < 6; j++) {
					MLET(past_matrix, i, j) = exp(i*0.05*0.5);
				}
			}
			MLET(past_matrix, 6,3) = exp(5.5*0.05*0.5);
			MLET(past_matrix, 6, 4) = exp(5.5*0.05*0.5);
			MLET(past_matrix, 6, 5) = exp(5.5*0.05*0.5);
			
			const PnlMat * result = under_test.simulate_asset_paths_from_time(2.75, past_matrix);
			PnlVect * vol = fake_model_input_parameters.get_volatility();
			for (int i = 0; i < under_test.underlying_number(); i++)
			{			
				double res = 5*exp(0.05*1.25 - 1.25*0.5 * (GET(vol, i) * GET(vol, i)));
				Assert::AreEqual(res, MGET(result, 8, i), 0.0001);
			}
			pnl_mat_free(&past_matrix);
		}


		TEST_METHOD(simulate_asset_paths_from_time_border_case)
		{
			const FakeBlackScholesModelInputParameters fake_model_input_parameters;
			const ConstantRandomGeneration fake_random_generator;
			models::BlackScholesModelRiskNeutral under_test(fake_model_input_parameters, fake_random_generator);
			PnlMat *past_matrix = pnl_mat_create_from_scalar(9, 6, 5);
			for (int i = 0; i < 9; i++){
				for (int j = 3; j < 6; j++) {
					MLET(past_matrix, i, j) = exp(i*0.05*0.5);
				}
			}

			const PnlMat * result = under_test.simulate_asset_paths_from_time(4, past_matrix);
			PnlVect * vol = fake_model_input_parameters.get_volatility();
			for (int i = 0; i < under_test.underlying_number(); i++)
			{
				Assert::AreEqual(5, MGET(result, 8, i), 0.0001);
			}
			pnl_mat_free(&past_matrix);
		}


		TEST_METHOD(simulate_asset_paths_from_zero)
		{
			const FakeBlackScholesModelInputParameters fake_model_input_parameters;
			const ConstantRandomGeneration fake_random_generator;
			models::BlackScholesModelRiskNeutral under_test(fake_model_input_parameters, fake_random_generator);
			PnlVect *spot = pnl_vect_create_from_scalar(6, 10);
			for (int i = 3; i < 6; i++){
				LET(spot, i) = 1;
			}
			const PnlMat * result = under_test.simulate_asset_paths_from_start(spot);
			PnlVect * vol = fake_model_input_parameters.get_volatility();
			for (int i = 0; i < under_test.underlying_number(); i++)
			{
				double sigma = GET(vol, i);
				double res = 10*exp(4 *0.05 - 4*0.5 * GET(vol, i) * GET(vol, i));
				Assert::AreEqual(res, MGET(result, 8, i), 0.0001);
			}
			pnl_vect_free(&spot);
		}

		TEST_METHOD(simulate_asset_paths_from_zero_from_time_coherence)
		{
			const FakeBlackScholesModelInputParameters fake_model_input_parameters;
			const SameSeedRandomGeneration fake_random_generator;
			const SameSeedRandomGeneration fake_random_generator2;
			models::BlackScholesModelRiskNeutral under_test(fake_model_input_parameters, fake_random_generator);
			models::BlackScholesModelRiskNeutral under_test2(fake_model_input_parameters, fake_random_generator2);
			PnlVect *spot = pnl_vect_create_from_scalar(6, 15);
			for (int i = 3; i < 6; i++){
				LET(spot, i) = 1;
			}

			const PnlMat * result_from_zero = under_test.simulate_asset_paths_from_start(spot);
			PnlMat *past_matrix = pnl_mat_create_from_scalar(1, 6, 15);
			for (int j = 3; j < 6; j++){
				MLET(past_matrix, 0, j) = 1;
			}


			const PnlMat * result_from_time = under_test2.simulate_asset_paths_from_time(0, past_matrix);
			PnlVect * vol = fake_model_input_parameters.get_volatility();
			for (int i = 0; i < under_test.underlying_number(); i++)
			{
				Assert::AreEqual(MGET(result_from_zero, 4, i), MGET(result_from_time, 4, i), 0.0001);
			}
			pnl_vect_free(&spot);
			pnl_mat_free(&past_matrix);
		}


		TEST_METHOD(shift_up_asset_paths_from_time)
		{
			const FakeBlackScholesModelInputParameters fake_model_input_parameters;
			const ConstantRandomGeneration fake_random_generator;
			models::BlackScholesModelRiskNeutral black_scholes_model(fake_model_input_parameters, fake_random_generator);

			PnlMat * past_values = pnl_mat_create_from_scalar(2, 6, 5);
			PnlMat * shifted_up_asset_path = pnl_mat_new();

			double shift = 0.01;
			double from_time = 0.5;
			PnlMat * asset_path = pnl_mat_create_from_scalar(9, 6, 5);
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
			const FakeBlackScholesModelInputParameters fake_model_input_parameters;
			const ConstantRandomGeneration fake_random_generator;
			models::BlackScholesModelRiskNeutral black_scholes_model(fake_model_input_parameters, fake_random_generator);

			PnlMat * asset_path = pnl_mat_create_from_scalar(9, 6, 5);
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