/*
A simple matrix class
c++ code
Author: Jos de Jong, Nov 2007. Updated March 2010

With this class you can:
  - create a 2D matrix with custom size
  - get/set the cell values
  - use operators +, -, *, /
  - use functions Ones(), Zeros(), Diag(), Det(), Inv(), Size()
  - print the content of the matrix

Usage:
  you can create a matrix by:
    Matrix A;
    Matrix A = Matrix(rows, cols);
    Matrix A = B;

  you can get and set matrix elements by:
    A(2,3) = 5.6;    // set an element of Matix A
    value = A(3,1);   // get an element of Matrix A
    value = A.get(3,1); // get an element of a constant Matrix A
    A = B;        // copy content of Matrix B to Matrix A

  you can apply operations with matrices and doubles:
    A = B + C;
    A = B - C;
    A = -B;
    A = B * C;
    A = B / C;

  the following functions are available:
    A = Ones(rows, cols);
    A = Zeros(rows, cols);
    A = Diag(n);
    A = Diag(B);
    d = Det(A);
    A = Inv(B);
    cols = A.GetCols();
    rows = A.GetRows();
    cols = Size(A, 1);
    rows = Size(A, 2);

  you can quick-print the content of a matrix in the console with:
    A.Print();
*/

#include <cstdlib>
#include <cstdio>
#include <math.h>

#define PAUSE {printf("Press \"Enter\" to continue\n"); fflush(stdin); getchar(); fflush(stdin);}

// Declarations
class Matrix;
double Det(const Matrix& a);
Matrix Diag(const int n);
Matrix Diag(const Matrix& v);
Matrix Inv(const Matrix& a);
Matrix Ones(const int rows, const int cols);
Matrix Zeros(const int rows, const int cols);

class Matrix {

private:
  int mRows;
  int mCols;
  double** p;     // pointer to a matrix with doubles

  void copy(const Matrix &m);
  void cleanup();

public:
  // Constructors
  Matrix();                      // default constructor
  Matrix(const Matrix &m);             // copy constructor
  Matrix(int rows, int columns);    // 2-argument constructor

  // Destructor
  ~Matrix();

  // Operators
  Matrix & operator=(const Matrix &rhs);
  double & operator()(const int r, const int c);

  Matrix & operator+=(const Matrix &rhs);
  Matrix & operator-=(const Matrix &rhs);
  Matrix & operator*=(const Matrix &rhs);

  const Matrix operator+(const Matrix &m) const;
  const Matrix operator-(const Matrix &m) const;
  const Matrix operator*(const Matrix &m) const;

  bool operator==(const Matrix &other) const;
  bool operator!=(const Matrix &other) const;

  // Mutator methods
  void setelem(int row, int column, double elem);

  void add(double v);
  void subtract(double v);
  void multiply(double v);
  void divide(double v);

  void normalize() const;
  void homogenize() const;

  // Matrix Transformations;
  void translationMatrix(double x, double y, double z);
  void scaleFactorMatrix(double x, double y, double z);
  void rotationMatrix(double x, double y, double z, double a);


  // Matrix calculations
  Matrix Minor(const int row, const int col) const;
  Matrix transpose() const;
  Matrix invert() const;


  double determinant() const;
  double dot(const Matrix &other) const;

  // Accessor methods
  int getrows() const;
  int getcols() const;
  double get(const int r, const int c) const;

  // Debugging functions
  void print() const;

};