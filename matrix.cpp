#include "matrix.h"
#include <cassert>
#include <iostream>
#include <math.h>
#include <iomanip>

// Default constructor:  initializes a 0x0 matrix.
Matrix::Matrix() {
  mRows = 0;
  mCols = 0;
  p = NULL;
}

// Copy constructor: deep copy a matrix.
Matrix::Matrix(const Matrix &m) {
  
  copy(m);  // simply call helper function to copy
}

// Initializes the a matrix of size rows by columns.
Matrix::Matrix(int rows, int cols) {
  assert (rows >= 0);
  assert (cols >= 0);
  mRows = rows;
  mCols = cols;

  p = new double*[rows];
  for (int r = 0; r < rows; r++) {
		p[r] = new double[cols];

    // initially fill in ones to make identity matrix
    for (int c = 0; c < cols; c++) {
    	if (r == c) {
    		p[r][c] = 0;
    	} else {
    		p[r][c] = 0;
    	}
    }
  }
}


// Destructor - Clean up the allocated array.
Matrix::~Matrix() {
  cleanup();  // simple call cleanup function to deallocate
}

// Private helper functions for constructors/destructors/assignemnt operator

// copy contents of m into the object
void Matrix::copy(const Matrix &m) {

  mRows = m.getrows();
  mCols = m.getcols();

  p = new double*[m.getrows()];
  for (int r = 0; r < m.getrows(); r++) {
    p[r] = new double[m.getcols()];

    // copy the values from the matrix m
    for (int c = 0; c < m.getcols(); c++) {
      p[r][c] = m.p[r][c];
    }
  }
}


// clean up the current contents of the object
void Matrix::cleanup() {
  for (int r = 0; r < mRows; r++) {
    delete [] p[r];
  }
  delete [] p;
  p = NULL;
}

// Operators

// index operator. You can use this class like myMatrix(row, col)
// the indexes are zero based.
double & Matrix::operator()(const int r, const int c)
{
	assert(p != NULL);
	assert(mRows > 0 && r <= mRows);
	assert(mCols > 0 && c <= mCols);
    
  return p[r-1][c-1];
}

// Assignment operator that checks for self-assignment
Matrix & Matrix::operator=(const Matrix &rhs) {
  // Only do assignment if RHS is a different object from this.
  if (this != &rhs) {
    // Deallocate, allocate new space, copy values...
    cleanup();
    copy(rhs);
  }

  return *this;
}

// add a matrix to self
Matrix & Matrix::operator+=(const Matrix &rhs) {

  assert(mRows == rhs.getrows());   // rhs must be same size as matrix
  assert(mCols == rhs.getcols());

  for (int r = 0; r < mRows; r++) {
    for (int c = 0; c < mCols; c++) {
      p[r][c] += rhs.p[r][c];
    }
  }

  return *this;
}

// subtract a matrix from self
Matrix & Matrix::operator-=(const Matrix &rhs) {

  assert(mRows == rhs.getrows());   // rhs must be same size as matrix
  assert(mCols == rhs.getcols());

  for (int r = 0; r < mRows; r++) {
    for (int c = 0; c < mCols; c++) {
      p[r][c] -= rhs.p[r][c];
    }
  }

  return *this;
}

// multiply self by another matrix (must be of compatible dimensions) to 
// create a new matrix object
Matrix & Matrix::operator*=(const Matrix &rhs) {

  assert(mCols == rhs.getrows()); // matrix multiplication is only defined if
                                    // rows in rhs are equal to columns in self

  Matrix tempResult(mRows, rhs.getcols()); // temp matrix with correct dimensions 

  for (int row = 0; row < mRows; row++) {
    for (int col = 0; col < rhs.getcols(); col++) {
      // Multiply the row of self by the column of rhs to get the row, column of product.
      double value = 0;
      for (int inner = 0; inner < mCols; inner++) {
        value += p[row][inner] * rhs.p[inner][col];
      }
      tempResult.p[row][col] = value;
    }
  }

  *this = tempResult; // Assign result to self
  return *this;
}

// add operation to create a new matrix object
const Matrix Matrix::operator+(const Matrix &m) const {

  Matrix result(*this);   // make a copy of self
  result += m; // add the RHS to the result.
  return result;
}

// subtract operation to create a new matrix object
const Matrix Matrix::operator-(const Matrix &m) const {

  Matrix result(*this);  // make a copy of self
  result -= m;  // subtract the RHS from the result
  return result;
}  

// multiply operation to create a new matrix objct
const Matrix Matrix::operator*(const Matrix &m) const {

  Matrix result(*this);  // make a copy of self
  result *= m;  // subtract the RHS from the result
  return result;
}

// return true iff each element of self is equal to argument matrix
bool Matrix::operator==(const Matrix &other) const {
  // Compare the values, and return a bool result.
  if (mRows != other.getrows()) {
    return false;
  };
  if (mCols != other.getcols()) {
    return false;
  };

  for (int r = 0; r < mRows; r++) {
    for (int c = 0; c < mCols; c++) {
      if (p[r][c] != other.p[r][c]) { // if an element doesn't match, not equal
      	return false;
    	}
    }
  }

  return true;
} 

// return true iff at least 1 element of self does not match argument matrix
bool Matrix::operator!=(const Matrix &other) const {
  return !(*this == other);  // must be opposite of == operator.
}  

// adds a value to every element in the matrix
void Matrix::add(double v) {
  for (int r = 0; r < mRows; r++) {
    for (int c = 0; c < mCols; c++) {
      p[r][c] += v;
    }
  }
}

// subtracts a value from every element in the matrix
void Matrix::subtract(double v) {
  for (int r = 0; r < mRows; r++) {
    for (int c = 0; c < mCols; c++) {
      p[r][c] -= v;
    }
  }
}

// Scalar multiplication: every value in the matrix is multiplied by some value.
void Matrix::multiply(double v) {
  for (int r = 0; r < mRows; r++) {
    for (int c = 0; c < mCols; c++) {
      p[r][c] *= v;
    }
  }
}

// Scalar Division: every value in the matrix is multiplied by some value.
void Matrix::divide(double v) {
	assert(v != 0);
  for (int r = 0; r < mRows; r++) {
    for (int c = 0; c < mCols; c++) {
      p[r][c] /= v;
    }
  }
}

// Basic Matrix Transformations:

// translation matrix transformation for 4x4 matrix
void Matrix::translationMatrix(double x, double y, double z) {
	assert (mRows == 4);
	assert (mCols == 4);

	// first, start with identity matrix
  for (int r = 0; r < mRows; r++) {
    for (int c = 0; c < mCols; c++) {
    	if (r == c) {
    		p[r][c] = 1;
    	} else {
    		p[r][c] = 0;
    	}
    }
  }

  // then, add x, y, z to (1,4), (2,4), (3,4) respectively
  p[0][3] = x;
  p[1][3] = y;
  p[2][3] = z;
}

// scale factor matrix transformation for 4x4 matrix
void Matrix::scaleFactorMatrix(double x, double y, double z) {
	assert (mRows == 4);
	assert (mCols == 4);

	// first, start with all 0 matrix
  for (int r = 0; r < mRows; r++) {
    for (int c = 0; c < mCols; c++) {
    	p[r][c] = 0;
    }
  }

  // then, add x, y, z, 1 along diagnal
  p[0][0] = x;
  p[1][1] = y;
  p[2][2] = z;
  p[3][3] = 1;
}

// rotate matrix transformation for 4x4 matrix
// where (x,y,z) is the vector to rotate around, and a is the angle
void Matrix::rotationMatrix(double x, double y, double z, double a) {
	assert (mRows == 4);
	assert (mCols == 4);

	// first, start with identity matrix
  for (int r = 0; r < mRows; r++) {
    for (int c = 0; c < mCols; c++) {
    	if (r == c) {
    		p[r][c] = 1;
    	} else {
    		p[r][c] = 0;
    	}
    }
  }

	double sumOfSquares = x*x + y*y + z*z;
  double length = sqrt(sumOfSquares);  // get length of vector (x,y,z)

  if (length == 0) {
    std::cerr << "ERROR: Cannot rotate around (0,0,0)!" << std::endl;
    std::exit(1);
  }

  // normalize (x,y,z) vector
  x /= length;
  y /= length;
  z /= length;


  // then, add derived 3x3 matrix at top left.
  p[0][0] = (x * x) + ((1 - (x * x)) * cos(a));
  p[1][0] = (x * y * (1 - cos(a))) + (z * sin(a));
  p[2][0] = (x * z * (1 - cos(a))) - (y * sin(a));

  p[0][1] = (x * y * (1 - cos(a))) - (z * sin(a));
  p[1][1] = (y * y) + ((1 - (y * y)) * cos(a));
  p[2][1] = (y * z * (1 - cos(a))) + (x * sin(a));

  p[0][2] = (x * z * (1 - cos(a))) + (y * sin(a));
  p[1][2] = y * z * (1 - cos(a)) - (x * sin(a));
  p[2][2] = (z * z) + ((1 - (z * z)) * cos(a));
}

// Matrix Calculations

/**
 * returns the minor from the given matrix where
 * the selected row and column are removed
 */
Matrix Matrix::Minor(const int row, const int col) const {
	assert (row > 0 && row <= mRows);
	assert (col > 0 && col <= mCols);
	int rows = mRows;
	int cols = mCols;
  Matrix res =  Matrix(rows - 1, cols - 1);

  // copy the content of the matrix to the minor, except the selected
  for (int r = 1; r <= (rows - (row >= rows)); r++) {
    for (int c = 1; c <= (cols - (col >= cols)); c++) {
      res(r - (r > row), c - (c > col)) = p[r-1][c-1];
    }
  }  

  return res;
}

/**
 * returns the a transposed version of the matrix.
 * does not affect the original matrix.
 */
Matrix Matrix::transpose() const {

  Matrix tranpose(mCols, mRows); // temp matrix with correct dimensions 

  for (int row = 0; row < mRows; row++) {
    for (int col = 0; col < mCols; col++) {
    	tranpose.p[col][row] = p[row][col];
    }
  }

  return tranpose;
}


Matrix Matrix::invert() const {
	assert (mRows == mCols);

	// stick the identity matrix on right side
	Matrix augmented(mRows, 2*mRows);
	double temporary, r;
	int i, j, k, temp;
	int dimension = mRows;

	/* copy matrix over to left side of augmented matrix */
  for (i = 1; i <= dimension; i++) {
  	for (j = 1; j <= dimension; j++) {
  		augmented(i,j) = p[i-1][j-1];
  	}
  }

  /* augmenting with identity matrix of similar dimensions */
  for (i = 1; i <= dimension; i++) {
  	for (j = dimension + 1; j <= 2*dimension; j++) {
      if (i == j - dimension) {
      	augmented(i,j) = 1;
      }
      else {
        augmented(i,j) = 0;
      }
  	}
  }

 	/* using gauss-jordan elimination */

  for(j = 1; j <= dimension; j++) {
  	temp = j;
    
 		/* finding maximum jth column element in last (dimension-j) rows */

  	for (i = j+1; i <= dimension; i++) {
  		if (abs(augmented(i,j)) > abs(augmented(temp,j))) {
  			temp = i;
  		}
  	}

 		/* swapping row which has maximum jth column element */

  	if (temp != j) {
  		for (k = 1; k <= 2*dimension; k++) {
  			temporary = augmented(j,k);
        augmented(j,k) = augmented(temp,k);
        augmented(temp,k) = temporary;
  		}
  	}

		/* performing row operations to form required identity matrix out of the input matrix */
  	for (i = 1; i <= dimension; i++) {
      if (i != j) {
      	r = augmented(i,j);
        for (k = 1; k <= 2*dimension; k++) {
        	augmented(i,k) -= (augmented(j,k) / augmented(j,j)) * r;
        }
      } else {
      	r = augmented(i,j);
        for(k = 1; k <= 2*dimension; k++) {
          augmented(i,k) /= r;
        }
      }
  	}
 	}
 	/* The inverse is left on the right side of the augmented matrix. Retrieve it. */
 	Matrix inverse(mRows, mCols);
 	for (i = 1; i <= dimension; i++) {
  	for (j = 1; j <= dimension; j++) {
  		inverse(i,j) = augmented(i,j + dimension);
  	}
  }

 	return inverse;
}



// return determinant of a square matrix
double Matrix::determinant() const {
	assert(mRows == mCols);  // must be square matrix

  double d = 0;    // value of the determinant
  int rows = mRows;
  int cols = mCols;

  if (rows == 1) {
    // this is a 1 x 1 matrix
    d = p[0][0];
  } else if (rows == 2) {
    // this is a 2 x 2 matrix
    // the determinant of [a11,a12;a21,a22] is det = a11*a22-a21*a12
    //d = this.get(1, 1) * this.get(2, 2) - this.get(2, 1) * this.get(1, 2);
    d = p[0][0] * p[1][1] - p[1][0] * p[0][1];
  } else {
    // this is a matrix of 3 x 3 or larger
    for (int c = 1; c <= cols; c++) {
      Matrix M = Minor(1, c);
      //d += pow(-1, 1+c) * a(1, c) * Det(M);
      d += (c%2 + c%2 - 1) * p[0][c-1] * M.determinant(); // faster than with pow()
    }
  }

  return d;
}

// returns the dot product of two equal sized vectors
// (each matrix must have only 1 column, and have the same number of rows)
double Matrix::dot(const Matrix &other) const {
	assert(mCols == 1);  // must be a vector
	assert(other.getcols() == 1); // must be a vector
	assert (mRows == other.getrows()); // must be equal size

	double dot = 0;
  for (int i = 1; i <= mRows; i++) {
  	dot += p[i-1][0] * other.p[i-1][0];
  } 

  return dot;
}

// normalize a vector.  A vector must have only 1 column.
void Matrix::normalize() const {
	assert(mCols == 1);

	double sumOfSquares = 0;
  for (int i = 1; i <= mRows; i++) {
  	sumOfSquares += p[i-1][0] * p[i-1][0];  // get sum of squares
  }
  double length = sqrt(sumOfSquares);  // get length of vector

  // divide each value in vector by length to normalize
  for (int i = 1; i <= mRows; i++) {
  	p[i-1][0] /= length;
  } 
}

// homogenize a vector.  A vector must have only 1 column.
// a 4d vector (x,y,z,w) will become (x/w, y/w, z/w, 1).
// a 3d vector (x,y,z) will become (x/z, y/z, 1).
void Matrix::homogenize() const {
	assert(mCols == 1);

	double factor = p[mRows-1][0];  // we will divide through by last elem
  for (int i = 1; i <= mRows; i++) {
  	p[i-1][0] /= factor; 
  }
}

// Mutators:

// sets the element at location [row, column] to have a value "elem"
void Matrix::setelem(int row, int column, double elem) {
  int index = (row * mCols) + column;
  assert(index >= 0);
  assert(index <= (mRows * mCols));
  p[row][column] = elem;
}

// Accessors:

// get the number of rows in the matrix
int Matrix::getrows() const {
  return mRows;
}

// get the number of columns in the matrix
int Matrix::getcols() const {
  return mCols;
}

// index operator. You can use this class like myMatrix.get(col, row)
// the indexes are zero based.
// use this function get if you want to read from a const Matrix
double Matrix::get(const int r, const int c) const {
 	assert(p != NULL);
	assert(mRows > 0 && r <= mRows);
	assert(mCols > 0 && c <= mRows);

  return p[r-1][c-1];
}

// Debugging purposes:

// print the contents of the matrix
void Matrix::print() const {
  if (p != NULL) {
    for (int r = 0; r < mRows; r++) {
      std::cerr << "[";
      for (int c = 0; c < mCols-1; c++) {
          //std::cerr << std::setw(10) << p[r][c] << " ";
          std::cerr << p[r][c] << " ";
        }
      std::cerr /*<< std::setw(10)*/ << p[r][mCols-1] << "]\n";
    }
  } else {
    // matrix is empty
    printf("[ ]\n");
  }
}

