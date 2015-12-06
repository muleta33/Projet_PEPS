#include <iostream>
#include <ctime>
#include "PnlRandomGeneration.hpp"
#include "BlackScholesModel.hpp"
#include "FakeBlackScholesModelInputParser.hpp"

using namespace std;
using namespace input_parsers;
using namespace models;
using namespace generators;

int main(int argc, char **argv)
{
	const FakeBlackScholesModelInputParser model_parser;
	const PnlRandomGeneration pnl_generator;
	BlackScholesModel bs_model(model_parser, pnl_generator);

	/*PnlVect * spot = pnl_vect_create_from_scalar(5, 100);
	const PnlMat * result = pnl_mat_create_from_zero(9, 5);
	result = bs_model.simulate_asset_paths_from_start(spot);

	pnl_mat_print(result);*/

	PnlMat * past = pnl_mat_create_from_scalar(9, 5, 100);
	const PnlMat * result = pnl_mat_create_from_zero(9, 5);
	result = bs_model.simulate_asset_paths_from_time(3.75, past);

	pnl_mat_print(result);
}