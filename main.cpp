#include <iostream> //I/O Lib
#include <fstream> //Files
#include <iomanip> //I/O manipulation
#include <stdexcept> //Exceptions
#include <sstream> //String Stream
#include <string> //Strings
#include <algorithm> //Swapping


template<typename T>

class Matrix {

private:
    int n; // This integer holds the dimension of the matrix (n x n)
    // T** data is a pointer to a pointer of type T
    T** data;
    
    void allocateMemory() {
        // Allocate memory for n row pointers
        data = new T*[n];
        // Loop from 0 to n-1: each iteration allocates memory for one row
        for (int i = 0; i < n; i++) {
            // For the i-th row, allocate memory for n elements of type T
            data[i] = new T[n];
        }
    }

public:
    
    Matrix(int size) : n(size) {
        // Call allocateMemory() to set up the dynamic 2D array using the current value of n
        allocateMemory();
    }

    //deep copy constructor
    Matrix(const Matrix &other) : n(other.n) {
        // Allocate memory for the new matrix
        allocateMemory();
        // Loop over each row and column
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                // Copy each element from other into the current matrix
                data[i][j] = other.data[i][j];
            }
        }
    }

    ~Matrix() {
        // Loop over each row and free the memory allocated for that row
        for (int i = 0; i < n; i++) {
            delete [] data[i];
        }
        // Free the memory allocated for the array of row pointers
        delete [] data;
    }

    Matrix& operator=(const Matrix &other) {
        //if the current object is the same as other, do nothing
        if (this == &other){
            return *this;
        }
        // Free the memory currently held by this object
        for (int i = 0; i < n; i++) {
            delete [] data[i];
        }
        delete [] data;

        // Update the matrix dimension n with the dimension of n
        n = other.n;
        // Allocate new memory for the current object
        allocateMemory();
        // Copy every element from other to this object
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                data[i][j] = other.data[i][j];
            }
        }
        // Return a reference to the current object
        return *this;
    }

    int size() const { return n; }

    T getElement(int row, int col) const {
        // Check if the given row and column are within valid bounds
        if (row < 0 || row >= n || col < 0 || col >= n){
            throw std::out_of_range("Index out of range.");
        }
        // Return the matrix element at the specified location
        return data[row][col];
    }

    friend std::istream& operator>>(std::istream &in, Matrix<T> &mat) {
        // Loop through each row
        for (int i = 0; i < mat.n; i++) {
            // Loop through each column
            for (int j = 0; j < mat.n; j++) {
                // Read a value from the stream and store it in the matrix at [i][j]
                in >> mat.data[i][j];
            }
        }
        return in;
    }

    friend std::ostream& operator<<(std::ostream &out, const Matrix<T> &mat) {
        // Loop through each row
        for (int i = 0; i < mat.n; i++) {
            // Loop through each column
            for (int j = 0; j < mat.n; j++) {
                // Print the element with a width of 8 characters for alignment.
                out << std::setw(8) << mat.data[i][j];
            }
            // After printing each row, output a newline to move to the next row
            out << "\n";
        }
        return out;
    }

    Matrix<T> operator+(const Matrix<T> &other) const {
        // Create a new matrix to store the addition result
        Matrix<T> result(n);
        // Iterate over each row
        for (int i = 0; i < n; i++) {
            // Iterate over each column
            for (int j = 0; j < n; j++) {
                result.data[i][j] = data[i][j] + other.data[i][j];
            }
        }
        return result;
    }

    Matrix<T> operator*(const Matrix<T> &other) const {
        // Create a new matrix to store the multiplication result
        Matrix<T> result(n);
        // Loop over rows
        for (int i = 0; i < n; i++) {
            // Loop over columns
            for (int j = 0; j < n; j++) {
                result.data[i][j] = 0;
                for (int k = 0; k < n; k++) {
                    result.data[i][j] += data[i][k] * other.data[k][j];
                }
            }
        }
        return result;
    }

    T getMainDiagonalSum() const {
        T sum = 0;
        // For a square matrix, iterate from 0 to n-1
        for (int i = 0; i < n; i++) {
            sum += data[i][i];
        }
        return sum;
    }

    T getSecondaryDiagonalSum() const {
        T sum = 0; 
        for (int i = 0; i < n; i++) {
            // Add the element from the secondary diagonal: column (n-1-i)
            sum += data[i][n - 1 - i];
        }
        // Return the computed secondary diagonal sum
        return sum;
    }

    T getDiagonalSum() const{
        T sum = getMainDiagonalSum() + getSecondaryDiagonalSum(); 
        if (n % 2 == 1){ // For an odd-dimension matrix, the center element is counted twice
            sum -= getElement(n/2, n/2);
        }
        return sum;
    }

    void swapRows(int row1, int row2) {
        // Check if both row indices are within valid range (0 to n-1)
        if (row1 < 0 || row1 >= n || row2 < 0 || row2 >= n){
            std::cerr << "Column indices out of bounds.\n";
            return;
        }
        // Use std::swap to exchange the pointers that represent the rows
        std::swap(data[row1], data[row2]);
    }

    void swapColumns(int col1, int col2) {
        // Validate that the column indices are within the proper range
        if (col1 < 0 || col1 >= n || col2 < 0 || col2 >= n){
            std::cerr << "Column indices out of bounds.\n";
            return;
        }
        // For each row, swap the elements in the two specified columns
        for (int i = 0; i < n; i++) {
            std::swap(data[i][col1], data[i][col2]);
        }
    }

    void updateElement(int row, int col, T newValue) {
        // Check if the indices are valid
        if (row < 0 || row >= n || col < 0 || col >= n){
            std::cerr << "Index out of bounds.\n";
            return;
        }
        // Set the element at (row, col) to the new value
        data[row][col] = newValue;
    }

};  

template <typename T>
void updateMatrixElement(Matrix<T>& mat, int row, int col, T newValue) {
    // Update the element at (row, col) with newValue
    mat.updateElement(row, col, newValue);
    std::cout << "\nUpdated Matrix:\n" << mat;
}

//1. Read values from a file into the matrix
void loadMatrices(const std::string &filename) {
    std::ifstream infile(filename); //open the file
    if (!infile) { // If the file does not open successfully
        std::cerr << "Error opening file: " << filename << "\n"; //output an error message and exit the function
        return;
    }

    int N, typeFlag;
    infile >> N >> typeFlag; //Read the first two numbers from the file, N and the type flag
    if (N <= 0) { //The matrix size needs to be greater than zero
        std::cerr << "Invalid matrix size.\n";
        return;
    }
    
    if (typeFlag == 0) { //if the type flag is 0, use integer matrices
       
        Matrix<int> mat1(N), mat2(N); // Create two Matrix objects of type int with dimensions N x N

        infile >> mat1 >> mat2; // Use the overloaded >> operator to fill mat1 and mat2 with data from the file
        std::cout << "\nMatrix 1:\n" << mat1; // Print both matrices
        std::cout << "\nMatrix 2:\n" << mat2;

        //2. Add two matrices and display the result
        Matrix<int> added = mat1 + mat2;
        std::cout << "\nAdded Matrix (Matrix1 + Matrix2):\n" << added;

        //3. Multiply two matrices and display the result:
        Matrix<int> mult = mat1 * mat2;
        std::cout << "\nMultiplied Matrix (Matrix1 x Matrix2):\n" << mult;

        //4. Get the sum of matrix diagonal elements:
        int diagSum  = mat1.getDiagonalSum();
        std::cout << "\nSum of main and secondary diagonal elements (Matrix1): " << diagSum << "\n";

        //5. Swap matrix rows and display the result
        int row1, row2;
        std::cout << "Enter row index1 to swap (0 to " << N-1 << "): ";
        std::cin >> row1;
        std::cout << "Enter row index2 to swap (0 to " << N-1 << "): ";
        std::cin >> row2;
        mat1.swapRows(row1, row2);
        std::cout << "\nMatrix 1 after swapping rows " << row1 << " and " << row2 << ":\n" << mat1;


        //6. Swap matrix columns and display the result:
        int col1, col2;
        std::cout <<"For matrix 1\n";
        std::cout << "Enter column index1 to swap (0 to " << N-1 << "): ";
        std::cin >> col1;
        std::cout << "Enter column index2 to swap (0 to " << N-1 << "): ";
        std::cin >> col2;
        mat1.swapColumns(col1, col2);
        std::cout << "\nMatrix 1 after swapping columns " << col1 << " and " << col2 << ":\n" << mat1;

        //7. Update matrix rows and display the result
        int updRow, updCol, newValue;
        std::cout <<"For matrix 1\n";
        std::cout << "Enter row index (0 to " << N-1 << "): ";
        std::cin >> updRow;
        std::cout << "Enter column index (0 to " << N-1 << "): ";
        std::cin >> updCol;
        std::cout << "Enter new value: ";
        std::cin >> newValue;
        updateMatrixElement(mat1, updRow, updCol, newValue);
    }

    else if (typeFlag == 1) { //if the type flag is 1, use double-precision floating-point matrices
        Matrix<double> mat1(N), mat2(N); // Create two Matrix objects of type double with dimensions N x N
        infile >> mat1 >> mat2; // Use the overloaded >> operator to fill mat1 and mat2 with data from the file
        std::cout << "\nMatrix 1:\n" << mat1; // Print both matrices
        std::cout << "\nMatrix 2:\n" << mat2;

        //2. Add two matrices and display the result
        Matrix<double> added = mat1 + mat2;
        std::cout << "\nAdded Matrix (Matrix1 + Matrix2):\n" << added;

        //3. Multiply two matrices and display the result:
        Matrix<double> mult = mat1 * mat2;
        std::cout << "\nMultiplied Matrix (Matrix1 x Matrix2):\n" << mult;

        //4. Get the sum of matrix diagonal elements:
        double diagSum  = mat1.getDiagonalSum();
        std::cout << "\nSum of main and secondary diagonal elements (Matrix1): " << diagSum << "\n";

        //5. Swap matrix rows and display the result
        int row1, row2;
        std::cout << "Enter row index1 to swap (0 to " << N-1 << "): ";
        std::cin >> row1;
        std::cout << "Enter row index2 to swap (0 to " << N-1 << "): ";
        std::cin >> row2;
        mat1.swapRows(row1, row2);
        std::cout << "\nMatrix 1 after swapping rows " << row1 << " and " << row2 << ":\n" << mat1;


        //6. Swap matrix columns and display the result:
        int col1, col2;
        std::cout <<"For matrix 1\n";
        std::cout << "Enter column index1 to swap (0 to " << N-1 << "): ";
        std::cin >> col1;
        std::cout << "Enter column index2 to swap (0 to " << N-1 << "): ";
        std::cin >> col2;
        mat1.swapColumns(col1, col2);
        std::cout << "\nMatrix 1 after swapping columns " << col1 << " and " << col2 << ":\n" << mat1;

        //7. Update matrix rows and display the result
        int updRow, updCol;
        double newValue;
        std::cout <<"For matrix 1\n";
        std::cout << "Enter row index (0 to " << N-1 << "): ";
        std::cin >> updRow;
        std::cout << "Enter column index (0 to " << N-1 << "): ";
        std::cin >> updCol;
        std::cout << "Enter new value: ";
        std::cin >> newValue;
        updateMatrixElement(mat1, updRow, updCol, newValue);
    }

    else {
        // If the type flag is neither 0 nor 1, then the file format is incorrect.
        std::cerr << "Invalid type flag in file.\n";
    }
    // Close the input file once we are done reading the matrices.
    infile.close();
}

int main() {
    std::string filename; // Declare a string to hold the file name
    std::cout << "Enter input file name: ";  // Prompt the user for the file name
    std::cin >> filename;
    loadMatrices(filename); // Call function that processes the input file
    return 0;  // Return 0 to indicate that the program ended successfully.
}
