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

// Método de decodificación 1 (invierte el proceso del método 1 de codificación)
string decodeMethod1(const string& binary, int n) {
    string decoded = binary;
    int blocks = binary.length() / n;

    for (int i = 0; i < blocks; ++i) {
        int start = i * n;
        int end = start + n;

        if (i == 0) {
            // Invertir todos los bits en el primer bloque (la misma operación que en codificación)
            for (int j = start; j < end; ++j) {
                decoded[j] = (binary[j] == '0') ? '1' : '0';
            }
        } else {
            // Para los bloques siguientes, necesitamos analizar el bloque anterior del texto original
            string originalPreviousBlock;
            // Reconstruimos el bloque anterior del texto original
            for (int j = start - n; j < start; ++j) {
                if (i == 1) {
                    // Para el segundo bloque, el bloque anterior es el primer bloque decodificado
                    originalPreviousBlock += decoded[j];
                } else {
                    // Para los demás bloques, el bloque anterior es del texto original
                    originalPreviousBlock += binary[j];
                }
            }

            int ones = 0, zeros = 0;
            for (char bit : originalPreviousBlock) {
                if (bit == '1') ones++;
                else zeros++;
            }

            if (ones == zeros) {
                // Invertir cada bit
                for (int j = start; j < end; ++j) {
                    decoded[j] = (binary[j] == '0') ? '1' : '0';
                }
            } else if (zeros > ones) {
                // Invertir cada 2 bits
                for (int j = start; j < end; j += 2) {
                    decoded[j] = (binary[j] == '0') ? '1' : '0';
                    if (j + 1 < end) decoded[j + 1] = (binary[j + 1] == '0') ? '1' : '0';
                }
            } else {
                // Invertir cada 3 bits
                for (int j = start; j < end; j += 3) {
                    decoded[j] = (binary[j] == '0') ? '1' : '0';
                    if (j + 1 < end) decoded[j + 1] = (binary[j + 1] == '0') ? '1' : '0';
                    if (j + 2 < end) decoded[j + 2] = (binary[j + 2] == '0') ? '1' : '0';
                }
            }
        }
    }

    return decoded;
}

// Método de decodificación 2 (invierte el proceso del método 2 de codificación)
string decodeMethod2(const string& binary, int n) {
    string decoded = binary;
    int blocks = binary.length() / n;

    for (int i = 0; i < blocks; ++i) {
        int start = i * n;
        for (int j = 0; j < n; ++j) {
            decoded[start + ((j + n - 1) % n)] = binary[start + j];
        }
    }

    return decoded;
}

int main() {
    int n, method;
    string inputFile, outputFile;

    cout << "Ingrese la semilla de decodificación (n): ";
    cin >> n;

    cout << "Ingrese el método de decodificación (1 o 2): ";
    cin >> method;

    cout << "Ingrese el nombre del archivo codificado: ";
    cin >> inputFile;

    cout << "Ingrese el nombre del archivo de salida: ";
    cin >> outputFile;

    try {
        // Leer el archivo codificado
        ifstream inFile(inputFile, ios::binary);
        if (!inFile) {
            throw runtime_error("No se pudo abrir el archivo de entrada.");
        }

        string content((istreambuf_iterator<char>(inFile)), istreambuf_iterator<char>());
        inFile.close();

        // Convertir el contenido a binario
        string binaryContent;
        for (char c : content) {
            binaryContent += charToBinary(c);
        }

        // Decodificar el contenido
        string decodedBinary;
        if (method == 1) {
            decodedBinary = decodeMethod1(binaryContent, n);
        } else if (method == 2) {
            decodedBinary = decodeMethod2(binaryContent, n);
        } else {
            throw runtime_error("Método de decodificación inválido.");
        }

        // Escribir el resultado en el archivo de salida
        ofstream outFile(outputFile);
        if (!outFile) {
            throw runtime_error("No se pudo abrir el archivo de salida.");
        }

        // Convertir la cadena binaria decodificada a caracteres
        for (size_t i = 0; i < decodedBinary.length(); i += 8) {
            string byte = decodedBinary.substr(i, 8);
            outFile.put(binaryToChar(byte));
        }
        outFile.close();

        cout << "Archivo decodificado exitosamente." << endl;
        cout << decodedBinary ;

    } catch (const exception& e) {
        cerr << "Error: " << e.what() << endl;
        return 1;
    }


    return 0;
}
