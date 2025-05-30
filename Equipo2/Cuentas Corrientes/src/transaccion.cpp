////Clase transacci�n - Astrid Ru�z 9959242976
#include "transaccion.h"
#include "bitacora.h"
#include <iostream>
#include <fstream>
#include <string>
using namespace std;

// Constructor por defecto
transaccion::transaccion() {}

// Constructor con par�metros
transaccion::transaccion(string id, string tipo, double monto, string fecha, string banco) {
    this->idTransaccion = id;
    this->tipo = tipo;
    this->monto = monto;
    this->fecha = fecha;
    this->banco = banco;
}

// Generar nuevo ID autom�tico leyendo el �ltimo del archivo
string transaccion::generarNuevoID() {
    ifstream archivo("transacciones.txt");
    string linea, ultimoID = "8700"; //inici� desde este para la bitacora

    if (archivo.is_open()) {
        while (getline(archivo, linea)) {
            size_t pos = linea.find("|");
            if (pos != string::npos) {
                ultimoID = linea.substr(0, pos);
            }
        }
        archivo.close();
    }

    int nuevoID = stoi(ultimoID) + 1; //aumentarle 1 al id
    return to_string(nuevoID);
}

// Registrar nueva transacci�n
void transaccion::registrarTransaccion(string tipoOperacion) {
    idTransaccion = generarNuevoID(); //el id que se genera
    tipo = tipoOperacion; //depende si es retiro, deposito o transaccion

    cout << "Ingrese monto: ";
    cin >> monto;
    cout << "Ingrese fecha (YYYY-MM-DD): ";
    cin >> fecha;
    cout << "Ingrese banco: ";
    cin >> banco;
}

// Mostrar una sola transacci�n
void transaccion::mostrarTransaccion() const {
    cout << "ID: " << idTransaccion << " | "
         << "Tipo: " << tipo << " | "
         << "Monto: Q" << monto << " | "
         << "Fecha: " << fecha << " | "
         << "Banco: " << banco << endl;
}

// Guardar transacci�n en el archivo
void transaccion::guardarEnArchivo() const {
    ofstream archivo("transacciones.txt", ios::app);
    if (archivo.is_open()) {
        archivo << idTransaccion << "|"
                << tipo << "|"
                << monto << "|"
                << fecha << "|"
                << banco << endl;
        archivo.close();
        cout << "\nTransacci�n registrada correctamente.\n";

        // Registrar en bit�cora
        bitacora b;
        b.insertar("usuario1", "Transacci�n", "Registro de nueva transacci�n ID: " + idTransaccion);
    } else {
        cout << "Error al abrir el archivo.\n";
    }
}

// Mostrar todas las transacciones desde archivo
void transaccion::mostrarTodasDesdeArchivo() {
    ifstream archivo("transacciones.txt");
    string linea;

    if (archivo.is_open()) {
        cout << "\n=== Listado de Transacciones Bancarias ===\n\n";
        while (getline(archivo, linea)) {
            cout << linea << endl;
        }
        archivo.close();
    } else {
        cout << "No se pudo abrir el archivo.\n";
    }
}

// Editar una transacci�n por ID
void transaccion::editarTransaccion(string id) {
    ifstream archivoOriginal("transacciones.txt");
    ofstream archivoTemp("temp.txt");
    string linea;
    bool encontrado = false;

    if (archivoOriginal.is_open() && archivoTemp.is_open()) {
        while (getline(archivoOriginal, linea)) {
            if (linea.find(id + "|") == 0) {
                encontrado = true;
                cout << "Transacci�n encontrada: " << linea << endl;
                transaccion nueva;
                nueva.idTransaccion = id;
                cout << "Ingrese nuevo tipo: ";
                cin >> nueva.tipo;
                cout << "Ingrese nuevo monto: ";
                cin >> nueva.monto;
                cout << "Ingrese nueva fecha: ";
                cin >> nueva.fecha;
                cout << "Ingrese nuevo banco: ";
                cin >> nueva.banco;

                archivoTemp << nueva.idTransaccion << "|"
                            << nueva.tipo << "|"
                            << nueva.monto << "|"
                            << nueva.fecha << "|"
                            << nueva.banco << endl;
            } else {
                archivoTemp << linea << endl;
            }
        }
        archivoOriginal.close();
        archivoTemp.close();
        remove("transacciones.txt");
        rename("temp.txt", "transacciones.txt");

        if (encontrado) {
            cout << "Transacci�n editada.\n";
            bitacora b;
            b.insertar("usuario1", "Transacci�n", "Edici�n de transacci�n ID: " + id);
        } else {
            cout << "No se encontr� la transacci�n.\n";
        }
    } else {
        cout << "Error al abrir los archivos.\n";
    }
}

// Eliminar una transacci�n por ID
void transaccion::eliminarTransaccion(string id) {
    ifstream archivoOriginal("transacciones.txt");
    ofstream archivoTemp("temp.txt");
    string linea;
    bool encontrado = false;

    if (archivoOriginal.is_open() && archivoTemp.is_open()) {
        while (getline(archivoOriginal, linea)) {
            if (linea.find(id + "|") == 0) {
                encontrado = true;
                cout << "Transacci�n eliminada: " << linea << endl;
            } else {
                archivoTemp << linea << endl;
            }
        }
        archivoOriginal.close();
        archivoTemp.close();
        remove("transacciones.txt");
        rename("temp.txt", "transacciones.txt");

        if (encontrado) {
            cout << "Transacci�n eliminada correctamente.\n";
            bitacora b;
            b.insertar("usuario1", "Transacci�n", "Eliminaci�n de transacci�n ID: " + id);
        } else {
            cout << "No se encontr� la transacci�n.\n";
        }
    } else {
        cout << "Error al abrir los archivos.\n";
    }
}
