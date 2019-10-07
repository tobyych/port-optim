/* part_a.cpp */

#include <iostream>

template <typename T>
Matrix<T> getMean(Matrix<T> const& data)
{
    Matrix<T> mean(1, data.get_col());
    for (int i = 0; i != data.get_row(); ++i){
        mean += data.Row(i);
    }
    mean /= data.get_row();
    return mean;
}


template<typename T>
Matrix<T> getRollMean(Matrix<T> const& data, std::size_t window, std::size_t step)
{
    Matrix<T> result;
    if(data.get_row() < window)
        return getMean(data);
    std::size_t beg = 0;
    std::size_t end = beg + window - 1;
    int count = 0;
    while(end < data.get_row())
    {
        Matrix<T> rollmean(data.Row(beg));
        for(int i = beg + 1; i != end; ++i)
            rollmean += data.Row(i);
        rollmean /= window;
        result.append(rollmean);
        beg += step;
        end += step;
        ++count;
    }
    result.reshape(count, data.get_col());
    return result;
}


template <typename T>
Matrix<T> getCovMatrix(Matrix<T> const& m, std::size_t window, std::size_t beg){
    Matrix<T> sliced = m.slice(beg, beg + window, 0, m.get_col());
    Matrix<T> mean(getMean(sliced));
    Matrix<T> result(m.get_col(), m.get_col());
    for(std::size_t i = 0; i != result.get_row(); ++i)
        for(std::size_t j = 0; j != result.get_col(); ++j) {
            result.at(i, j) = dotproduct((sliced.Column(i) - mean.at(0, i)), (sliced.Column(j) - mean.at(0, j)));
        }
    return result / double(window - 1);
}       