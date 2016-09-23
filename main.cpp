#include <iostream>
#include "preparaiop.h"


using namespace std;

enum Colores{cmdNegro,cmdAzul,cmdVerde,cmdTurquesa,cmdRojo,cmdVioleta,cmdVerdeClaro,cmdGrisClaro,
cmdGris,cmdAzulFluo,cmdVerdeFluo,cmdTurquesaFluo,cmdRojoFluo,cmdVioletaFluo,cmdAmarillo,cmdBlanco}color;

void setColor(int TextColor,int BackgroundColor)
{
    /*
0 : Noir
1 : Bleu foncé
2 : Vert foncé
3 : Turquoise
4 : Rouge foncé
5 : Violet
6 : Vert caca d’oie
7 : Gris clair
8 : Gris foncé
9 : Bleu fluo
A : Vert fluo
B : Turquoise
C : Rouge fluo
D : Violet 2
E : Jaune
F : Blanc
    */
    HANDLE H=GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(H,BackgroundColor*16+TextColor);
}

string preguntaDir(string question){
    setColor(cmdBlanco,cmdNegro);
    //Preguntamos al usuario y recogemos su respuesta
    string response;
    cout << "* " << question << endl;
    setColor(cmdVerdeFluo,cmdNegro);

    getline (cin, response);
    //Reemplazamos comillas dobles si existen
    response = Constant::replaceAll(response, "\"","");
    Dirutil dir;
    //Obtenemos el nombre corto del fichero para eliminar espacios problematicos
    string shortFileName = dir.GetShortUtilName(response);

    if (!shortFileName.empty()){
        response = shortFileName;
    }

    if (!dir.existe(response)){
        throw Excepcion(EFIO, "No se ha encontrado la ruta: " +response);
    }


    return response;
}


string preguntaDatos(string question){
    setColor(cmdBlanco,cmdNegro);
    //Preguntamos al usuario y recogemos su respuesta
    string response;
    cout << "* " << question << endl;
    setColor(cmdVerdeFluo,cmdNegro);
    getline (cin, response);
    return response;
}



void mostrarTexto(string texto){
    int relleno = 76 - texto.length();
    cout << texto;
    if (relleno > 0){
        for (int i=0; i < relleno; i++){
            cout << " ";
        }
    }
    cout << endl;
}




/**
* elegirCadenaConexion
*/
string elegirCadenaConexion(string ficheroConexiones, bool flagParaManazas){
    listaSimple <string>lista;
    lista.loadStringsFromFile(ficheroConexiones);
    string cadena;
    string retorno = "";

    if (flagParaManazas == false){
        setColor(cmdBlanco,cmdNegro);
        cout << "Elija la cadena de conexion para sqlplus" << endl;


        setColor(cmdAmarillo,cmdNegro);
        for (unsigned int i=0; i < lista.getSize(); i++){
            vector <string> datos = Constant::split(lista.get(i), ";");
            cout << i << " - " << datos.at(1) << endl;
        }
        setColor(cmdVerdeFluo,cmdNegro);
        getline (cin, cadena);
    } else {
        cadena = "0";
    }

    if (!cadena.empty()){
        int idCadena = Constant::strToTipo<int>(cadena);
        vector <string> datos = Constant::split(lista.get(idCadena), ";");
        retorno = datos.at(0);
    }
    //cout << retorno << endl;
    return retorno;
}

string elegirCadenaConexion(string ficheroConexiones){
    return elegirCadenaConexion(ficheroConexiones, false);
}

int main(){

    Dirutil dir;
    Constant::setAppDir(dir.getDirActual());
    string respuesta;
    PreparaIOP *prepare = new PreparaIOP();
//    HANDLE hConsole;
//    hConsole = GetStdHandle(STD_OUTPUT_HANDLE);


    while (respuesta.compare("0") != 0){
        // pick the colorattribute k you want
        //SetConsoleTextAttribute(hConsole, 289);
        setColor(0xF,0x1);
        for (int i=0; i < 20; i++)
            cout << (char)22;
        cout << " ";
        cout << "PreparaIOP - Credits: Daniel Marco";
        cout << " ";
        for (int i=0; i < 20; i++)
            cout << (char)22;
        cout << endl;
        //SetConsoleTextAttribute(hConsole, 0);
        setColor(0x1, 0xB);
        mostrarTexto("1.  Generar estructura de carpetas de entrega");
        mostrarTexto("2.  Concatenar fuentes para entrega");
        mostrarTexto("3.  Copiar fuentes para rollback");
        setColor(0x6, 0xB);
        mostrarTexto("4.  Autogenerar entrega");
        setColor(0x1, 0xB);
        mostrarTexto("5.  Extraer paquetes de una entrega");
        mostrarTexto("6.  Generar estructura de nuevo JV");
        setColor(0xB, 0x1);
        mostrarTexto("7.  Extraer paquetes de BDD");
        mostrarTexto("8.  Extraer DDL de tablas y constraints (Con parent y foreign keys)");
        mostrarTexto("9.  Extraer DDL de tablas y constraints (Sin foreign keys)");
        mostrarTexto("10. Extraer Datos de tablas");
        mostrarTexto("11. Extraer Procedimientos");
        mostrarTexto("12. Extraer Funciones");
        mostrarTexto("13. Extraer Sequences");
        mostrarTexto("14. Extraer Types");
        mostrarTexto("15. Extraer Views");
        mostrarTexto("16. Extraer Constraints");
        setColor(0xE, 0x2);
        mostrarTexto("17. * Extraer Todo!!!");
        setColor(0x2, 0xA);
        mostrarTexto("18. Ejecutar sqlplus");
        mostrarTexto("19. Ejecutar scripts de un directorio");
        mostrarTexto("20. Generar drops de tablas, paquetes, vistas y secuencias");
        mostrarTexto("21. Recompilar");
        setColor(0x0, 0x7);
        mostrarTexto("0. Salir");

        setColor(0xF,0x1);
        for (int i=0; i < 76; i++)
            cout << (char)22;
        cout << endl;
        setColor(0xA,0x0);

        //Esperamos a la respuesta
        getline (cin, respuesta);
        try{
            if (respuesta.compare("1") == 0){
                string dirEntrega = preguntaDir("Introduzca el directorio donde desea crear la entrega");
                if (dir.existe(dirEntrega)){
                    string entorno;
                    cout << "Introduzca el entorno donde se ejecutara: (int, pre, pro)" << endl;
                    getline (cin, entorno);
                    string entornoLow = entorno;
                    Constant::lowerCase(&entornoLow);
                    if (entornoLow.compare("int") || entornoLow.compare("pre") || entornoLow.compare("pro")){
                        string fecha;
                        cout << "Introduzca la fecha de la entrega: YYYYMMDD" << endl;
                        getline (cin, fecha);
                        prepare->generaEstructuraPaquetes(dirEntrega, entorno, fecha);
                    } else {
                      cout << "El entorno especificado no es correcto" << endl;
                    }
                } else {
                    cout << "El directorio especificado no existe" << endl;
                }
            } else if (respuesta.compare("2") == 0){
                respuesta = preguntaDir("Introduzca directorio de ficheros de la entrega");
                prepare->generaPaquetes(respuesta, "02-script_XXX_packages_");
            } else if (respuesta.compare("3") == 0){
                string modFilesDir = preguntaDir("Introduzca directorio de ficheros de los que generar el rollback");
                string srcDir = preguntaDir("Introduzca directorio de ficheros origen");
                string dstDir = preguntaDir("Introduzca directorio donde se copiaran los ficheros");
                prepare->generaRollback(srcDir, dstDir, modFilesDir);
            } else if (respuesta.compare("4") == 0){
                respuesta = preguntaDir("Introduzca dir. raiz de la entrega (contiene dirs: fuentes, rollback, svn, IOP_INTERCAM_XXX.doc)");
                prepare->autoGenerarEntrega(respuesta);
            }  else if (respuesta.compare("5") == 0){
                respuesta = preguntaDir("Introduzca fichero de paquetes");
                prepare->extraePaquetes(respuesta);
            }  else if (respuesta.compare("6") == 0){
                respuesta = preguntaDir("Introduzca directorio en el que generar la carpeta del JV");
                string codigoJV;
                cout << "Introduzca el codigo del JV. Ej: JV-12345" << endl;
                getline (cin, codigoJV);
                prepare->generaEstructuraJV(respuesta, codigoJV);
            } else if (respuesta.compare("7") == 0){
                string ficheroPaquetes = preguntaDir("Especifique el fichero que contiene la lista de los paquetes a descargar");
                string cadena = elegirCadenaConexion(Constant::getAppDir() + FILE_SEPARATOR + "CadenaConexion.txt");
                prepare->descargaPaquetes(ficheroPaquetes, cadena);
            } else if (respuesta.compare("8") == 0){
                string ficheroTablas = preguntaDir("Especifique el fichero que contiene la lista de las tablas para descargar su DDL");
                string cadena = elegirCadenaConexion(Constant::getAppDir() + FILE_SEPARATOR + "CadenaConexion.txt");
                prepare->descargaDDLTablas(ficheroTablas, cadena);
            } else if (respuesta.compare("9") == 0){
                string ficheroTablas = preguntaDir("Especifique el fichero que contiene la lista de las tablas para descargar su DDL");
                string cadena = elegirCadenaConexion(Constant::getAppDir() + FILE_SEPARATOR + "CadenaConexion.txt");
                prepare->descargaDDLTablas(ficheroTablas, cadena, dir.getFolder(ficheroTablas), false);
            } else if (respuesta.compare("10") == 0){
                string ficheroTablas = preguntaDir("Especifique el fichero que contiene la lista de las tablas para descargar sus datos");
                string cadena = elegirCadenaConexion(Constant::getAppDir() + FILE_SEPARATOR + "CadenaConexion.txt");
                prepare->descargaDatosTablas(ficheroTablas, cadena);
            } else if (respuesta.compare("11") == 0){
                //string ficheroTablas = preguntaDir("Especifique el fichero que contiene la lista de Prodedimientos");
                string cadena = elegirCadenaConexion(Constant::getAppDir() + FILE_SEPARATOR + "CadenaConexion.txt");
                string ficheroProcs = preguntaDir("Especifique el directorio al que exportar los procedimientos");
                prepare->creaFicheroListaObjetos(ficheroProcs + FILE_SEPARATOR + "lista.txt", cadena, "PROCEDURE");
                prepare->descargaDatosProcFunc(ficheroProcs + FILE_SEPARATOR + "lista.txt", cadena, "PROCEDURE");
                dir.borrarArchivo(ficheroProcs + FILE_SEPARATOR + "lista.txt");
            } else if (respuesta.compare("12") == 0){
                string cadena = elegirCadenaConexion(Constant::getAppDir() + FILE_SEPARATOR + "CadenaConexion.txt");
                string ficheroProcs = preguntaDir("Especifique el directorio al que exportar las funciones");
                prepare->creaFicheroListaObjetos(ficheroProcs + FILE_SEPARATOR + "lista.txt", cadena, "FUNCTION");
                prepare->descargaDatosProcFunc(ficheroProcs + FILE_SEPARATOR + "lista.txt", cadena, "FUNCTION");
                dir.borrarArchivo(ficheroProcs + FILE_SEPARATOR + "lista.txt");
            } else if (respuesta.compare("13") == 0){
                string ficheroTablas = preguntaDir("Especifique el directorio al que exportar el fichero de secuencias");
                string cadena = elegirCadenaConexion(Constant::getAppDir() + FILE_SEPARATOR + "CadenaConexion.txt");
                prepare->descargaSecuencias(dir.getFolder(ficheroTablas), cadena);
            } else if (respuesta.compare("14") == 0){
                string ficheroTablas = preguntaDir("Especifique el directorio al que exportar el fichero de tipos");
                string cadena = elegirCadenaConexion(Constant::getAppDir() + FILE_SEPARATOR + "CadenaConexion.txt");
                prepare->DescargaTiposEnFicheros(dir.getFolder(ficheroTablas) + FILE_SEPARATOR + "Tipos", cadena);
                prepare->buscaTipos(dir.getFolder(ficheroTablas) + FILE_SEPARATOR + "Tipos", cadena);
            } else if (respuesta.compare("15") == 0){
                string ficheroTablas = preguntaDir("Especifique el directorio al que exportar el fichero de vistas");
                string cadena = elegirCadenaConexion(Constant::getAppDir() + FILE_SEPARATOR + "CadenaConexion.txt");
                prepare->descargaVistas(dir.getFolder(ficheroTablas), cadena);
            } else if (respuesta.compare("16") == 0){
                string ficheroTablas = preguntaDir("Especifique el fichero con las tablas de las que se quiere obtener sus constraints");
                string cadena = elegirCadenaConexion(Constant::getAppDir() + FILE_SEPARATOR + "CadenaConexion.txt");
                prepare->creaConstraints(ficheroTablas);
                string destino = dir.getFolder(ficheroTablas) + FILE_SEPARATOR + "Constraints_tmp.sql";
                prepare->lanzaScriptBBDD(destino, cadena);
            } else if (respuesta.compare("17") == 0){
                string ficheroTablasDDL = preguntaDir("Especifique el fichero que contiene la lista de las tablas para obtener su DDL");
                string ficheroTablasMetadatos = preguntaDir("Especifique el fichero que contiene la lista de las tablas para obtener sus metadatos");
                string ficheroPaquetes = preguntaDir("Especifique el fichero que contiene la lista de los paquetes");
                string cadena = elegirCadenaConexion(Constant::getAppDir() + FILE_SEPARATOR + "CadenaConexion.txt");
                string exportDir = preguntaDir("Especifique la carpeta de destino donde se crearan los export");
                prepare->extraerTodo(ficheroTablasDDL, ficheroTablasMetadatos, ficheroPaquetes, cadena, exportDir);

                string cadenaLocal = elegirCadenaConexion(Constant::getAppDir() + FILE_SEPARATOR + "CadenaConexion.txt", true);
                string importar = preguntaDatos("Deseas importar todos los datos a tu bdd local? (S/N): N\n* COMPRUEBA LA CADENA DE CONEXION!!!!\n* -> " + cadenaLocal);
                if (importar.compare("S") == 0 || importar.compare("s") == 0){
                    prepare->importarTodo(ficheroTablasDDL, ficheroTablasMetadatos, ficheroPaquetes, cadenaLocal, exportDir);
                }

            } else if (respuesta.compare("18") == 0){
                string cadena = elegirCadenaConexion(Constant::getAppDir() + FILE_SEPARATOR + "CadenaConexion.txt", prepare->MANAZASFLAG);
                string fichero = "";
                cout << "Indique el fichero que desea ejecutar " << endl;
                cout << "(Intro si unicamente quiere conectar a la bbdd con una consola de oracle):" << endl;
                getline (cin, fichero);
                prepare->lanzaScriptBBDD(fichero, cadena, false, true);
            } else if (respuesta.compare("19") == 0){
                cout << "****Especifique la bbdd en la que se ejecutarán los scripts****" << endl;
                string cadena = elegirCadenaConexion(Constant::getAppDir() + FILE_SEPARATOR + "CadenaConexion.txt");
                string directorio = "";
                cout << "Indique el directorio donde estan todos los scripts sql a ejecutar" << endl;
                getline (cin, directorio);
                prepare->ejecutaScriptsDir(directorio, cadena);
            } else if (respuesta.compare("20") == 0){
                cout << "****Especifique la bbdd de la que generar los drops****" << endl;
                string cadena = elegirCadenaConexion(Constant::getAppDir() + FILE_SEPARATOR + "CadenaConexion.txt");
                string directorio = "";
                cout << "Indique el directorio donde desea generar el fichero con los drops" << endl;
                getline (cin, directorio);
                prepare->generarDrops(directorio, cadena);
            } else if (respuesta.compare("21") == 0){
                cout << "****Especifique la bbdd en la que quiere recompilar****" << endl;
                string cadena = elegirCadenaConexion(Constant::getAppDir() + FILE_SEPARATOR + "CadenaConexion.txt");
                string directorio = "";
                cout << "Indique el directorio donde desea generar el log de la recompilacion" << endl;
                getline (cin, directorio);
                prepare->recompilar(directorio, cadena);
            }
        } catch(Excepcion &e) {
            cout << "Error: " +  string(e.getMessage()) << endl;
        }

    } //End while

    delete prepare;
    return 0;
}

