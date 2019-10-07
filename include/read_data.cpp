/* read_data.cpp */
#include <fstream>
#include "csv.h"
#include "Matrix.h"

template <typename T>
Matrix<T> readData(std::string fileName)
{
	std::ifstream file(fileName);
	Csv csv(file);
	std::string line;
	if (file.is_open())
	{
		std::size_t row = 0;
        std::size_t col = 0;
        std::vector<T> vec;
		while (csv.getline(line) != 0) {
            col = csv.getnfield();
         	for (int j = 0; j != csv.getnfield(); ++j)
               vec.push_back(stod(csv.getfield(j)));
            ++row;
		}
		file.close();
        return Matrix<T>(vec, row, col);
	}
	else { std::cout << fileName << " missing\n"; exit(0); }
}
