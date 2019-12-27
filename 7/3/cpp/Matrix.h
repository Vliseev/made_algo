#pragma once
#include <algorithm>
#include <cstddef>
#include <exception>
#include <stdexcept>

template <class T>
class Matrix {
    size_t n_row_;
    size_t n_col_;
    T *data_ = nullptr;

   public:
    Matrix(size_t n_row, size_t n_col);
    ~Matrix();
    size_t GetNRows() const {
        return n_row_;
    };
    size_t GetColumns() const {
        return n_col_;
    };

    T *operator()(size_t i);
    T operator()(size_t i, size_t j);
    T &operator()(size_t i, size_t j) const;

    bool operator==(const Matrix &rhs) const;
    bool operator!=(const Matrix &rhs) const;
    Matrix(const Matrix &);
    Matrix(Matrix &&) noexcept;

    Matrix &operator=(const Matrix &);
    Matrix &operator=(Matrix &&) noexcept;
};
template <class T>
Matrix<T>::Matrix(size_t n_row, size_t n_col)
    : n_row_(n_row), n_col_(n_col), data_(new T[n_col * n_row]) {
}
template <class T>
Matrix<T>::~Matrix() {
    delete[] data_;
}
template <class T>
bool Matrix<T>::operator==(const Matrix &rhs) const {
    return n_row_ == rhs.n_row_ && n_col_ == rhs.n_col_ &&
           std::equal(data_, data_ + n_row_ * n_col_, rhs.data_);
}
template <class T>
bool Matrix<T>::operator!=(const Matrix &rhs) const {
    return !this == rhs;
}
template <class T>
T *Matrix<T>::operator()(size_t i) {
    if (i >= n_row_)
        throw std::out_of_range("invalid row");
    return data_ + i * n_col_ * sizeof(T);
}
template <class T>
T Matrix<T>::operator()(size_t i, size_t j) {
    if (i >= n_row_ || j >= n_col_)
        throw std::out_of_range("invalid idx");
    return (data_ + i * n_col_ + j) * sizeof(T);
}
template <class T>
T &Matrix<T>::operator()(size_t i, size_t j) const {
    if (i >= n_row_ || j >= n_col_)
        throw std::out_of_range("invalid idx");
    return (data_ + i * n_col_ + j) * sizeof(T);
}
template <class T>
Matrix<T>::Matrix(const Matrix &other)
    : n_col_(other.n_col_),
      n_row_(other.n_row_),
      data_(new T[other.n_col_ * other.n_row_]) {
    std::copy(other.data_, other.data_ + n_row_ * n_col_, data_);
}
template <class T>
Matrix<T> &Matrix<T>::operator=(const Matrix &other) {
    if (std::addressof(other) == this)
        return *this;
    n_col_ = other.n_col_;
    n_row_ = other.n_row_;
    data_ = new T[other.n_col_ * other.n_row_];
    std::copy(other.data_, other.data_ + n_row_ * n_col_, data_);
    return *this;
}
template <class T>
Matrix<T>::Matrix(Matrix &&other) noexcept
    : n_col_(other.n_col_), n_row_(other.n_row_) {
    data_ = other.data_;
    other.data_ = nullptr;
}
template <class T>
Matrix<T> &Matrix<T>::operator=(Matrix &&other) noexcept {
    if (std::addressof(other) == this)
        return *this;

    n_col_ = other.n_col_;
    n_row_ = other.n_row_;
    data_ = other.data_;
    other.data_ = nullptr;
    return *this;
}
