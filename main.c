/*
        TRABAJO PRACTICO LABORATORIO DE COMPUTACIÓN II

        AUTOR: ANASTASIO MATIAS
        FECHA DE ENTREGA: 20/06/2023

        ARCHIVOS QUE SON PARTE DEL PROGRAMA:
            -main.c
            -lib.h
            -lib.c
            -prestamos.csv
            -matiasAnastasio.cbp
            -matiasAnastasio.depend
            -matiasAnastasio.layout
            -creditos.dat (se crea al usar el programa)
            -bajas/clientes_bajas_AAAAMMDD.xyz (se crea al usar el programa)


*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <dirent.h>
#include <unistd.h>
#include "lib.h"

//FUNCION PARA MOSTRAR EL MENU PRINCIPAL
int menuPrincipal(){
    int op, valido=0;
    printf("\n\t%cQu%c acci%cn desea realizar? \n\n",168,130,162);
    printf("\t\t 1: Listar txt: muestra el contenido del archivo prestamos.csv.\n\n");
    printf("\t\t 2: Crear: crear el archivo creditos.dat.\n\n");
    printf("\t\t 3: Migrar: migrar los datos del archivo prestamos.csv a creditos.dat.\n\n");
    printf("\t\t 4: Listar dat: muestra el contenido del archivo creditos.dat.\n\n");
    printf("\t\t 5: Alta: dar de alta un nuevo cliente.\n\n");
    printf("\t\t 6: Buscar: buscar un registro.\n\n");
    printf("\t\t 7: Modificar: modificar un registro.\n\n");
    printf("\t\t 8: Baja Logica: dar de baja a un cliente.\n\n");
    printf("\t\t 9: Baja Fisica: mover los clientes inactivos a un archivo de bajas.\n\n");
    printf("\t\t10: Listar XYZ: mostrar los archivos creados en la opcion anterior.\n\n");
    printf("\t\t 0: Salir\n\n");
    do {
        printf("\tIngrese el n%cmero de la opci%cn: ",163,162);
        if (scanf("%d", &op) == 1 && op >= 0 && op <= 10) {
            valido = 1;
        } else {
            printf("\tNo se ingreso un numero valido. Intente nuevamente.\n");
            while (getchar() != '\n');
        }
    } while (!valido);
    return op;
}

//FUNCION PARA LEER EL ARCHIVO PRESTAMOS.CSV
void leerArchivoCSV(prestamo datos[], char titulos[][30], int * registros){
    FILE * archivoCSV;
    archivoCSV= fopen("prestamos.csv", "r");

    fscanf(archivoCSV,"%29[^;];%29[^;];%29[^;];%29[^;];%29[^;];%29[^;];%29[^;];%29[^;];%29[^;];%29[^;];%29[^\n]",titulos[0],titulos[1],titulos[2],titulos[3],titulos[4],titulos[5],titulos[6],titulos[7],titulos[8],titulos[9],titulos[10]);
    int leer = 0,registro=0, i;
    do{
        leer = fscanf(archivoCSV,"%d;%19[^ ] %19[^;];%f;%19[^;];%d;%d;%d;%d;%19[^;];%19[^;];%19[^\n]",&datos[registro].orden,datos[registro].nombre,datos[registro].apellido,&datos[registro].importe,datos[registro].tipoCredito,&datos[registro].dia,&datos[registro].mes,&datos[registro].anio,&datos[registro].nCuotas,datos[registro].sCuota,datos[registro].sIva,datos[registro].sTotal);
        if(leer==12) registro+=1;
        if(leer!=12 && !feof(archivoCSV)){
            printf("Formato del archivo incorrecto!");
        }
    }while(!feof(archivoCSV));

    fclose(archivoCSV);
    *registros=registro;
    for(i=0;i<registro;i+=1){
        datos[i].cuota=strAFloat(datos[i].sCuota);
        datos[i].iva=strAFloat(datos[i].sIva);
        datos[i].total=strAFloat(datos[i].sTotal);
    }
}

//FUNCION PARA LISTAR EL ARCHIVO PRESTAMOS.CSV
void listarTxt (){
    int i, cantRegistros;
    char titulos[11][30];
    prestamo prestamos[100];
    leerArchivoCSV(prestamos,titulos,&cantRegistros);
    printf("\n\t%-5s | %-19s | %-7s | %-15s | %-3s | %-3s | %-5s | %-11s | %-13s | %-7s | %-11s\n",titulos[0],titulos[1],titulos[2],titulos[3],titulos[4],titulos[5],titulos[6],titulos[7],titulos[8],titulos[9],titulos[10]);
    for(i=0;i<cantRegistros;i+=1){
        printf("\t %-4d | %-9s %-9s | %-7.2f | %-15s | %-3d | %-3d | %-5d | %-11d | %-13.2f | %-7.2f | %-11.2f\n",prestamos[i].orden,prestamos[i].nombre,prestamos[i].apellido,prestamos[i].importe,prestamos[i].tipoCredito,prestamos[i].dia,prestamos[i].mes,prestamos[i].anio,prestamos[i].nCuotas, prestamos[i].cuota,prestamos[i].iva,prestamos[i].total);
    }
    printf("\n\t");
}

//FUNCION PARA CREAR EL ARCHIVO BINARIO CREDITOS.DAT
void crearBinario(){

    char sobrescribir='s';
    system("cls");
    FILE *creditos;
    if(creditos=fopen("creditos.dat","rb")){
        fclose(creditos);
        printf("\n\tEl archivo ya existe, desea sobrescribirlo?(S/N): ");
        fflush(stdin);
        sobrescribir=getchar();
        while(sobrescribir!='N' && sobrescribir!='n' && sobrescribir!='S' && sobrescribir!='s'){
            printf("\nPor favor ingrese un caracter valido: ");
            fflush(stdin);
            sobrescribir=getchar();
        }
        printf("\n\n\t");
    }
    if(sobrescribir=='s' || sobrescribir=='S'){
        creditos=fopen("creditos.dat","wb");
        fclose(creditos);
        printf("\n\tArchivo creado\n\n\t");
    }

}

//FUNCION PARA MIGRAR LOS DATOS DE PRESTAMOS.CSV A CREDITOS.DAT
void migrarDatos(){
    int i, cantRegistros;
    char titulos[11][30];
    prestamo prestamos[100];
    leerArchivoCSV(prestamos,titulos,&cantRegistros);
    credito creditos[cantRegistros];
    strcpy(titulos[1],"Nombre     | Apellido ");
    for(i=0;i<cantRegistros;i+=1){
        strcpy(creditos[i].nombre,prestamos[i].nombre);
        strcpy(creditos[i].apellido,prestamos[i].apellido);
        strupr(creditos[i].apellido);
        strcpy(creditos[i].tipoCredito,prestamos[i].tipoCredito);
        strupr(creditos[i].tipoCredito);
        mesAStr(prestamos[i].mes,creditos[i].sMes);
        creditos[i].importe = prestamos[i].importe;
        creditos[i].dia = prestamos[i].dia;
        creditos[i].mes = prestamos[i].mes;
        creditos[i].anio = prestamos[i].anio;
        creditos[i].nCuotas = prestamos[i].nCuotas;
        creditos[i].orden = prestamos[i].orden;
        creditos[i].activo=1;
        creditos[i].cuota=prestamos[i].cuota;
        creditos[i].iva=prestamos[i].iva;
        creditos[i].total=prestamos[i].total;
    }

    FILE *creditosArchivo;
    creditosArchivo= fopen("creditos.dat","wb");
    fwrite(titulos,sizeof(char)*30,11,creditosArchivo);
    fwrite(creditos,sizeof(credito),cantRegistros,creditosArchivo);
    fclose(creditosArchivo);
    printf("\n\tDatos Migrados.\n\n\t");
}

//FUNCION PARA LISTAR TODO
void listarTodo(){
    system("cls");
    char titulos[11][30];
    credito creditos;
    FILE *creditosArchivo;
    creditosArchivo= fopen("creditos.dat","rb");
    fread(titulos,sizeof(char)*30,11,creditosArchivo);
    imprimirTitulos(titulos);
    fread(&creditos,sizeof(credito),1,creditosArchivo);
    while(!feof(creditosArchivo)){
        imprimirRegistro(creditos);
        fread(&creditos,sizeof(credito),1,creditosArchivo);
    }
    printf("\n\n\t");
}

//FUNCION PARA LISTAR ACTIVOS
void listarActivos(){
    system("cls");
    char titulos[11][30];
    credito creditos;
    FILE *creditosArchivo;
    creditosArchivo= fopen("creditos.dat","rb");
    fread(titulos,sizeof(char)*30,11,creditosArchivo);
    imprimirTitulos(titulos);
    fread(&creditos,sizeof(credito),1,creditosArchivo);
    while(!feof(creditosArchivo)){
        if(creditos.activo==1){
            imprimirRegistro(creditos);
        }
        fread(&creditos,sizeof(credito),1,creditosArchivo);
    }
    printf("\n\n\t");
}

//FUNCION PARA LISTAR POR TIPO DE CREDITO
void listarCredito(){
    system("cls");
    int opcion, valido=0;
        system("cls");
        printf("\n\tSeleccione una opcion");
        printf("\n\n\t\t1: A sola firma.");
        printf("\n\n\t\t2: Con garantia.");
        do {
        printf("\n\n\tIngrese el n%cmero de la opci%cn: ",163,162);
        if (scanf("%d", &opcion) == 1 && opcion >= 1 && opcion <= 2) {
            valido = 1;
        } else {
            printf("\tNo se ingreso un numero valido. Intente nuevamente.\n");
            while (getchar() != '\n');
        }
        } while (!valido);
    system("cls");
    char titulos[11][30];
    credito creditos;
    FILE *creditosArchivo;
    creditosArchivo= fopen("creditos.dat","rb");
    fread(titulos,sizeof(char)*30,11,creditosArchivo);
    imprimirTitulos(titulos);
    fread(&creditos,sizeof(credito),1,creditosArchivo);
    while(!feof(creditosArchivo)){
        if(opcion==1){
            if(strcmp(creditos.tipoCredito,"A SOLA FIRMA")==0){
                imprimirRegistro(creditos);
            }
        }else{
            if(strcmp(creditos.tipoCredito,"CON GARANTIA")==0){
                imprimirRegistro(creditos);
            }
        }
        fread(&creditos,sizeof(credito),1,creditosArchivo);
    }
    printf("\n\n\t");
}

//FUNCION PARA LISTAR POR RANGOS DE FECHAS
void listarFecha(){
    system("cls");
    int diaI=1,mesI=1,anioI=2000,diaF=31,mesF=12,anioF=2023,maximo,mesMinimo=1,diaMinimo=1;
    printf("\n\tIngrese desde que fecha quiere buscar:\n");
    do{
        if(anioI<2000 || anioI>2023){
            printf("\n\n\tIngrese un anio correcto\n");
        }
        anioI=validarEntero("\n\tAnio:");
        //printf("\n\tAnio:");
        //scanf("%d",&anioI);
    }while(anioI<2000 || anioI>2023);
    do{
        if(mesI<1 || mesI>12){
            printf("\n\n\tIngrese un mes correcto\n");
        }
        mesI=validarEntero("\n\tMes:");
    }while(mesI<1 || mesI>12);
    if(mesI==4 || mesI==6 || mesI==9 || mesI==11){
        maximo=30;
    }else if(mesI==2){
        maximo=28;
    }else{
        maximo=31;
    }
    do{
        if(diaI<1 || diaI>maximo){
            printf("\n\n\tIngrese un dia correcto\n");
        }
        diaI=validarEntero("\n\tDia:");
    }while(diaI<1 || diaI>maximo);
    system("cls");
    printf("\n\tFecha inicio: %d/%d/%d",diaI,mesI,anioI);
    printf("\n\n\tIngrese hasta que fecha quiere buscar:\n");
    do{
        if(anioF<anioI || anioF>2023){
            printf("\n\n\tIngrese un anio correcto\n");
        }
        anioF=validarEntero("\n\tAnio:");
    }while(anioF<anioI || anioF>2023);

    if(anioF==anioI){
        mesMinimo=mesI;
    }

    do{
        if(mesF<mesMinimo || mesF>12){
            printf("\n\n\tIngrese un mes correcto\n");
        }
        mesF=validarEntero("\n\tMes:");
    }while(mesF<mesMinimo || mesF>12);

    if(mesF==4 || mesF==6 || mesF==9 || mesF==11){
        maximo=30;
    }else if(mesF==2){
        maximo=28;
    }else{
        maximo=31;
    }
    diaF=diaI;
    if(anioI==anioF && mesI==mesF){
        diaMinimo=diaI;
        diaF=diaI;
    }
    do{
        if(diaF<diaMinimo || diaF>maximo){
            printf("\n\n\tIngrese un dia correcto\n");
        }
        diaF=validarEntero("\n\tDia:");
    }while(diaF<diaMinimo || diaF>maximo);

    system("cls");
    printf("\n\tDesde: %d/%d/%d, hasta: %d/%d/%d\n\n", diaI,mesI,anioI,diaF,mesF,anioF);

    int fechaInicio,fechaFinal, fecha;
    fechaInicio= anioI*10000+mesI*100+diaI;
    fechaFinal= anioF*10000+mesF*100+diaF;

    char titulos[11][30];
    credito creditos;
    FILE *creditosArchivo;
    creditosArchivo= fopen("creditos.dat","rb");
    fread(titulos,sizeof(char)*30,11,creditosArchivo);
    imprimirTitulos(titulos);
    fread(&creditos,sizeof(credito),1,creditosArchivo);
    while(!feof(creditosArchivo)){
        fecha=creditos.anio*10000+creditos.mes*100+creditos.dia;
        if(fecha>=fechaInicio && fecha<=fechaFinal){
            imprimirRegistro(creditos);
        }
        fread(&creditos,sizeof(credito),1,creditosArchivo);
    }
    printf("\n\n\t");


}

//FUNCION PARA LISTAR EL ARCHIVO CREDITOS.DAT
void listarDat(){
    FILE *creditos;
    if(creditos=fopen("creditos.dat","rb")){
        fclose(creditos);
        if(tieneContenido()>0){
            int opcion,valido=0;
            system("cls");
            printf("\n\tCual de las siguientes opciones desea listar?");
            printf("\n\n\t\t1: Listar todo.");
            printf("\n\n\t\t2: Listar solo activos.");
            printf("\n\n\t\t3: Listar por tipo de credito.");
            printf("\n\n\t\t4: Listar por rango de tiempo.");
            printf("\n\n\t\t0: Volver.");
            do {
                printf("\n\n\tIngrese el n%cmero de la opci%cn: ",163,162);
                if (scanf("%d", &opcion) == 1 && opcion >= 0 && opcion <= 4) {
                    valido = 1;
                } else {
                    printf("\tNo se ingreso un numero valido. Intente nuevamente.\n");
                    while (getchar() != '\n');
                }
            } while (!valido);

            switch(opcion){
            case 1:
                listarTodo();
                break;
            case 2:
                listarActivos();
                break;
            case 3:
                listarCredito();
                break;
            case 4:
                listarFecha();
                break;
            }
        }else{
            printf("\n\tEl archivo creditos.dat esta vacio, por favor primero migre los datos.\n\n\t");
        }
    }else{
        printf("\n\tNo existe el archivo creditos.dat\n\n\t");
    }

}

//FUNCION PARA INGRESAR LOS DATOS QUE SE GUARDARAN EN LA FUNCION DE ALTA
void cargarRegistro(int orden,credito *creditos){
    //NUMERO DE ORDEN
        creditos->orden=orden;
        //ACTIVO
        creditos->activo=1;
        //NOMBRE
        int valido=0;
        do{
            printf("\n\tIngrese su nombre: ");
            fflush(stdin);
            fgets(creditos->nombre,sizeof(creditos->nombre),stdin);
            valido=validarPalabras(creditos->nombre);
            if(valido){
                printf("\n\tSu nombre no puede contener numeros.");
            }
        }while(valido);
        creditos->nombre[strcspn(creditos->nombre, "\n")] = '\0';
        //APELLIDO
        valido=0;
        do{
            printf("\n\tIngrese su apellido: ");
            fflush(stdin);
            fgets(creditos->apellido,sizeof(creditos->apellido),stdin);
            valido=validarPalabras(creditos->apellido);
            if(valido){
                printf("\n\tSu apellido no puede contener numeros.");
            }
        }while(valido);
        creditos->apellido[strcspn(creditos->apellido, "\n")] = '\0';
        strupr(creditos->apellido);
        //TIPO DE CREDITO
        int opcion;
        valido=0;
        printf("\n\tQue tipo de credito es?");
        printf("\n\n\t\t1: A sola firma.");
        printf("\n\n\t\t2: Con garantia.");
        do {
        printf("\n\n\tIngrese el n%cmero de la opci%cn: ",163,162);
        if (scanf("%d", &opcion) == 1 && opcion >= 1 && opcion <= 2) {
            valido = 1;
        } else {
            printf("\tNo se ingreso un numero valido. Intente nuevamente.\n");
            while (getchar() != '\n');
        }
        } while (!valido);
        if(opcion==1){
            strcpy(creditos->tipoCredito,"A SOLA FIRMA");
        }else{
            strcpy(creditos->tipoCredito,"CON GARANTIA");
        }

        //FECHA
        Fecha fechaMinima,fechaMaxima=obtenerFechaActual();
        fechaMinima.dia=21;
        fechaMinima.mes=6;
        fechaMinima.anio=2022;
        int diaMax,diaMin=1,mesMax=12,mesMin=1,dia,mes,anio;
        do{
            anio=validarEntero("\n\tIngrese el anio:");
            if(anio<fechaMinima.anio || anio>fechaMaxima.anio){
                printf("\n\tIngrese un anio correcto");
            }
        }while(anio<fechaMinima.anio || anio>fechaMaxima.anio);
        if(anio==fechaMinima.anio){
            mesMin=fechaMinima.mes;
        }else if(anio==fechaMaxima.anio){
            mesMax=fechaMaxima.mes;
        }
        do{
            mes=validarEntero("\n\tIngrese el mes:");
            if(mes<mesMin || mes>mesMax){
                printf("\n\tIngrese un mes correcto");
            }
        }while(mes<mesMin || mes>mesMax);
        if(mes==4 || mes==6 || mes==9 || mes==11){
            diaMax=30;
        }else if(mes==2){
            diaMax=28;
        }else{
            diaMax=31;
        }
        if(anio==fechaMinima.anio && mes==fechaMinima.mes){
            diaMin=fechaMinima.dia;
        }else if(anio==fechaMaxima.anio && mes==fechaMaxima.mes){
            diaMax=fechaMaxima.dia;
        }
        do{
            dia=validarEntero("\n\tIngrese el dia:");
            if(dia<diaMin || dia>diaMax){
                printf("\n\tIngrese un dia correcto");
            }
        }while(dia<diaMin || dia>diaMax);
        creditos->dia=dia;
        creditos->mes=mes;
        creditos->anio=anio;
        mesAStr(creditos->mes,creditos->sMes);

        //IMPORTE
        creditos->importe=validarFlotante("\n\tIngrese el importe: ");
        //NUMERO DE CUOTAS
        creditos->nCuotas=validarEntero("\n\tIngrese la cantidad de cuotas: ");
        //IMPORTE CUOTA
        creditos->cuota=creditos->importe/creditos->nCuotas;
        //IVA
        creditos->iva=creditos->cuota*0.21;
        //TOTAL CUOTA
        creditos->total=creditos->cuota*1.21;
}

//FUNCION PARA DAR DE ALTA NUEVOS CLIENTES
void alta(){
    FILE *creditos;
    if(creditos=fopen("creditos.dat","rb")){
        fclose(creditos);
        if(tieneContenido()>0){
            system("cls");
            int i,orden = validarEntero("\n\tIngrese el numero de orden para el nuevo registro: "), cantRegistros=0, usado,pos,posInicial;
            char titulos[11][30];
            credito creditos;
            FILE *archivoCreditos;
            archivoCreditos=fopen("creditos.dat","r+b");
            fread(titulos,sizeof(char)*30,11,archivoCreditos);
            posInicial=ftell(archivoCreditos);
            do{
                //VER SI EL NUMERO DE ORDEN ESTA USADO
                usado=0;
                fread(&creditos,sizeof(credito),1,archivoCreditos);
                while(!feof(archivoCreditos)){
                    if(creditos.orden==orden){
                        usado=1;
                        break;
                    }
                    cantRegistros+=1;
                    fread(&creditos,sizeof(credito),1,archivoCreditos);
                }
                if(usado){
                    printf("\n\tEl numero de orden: %d se encuentra en uso, por favor elija otro numero.\n",orden);
                    orden= validarEntero("\n\tIngrese el numero de orden para el nuevo registro: ");
                }
            }while(usado);
            system("cls");
            if(orden>cantRegistros){
            //LLENAR ESPACIOS EN BLANCO
                strcpy(creditos.nombre,"");
                strcpy(creditos.apellido,"");
                strcpy(creditos.tipoCredito,"");
                strcpy(creditos.sMes,"");
                creditos.importe = 0;
                creditos.dia = 0;
                creditos.mes = 0;
                creditos.anio = 0;
                creditos.nCuotas = 0;
                creditos.orden = 0;
                creditos.activo=0;
                creditos.cuota=0;
                creditos.iva=0;
                creditos.total=0;
                fseek(archivoCreditos,0,SEEK_END);
                for(i=1;i<orden-cantRegistros;i+=1){
                    fwrite(&creditos,sizeof(credito),1,archivoCreditos);
                }
                cargarRegistro(orden,&creditos);
                //GUARDAR EN EL ARCHIVO
                fseek(archivoCreditos,0,SEEK_END);
                fwrite(&creditos,sizeof(credito),1,archivoCreditos);
            }else{
                cargarRegistro(orden,&creditos);
                pos=posInicial+sizeof(credito)*(orden-1);
                fseek(archivoCreditos,pos,SEEK_SET);
                fwrite(&creditos,sizeof(credito),1,archivoCreditos);
            }

            fclose(archivoCreditos);
        }else{
            printf("\n\tEl archivo creditos.dat esta vacio, por favor primero migre los datos.\n\n\t");
        }
    }else{
        printf("\n\tNo existe el archivo creditos.dat\n\n\t");
    }
}

//FUNCION PARA BUSCAR UN REGISTRO POR NUMERO DE ORDEN O POR APELLIDO
int buscar(int orden, char apellido[], credito *creditos,char titulos[][30]){
    int posicion=0;
    FILE *archivoCreditos;
    archivoCreditos = fopen("creditos.dat","rb");
    fread(titulos,sizeof(char)*30,11,archivoCreditos);
    fread(creditos,sizeof(credito),1,archivoCreditos);
    while(!feof(archivoCreditos)){
        if(creditos->orden==orden || strcmp(creditos->apellido,apellido)==0){
            posicion=ftell(archivoCreditos)-sizeof(credito);
            break;
        }
        fread(creditos,sizeof(credito),1,archivoCreditos);
    }
    fclose(archivoCreditos);
    return posicion;
}

//FUNCION PARA MOSTRAR EL MENU DE BUSQUEDA
void buscarMenu(){
    FILE *creditos;
    if(creditos=fopen("creditos.dat","rb")){
        fclose(creditos);
        if(tieneContenido()>0){
            int opcion, orden=999, valido=0;
            int encontrado;
            credito creditos;
            char titulos[11][30], apellido[11]="xxxxxxxxxxx";
            printf("\n\tComo desea buscar?");
            printf("\n\n\t\t1: Por numero de orden.");
            printf("\n\n\t\t2: Por apellido\n");
            do{
                opcion= validarEntero("\n\tIngrese el numero de la opcion: ");
                if(opcion!=1 && opcion!=2) printf("\n\tIngrese una opcion valida.\n");
            }while(opcion!=1 && opcion!=2);
            if(opcion==1){
                orden = validarEntero("\n\tIngrese el numero de orden: ");
            }else{
                do{
                    printf("\n\tIngrese el apellido: ");
                    fflush(stdin);
                    fgets(apellido,sizeof(apellido),stdin);
                    valido=validarPalabras(apellido);
                    if(valido){
                        printf("\n\tEl apellido no puede contener numeros.\n");
                    }
                }while(valido);
                apellido[strcspn(apellido, "\n")] = '\0';
                strupr(apellido);
            }
            encontrado = buscar(orden,apellido,&creditos,titulos);
            if(encontrado!=0){
                imprimirTitulos(titulos);
                imprimirRegistro(creditos);
                printf("\n\n\t");
            }else{
                printf("\n\tEl registro solicitado no existe.\n\n\t");
            }
        }else{
                printf("\n\tEl archivo creditos.dat esta vacio, por favor primero migre los datos.\n\n\t");
            }
    }else{
        printf("\n\tNo existe el archivo creditos.dat\n\n\t");
    }
}

//FUNCION PARA MODIFICAR EL ARCHIVO CREDITOS.DAT
void actualizarArchivoDat(credito creditos,int posicion){
    FILE *archivoCreditos;
    archivoCreditos= fopen("creditos.dat","r+b");
    fseek(archivoCreditos,posicion,SEEK_SET);
    fwrite(&creditos,sizeof(credito),1,archivoCreditos);
    fclose(archivoCreditos);
}

//FUNCION PARA MODIFICAR TIPO DE CREDITO
void modificarTipoCredito(char tipoCredito[]){
    int opcion,valido=0;
        printf("\n\tQue tipo de credito es?");
        printf("\n\n\t\t1: A sola firma.");
        printf("\n\n\t\t2: Con garantia.");
        do {
        printf("\n\n\tIngrese el n%cmero de la opci%cn: ",163,162);
        if (scanf("%d", &opcion) == 1 && opcion >= 1 && opcion <= 2) {
            valido = 1;
        } else {
            printf("\tNo se ingreso un numero valido. Intente nuevamente.\n");
            while (getchar() != '\n');
        }
        } while (!valido);
        if(opcion==1){
            strcpy(tipoCredito,"A SOLA FIRMA");
        }else{
            strcpy(tipoCredito,"CON GARANTIA");
        }
}

//FUNCION PARA MODIFICAR IMPORTE
void modificarImporte(credito *creditos){
    //IMPORTE
    creditos->importe=validarFlotante("\n\tIngrese el importe: ");
    //NUMERO DE CUOTAS
    creditos->nCuotas=validarEntero("\n\tIngrese la cantidad de cuotas: ");
    //IMPORTE CUOTA
    creditos->cuota=creditos->importe/creditos->nCuotas;
    //IVA
    creditos->iva=creditos->cuota*0.21;
    //TOTAL CUOTA
    creditos->total=creditos->cuota*1.21;
}

//FUNCION PARA MOSTRAR EL MENU DE MODIFICACION
void modificarMenu(){
    FILE *creditos;
    if(creditos=fopen("creditos.dat","rb")){
        fclose(creditos);
        if(tieneContenido()>0){
            int orden,posicion;
            char titulos[11][30],confirmacion='N';
            credito creditos;
            orden = validarEntero("\n\tIngrese el numero de orden: ");
            posicion=buscar(orden,"-",&creditos,titulos);
            if(posicion!=0){
                imprimirTitulos(titulos);
                imprimirRegistro(creditos);
                printf("\n\n\t");
                do{
                    printf("\n\tDesea modificar el siguiente registro?(S/N): ");
                    fflush(stdin);
                    confirmacion=getchar();
                    confirmacion=toupper(confirmacion);
                    if(confirmacion!='N' && confirmacion!='S') printf("\n\tIngrese un S o N.\n");
                }while(confirmacion!='N' && confirmacion!='S');
                if(confirmacion=='S'){
                    while(confirmacion=='S'){
                        system("cls");
                        imprimirTitulos(titulos);
                        imprimirRegistro(creditos);
                        int opcion;
                        printf("\n\n\tQue desea modificar?");
                        printf("\n\n\t\t1: Tipo de credito.");
                        printf("\n\n\t\t2: Importe.");
                        do{
                            opcion=validarEntero("\n\n\tIngrese el numero de la opcion:");
                            if(opcion!=1 && opcion!=2) printf("\n\tSeleccione una opcion valida.");
                        }while(opcion!=1 && opcion!=2);
                        if(opcion==1){
                            modificarTipoCredito(creditos.tipoCredito);
                            printf("%s",creditos.tipoCredito);
                        }else{
                            modificarImporte(&creditos);
                        }
                        system("cls");
                        imprimirTitulos(titulos);
                        imprimirRegistro(creditos);
                        do{
                            printf("\n\n\tDesea modificar algo mas?(S/N): ");
                            fflush(stdin);
                            confirmacion=getchar();
                            confirmacion=toupper(confirmacion);
                            if(confirmacion!='N' && confirmacion!='S') printf("\n\tIngrese un S o N.\n");
                        }while(confirmacion!='N' && confirmacion!='S');
                    }
                    actualizarArchivoDat(creditos,posicion);
                    system("cls");
                    printf("\n\tRegistro modificado con exito.\n\n\t");
                }
                printf("\n\t");
            }else{
                printf("\n\tEl registro solicitado no existe.\n\n\t");
            }
        }else{
                printf("\n\tEl archivo creditos.dat esta vacio, por favor primero migre los datos.\n\n\t");
            }
    }else{
        printf("\n\tNo existe el archivo creditos.dat\n\n\t");
    }
}

//FUNCION QUE REEMPLAZA EL CAMPO ACTIVO DEL REGISTRO POR -1
void bajaLogica(){
    FILE *creditos;
    if(creditos=fopen("creditos.dat","rb")){
        fclose(creditos);
        if(tieneContenido()>0){
            int orden,posicion;
            char titulos[11][30],confirmacion='N';
            credito creditos;
            do{
            orden = validarEntero("\n\tIngrese el numero de orden: ");
            posicion=buscar(orden,"-",&creditos,titulos);
            if(posicion!=0){
                imprimirTitulos(titulos);
                imprimirRegistro(creditos);
                printf("\n\n\t");
                do{
                    printf("\n\tDesea dar de baja el siguiente registro?(S/N): ");
                    fflush(stdin);
                    confirmacion=getchar();
                    confirmacion=toupper(confirmacion);
                    if(confirmacion!='N' && confirmacion!='S') printf("\n\tIngrese un S o N.\n");
                }while(confirmacion!='N' && confirmacion!='S');
                if(confirmacion=='S'){
                    creditos.activo=-1;
                    actualizarArchivoDat(creditos,posicion);
                    listarActivos();
                }
                printf("\n\t");
            }else{
                printf("\n\tEl registro solicitado no existe.\n\n\t");
            }
            }while(posicion==0);
        }else{
            printf("\n\tEl archivo creditos.dat esta vacio, por favor primero migre los datos.\n\n\t");
        }
    }else{
        printf("\n\tNo existe el archivo creditos.dat\n\n\t");
    }
}

//FUNCION QUE GUARDA LOS REGISTROS CON EL CAMPO ACTIVO=-1 EN UN ARCHIVO XYZ
void bajaFisica(){
    FILE *creditos;
    if(creditos=fopen("creditos.dat","rb")){
        fclose(creditos);
        int cantidadBajas=0;
        char titulos[11][30],fechaStr[9],nombreArchivo[34]="bajas/clientes_bajas_";
        credito creditos, creditosVacio;
        int posicion, existe=0;
        crearCarpeta();
        //CREO EL NOMBRE DEL ARCHIVO EN BASE A HOY
        Fecha fechaActual=obtenerFechaActual();
        int fechaCat;
        fechaCat= fechaActual.anio*10000+fechaActual.mes*100+fechaActual.dia;
        sprintf(fechaStr,"%d",fechaCat);
        strcat(nombreArchivo,fechaStr);
        strcat(nombreArchivo,".xyz");
        //CREO UN STRUCT VACIO
        strcpy(creditosVacio.nombre,"");
        strcpy(creditosVacio.apellido,"");
        strcpy(creditosVacio.tipoCredito,"");
        strcpy(creditosVacio.sMes,"");
        creditosVacio.importe = 0;
        creditosVacio.dia = 0;
        creditosVacio.mes = 0;
        creditosVacio.anio = 0;
        creditosVacio.nCuotas = 0;
        creditosVacio.orden = 0;
        creditosVacio.activo=0;
        creditosVacio.cuota=0;
        creditosVacio.iva=0;
        creditosVacio.total=0;

        FILE *archivoCreditos, *archivoBaja;
        archivoCreditos= fopen("creditos.dat","r+b");
        archivoBaja= fopen(nombreArchivo,"r");
        fread(titulos,sizeof(char)*30,11,archivoCreditos);
        //Verifico si existe un archivo de este dia para no sobrescribirlo
        if(archivoBaja!=NULL){
            existe=1;
            fclose(archivoBaja);
        }

        fread(&creditos,sizeof(credito),1,archivoCreditos);
        while(!feof(archivoCreditos)){
            if(creditos.activo==-1){
                if(existe){
                    archivoBaja=fopen(nombreArchivo,"a");
                }else{
                    archivoBaja=fopen(nombreArchivo,"w");
                    fprintf(archivoBaja,"\n\t%-5s | %-23s | %-10s | %-15s | %-3s | %-3s | %-5s | %-11s | %-13s | %-10s | %-11s\n\n",titulos[0],titulos[1],titulos[2],titulos[3],titulos[4],titulos[5],titulos[6],titulos[7],titulos[8],titulos[9],titulos[10]);
                }
                cantidadBajas+=1;
                posicion=ftell(archivoCreditos)-sizeof(credito);
                fprintf(archivoBaja,"\t %-4d | %-10s | %-10s | %-10.2f | %-15s | %-3d | %-3s | %-5d | %-11d | %-13.2f | %-10.2f | %-11.2f\n",creditos.orden,creditos.nombre,creditos.apellido,creditos.importe,creditos.tipoCredito,creditos.dia,creditos.sMes,creditos.anio,creditos.nCuotas,creditos.cuota,creditos.iva,creditos.total);
                fseek(archivoCreditos,posicion,SEEK_SET);
                fwrite(&creditosVacio,sizeof(credito),1,archivoCreditos);
                fseek(archivoCreditos,posicion+sizeof(credito),SEEK_SET);
                fclose(archivoBaja);
            }
            fread(&creditos,sizeof(credito),1,archivoCreditos);
        }
        fclose(archivoCreditos);
        if(cantidadBajas>0){
            printf("\n\tBaja realizada.\n\n\t");
        }else{
            printf("\n\tNo hay registros para dar de baja.\n\n\t");
        }

    }else{
        printf("\n\tNo existe el archivo creditos.dat\n\n\t");
    }
}

//FUNCION PARA MOSTRAR EL ARCHIVO XYZ
void listarXYZ(){
    crearCarpeta();
    int cantidadArchivos=mostrarArchivosXYZ();
    if(cantidadArchivos>0){

        int existe=0;
        do{

            Fecha fechaMinima,fechaMaxima=obtenerFechaActual();
            fechaMinima.dia=21;
            fechaMinima.mes=6;
            fechaMinima.anio=2022;
            int diaMax,diaMin=1,mesMax=12,mesMin=1,dia,mes,anio;
            do{
                anio=validarEntero("\n\tIngrese el anio:");
                if(anio<fechaMinima.anio || anio>fechaMaxima.anio){
                    printf("\n\tIngrese un anio correcto");
                }
            }while(anio<fechaMinima.anio || anio>fechaMaxima.anio);
            if(anio==fechaMinima.anio){
                mesMin=fechaMinima.mes;
            }else if(anio==fechaMaxima.anio){
                mesMax=fechaMaxima.mes;
            }
            do{
                mes=validarEntero("\n\tIngrese el mes:");
                if(mes<mesMin || mes>mesMax){
                    printf("\n\tIngrese un mes correcto");
                }
            }while(mes<mesMin || mes>mesMax);
            if(mes==4 || mes==6 || mes==9 || mes==11){
                diaMax=30;
            }else if(mes==2){
                diaMax=28;
            }else{
                diaMax=31;
            }
            if(anio==fechaMinima.anio && mes==fechaMinima.mes){
                diaMin=fechaMinima.dia;
            }else if(anio==fechaMaxima.anio && mes==fechaMaxima.mes){
                diaMax=fechaMaxima.dia;
            }
            do{
                dia=validarEntero("\n\tIngrese el dia:");
                if(dia<diaMin || dia>diaMax){
                    printf("\n\tIngrese un dia correcto");
                }
            }while(dia<diaMin || dia>diaMax);

            char letra, fechaStr[9],nombreArchivo[34]="bajas/clientes_bajas_";
            int fechaCat;
            fechaCat= anio*10000+mes*100+dia;
            sprintf(fechaStr,"%d",fechaCat);
            strcat(nombreArchivo,fechaStr);
            strcat(nombreArchivo,".xyz");

            FILE *archivoBaja;
            archivoBaja=fopen(nombreArchivo,"r");
            if(archivoBaja!=NULL){
                    existe=1;
                    system("cls");
                    while(!feof(archivoBaja)){
                        letra=fgetc(archivoBaja);
                        printf("%c",letra);
                    }
                        printf("\n");
                    fclose(archivoBaja);
            }else{
                printf("\n\tNo existe un archivo creado en esa fecha");
            }

        }while(!existe);
    }
}

int main()
{
    int opcion=menuPrincipal();
    while(opcion!=0){
        system("cls");
        switch(opcion){
        case 1:
            listarTxt();
            system("pause");
            system("cls");
            break;
        case 2:
            crearBinario();
            system("pause");
            system("cls");
            break;
        case 3:
            migrarDatos();
            system("pause");
            system("cls");
            break;
        case 4:
            listarDat();
            system("pause");
            system("cls");
            break;
        case 5:
            alta();
            system("pause");
            system("cls");
            break;
        case 6:
            buscarMenu();
            system("pause");
            system("cls");
            break;
        case 7:
            modificarMenu();
            system("pause");
            system("cls");
            break;
        case 8:
            bajaLogica();
            system("pause");
            system("cls");
            break;
        case 9:
            bajaFisica();
            system("pause");
            system("cls");
            break;
        case 10:
            listarXYZ();
            system("pause");
            system("cls");
            break;
        }
        opcion=menuPrincipal();
    }
    return 0;
}
