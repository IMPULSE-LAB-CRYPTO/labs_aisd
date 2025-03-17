#include <iostream>
#include <iomanip>
#include <stdexcept>
#include <complex>
#include <random>


using namespace std;

template<typename T>
class Matrix {
private:
    size_t rows;
    size_t cols;
    T** data;

public:
    static const T epsilon;

    /// ����������� � �����������
    Matrix(size_t rows, size_t cols, T value = T()) : rows(rows), cols(cols) {
        data = new T * [rows];
        for (size_t i = 0; i < rows; i++) {
            data[i] = new T[cols];
            for (size_t j = 0; j < cols; j++) {
                data[i][j] = value;
            }
        }
    }

    /// ����������� � ������ �����������
    Matrix(size_t rows, size_t cols, T lower_bound, T upper_bound) : rows(rows), cols(cols) {
        std::random_device rd; 
        std::mt19937 gen(rd());
        std::uniform_real_distribution<> dis(lower_bound, upper_bound);

        data = new T* [rows];
        for (size_t i = 0; i < rows; i++) {
            data[i] = new T[cols];
            for (size_t j = 0; j < cols; j++) {
                data[i][j] = static_cast<T>(dis(gen));
            }
        }
    }

    /// ����������� �����������
    Matrix(const Matrix& other) : rows(other.rows), cols(other.cols) {
        data = new T* [rows];
        for (size_t i = 0; i < rows; i++) {
            data[i] = new T[cols];
            for (size_t j = 0; j < cols; j++) {
                data[i][j] = other.data[i][j];
            }
        }
    }

    /// ����������
    ~Matrix() {
        for (size_t i = 0; i < rows; i++) {
            delete[] data[i];
        }
        delete[] data;
    }

    /// �������� () ��� ������/������ �������� ������� �� ��������� ��������
    T& operator()(size_t row, size_t col) {
        if (row >= rows || col >= cols) {
            throw std::out_of_range("������ ��� ���������");
        }
        return data[row][col];
    }

    const T& operator()(size_t row, size_t col) const {
        if (row >= rows || col >= cols) {
            throw std::out_of_range("������ ��� ���������");
        }
        return data[row][col];
    }

    /// ��������� ��������� �� ��������� � �����������
    bool operator==( Matrix& other) {
        if (rows != other.rows || cols != other.cols) {
            return false;
        }
        for (size_t i = 0; i < rows; i++) {
            for (size_t j = 0; j < cols; j++) {
                if (std::abs(data[i][j] - other.data[i][j]) > (epsilon)) {
                    return false;
                }
            }
        }
        return true;
    }

    template <>
    bool operator==<complex <double>> (const Matrix<complex <double>> &other) {
        complex<double> z(1.0, 2.0);
        if (rows != other.rows || cols != other.cols) {
            return false;
        }
        for (size_t i = 0; i < rows; i++) {
            for (size_t j = 0; j < cols; j++) {
                if (std::abs(data[i][j] - other.data[i][j]) > z {
                    return false;
                }
            }
        }
        return true;
    }

    bool operator!=(const Matrix& other) const {
        return !(*this == other);
    }

    /// ��������� �������� � ��������� ������
    Matrix operator+(const Matrix& other) const {
        if (rows != other.rows || cols != other.cols) {
            throw std::invalid_argument("������� ������ ����� ���������� ������� ��� ��������");
        }
        Matrix result(rows, cols);
        for (size_t i = 0; i < rows; i++) {
            for (size_t j = 0; j < cols; j++) {
                result(i, j) = data[i][j] + other.data[i][j];
            }
        }
        return result;
    }

    Matrix operator-(const Matrix& other) const {
        if (rows != other.rows || cols != other.cols) {
            throw std::invalid_argument("������� ������ ����� ���������� ������� ��� ��������");
        }
        Matrix result(rows, cols);
        for (size_t i = 0; i < rows; i++) {
            for (size_t j = 0; j < cols; j++) {
                result(i, j) = data[i][j] - other.data[i][j];
            }
        }
        return result;
    }

    /// �������� ��������� ������
    Matrix operator*(const Matrix& other) const {
        if (cols != other.rows) {
            throw std::invalid_argument("������� ������ ����� ��������������� ������� ��� ��������� �� ���������");
        }
        Matrix result(rows, other.cols);
        for (size_t i = 0; i < rows; i++) {
            for (size_t j = 0; j < other.cols; j++) {
                result(i, j) = T();
                for (size_t k = 0; k < cols; k++) {
                    result(i, j) += data[i][k] * other.data[k][j];
                }
            }
        }
        return result;
    }

    /// �������� ��������� ������� �� ������
    Matrix operator*(T scalar) const {
        Matrix result(rows, cols);
        for (size_t i = 0; i < rows; i++) {
            for (size_t j = 0; j < cols; j++) {
                result(i, j) = data[i][j] * scalar;
            }
        }
        return result;
    }

    /// �������� ������� ������� �� ������
    Matrix operator/(T scalar) const {
        if (scalar == T()) {
            throw std::invalid_argument("������� �� ����");
        }
        Matrix result(rows, cols);
        for (size_t i = 0; i < rows; i++) {
            for (size_t j = 0; j < cols; j++) {
                result(i, j) = data[i][j] / scalar;
            }
        }
        return result;
    }

    /// ���������� ����� �������
    T trace() const {
        if (rows != cols) {
            throw std::invalid_argument("������� ������ ���� ���������� ��� ���������� �����");
        }
        T trace = T();
        for (size_t i = 0; i < rows; i++) {
            trace += data[i][i];
        }
        return trace;
    }

    /// �������� ������
    friend std::ostream& operator<<(std::ostream& os, const Matrix& matrix) {
        for (size_t i = 0; i < matrix.rows; i++) {
            for (size_t j = 0; j < matrix.cols; j++) {
                os << std::setw(10) << matrix.data[i][j] << " ";
            }
            os << endl;
        }
        return os;
    }

    /// ��������� ���������� �����
    size_t getRows() const {
        return rows;
    }

    /// ��������� ���������� ��������
    size_t getCols() const {
        return cols;
    }
};



template<typename T>
const T Matrix<T>::epsilon = static_cast<T>(1e-5);

/// ������� ������: ���������� �������� ������� ����������� 3x3
template<typename T>
Matrix<T> inverse(const Matrix<T>& mat) {
    if (mat.getRows() != 3 || mat.getCols() != 3) {
        throw std::invalid_argument("������� ������ ���� 3 �� 3, ����� ����� ��������");
    }

    Matrix<T> inv(3, 3);
    T det = mat(0, 0) * (mat(1, 1) * mat(2, 2) - mat(2, 1) * mat(1, 2)) -
        mat(0, 1) * (mat(1, 0) * mat(2, 2) - mat(1, 2) * mat(2, 0)) +
        mat(0, 2) * (mat(1, 0) * mat(2, 1) - mat(1, 1) * mat(2, 0));

    if (std::abs(det) < Matrix<T>::epsilon) {
        throw std::invalid_argument("�� ����������� ������� ������ ����� ��������");
    }
    
    inv(0, 0) = (mat(1, 1) * mat(2, 2) - mat(2, 1) * mat(1, 2)) / det;
    inv(0, 1) = (mat(0, 2) * mat(2, 1) - mat(0, 1) * mat(2, 2)) / det;
    inv(0, 2) = (mat(0, 1) * mat(1, 2) - mat(0, 2) * mat(1, 1)) / det;
    inv(1, 0) = (mat(1, 2) * mat(2, 0) - mat(1, 0) * mat(2, 2)) / det;
    inv(1, 1) = (mat(0, 0) * mat(2, 2) - mat(0, 2) * mat(2, 0)) / det;
    inv(1, 2) = (mat(1, 0) * mat(0, 2) - mat(0, 0) * mat(1, 2)) / det;
    inv(2, 0) = (mat(1, 0) * mat(2, 1) - mat(2, 0) * mat(1, 1)) / det;
    inv(2, 1) = (mat(2, 0) * mat(0, 1) - mat(0, 0) * mat(2, 1)) / det;
    inv(2, 2) = (mat(0, 0) * mat(1, 1) - mat(1, 0) * mat(0, 1)) / det;

    return inv;
}




int main() {
    setlocale(LC_ALL, "ru_RU");
    try {
        complex<double> Z = 1.0 + 2i;
        complex<double> Z2 = 10.0 + 2i;
        //������ �������
        Matrix <complex <double>> TestRandom(2, 2, Z, Z2);
        cout << "Matrix TestRandom:" << endl << TestRandom << endl;

        //����������� �����������
        Matrix CopyRandom(TestRandom);
        cout << "Matrix CopyRandom:" << endl << CopyRandom << endl;

        ////���������
        //cout << "�������� ��������� ������ �������" << endl;
        //bool Comparison = (CopyRandom == TestRandom);
        //cout << "��������� ���������: " << Comparison <<endl;

        //��������� ������
        Matrix Composition = TestRandom * CopyRandom;
        cout << endl << "Matrix Composition:" << endl << Composition << endl;

        //��������� �� ������
        Matrix CompositionOnScalar = Composition * 2;
        cout << "Matrix CompositionOnScalar:" << endl << CompositionOnScalar << endl;

        //���������
        Matrix Substraction = Composition - TestRandom;
        cout << "Matrix Substraction:" << endl << Substraction << endl;

        //������� �� ������
        Matrix DivisionOnScalar = CompositionOnScalar / 2;
        cout << "Matrix DivisionOnScalar:" << endl << DivisionOnScalar << endl;


        //������� ������
        Matrix<double> A(3, 3, 1.0, 10.0);
        cout << "Matrix A:" << endl << A << endl;

        Matrix<double> invA = inverse(A);
        cout << "Inverse of A:" << endl << invA << endl;

        Matrix Res = A * invA;
        cout << "check:" << endl << Res << endl;
    }

    catch (const std::exception& e) {
        cerr << "Error: " << e.what() << endl;
    }

    return 0;
}
