#include "lib.h"

//FUNCION QUE DEVUELVE LA FECHA ACTUAL
Fecha obtenerFechaActual() {
    Fecha fecha;
    time_t fechaActual = time(NULL);
    struct tm* fechaLocal = localtime(&fechaActual);
    fecha.dia = fechaLocal->tm_mday;
    fecha.mes = fechaLocal->tm_mon + 1;
    fecha.anio = fechaLocal->tm_year + 1900;

    return fecha;
}

//FUNCION PARA VALIDAR QUE NO SE INGRESE UN CARACTER EN LOS NUMEROS ENTEROS
int validarEntero(char peticion[255]){
    int num,valido = 0;
    do {
        printf("%s ",peticion);
        if (scanf("%d", &num) == 1 && num > 0) {
            valido = 1;
        } else {
            printf("\n\tNo se ingreso un numero positivo. Intente nuevamente.\n");
            while (getchar() != '\n');
        }
    } while (!valido);
    return num;
}

//FUNCION PARA VALIDAR QUE NO SE INGRESE UN CARACTER EN LOS NUMEROS FLOTANTES
int validarFlotante(char peticion[255]){
    int valido = 0;
    float num;
    do {
        printf("%s ",peticion);
        if (scanf("%f", &num) == 1 && num > 0) {
            valido = 1;
        } else {
            printf("\n\tNo se ingreso un numero positivo. Intente nuevamente.\n");
            while (getchar() != '\n');
        }
    } while (!valido);
    return num;
}

//FUNCION PARA VALIDAR QUE LAS PALABRAS NO CONTENGAN NUMEROS
int validarPalabras(char *palabra){
    while(*palabra){
        if(isdigit(*palabra)){
            return 1;
        }
        palabra++;
    }
    return 0;
}

//FUNCION PARA IMPRIMIR LOS TITULOS
void imprimirTitulos(char titulos[][30]){
    printf("\n\t%-5s | %-23s | %-10s | %-15s | %-3s | %-3s | %-5s | %-11s | %-13s | %-10s | %-11s\n",titulos[0],titulos[1],titulos[2],titulos[3],titulos[4],titulos[5],titulos[6],titulos[7],titulos[8],titulos[9],titulos[10]);
}

//FUNCION PARA IMPRIMIR UN REGISTRO
void imprimirRegistro(credito creditos){
    printf("\n\t %-4d | %-10s | %-10s | %-10.2f | %-15s | %-3d | %-3s | %-5d | %-11d | %-13.2f | %-10.2f | %-11.2f",creditos.orden,creditos.nombre,creditos.apellido,creditos.importe,creditos.tipoCredito,creditos.dia,creditos.sMes,creditos.anio,creditos.nCuotas,creditos.cuota,creditos.iva,creditos.total);
}

//FUNCION PARA CONVERTIR LOS NUMEROS FLOTANTES DE XXX,XX A XXX.XX
float strAFloat(char num[]){
    char d[]=",";
    char p[]=".";
    float numero;
    char *entero=strtok(num,d);
    char *decimal=strtok(NULL,d);
    if(decimal!=NULL){
        size_t largoTotal = strlen(entero) + strlen(decimal) + strlen(p) + 1;
        char completo[largoTotal];
        strcpy(completo,entero);
        strcat(completo,p);
        strcat(completo,decimal);
        numero=atof(completo);
    }else{
        numero=atof(entero);
    }
    return numero;
}

//FUNCION PARA CONVERTIR LOS MESES DE NUMERO A 3 LETRAS
void mesAStr(int mes, char strMes[]){
    switch(mes){
    case 1:
        strcpy(strMes,"Ene");
        break;
    case 2:
        strcpy(strMes,"Feb");
        break;
    case 3:
        strcpy(strMes,"Mar");
        break;
    case 4:
        strcpy(strMes,"Abr");
        break;
    case 5:
        strcpy(strMes,"May");
        break;
    case 6:
        strcpy(strMes,"Jun");
        break;
    case 7:
        strcpy(strMes,"Jul");
        break;
    case 8:
        strcpy(strMes,"Ago");
        break;
    case 9:
        strcpy(strMes,"Sep");
        break;
    case 10:
        strcpy(strMes,"Oct");
        break;
    case 11:
        strcpy(strMes,"Nov");
        break;
    case 12:
        strcpy(strMes,"Dic");
        break;
    }
}

//FUNCION ENCARGADA DE MOSTRAR LOS ARCHIVOS XYZ DISPONIBLES
int mostrarArchivosXYZ(){
    int cant=0;
    system("cls");
    DIR *dir;
    struct dirent *entrada;
    dir = opendir("bajas");
    if (dir == NULL) {
        printf("No se pudo abrir el directorio.\n");
        return 0;
    }
    printf("\n\tArchivos disponibles para leer(la fecha esta en formato AAAAMMDD):\n");
    while ((entrada = readdir(dir)) != NULL) {
        char *extension = strrchr(entrada->d_name, '.');
        if (extension != NULL && strcmp(extension, ".xyz") == 0) {
            printf("\n\t\t%s\n", entrada->d_name);
            cant+=1;
        }
    }
    if(cant==0){
        printf("\n\tNo se encontraron archivos.\n\n\t");
    }
    closedir(dir);
    return cant;
}

//FUNCION PARA CREAR LA CARPETA BAJAS
void crearCarpeta(){
    char *carpeta="bajas";
    if (access(carpeta, F_OK) == -1) {
        mkdir(carpeta);
    }
}

//FUNCION PARA VER SI TIENE CONTENIDO
int tieneContenido(){
    int pos;
    FILE *creditos;
    creditos=fopen("creditos.dat","rb");
    fseek(creditos,0,SEEK_END);
    pos=ftell(creditos);
    fclose(creditos);
    return pos;
}
