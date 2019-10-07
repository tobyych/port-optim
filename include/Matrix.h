/* Matrix.h */

#ifndef MATRIX_H
#define MATRIX_H

#include <fstream>
#include <iostream>
#include <random>
#include <vector>

// forward declarations so that we can declare these functions as friend inside the Matrix class
template <typename T> class Matrix;
template <typename T> void swap(Matrix<T>&, Matrix<T>&);
template <typename T> std::ostream& operator<<(std::ostream& os, Matrix<T> const& rhs);
template <typename T> bool operator==(Matrix<T> const &lhs, Matrix<T> const &rhs);
template <typename T> bool operator!=(Matrix<T> const &lhs, Matrix<T> const &rhs);
template <typename T> Matrix<T> mmult(Matrix<T> const& lhs, Matrix<T> const& rhs);
template <typename T> T dotproduct(Matrix<T> const& lhs, Matrix<T> const& rhs);
template <typename T> void to_csv(Matrix<T> const&, const char*);

template <typename T>
class Matrix
{
public:
    Matrix(): data(), row(0), col(0) { }    // default constructor
    Matrix(std::size_t row, std::size_t col): row(row), col(col) 
    { 
        data.reserve(row * col);    // reserve memory to avoid reallocation during the sequential insertions
        for(std::size_t e = 0; e != row * col; ++e)
            data.push_back(0);
    } // base constructor: all elements are set to zeros
    Matrix(std::vector<T> input, std::size_t row, std::size_t col): row(row), col(col)
    {
        if (input.size() != row * col) 
            throw std::invalid_argument("Matrix dimensions and elements must match");
        data = input;
    } // constructor that reads data from STL vector
    Matrix(Matrix const& c):                          // copy constructor
           data(c.data), row(c.row), col(c.col) { }   
    Matrix& operator=(Matrix const&);                 // assign constructor
    ~Matrix() = default;                              // destructor set to default
    
    // data accessing and getter functions
    T& at(std::size_t, std::size_t);
    T at(std::size_t, std::size_t) const;
    inline std::size_t get_row() const { return row; }
    inline std::size_t get_col() const { return col; }
    Matrix Row(std::size_t) const;
    Matrix Column(std::size_t) const;

    // helper function
    Matrix slice(std::size_t, std::size_t, std::size_t, std::size_t) const;
    Matrix& reshape(std::size_t, std::size_t);
    Matrix& append(Matrix const&);
    Matrix t() const;

    // operators overload
    Matrix& operator+=(T a);
    Matrix& operator-=(T a);
    Matrix& operator*=(T a);
    Matrix& operator/=(T a);
    Matrix& operator+=(Matrix const& a);
    Matrix& operator-=(Matrix const& a);
    Matrix operator-() const;               // unary negation

    // friend declarations
    friend void swap <>(Matrix&, Matrix&);
    friend std::ostream& operator<< <>(std::ostream&, Matrix<T> const&);
    friend bool operator== <>(Matrix<T> const&, Matrix<T> const&);
    friend bool operator!= <>(Matrix<T> const&, Matrix<T> const&);
    friend Matrix<T> mmult <>(Matrix<T> const&, Matrix<T> const&);
    friend T dotproduct <>(Matrix<T> const&, Matrix<T> const&);
    friend void to_csv <>(Matrix<T> const&, const char*);
private:
    std::size_t row;     // number of rows
    std::size_t col;     // number of columns
    std::vector<T> data; // data stored sequentially in a STL vector
};

// member functions:

template <typename T>
Matrix<T> &Matrix<T>::operator=(const Matrix &rhs)
{
    if (&rhs == this)
        return *this;
    Matrix<T> temp(rhs);
    swap(*this, temp);
    return *this;
}


template <typename T>
T &Matrix<T>::at(std::size_t i, std::size_t j)
{
    if (i > row || j > col)
        throw std::invalid_argument("Indices out of bound");
    return data[i * col + j];
}


template <typename T>
T Matrix<T>::at(std::size_t i, std::size_t j) const
{
    if (i > row || j > col)
        throw std::invalid_argument("Indices out of bound");
    return data[i * col + j];
}


template <typename T>
Matrix<T> Matrix<T>::Row(std::size_t i) const
{
    std::vector<T> vec;
    for(std::size_t j = 0; j != col; ++j)
        vec.push_back(this->at(i, j));
    return Matrix<T>(vec, 1, col);
}


template <typename T>
Matrix<T> Matrix<T>::Column(std::size_t j) const
{
    std::vector<T> vec;
    for(std::size_t i = 0; i != row; ++i)
        vec.push_back(this->at(i, j));
    return Matrix<T>(vec, row, 1);
}


template <typename T>
Matrix<T> Matrix<T>::slice(std::size_t row_beg, std::size_t row_end, std::size_t col_beg, std::size_t col_end) const{
    /*
    note that row_end and col_end are implemented as one pass the end of container,
    similar to the python slicing of lists
    */
    std::size_t nrow = row_end - row_beg;
    std::size_t ncol = col_end - col_beg;
    std::vector<T> vec;
    for(std::size_t i = row_beg; i != row_end; ++i)
        for(std::size_t j = col_beg; j != col_end; ++j)
            vec.push_back(this->at(i, j));
    return Matrix<T>(vec, nrow, ncol);
}


template <typename T>
Matrix<T>& Matrix<T>::reshape(std::size_t new_row, std::size_t new_col)
{
    row = new_row;
    col = new_col;
    return *this;
}


template <typename T>
Matrix<T>& Matrix<T>::append(Matrix<T> const& a)
{
    data.insert(data.end(), a.data.begin(), a.data.end());
    return *this;
}


template <typename T>
Matrix<T> Matrix<T>::t() const
{
    std::vector<T> data_transposed;
    for (std::size_t i = 0; i != data.size(); ++i)
    {
        data_transposed.push_back(data[col * (i % row) + i / row]);
    }
    return Matrix<T>(data_transposed, col, row);
}


template <typename T>
Matrix<T> &Matrix<T>::operator+=(T a)
{
    for (auto& i : data)
        i += a;
    return *this;
}


template <typename T>
Matrix<T> &Matrix<T>::operator-=(T a)
{
    for (auto& i : data)
        i -= a;
    return *this;
}

template <typename T>
Matrix<T> &Matrix<T>::operator*=(T a)
{
    for (auto& i : data)
        i *= a;
    return *this;
}

template <typename T>
Matrix<T> &Matrix<T>::operator/=(T a)
{
    for (auto& i : data)
        i /= a;
    return *this;
}

template <typename T>
Matrix<T> &Matrix<T>::operator+=(Matrix const &rhs)
{
    if (row != rhs.row || col != rhs.col)
        throw std::invalid_argument("Matrices of unequal dimensions");
    for (std::size_t i = 0; i != data.size(); ++i)
            data[i] += rhs.data[i];
    return *this;
}

template <typename T>
Matrix<T> &Matrix<T>::operator-=(Matrix const &rhs)
{
    if (row != rhs.row || col != rhs.col)
        throw std::invalid_argument("Matrices of unequal dimensions");
    for (std::size_t i = 0; i != data.size(); ++i)
            data[i] -= rhs.data[i];
    return *this;
}

template <typename T>
Matrix<T> Matrix<T>::operator-() const
{
    std::vector<T> neg;
    for (auto i : data)
        neg.push_back(-i);
    return Matrix<T>(neg, row, col);
}

// free functions:

template <typename T>
Matrix<T> operator+(Matrix<T> const &a, T b)
{
    Matrix<T> temp(a);
    temp += b;
    return temp;
}


template <typename T>
Matrix<T> operator-(Matrix<T> const &a, T b)
{
    Matrix<T> temp(a);
    temp -= b;
    return temp;
}


template <typename T>
Matrix<T> operator*(Matrix<T> const &a, T b)
{
    Matrix<T> temp(a);
    temp *= b;
    return temp;
}


template <typename T>
Matrix<T> operator/(Matrix<T> const &a, T b)
{
    Matrix<T> temp(a);
    temp /= b;
    return temp;
}


template <typename T>
Matrix<T> operator+(T a, Matrix<T> const &b)
{
    Matrix<T> temp(b);
    temp += a;
    return temp;
}


template <typename T>
Matrix<T> operator-(T a, Matrix<T> const &b)
{
    Matrix<T> temp(b);
    temp -= a;
    return temp;
}


template <typename T>
Matrix<T> operator*(T a, Matrix<T> const &b)
{
    Matrix<T> temp(b);
    temp *= a;
    return temp;
}


template <typename T>
Matrix<T> operator/(T a, Matrix<T> const &b)
{
    Matrix<T> temp(b);
    temp /= a;
    return temp;
}


template <typename T>
Matrix<T> operator+(Matrix<T> const &a, Matrix<T> const &b)
{
    Matrix<T> temp(a);
    temp += b;
    return temp;
}


template <typename T>
Matrix<T> operator-(Matrix<T> const &a, Matrix<T> const &b)
{
    Matrix<T> temp(a);
    temp -= b;
    return temp;
}


template <typename T>
Matrix<T> random(std::size_t row, std::size_t col)
{
    std::vector<T> vec;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(-1.0, 1.0);
    for (std::size_t i = 0; i != row * col; ++i)
        vec.push_back(dis(gen));
    return Matrix<T>(vec, row, col);
}

// friend functions:

template <typename T>
void swap(Matrix<T>& lhs, Matrix<T>& rhs)
{
    using std::swap;
    lhs.data.swap(rhs.data);
    swap(lhs.row, rhs.row);
    swap(lhs.col, rhs.col);
    return;
}


template <typename T>
std::ostream &operator<<(std::ostream &os, const Matrix<T> &m)
{
    for (std::size_t i = 0; i != m.data.size(); ++i)
    {
        os << m.data[i] << ' ';
        if ((i + 1) % m.col == 0)
            os << '\n';
    }
    return os;
}


template <typename T>
bool operator==(Matrix<T> const &lhs, Matrix<T> const &rhs)
{
    return (lhs.data == rhs.data && lhs.row == rhs.row && lhs.col == rhs.col);
}


template <typename T>
bool operator!=(Matrix<T> const &lhs, Matrix<T> const &rhs)
{
    return !(lhs == rhs);
}


template <typename T>
Matrix<T> mmult(Matrix<T> const& lhs, Matrix<T> const& rhs)
{
    // add assertion that this is not dot product of vectors
    if(lhs.col != rhs.row)
        throw std::invalid_argument("The two matrix cannot be multiplied together");
    std::vector<T> vec;
    T sum = 0;
    for(std::size_t j = 0; j < lhs.row; j++) {
        for(std::size_t k = 0; k < rhs.col; k++) {
            for(std::size_t i = 0; i < lhs.col; i++) {
                sum += lhs.data[i + j * lhs.col] * rhs.data[k + i * rhs.col];  
            }
            vec.push_back(sum);
            sum = 0;
        }
    }
    return Matrix<T>(vec,lhs.row,rhs.col);
}


template <typename T>
T dotproduct(Matrix<T> const& lhs, Matrix<T> const& rhs)
{
    // add assertion that the input matrices are vectors, i.e. either row or col has to be 1
    // assume lhs and rhs are both column vectors
    if(lhs.data.size() != rhs.data.size())
        throw std::invalid_argument("The dot product of these two vectors is not defined");
    T sum = 0;
    for(std::size_t i = 0; i != lhs.data.size(); ++i)
        sum += lhs.data[i] * rhs.data[i];
    return sum;
}


template <typename T>
void to_csv(Matrix<T> const& m, const char* filename)
{
    const char delimiter = ',';
    std::ofstream o(filename);
    for (std::size_t i = 0; i != m.data.size(); ++i)
    {
        if ((i + 1) % m.col == 0)
            o << m.data[i] << '\n';
        else
            o << m.data[i] << delimiter;
    }
    o.close();
}


#endif