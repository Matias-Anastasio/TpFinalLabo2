#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <dirent.h>
#include <unistd.h>

//STRUCT PARA MANEJAR EL ARCHIVO PRESTAMOS.CSV
typedef struct{
    int orden, dia, mes, anio, nCuotas;
    char nombre[10], apellido[10],tipoCredito[20], sMes[3], sCuota[20], sIva[20], sTotal[20];
    float importe, cuota, iva, total;
}prestamo;

//STRUCT PARA MANEJAR EL ARCHIVO CREDITOS.DAT
typedef struct{
    int orden, dia, mes, anio, nCuotas, activo;
    char nombre[11], apellido[11], tipoCredito[20], sMes[3];
    float importe, cuota, iva, total;
}credito;

//STRUCT DE LA FECHA
typedef struct {
    int dia, mes, anio;
} Fecha;

Fecha obtenerFechaActual();
int validarEntero(char peticion[255]);
int validarFlotante(char peticion[255]);
int validarPalabras(char *palabra);
void imprimirTitulos(char titulos[][30]);
void imprimirRegistro(credito creditos);
float strAFloat(char num[]);
void mesAStr(int mes, char strMes[]);
int mostrarArchivosXYZ();
void crearCarpeta();
int tieneContenido();
