//Samuel Lajara Agullo / Vicente Cancela Perez / Toni Bailen Sevilla
#include <iostream>
#include <stack>
#include <stdlib.h>
#include <string.h>
#include <fstream>
#include <time.h>

using namespace std;

float** obtenerDatos(float **Coste, int &n)
{
    ifstream fDatos;
    string ruta;
    char linea[200], *tok;
    int col1, col2;

    cout << "\nRuta: ";
    cin >> ruta;

    fDatos.open(ruta);
    fDatos.getline(linea,200);
    tok = strtok(linea, " \t\r\n");
    n = atoi(tok);  
    
    Coste = new float* [n+1];

    if(Coste == NULL)
    {
        return NULL;
    }

    for(int i = 0; i <= n; i++)
    {
        Coste[i] = new float [n+1];

        if(Coste[i] == NULL)
        {
            return NULL;
        }
    }

    for(int k = 0; k <= n; k++)
    {
        for(int z = 0; z <= n; z++)
        {
            Coste[k][z] = 99999;
        }
    }

    while (fDatos.peek() != EOF)
    {
        fDatos.getline(linea,200);
        tok = strtok(linea, " \t\r\n");
        col1 = atoi(tok);
        tok = strtok(NULL, " \t\r\n");
        col2 = atoi(tok);
        tok = strtok(NULL, " \t\r\n");
        Coste[col1][col2] = atoi(tok);
    }   
    
    fDatos.close();
    return Coste;
}

bool Ciclos(int *V, int n)
{
    int i;
    int j;
    for (i = 1; i <= n; i++)
    {
        for (j = i+1; j <= n; j++)
        {
            if (V[i] != -1)
            {
                if (V[i] == V[j])
                {
                    return true;
                }  
            }                    
        }       
    }
    return false;
}

float CalcularCoste(float **Coste, int *V, int n)
{
    float suma = 0;
    int i;
    int k = 0;

    for (i = 1; i <= n; i++)
    {
        suma = suma + Coste[k][V[i]];
        k = V[i];

        if (i == n)
        {
            suma = suma + Coste[V[i]][0];
        }
    }
    return suma;
}

float CalcularCoste2(float **Coste, int *V, int n, float costeOptimo)
{
    float suma = 0;
    int i;
    int k = 0;
    for (i = 1; i <= n; i++)
    {
        if (costeOptimo > suma + Coste[k][V[i]])
        {
            suma = suma + Coste[k][V[i]];
            k = V[i];

            if (i == n)
            {
                suma = suma + Coste[V[i]][0];
            }
        }

        else
        {
            return 99999;
        }
    }
    return suma;
}

void Viajante(float **Coste, int *V, int *Voptimo, float &costeOptimo, int k, int n, int &valor, int ocultar)
{
    float costeV;
    V[k] = 0;
    while (V[k] != n)
    {
        V[k] = V[k]+1;
        
        if (Coste[V[k-1]][V[k]] != 99999 && Ciclos(V, n) == false)
        {
            if (k == n)
            {
                if (Coste[V[k]][V[0]] != 99999)
                {
                    costeV = CalcularCoste(Coste, V, n);
                    valor = valor + 1;

                    if (ocultar == 0)
                    {
                        cout << endl << "Recorrido " << valor << ":";
                        cout << "(Super, ";

                        for (int i = 0; i < n; i++)
                        {
                            if (i != n-1)
                            {
                                cout <<V[i+1] << ", ";
                            }

                            else
                            {
                                cout <<V[i+1] << ", ";
                                cout << "Super";
                            }
                        }
                        cout << ")" << endl;
                        cout << "Coste: " << costeV << endl;
                    }

                    if (costeV < costeOptimo)
                    {
                        for (int i = 1; i <= n; i++)
                        {
                            Voptimo[i] = V[i];
                        }
                        costeOptimo = costeV;
                    }
                }
            }

            else
            {
                Viajante(Coste, V, Voptimo, costeOptimo, k+1, n, valor, ocultar);
                V[k+1] = -1;
            }
        }    
    }
}

void Viajante2(float **Coste, int *V, int *Voptimo, float &costeOptimo, int k, int n, int &valor)
{
    float costeV;
    V[k] = 0;
    while (V[k] != n)
    {
        V[k] = V[k]+1;
        
        if (Coste[V[k-1]][V[k]] != 99999 && Ciclos(V, n) == false)
        {
            costeV = CalcularCoste2(Coste, V, k, costeOptimo);

            if (costeV != 99999)
            {
                if (k == n)
                {
                    if (Coste[V[k]][V[0]] != 99999)
                    {
                        valor = valor + 1;

                        if (costeV < costeOptimo)
                        {
                            costeOptimo = costeV;
                            for (int i = 1; i <= n; i++)
                            {
                                Voptimo[i] = V[i];
                            }
                        }
                    }
                }

                else
                {
                    Viajante2(Coste, V, Voptimo, costeOptimo, k+1, n, valor);
                    V[k+1] = -1;
                }
            }

            else
            {
                V[k+1] = -1;
            }            
        }    
    }
}

void mostrarMatriz(float **Coste, int n)
{
    for(int k = 0; k <= n; k++)
    {
        for(int z = 0; z <= n; z++)
        {
            cout<< Coste[k][z];
            cout << "  ";
        }
        cout << endl;
    }
}

int main()
{
    cout << "\nPROYECTO FINAL MPA\n";
    cout << "-------------------------------------------------------\n";
    int valor = 1;
    char num;
    int k = 1;
    float costeOptimo = 99999;
    int ocultar = 0;
    int n;
    float **Coste;
    int *V;
    int *Voptimo;
    int *C;
    float *S;

    clock_t tinicio, tfin;
    double tiempo;

    while (1)
    {
        cout << endl;
        cout << "1.- Obtener lista de clientes" << endl;
        cout << "2.- Ocultar listado de recorridos en opcion 3" << endl;
        cout << "3.- Listado de recorridos resaltando el mas corto" << endl;
        cout << "4.- Recorrido mas corto (algoritmo mejorado)" << endl;
        cout << "5.- Salir" << endl;
        cin >> num;

        switch (num)
        {
            case '1':
                cout << endl << "Obtener lista de clientes" << endl;
                Coste = obtenerDatos(Coste, n);
                cout << "Red de clientes obtenida:" <<endl;
                mostrarMatriz(Coste,n);
            break;

            case '2':
                if (ocultar == 0)
                {
                    ocultar = 1;
                    cout << endl << "Listados ocultados" << endl;
                }

                else
                {
                    ocultar = 0;
                    cout << endl << "Listados mostrandose" << endl;
                }
            break;

            case '3':
                costeOptimo = 99999;
                valor = 0;
                cout << endl << "Listado de recorridos resaltando el mas corto" << endl;
                V = new int [n+1];
                Voptimo = new int [n+1];
                
                for (int i = 0; i <= n; i++)
                {
                    V[i] = i;
                    Voptimo[i] = 0;
                }

                tinicio = clock(); 
                Viajante(Coste, V, Voptimo, costeOptimo, k, n, valor, ocultar);
                tfin = clock();
                tiempo = (double)(tfin-tinicio) / CLOCKS_PER_SEC * 1000;

                cout << endl <<  "Tiempo de ejecucion: " << tiempo << "ms" << endl;
                cout << "Recorridos obtenidos: " << valor << endl;
                cout << "Recorrido mas corto: ";
                cout << "(Super, ";

                for (int i = 0; i < n; i++)
                {
                    if (i != n-1)
                    {
                        cout <<Voptimo[i+1] << ", ";
                    }

                    else
                    {
                        cout <<Voptimo[i+1] << ", ";
                        cout << "Super";
                    }
                }
                cout << ")" << endl;
                cout << "Coste: " << costeOptimo << endl;
            break;

            case '4':
                costeOptimo = 99999;
                valor = 0;
                cout << endl << "Listado de recorridos resaltando el mas corto mejorado" << endl;
                V = new int [n+1];
                Voptimo = new int [n+1];
                
                for (int i = 0; i <= n; i++)
                {
                    V[i] = i;
                    Voptimo[i] = 0;
                }

                tinicio = clock();
                Viajante2(Coste, V, Voptimo, costeOptimo, k, n, valor);
                tfin = clock();
                tiempo = (double)(tfin-tinicio) / CLOCKS_PER_SEC * 1000;

                cout << endl <<  "Tiempo de ejecucion: " << tiempo << "ms" << endl;
                cout << "Recorridos obtenidos: " << valor << endl;
                cout << "Recorrido mas corto: ";
                cout << "(Super, ";
                
                for (int i = 0; i < n; i++)
                {
                    if (i != n-1)
                    {
                        cout <<Voptimo[i+1] << ", ";
                    }

                    else
                    {
                        cout <<Voptimo[i+1] << ", ";
                        cout << "Super";
                    }
                }
                cout << ")" << endl;
                cout << "Coste: " << costeOptimo << endl;
            break;

            case '5':
                return -1;
            break;

            default:
                cout << endl << "Introduce una opcion correcta" << endl;
            break;
        }
    }
}