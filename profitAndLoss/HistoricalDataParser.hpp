#include "pnl/pnl_matrix.h"
#include <iostream>
#include <string>
#include <fstream>
#include <boost/lexical_cast.hpp>

using namespace std;

class HistoricalDataParser
{
public:

	const PnlMat * get_past_data(int number_past_data) const {
		PnlMat * result = pnl_mat_create(number_past_data, underlying_number_);
		pnl_mat_extract_subblock(result, data_, 0, number_past_data,  0, underlying_number_);
		return result;
	}
	const PnlMat * get_market_data(int number_past_data, int number_market_data) const {
		PnlMat * result = pnl_mat_create(number_market_data, underlying_number_);
		pnl_mat_extract_subblock(result, data_, number_past_data, number_market_data, 0, underlying_number_);
		return result;
	}

	const PnlMat * get_all_market_data() const {
		return data_;
	}

	HistoricalDataParser(const char * file_name, int data_number, int underlying_number)
	{
		underlying_number_ = underlying_number * 2;
		parse_csv_file(file_name, data_number, underlying_number * 2);
	}

	~HistoricalDataParser()
	{
		pnl_mat_free(&data_);
		data_ = nullptr;
	}

private:
	int underlying_number_;
	PnlMat * data_;
	int parse_csv_file(const char * file_name, int data_number, int underlying_number) {
		ifstream file(file_name, ios::in);  // on ouvre le fichier en lecture

		if (file)  // si l'ouverture a réussi
		{
			data_ = pnl_mat_create(data_number + 1, underlying_number);
			string content;
			getline(file, content); // première ligne d'en-tête sert à rien

			for (int i = 0; i <= data_number; i++) {

				for (int j = 0; j < underlying_number - 1; j++) {
					getline(file, content, ';');
					MLET(data_, i, j) = boost::lexical_cast<double>(content);
				}

				getline(file, content);
				MLET(data_, i, underlying_number - 1) = boost::lexical_cast<double>(content);
			}

			file.close();  // on ferme le fichier
		}
		else  // sinon
			return 1;

		return 0;
	}
};