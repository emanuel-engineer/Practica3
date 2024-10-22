#include <iostream>
#include <fstream>
#include <vector>
#include <bitset>
#include <string>
using namespace std;
// Función para convertir un caracter a su representación binaria
string charToBinary(char c) {
    return bitset<8>(c).to_string();
}
// Función para convertir una cadena binaria a un caracter
char binaryToChar(const string& binary) {
    return static_cast<char>(bitset<8>(binary).to_ulong());
}
// Método de codificación 1
string encodeMethod1(const string& binary, int n) {
    string encoded = binary;
    int blocks = binary.length() / n;
    for (int i = 0; i < blocks; ++i) {
        int start = i * n;
        int end = start + n;
        if (i == 0) {
            // Invertir todos los bits en el primer bloque
            for (int j = start; j < end; ++j) {
                encoded[j] = (binary[j] == '0') ? '1' : '0';
            }
        } else {
            int ones = 0, zeros = 0;
            for (int j = start - n; j < start; ++j) {
                if (binary[j] == '1') ones++;
                else zeros++;
            }
            if (ones == zeros) {
                // Invertir cada bit
                for (int j = start; j < end; ++j) {
                    encoded[j] = (binary[j] == '0') ? '1' : '0';
                }
            } else if (zeros > ones) {
                // Invertir cada 2 bits
                for (int j = start; j < end; j += 2) {
                    encoded[j] = (binary[j] == '0') ? '1' : '0';
                    if (j + 1 < end) encoded[j + 1] = (binary[j + 1] == '0') ? '1' : '0';
                }
            } else {
                // Invertir cada 3 bits
                for (int j = start; j < end; j += 3) {
                    encoded[j] = (binary[j] == '0') ? '1' : '0';
                    if (j + 1 < end) encoded[j + 1] = (binary[j + 1] == '0') ? '1' : '0';
                    if (j + 2 < end) encoded[j + 2] = (binary[j + 2] == '0') ? '1' : '0';
                }
            }
        }
    }
    return encoded;
}
// Método de codificación 2
string encodeMethod2(const string& binary, int n) {
    string encoded = binary;
    int blocks = binary.length() / n;
    for (int i = 0; i < blocks; ++i) {
        int start = i * n;
        for (int j = 0; j < n; ++j) {
            encoded[start + j] = binary[start + ((j + n - 1) % n)];
        }
    }
    return encoded;
}
int main() {
    int n, method;
    string inputFile, outputFile;
    cout << "Ingrese la semilla de codificacion (n): ";
    cin >> n;
    cout << "Ingrese el metodo de codificacion (1 o 2): ";
    cin >> method;
    cout << "Ingrese la direccion del archivo de entrada: ";
    cin >> inputFile;
    cout << "Ingrese la direccion del archivo de salida: ";
    cin >> outputFile;
    // Leer el archivo de entrada
    ifstream inFile(inputFile, ios::binary);
    if (!inFile) {
        cerr << "No se pudo abrir el archivo de entrada." << endl;
        return 1;
    }
    string content((istreambuf_iterator<char>(inFile)), istreambuf_iterator<char>());
    inFile.close();
    // Convertir el contenido a binario
    string binaryContent;
    for (char c : content) {
        binaryContent += charToBinary(c);
    }
    // Codificar el contenido
    string encodedBinary;
    if (method == 1) {
        encodedBinary = encodeMethod1(binaryContent, n);
    } else if (method == 2) {
        encodedBinary = encodeMethod2(binaryContent, n);
    } else {
        cerr << "Metodo de codificacion invalido." << endl;
        return 1;
    }
    // Escribir el resultado en el archivo de salida
    ofstream outFile(outputFile, ios::binary);
    if (!outFile) {
        cerr << "No se pudo abrir el archivo de salida." << endl;
        return 1;
    }
    for (size_t i = 0; i < encodedBinary.length(); i += 8) {
        string byte = encodedBinary.substr(i, 8);
        outFile.put(binaryToChar(byte));
    }
    outFile.close();
    cout << "Archivo codificado exitosamente." << endl;
    return 0;
}
