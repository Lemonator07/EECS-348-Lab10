#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <vector>
#include <array>
using namespace std;

class Matrix
{
private:
    int size;
    vector<int> data;

public:
    Matrix() : size(0) {}
    explicit Matrix(int n) : size(n), data(n * n, 0) {}

    bool readFromStream(istream &in, int n)
    {
        size = n;
        data.assign(n * n, 0);
        for (int i = 0; i < n * n; i++)
            if (!(in >> data[i]))
                return false;
        return true;
    }

    int getSize() const { return size; }
    int get(int r, int c) const { return data[r * size + c]; }
    void set(int r, int c, int value) { data[r * size + c] = value; }

    Matrix operator+(const Matrix &other) const
    {
        Matrix result(size);
        for (int r = 0; r < size; r++)
            for (int c = 0; c < size; c++)
                result.set(r, c, get(r, c) + other.get(r, c));
        return result;
    }

    Matrix operator*(const Matrix &other) const
    {
        Matrix result(size);
        for (int r = 0; r < size; r++)
            for (int c = 0; c < size; c++)
            {
                int value = 0;
                for (int i = 0; i < size; i++)
                    value += (get(r, i) * other.get(i, c));
                result.set(r, c, value);
            }
        return result;
    }

    std::array<int, 2> DiagonalSums() const
    {
        int diagonal1 = 0;
        int diagonal2 = 0;
        for (int i = 0; i < size; i++)
        {
            diagonal1 += get(i, i);
            diagonal2 += get(i, size - 1 - i);
        }
        return {diagonal1, diagonal2};
    }

    Matrix swapRows(int row1, int row2) const
    {
        if (row1 < 0 || row1 >= size || row2 < 0 || row2 >= size)
        {
            cerr << "Error: row index out of range\n";
            return *this; // return unchanged copy
        }

        Matrix result = *this;
        for (int c = 0; c < size; c++)
        {
            int temp = result.get(row1, c);
            result.set(row1, c, result.get(row2, c));
            result.set(row2, c, temp);
        }
        return result;
    }

    Matrix swapColumns(int Col1, int Col2) const
    {
        if (Col1 < 0 || Col1 >= size || Col2 < 0 || Col2 >= size)
        {
            cerr << "Error: Column index out of range\n";
            return *this; // return unchanged copy
        }

        Matrix result = *this;
        for (int r = 0; r < size; r++)
        {
            int temp = result.get(r, Col1);
            result.set(r, Col1, result.get(r, Col2));
            result.set(r, Col2, temp);
        }
        return result;
    }

    Matrix updateElement(int row = 0, int col = 0, int value = 100) const
    {
        Matrix result = *this;
        if (row < 0 || row >= size || col < 0 || col >= size)
        {
            cerr << "Error: invalid indices (" << row << ", " << col << ")\n";
            return result;
        }
        result.set(row, col, value);
        return result;
    }

    void print() const
    {
        for (int r = 0; r < size; r++)
        {
            for (int c = 0; c < size; c++)
                cout << setw(6) << get(r, c);
            cout << '\n';
        }
    }
};

int main()
{
    string filename;
    cout << "Enter input file name: ";
    cin >> filename;

    ifstream fin(filename);
    if (!fin)
    {
        cerr << "Could not open " << filename << '\n';
        return 1;
    }

    int n;
    if (!(fin >> n) || n <= 0)
    {
        cerr << "Invalid matrix size\n";
        return 1;
    }

    Matrix a, b;
    if (!a.readFromStream(fin, n) || !b.readFromStream(fin, n))
    {
        cerr << "Error reading matrix data\n";
        return 1;
    }

    cout << "\nMatrix A:\n";
    a.print();
    cout << "\nMatrix B:\n";
    b.print();
    Matrix sumadd = a + b; // uses the overloaded operator
    cout << "\nA + B:\n";
    sumadd.print();
    Matrix summult = a * b; // uses the overloaded operator
    cout << "\nA * B:\n";
    summult.print();
    array<int, 2> diagonals = a.DiagonalSums();
    cout << "\nDiagonal sums for Matrix A:\n";
    cout << "Main diagonal sum: " << diagonals[0] << '\n';
    cout << "Secondary diagonal sum: " << diagonals[1] << '\n';

    int row1, row2;
    cout << "Input first row to swap: ";
    cin >> row1;
    cout << "Input second row to swap: ";
    cin >> row2;
    Matrix swappedR = a.swapRows(row1, row2);
    cout << "\nMatrix A after swapping rows " << row1 << " and " << row2 << ":\n";
    swappedR.print();

    int col1, col2;
    cout << "Input first col to swap: ";
    cin >> col1;
    cout << "Input second col to swap: ";
    cin >> col2;
    Matrix swappedC = a.swapColumns(col1, col2);
    cout << "\nMatrix A after swapping columns " << col1 << " and " << col2 << ":\n";
    swappedC.print();

    int row, col, value;
    cout << "Input row index: ";
    cin >> row;
    cout << "Input column index: ";
    cin >> col;
    cout << "Input new value: ";
    cin >> value;

    Matrix updated = a.updateElement(row, col, value);
    cout << "\nMatrix A after updating (" << row << ", " << col << ") to " << value << ":\n";
    updated.print();

    return 0;
}