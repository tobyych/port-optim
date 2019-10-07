/* part_b.cpp */

template <typename T>
Matrix<T> getQMatrix(Matrix<T> const& a, Matrix<T> const& b, Matrix<T> const& c)
{
    // need to assert that b and c have same number of rows as a
    Matrix<T> Q(a.get_row() + b.get_col() + c.get_col(), a.get_col() + b.get_col() + c.get_col());
    for(std::size_t i = 0; i != a.get_row(); ++i)
        for(std::size_t j = 0; j != a.get_col(); ++j)
            Q.at(i, j) = a.at(i, j);
    for(std::size_t i = 0; i != b.get_row(); ++i)
        for(std::size_t j = 0; j != b.get_col(); ++j)
        {
            Q.at(i, j + a.get_col()) = b.at(i, j);
            Q.at(j + a.get_col(), i) = b.at(i, j);
        }
    for(std::size_t i = 0; i != c.get_row(); ++i)
        for(std::size_t j = 0; j != c.get_col(); ++j)    
        {
            Q.at(i, j + a.get_col() + b.get_col()) = c.at(i, j);
            Q.at(j + a.get_col() + b.get_col(), i) = c.at(i, j);
        }
    return Q;
}

template <typename T>
Matrix<T> conjugateGradient(Matrix<T>& x, Matrix<T> const& Q, Matrix<T> const& b, T tol, std::size_t max_iter)
{
    Matrix<T> s = b - mmult(Q, x);
    Matrix<T> p = s;
    Matrix<T> s_prev; 
    T alpha;
    T beta;
    std::size_t count = 0;
    while(dotproduct(s, s) > tol && count <= max_iter)
    {
        alpha = dotproduct(s, s) / dotproduct(p, mmult(Q, p));
        x += alpha * p;
        s_prev = s;
        s -= alpha * mmult(Q, p);
        beta = dotproduct(s, s) / dotproduct(s_prev, s_prev);
        p = s + beta * p;
        ++count;
    }
    // std::cout << "Solution returned after " << count << " iteration\n";
    return x;
}