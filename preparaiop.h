#ifndef PREPARAIOP_H
#define PREPARAIOP_H

#include "Constant.h"
#include <iostream>
#include "Dirutil.h"
#include <unistd.h>  // read, write, close
#include <cstdio>    // BUFSIZ
#include <fcntl.h>    /* For O_RDWR */
#include <iostream>
#include <fstream>
#include "ListaSimple.h"


using namespace std;

#define fileNameScriptDDL "02-script_XXX_DDL_"
#define fileNameScriptPKG "02-script_XXX_packages_"
#define fileNameScriptDROP "drops.sql"
#define fileNameScriptSECU "secuencias"
#define fileNameScriptVIST "vistas"
#define fileNameScriptTIPO "tipos"
#define fileNameScriptCONST "Constraints.sql"

class PreparaIOP
{
    public:
        /** Default constructor */
        PreparaIOP();
        /** Default destructor */
        virtual ~PreparaIOP();
        void generaPaquetes(string dirOrigen, string nomFichero);
        /**Generamos los paquetes*/
        void generaPaquetes(string dirOrigen, string extOrigen, string nomFichero);
        /**Generamos los metadatos*/
        void generaMetadata(string dirOrigen, string claveDir, string nomFichero);
        /**Preparamos la descarga de paquetes*/
        void descargaPaquetes(string filePaquetes, string cadenaConexion);
        void descargaPaquetes(string filePaquetes, string dirDestino, string cadenaConexion);
        /**Preparamos la descarga de tablas*/
        void descargaDDLTablas(string ficheroTablas, string cadenaConexion);
        string elegirCadenaConexion(string ficheroConexiones);
        void extraePaquetes(string dirOrigen);
        void generaRollback(string srcDir, string dstDir,string modFilesDir);
        void generaEstructuraPaquetes(string dirEntrega, string entorno, string fecha);
        void generaOtros(string dirOrigen, string claveDir, string nomFichero);
        void generaEstructuraJV(string dirEntrega, string codigoJV);
        void descargaDatosTablas(string filePaquetes, string cadenaConexion);
        void descargaDatosTablas(string filePaquetes, string dirDestino, string cadenaConexion);
        void descargaSecuencias(string rutaFichero, string cadenaConexion);
        void autoGenerarEntrega(string directorio);
        void lanzaScriptBBDD(string fichero,string cadenaConexion);
        void lanzaScriptBBDD(string fichero, string cadenaConexion, bool deleteFile, bool showLogs);
        void descargaDDLTablas(string ficheroTablas, string cadenaConexion,string dirDestino, bool conFK);
        void creaConstraints(string ficheroTablas);
        void ejecutaScriptsDir(string directorio, string cadenaConexion);
        void creaConstraints(string ficheroTablas, string dirDestino);
        void generarExcelFromScript(string destino, string cadena);

        void creaExportTipos(string destino);
        void descargaTipos(string rutaFichero, string cadenaConexion);
        void buscaTipos(string destino, string cadenaConexion);
        void obtenerTiposEnOrden(string fListaTipos, string fResListaTipos, string cadenaConexion);
        void descargaVistas(string rutaFichero, string cadenaConexion);
        void generarBatEntrega(string directorio, bool isRollback);
        void extraerTodo(string ficheroTablasDDL, string ficheroTablasMetadatos, string ficheroPaquetes, string cadena, string destino);
        void generarDrops(string destino, string cadena);
        void recompilar(string destino, string cadena);
        void DescargaTiposEnFicheros(string destino, string cadenaConexion);
        void creaExportProcFunc(string destino, string objeto, string owner, string tipo);
        void creaFicheroListaObjetos(string destino, string cadenaConexion, string tipo);
        void descargaDatosProcFunc(string filePaquetes, string cadenaConexion, string tipo);
        void importarTodo(string ficheroTablasDDL, string ficheroTablasMetadatos, string ficheroPaquetes, string cadena, string destino);
         
        bool MANAZASFLAG;



    protected:
    private:
        void catFile(string origen, string destino, bool addEndPackages);
        void creaCabeceraPaquetes(string destino);
        void creaCabeceraMetadata(string destino);
        void creaFinalMetadata(string destino);
        void fileWriteln(ofstream *file, string line);
        void fileWrite(ofstream *file, string line);
        void creaExportPaquete(string destino, string paquete, string owner);
        void creaExportTablas(string destino, string tabla);
        void creaExportTablas(string destino, string tabla, bool refConstraints);
        void exportaDatosTablas(string destino, string tabla);
        void generaQueryExportTablas(string rutaFicheroConEstructura);
        void generaPLSQLExportTablas(string destino);
        std::vector<std::string> split(const std::string &s, char delim);
        std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems);
        string sqlIfNull(string column, string sentence);
        void copyFile(string srcFile, string dstFile);
        char *UnicodeToCodePage(int codePage, const wchar_t *src);
        wchar_t *CodePageToUnicode(int codePage, const char *src);
        void creaExportSecuencias(string destino);
        void comprobarDir(string ruta,vector<string> *errores);
        string makeStrExpPackage(string upOwner, string paquete, string parte);
        void catText(string origen, string texto);


        void inicializa();

        struct t_tableInfo{
            //t_color(int r, int g, int b){this->r = r; this->g = g; this->b = b;}
            string tabla;
            string esquema;
            vector<string> columnas;
            vector<string> tipoColumnas;
        } tablaInfo;

        Dirutil dir;
        string FORMATO_FECHA;
        string SQLCONNECTIOP;
        bool PACKAGEWITHESQUEMA;
        string INSTANT_CLIENT_DIR;
        string CSV_DELIMITER;



};

#endif // PREPARAIOP_H
