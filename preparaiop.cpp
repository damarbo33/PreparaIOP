#include "preparaiop.h"

/**
*
*/
PreparaIOP::PreparaIOP(){
    //Ponemos valores por defecto
    FORMATO_FECHA = "DD/MM/RRRR HH24:MI:SS";
    putenv("NLS_LANG=SPANISH_SPAIN.WE8ISO8859P15");
    PACKAGEWITHESQUEMA = false;
    MANAZASFLAG = true;
    inicializa();
}

/**
* Destructor
*/
PreparaIOP::~PreparaIOP(){
}

/**
* Se inicializan variables importantes
*/
void PreparaIOP::inicializa(){
    listaSimple <string>lista;
    lista.loadStringsFromFile(Constant::getAppDir() + FILE_SEPARATOR + "config.ini");
    string line = "";
    size_t pos = 0;
    string variable;
    string valor;

    for (unsigned int i=0; i < lista.getSize(); i++){
        line = lista.get(i);
        pos = line.find_first_of("=");
        if (pos != string::npos && line.find("#") != 0){
            variable = line.substr(0, pos);
            valor = line.substr(pos+1);
            //Constant::lowerCase(&variable);
            if (variable.compare("nlslang") == 0){
                //putenv("NLS_LANG=SPANISH_SPAIN.WE8ISO8859P15");
                putenv(valor.c_str());
            } else if (variable.compare("fmtdate") == 0){
                FORMATO_FECHA = valor;
            } else if (variable.compare("sqlconnectiop") == 0){
                SQLCONNECTIOP = valor;
            } else if (variable.compare("packageWithEsquema") == 0){
                PACKAGEWITHESQUEMA = (valor.compare("1") == 0);
            } else if (variable.compare("manazasFlag") == 0){
                MANAZASFLAG = (valor.compare("1") == 0);
            }
        }
    }
}

/**
*
*/
void PreparaIOP::generaPaquetes(string dirOrigen, string nomFichero){
    cout << "Preparando IOP de paquetes: " << dirOrigen << endl;
    int ficherosPreparados = 0;
    vector <FileProps> * files = new vector <FileProps>();

    string filenameDest;
    if (nomFichero.find(Constant::getFileSep()) == string::npos){
        filenameDest = dirOrigen + Constant::getFileSep()
                            + nomFichero + Constant::fechaAnyo() + ".sql";
    } else {
        filenameDest = nomFichero + Constant::fechaAnyo() + ".sql";
    }

    //Borramos el fichero para crearlo de nuevo
    dir.borrarArchivo(filenameDest);
    //Se lista el directorio
    dir.listarDirRecursivo(dirOrigen, files);
    //Creamos la cabecera necesaria
    creaCabeceraPaquetes(filenameDest);
    //Concatenamos los ficheros
    string extension;
    string fileName;
    //for (unsigned int i=0; i < files->size(); i++){
    //Buscamos los ficheros al reves por un caso especial de instant ticket
    //Asi concatena antes las cabeceras de los paquetes y luego los bodys
    for (int i = (files->size() - 1); i >= 0 ; i--){
        fileName = files->at(i).filename;
        extension = dir.getExtension(fileName);
        Constant::lowerCase(&extension);
        if ((extension.compare(".sql") == 0 || extension.compare(".pls") == 0)){
            cout << fileName << endl;
            catFile(files->at(i).dir + Constant::getFileSep() + fileName,
                filenameDest, true);
            ficherosPreparados++;
        }
    }
    delete files;



    //Si al final no hemos procesado ningun fichero, borramos el archivo
    if (ficherosPreparados == 0){
        dir.borrarArchivo(filenameDest);
    }
    cout << "Se han anyadido: " << ficherosPreparados << " ficheros" << endl;
}

/**
*
*/
void PreparaIOP::generaMetadata(string dirOrigen, string claveDir, string nomFichero){
    cout << "Preparando IOP de metadata: " << dirOrigen << endl;
    int ficherosPreparados = 0;
    vector <FileProps> * files = new vector <FileProps>();
//    string filenameDest = dirOrigen + Constant::getFileSep() + ".." + Constant::getFileSep()
//                            + nomFichero + Constant::fechaAnyo() + ".sql";
    string filenameDest;
    if (nomFichero.find(Constant::getFileSep()) == string::npos){
        filenameDest = dirOrigen + Constant::getFileSep()
                            + nomFichero + Constant::fechaAnyo() + ".sql";
    } else {
        filenameDest = nomFichero + Constant::fechaAnyo() + ".sql";
    }

    //Borramos el fichero para crearlo de nuevo
    dir.borrarArchivo(filenameDest);
    //Se lista el directorio
    dir.listarDirRecursivo(dirOrigen, files);
    //Creamos la cabecera necesaria
    creaCabeceraMetadata(filenameDest);
    string extension;
    //Concatenamos los ficheros
    for (unsigned int i=0; i < files->size(); i++){
        extension = dir.getExtension(files->at(i).filename);
        if ((extension.compare(".sql") == 0 || extension.compare(".pls") == 0)
            && string(files->at(i).dir + Constant::getFileSep()).find(claveDir) != string::npos){
            cout << files->at(i).filename << endl;
             catFile(files->at(i).dir + Constant::getFileSep() + files->at(i).filename,
                filenameDest, false);
            ficherosPreparados++;
        }
    }
    delete files;
    //Se crea el final de los metadatos
    creaFinalMetadata(filenameDest);
    //Si al final no hemos procesado ningun fichero, borramos el archivo
    if (ficherosPreparados == 0){
        dir.borrarArchivo(filenameDest);
    }
    cout << "Se han anyadido: " << ficherosPreparados << " ficheros" << endl;
}

/**
*
*/
void PreparaIOP::generaOtros(string dirOrigen, string claveDir, string nomFichero){
    cout << "Preparando IOP de paquetes: " << dirOrigen << endl;
    int ficherosPreparados = 0;
    vector <FileProps> * files = new vector <FileProps>();

    string filenameDest;
    if (nomFichero.find(Constant::getFileSep()) == string::npos){
        filenameDest = dirOrigen + Constant::getFileSep()
                            + nomFichero + Constant::fechaAnyo() + ".sql";
    } else {
        filenameDest = nomFichero + Constant::fechaAnyo() + ".sql";
    }

    //Borramos el fichero para crearlo de nuevo
    dir.borrarArchivo(filenameDest);
    //Se lista el directorio
    dir.listarDirRecursivo(dirOrigen, files);
    //Creamos la cabecera necesaria
    creaCabeceraPaquetes(filenameDest);
    //Concatenamos los ficheros
    string extension;
    for (int unsigned i=0; i < files->size(); i++){
        extension = dir.getExtension(files->at(i).filename);
        Constant::lowerCase(&extension);
        if ((extension.compare(".sql") == 0 || extension.compare(".pls") == 0)
            && string(files->at(i).dir + Constant::getFileSep()).find(claveDir) != string::npos){
            cout << files->at(i).filename << endl;
            catFile(files->at(i).dir + Constant::getFileSep() + files->at(i).filename,
                filenameDest, false);
            ficherosPreparados++;
        }
    }
    delete files;


    //Si al final no hemos procesado ningun fichero, borramos el archivo
    if (ficherosPreparados == 0){
        dir.borrarArchivo(filenameDest);
    }
    cout << "Se han anyadido: " << ficherosPreparados << " ficheros" << endl;
}

/**
*
*/
void PreparaIOP::extraePaquetes(string dirOrigen){
    cout << "Extrayendo paquetes: " << dirOrigen << endl;
    listaSimple <string>lista;
    lista.loadStringsFromFile(dirOrigen);
    string fichero;
    string claveInicio = "-- Inicio ";
    string claveFin = " -- Fin ";
    ofstream *file = new ofstream();
    string lastLine;

    for (unsigned int i=0; i < lista.getSize(); i++){
        int posClave = lista.get(i).find(claveInicio);
        if (posClave == 0){
            fichero = lista.get(i).substr(claveInicio.length());
            string rutaFile = dir.getFolder(dirOrigen) + FILE_SEPARATOR + fichero;
            file->open(rutaFile.c_str(), ios::out | ios::trunc);
            cout << fichero << endl;
        } else if (lista.get(i).find(claveFin) == 0){
            file->close();//Cerramos el fichero
        } else {
            if (!(i > 0 && lista.get(i).compare(lastLine) == 0 && lastLine.compare("/") == 0)){
                fileWriteln(file, lista.get(i));
                if (!lista.get(i).empty()){
                    lastLine = lista.get(i);
                }
            }
        }
    }

    cout << "Paquetes extraidos" << endl;
}

/**
*
*/
void PreparaIOP::descargaPaquetes(string filePaquetes, string cadenaConexion){
    descargaPaquetes(filePaquetes, dir.getFolder(filePaquetes), cadenaConexion);
}

/**
*
*/
void PreparaIOP::descargaPaquetes(string filePaquetes, string dirDestino, string cadenaConexion){
    cout << "Preparando descarga de paquetes: " << filePaquetes << endl;
    listaSimple <string>lista;
    lista.loadStringsFromFile(filePaquetes);
    cout << "Descargando " << lista.getSize() << " paquetes." << endl;
    //Para cada uno de los paquetes definidos en el fichero especificado
    vector <string> datos = Constant::split(cadenaConexion, "/");
    string owner = datos.at(0);

    for (unsigned int i=0; i < lista.getSize(); i++){
        if (!lista.get(i).empty() && lista.get(i).compare("") != 0){
            //Primero creamos un fichero sql que tendra los comandos necesarios para hacer la descarga de los paquetes
            cout << ceil(i/(float)lista.getSize()*100)
                 << "% . " <<  lista.get(i) << " exportando paquete..." << endl;
            string destino = dirDestino + FILE_SEPARATOR + lista.get(i);
            creaExportPaquete(destino, lista.get(i), owner);
            lanzaScriptBBDD(destino + "_tmp.sql", cadenaConexion);
        }
    }
    cout << "Fin descarga de paquetes: " << filePaquetes << endl;
}

void PreparaIOP::descargaDDLTablas(string ficheroTablas, string cadenaConexion){
    descargaDDLTablas(ficheroTablas,cadenaConexion, dir.getFolder(ficheroTablas), true);
}

/**
*
*/
void PreparaIOP::descargaDDLTablas(string ficheroTablas, string cadenaConexion, string dirDestino, bool conFK){
    cout << "Preparando descarga de DDL: " << ficheroTablas << endl;
    listaSimple <string>lista;
    lista.loadStringsFromFile(ficheroTablas);
    //Para cada uno de las tablas definidas en el fichero especificado
    for (unsigned int i=0; i < lista.getSize(); i++){
        if (!lista.get(i).empty()){
            string tablename = lista.get(i);
            tablename = Constant::trim(tablename);
            //Primero creamos un fichero sql que tendra los comandos necesarios para hacer la descarga de las tablas
            cout << ceil(i/(float)lista.getSize()*100)
                 << "% . " <<  tablename << " exportando ddl..." << endl;


            string destino = dirDestino + FILE_SEPARATOR + tablename;
            creaExportTablas(destino, tablename, conFK);
            //Finalmente ejecutamos este fichero creado con sqlplus
            lanzaScriptBBDD(destino + "_tmp.sql", cadenaConexion);
        }
    }

    if (conFK == false){
        cout << "Lanzando extraccion de Constraints. Esta operacion puede tardar unos minutos..." << endl;
        creaConstraints(ficheroTablas);
        string destino = dir.getFolder(ficheroTablas) + FILE_SEPARATOR + "Constraints_tmp.sql";
        lanzaScriptBBDD(destino, cadenaConexion);
    }
    cout << "Fin descarga de DDL: " << ficheroTablas << endl;
}

void PreparaIOP::creaConstraints(string ficheroTablas){
    creaConstraints(ficheroTablas, dir.getFolder(ficheroTablas));
}

/**
*
*/
void PreparaIOP::creaConstraints(string ficheroTablas, string dirDestino){
    cout << "Preparando descarga de constraints: " << ficheroTablas << endl;
    listaSimple <string>lista;
    lista.loadStringsFromFile(ficheroTablas);

    string filenamePL = dirDestino + FILE_SEPARATOR + "Constraints_tmp.sql";
    string filenameEXT = dirDestino + FILE_SEPARATOR + fileNameScriptCONST;

    ofstream *file = new ofstream();
    file->open(filenamePL.c_str(), ios::out | ios::trunc);

    if (file->is_open()){
        fileWriteln(file, "SET LONG 20000000;");
        fileWriteln(file, "set linesize 32000;");
        fileWriteln(file, "SET ECHO OFF;");
        fileWriteln(file, "SET HEADING OFF;");
        fileWriteln(file, "SET PAGESIZE 10000;");
        fileWriteln(file, "SET VERIFY OFF;");
        fileWriteln(file, "SET FEEDBACK OFF;");
        fileWriteln(file, "SET TERMOUT OFF;");
        fileWriteln(file, "SET TRIMSPOOL on;");
        fileWriteln(file, "set longchunksize 200000;");
        fileWriteln(file, "begin");
        //Esto para que no formatee la salida
//        fileWriteln(file, "DBMS_METADATA.SET_TRANSFORM_PARAM(DBMS_METADATA.SESSION_TRANSFORM,'PRETTY',false);");
        //Esto es indispensable para que aparezcan los terminadores de sentencia sql (el punto y coma al final de la sentencia)
        fileWriteln(file, "DBMS_METADATA.SET_TRANSFORM_PARAM(DBMS_METADATA.SESSION_TRANSFORM, 'SQLTERMINATOR', true);");
        //Esto hace que no tenga tanto detalle
        fileWriteln(file, "DBMS_METADATA.SET_TRANSFORM_PARAM(DBMS_METADATA.SESSION_TRANSFORM, 'SEGMENT_ATTRIBUTES', true ); ");
        fileWriteln(file, "DBMS_METADATA.SET_TRANSFORM_PARAM(DBMS_METADATA.SESSION_TRANSFORM, 'STORAGE', false);");
        fileWriteln(file, "DBMS_METADATA.SET_TRANSFORM_PARAM(DBMS_METADATA.SESSION_TRANSFORM, 'CONSTRAINTS_AS_ALTER', true);");
        fileWriteln(file, "DBMS_METADATA.SET_TRANSFORM_PARAM(DBMS_METADATA.SESSION_TRANSFORM, 'SIZE_BYTE_KEYWORD', true);");
        fileWriteln(file, "end;");
        fileWriteln(file, "/");
        fileWriteln(file, "SET SERVEROUTPUT ON;");
        fileWriteln(file, "SPOOL " + filenameEXT + ";");

        //Para cada uno de las tablas definidas en el fichero especificado
        for (unsigned int i=0; i < lista.getSize(); i++){
            if (!lista.get(i).empty()){
                if (file->is_open()){
                    //fileWriteln(file, "select dbms_metadata.get_dependent_ddl('CONSTRAINT', UPPER('" + lista.get(i) + "')) from dual;");
                    //fileWriteln(file, "select dbms_metadata.get_dependent_ddl('REF_CONSTRAINT', UPPER('" + lista.get(i) + "')) from dual;");
//                    fileWriteln(file, "select case when cuenta > 0 THEN dbms_metadata.get_dependent_ddl('CONSTRAINT', UPPER('" + lista.get(i) + "')) ELSE TO_CLOB('') END AS PRIMARY_CONSTRAINT from("
//                    + "select count(*) as cuenta from all_constraints where table_name = '" + lista.get(i) + "' and constraint_type IN ('P','C') );");

                    fileWriteln(file, "select case when cuenta > 0 THEN dbms_metadata.get_dependent_ddl('REF_CONSTRAINT', UPPER('" + lista.get(i) + "')) ELSE TO_CLOB('') END AS FOREIGN_CONSTRAINT from("
                    + "select count(*) as cuenta from all_constraints where UPPER(table_name) = UPPER('" + lista.get(i) + "') and constraint_type = 'R') ;");
                }
            }
        }
        fileWriteln(file, "select 'exit;' from dual;");
        fileWriteln(file, "SPOOL OFF;");
        fileWriteln(file, "exit;");
        fileWriteln(file, "");
    }
    file->close();//Cerramos el fichero
    delete file;
}


/**
*
*/
void PreparaIOP::descargaSecuencias(string rutaFichero, string cadenaConexion){
    cout << "Preparando descarga de secuencias: " << rutaFichero << endl;
    string filename = dir.getFolder(rutaFichero) + FILE_SEPARATOR + fileNameScriptSECU;
    //Primero creamos un fichero sql que tendra los comandos necesarios para hacer la descarga de las secuencias
    creaExportSecuencias(filename);
    //Finalmente ejecutamos este fichero creado con sqlplus
    cout << "exportando de bbdd..." << endl;
    lanzaScriptBBDD(filename + "_tmp.sql", cadenaConexion);
}

/**
*
*/
void PreparaIOP::descargaTipos(string rutaFichero, string cadenaConexion){
    cout << "Preparando descarga de tipos y vistas: " << rutaFichero << endl;
    string filename = dir.getFolder(rutaFichero) + FILE_SEPARATOR + "tipos";
    cout << "exportando de bbdd..." << endl;
    buscaTipos(filename, cadenaConexion);
    DescargaTiposEnFicheros(filename, cadenaConexion);
    //lanzaScriptBBDD(filename + "_tmp.sql", cadenaConexion);
}

/**
*
*/
void PreparaIOP::descargaVistas(string rutaFichero, string cadenaConexion){
    cout << "Preparando descarga de tipos y vistas: " << rutaFichero << endl;
    string filename = dir.getFolder(rutaFichero) + FILE_SEPARATOR + "vistas";
    ofstream *file = new ofstream();
    Constant::lowerCase(&filename);
    string fddl = filename + "_tmp.sql";
    string fResddl = filename + ".sql";

    file->open(fddl.c_str(), ios::out | ios::trunc);

    if (file->is_open()){
        fileWriteln(file, "SET LONG 20000000");
        fileWriteln(file, "set linesize 32000");
        fileWriteln(file, "SET ECHO OFF");
        fileWriteln(file, "SET TRIMSPOOL on");
        fileWriteln(file, "SET HEADING OFF");
        fileWriteln(file, "SET PAGESIZE 10000");
        fileWriteln(file, "SET VERIFY OFF");
        fileWriteln(file, "SET FEEDBACK OFF");
        fileWriteln(file, "SET TERMOUT OFF");
        fileWriteln(file, "set longchunksize 200000");
        fileWriteln(file, "SET SERVEROUTPUT ON");
        fileWriteln(file, "SPOOL " + fResddl + ";");

        fileWriteln(file, "select 'SET DEFINE OFF' from dual;");
        fileWriteln(file, "select 'ALTER SESSION SET NLS_LANGUAGE= ''SPANISH'';' from dual;");
        fileWriteln(file, "select 'ALTER SESSION SET NLS_ISO_CURRENCY = ''SPAIN'';' from dual;");
        fileWriteln(file, "select 'ALTER SESSION SET NLS_TERRITORY =''SPAIN'';' from dual;");

        fileWriteln(file, "DECLARE");
        fileWriteln(file, "      --Cursor para las vistas");
        fileWriteln(file, "      CURSOR cVista IS");
        fileWriteln(file, "        SELECT VIEW_NAME, TEXT");
        fileWriteln(file, "        FROM user_views;");
        fileWriteln(file, "      l_datos_cursor cVista%ROWTYPE;");
        fileWriteln(file, "      ");
        fileWriteln(file, "      v_clob CLOB;");
        fileWriteln(file, "      ");
        fileWriteln(file, "BEGIN");
        fileWriteln(file, "        OPEN cVista;");
        fileWriteln(file, "        LOOP");
        fileWriteln(file, "          FETCH cVista INTO l_datos_cursor;");
        fileWriteln(file, "          EXIT WHEN cVista%NOTFOUND;");
        fileWriteln(file, "          dbms_output.put_line(TRIM(dbms_metadata.get_ddl('VIEW',l_datos_cursor.VIEW_NAME)));");
        fileWriteln(file, "          dbms_output.put_line('/');");
        fileWriteln(file, "        END LOOP;");
        fileWriteln(file, "        CLOSE cVista;");
        fileWriteln(file, "        dbms_output.put_line('exit;');");
        fileWriteln(file, "        ");
        fileWriteln(file, "END;");
        fileWriteln(file, "/");
        fileWriteln(file, "SPOOL OFF;");
        fileWriteln(file, "exit;");
        file->close();
        Traza::print("writeToFile::Fichero escrito correctamente", W_DEBUG);
    } else {
        Traza::print("writeToFile::Unable to write file", W_ERROR);
        file->close();//Cerramos el fichero
        delete file;
        throw(Excepcion(EFIO));
    }
    delete file;
    lanzaScriptBBDD(fddl, cadenaConexion);
}


/**
*
*/
void PreparaIOP::descargaDatosTablas(string filePaquetes, string cadenaConexion){
    descargaDatosTablas(filePaquetes, dir.getFolder(filePaquetes), cadenaConexion);
}

/**
*
*/
void PreparaIOP::descargaDatosTablas(string filePaquetes, string dirDestino, string cadenaConexion){
    cout << "Preparando descarga de paquetes: " << filePaquetes << endl;
    listaSimple <string>lista;
    lista.loadStringsFromFile(filePaquetes);
    //Para cada uno de los paquetes definidos en el fichero especificado
    for (unsigned int i=0; i < lista.getSize(); i++){
        if (!lista.get(i).empty()){
            string destino = dirDestino + FILE_SEPARATOR + lista.get(i);
            //Primero creamos un fichero sql que tendra los comandos necesarios para hacer la descarga de los paquetes
            exportaDatosTablas(destino, lista.get(i));
            //Finalmente ejecutamos este fichero creado con sqlplus
            cout << ceil(i/(float)lista.getSize()*100)
                 << "% . " <<  lista.get(i) << " exportando datos de la tabla..." << endl;
            lanzaScriptBBDD(destino + "_tmp.sql", cadenaConexion);
            generaQueryExportTablas(destino + "_tmp2.sql");
            generaPLSQLExportTablas(destino + "(datos)");
            lanzaScriptBBDD(destino + "(datos)_tmp.sql", cadenaConexion);
        }
    }
    cout << "Fin descarga de paquetes: " << filePaquetes << endl;
}

/**
*
*/
void PreparaIOP::descargaDatosProcFunc(string filePaquetes, string cadenaConexion, string tipo){

    cout << "Preparando descarga de paquetes: " << filePaquetes << endl;
    listaSimple <string>lista;
    lista.loadStringsFromFile(filePaquetes);
    //Para cada uno de los paquetes definidos en el fichero especificado
    vector <string> datos = Constant::split(cadenaConexion, "/");
    string owner = datos.at(0);

    for (unsigned int i=0; i < lista.getSize(); i++){
        if (!lista.get(i).empty() && lista.get(i).compare("") != 0){
            //Primero creamos un fichero sql que tendra los comandos necesarios para hacer la descarga de los paquetes
            cout << ceil(i/(float)lista.getSize()*100)
                 << "% . " <<  lista.get(i) << " exportando..." << endl;
            string destino = dir.getFolder(filePaquetes) + FILE_SEPARATOR + lista.get(i);
            creaExportProcFunc(destino, lista.get(i), owner, tipo);
            lanzaScriptBBDD(destino + "_tmp.sql", cadenaConexion);
        }
    }
    cout << "Fin descarga de paquetes: " << filePaquetes << endl;
}

/**
*
*/
void PreparaIOP::creaCabeceraPaquetes(string destino){
    ofstream *file = new ofstream();
    file->open(destino.c_str(), ios::out | ios::trunc);

    if (file->is_open()){
        fileWriteln(file, "SET DEFINE OFF");
        fileWriteln(file, "ALTER SESSION SET NLS_LANGUAGE= 'SPANISH';");
        fileWriteln(file, "ALTER SESSION SET NLS_ISO_CURRENCY = 'SPAIN';");
        fileWriteln(file, "ALTER SESSION SET NLS_TERRITORY ='SPAIN';");
        file->close();
        Traza::print("writeToFile::Fichero escrito correctamente", W_DEBUG);
    } else {
        Traza::print("writeToFile::Unable to write file", W_ERROR);
        file->close();//Cerramos el fichero
        delete file;
        throw(Excepcion(EFIO));
    }
    delete file;
}

/**
*
*/
void PreparaIOP::creaCabeceraMetadata(string destino){
    ofstream *file = new ofstream();
    file->open(destino.c_str(), ios::out | ios::trunc);

    if (file->is_open()){
        fileWriteln(file, "SET DEFINE OFF");
        fileWriteln(file, "ALTER SESSION SET NLS_LANGUAGE= 'SPANISH';");
        fileWriteln(file, "ALTER SESSION SET NLS_ISO_CURRENCY = 'SPAIN';");
        fileWriteln(file, "ALTER SESSION SET NLS_TERRITORY ='SPAIN';");
        fileWriteln(file, "DELETE FROM I23COMPCANPRODERR;");
        file->close();
        Traza::print("writeToFile::Fichero escrito correctamente", W_DEBUG);
    } else {
        Traza::print("writeToFile::Unable to write file", W_ERROR);
        file->close();//Cerramos el fichero
        delete file;
        throw(Excepcion(EFIO));
    }
    delete file;
}



/**
*
*/
void PreparaIOP::creaFinalMetadata(string destino){
    ofstream *file = new ofstream();
    file->open(destino.c_str(), ios::out | ios::app);

    if (file->is_open()){
        fileWriteln(file, "INSERT INTO I23COMPCANPRODERR (");
        fileWriteln(file, "              I23IDERROR, I23CDAPLI, I23PROCEDENCIA, I23IDCOMP,");
        fileWriteln(file, "              I23TIPOERROR, I23RECUPERABLE, I23TIPOERRORIMP, I23RECUPERABLEIMP,");
        fileWriteln(file, "              I23NUMCSB, I23TIPO, I23OP, I23CODERROR, I23DESERROR, I23PRIORIDAD");
        fileWriteln(file, ") ");
        fileWriteln(file, "SELECT I31IDERROR, I31CDAPLI, I31PROCEDENCIA, I31IDCOMP,");
        fileWriteln(file, "       I31TIPOERROR, I31RECUPERABLE, I31TIPOERRORIMP, I31RECUPERABLEIMP,");
        fileWriteln(file, "       I31NUMCSB, I31TIPO, I31OP, I31CODERROR, I31DESERROR, I31PRIORIDAD");
        fileWriteln(file, "FROM");
        fileWriteln(file, "(");
        fileWriteln(file, "    SELECT I26PRIORIDAD AS I31PRIORIDAD, I31IDERROR, I31IDCOMP, I31CDAPLI, I31PROCEDENCIA,");
        fileWriteln(file, "           I31NUMCSB, I31TIPO, I31OP, I31CODERROR,");
        fileWriteln(file, "           I31TIPOERROR, I31RECUPERABLE,");
        fileWriteln(file, "           I31TIPOERRORIMP, I31RECUPERABLEIMP, I31DESERROR");
        fileWriteln(file, "    FROM");
        fileWriteln(file, "    (");
        fileWriteln(file, "        SELECT I29ID AS I31IDERROR, I30IDCOMP AS I31IDCOMP, I96CDAPLI AS I31CDAPLI,I97IDPROCEDENCIA AS I31PROCEDENCIA,");
        fileWriteln(file, "               I29NUMCSB AS I31NUMCSB, I29TIPO AS I31TIPO, I29OP AS I31OP, I29CODERROR AS I31CODERROR,");
        fileWriteln(file, "               NVL (I31TIPOERROR, I30TIPOERROR) AS I31TIPOERROR,");
        fileWriteln(file, "               DECODE (I31TIPOERROR, NULL, I30RECUPERABLE, I31RECUPERABLE) AS I31RECUPERABLE,");
        fileWriteln(file, "               NVL (I31TIPOERRORIMP, I30TIPOERRORIMP) AS I31TIPOERRORIMP,");
        fileWriteln(file, "               DECODE (I31TIPOERRORIMP, NULL, I30RECUPERABLEIMP, I31RECUPERABLEIMP) AS I31RECUPERABLEIMP, I29DESERROR AS I31DESERROR");
        fileWriteln(file, "        FROM");
        fileWriteln(file, "        (");
        fileWriteln(file, "            -- Producto Cartesiano con la tabla de Canales");
        fileWriteln(file, "            SELECT I29ID, I29NUMCSB, I29TIPO, I29OP, I29CODERROR,I97IDPROCEDENCIA,");
        fileWriteln(file, "                   I30IDCOMP, I96CDAPLI, I30TIPOERROR, I30RECUPERABLE, I30TIPOERRORIMP, I30RECUPERABLEIMP, I29DESERROR");
        fileWriteln(file, "            FROM I96APLIORI, I29REMERR, I30COMPERR,I97PROCEDENCIA");
        fileWriteln(file, "            WHERE I29IDCOMP = I30IDCOMP");
        fileWriteln(file, "        ),");
        fileWriteln(file, "        I31COMPCANALERR");
        fileWriteln(file, "        WHERE I29ID     = I31IDERROR (+) AND");
        fileWriteln(file, "              I30IDCOMP = I31IDCOMP (+) AND");
        fileWriteln(file, "              I96CDAPLI = I31CDAPLI (+) AND ");
        fileWriteln(file, "              I97IDPROCEDENCIA = I31PROCEDENCIA (+)");
        fileWriteln(file, "    ) A,");
        fileWriteln(file, "    I26TIPOERR");
        fileWriteln(file, "    WHERE I31TIPOERROR = I26TPERROR");
        fileWriteln(file, "    ORDER BY I26PRIORIDAD, I31IDERROR, I31IDCOMP, I31CDAPLI, I31PROCEDENCIA");
        fileWriteln(file, ");");
        fileWriteln(file, "commit;");
        file->close();
        Traza::print("writeToFile::Fichero escrito correctamente", W_DEBUG);
    } else {
        Traza::print("writeToFile::Unable to write file", W_ERROR);
        file->close();//Cerramos el fichero
        delete file;
        throw(Excepcion(EFIO));
    }
    delete file;
}

/**
*
*/
void PreparaIOP::fileWriteln(ofstream *file, string line){
    line.append(VBCRLF);
    file->write(line.c_str(), line.length());
}
/**
*
*/
void PreparaIOP::fileWrite(ofstream *file, string line){
    file->write(line.c_str(), line.length());
}

/**
*
*/
void PreparaIOP::catFile(string origen, string destino, bool addEndPackages){
    // BUFSIZE defaults to 8192
    // BUFSIZE of 1 means one chareter at time
    // good values should fit to blocksize, like 1024 or 4096
    // higher values reduce number of system calls
    size_t BUFFER_SIZE = 4096;
    char buf[BUFFER_SIZE];
    size_t size;

    int source = open(origen.c_str(), O_RDONLY, 0);
    int dest = open(destino.c_str(), O_WRONLY | O_CREAT | O_APPEND, 0644);
    //Incluimos cual es el nombre del fichero que anyadimos
    string comment = "-- Inicio " + dir.getFileName(origen) + VBCRLF;
    write(dest, comment.c_str(), comment.length());

    while ((size = read(source, buf, BUFFER_SIZE)) > 0) {
        write(dest, buf, size);
    }
    //Incluimos cual es el nombre del fichero que anyadimos
    comment = (addEndPackages ? (string(VBCRLF) + "/") : "") + string(VBCRLF) + " -- Fin " + dir.getFileName(origen) + string(VBCRLF);
    write(dest, comment.c_str(), comment.length());

    close(source);
    close(dest);
}

/**
*
*/
void PreparaIOP::catText(string origen, string texto){
    int dest = open(origen.c_str(), O_WRONLY | O_CREAT | O_APPEND, 0644);
    write(dest, texto.c_str(), texto.length());
    close(dest);
}

/**
*
*/
void PreparaIOP::creaExportPaquete(string destino, string paquete, string owner){
    ofstream *file = new ofstream();
    Constant::lowerCase(&destino);
    Constant::lowerCase(&paquete);

    string salida = destino + "_tmp.sql";

    file->open(salida.c_str(), ios::out | ios::trunc);
    Constant::upperCase(&paquete);
    string upOwner = owner;
    Constant::upperCase(&upOwner);

    if (file->is_open()){
        fileWriteln(file, "SET LONG 20000000");
        fileWriteln(file, "set linesize 32000");
        fileWriteln(file, "SET ECHO OFF");
        fileWriteln(file, "SET TRIMSPOOL on");
        fileWriteln(file, "SET HEADING OFF");
        fileWriteln(file, "SET PAGESIZE 10000");
        fileWriteln(file, "SET VERIFY OFF");
        fileWriteln(file, "SET FEEDBACK OFF");
        fileWriteln(file, "SET TERMOUT OFF");
        fileWriteln(file, "SPOOL " + destino + ".sql;");
        //Especificamos el comando para obtener el paquete y su body

        fileWriteln(file, makeStrExpPackage(upOwner, paquete, "PACKAGE"));
        fileWriteln(file, "select '/' from dual;");

        fileWriteln(file, makeStrExpPackage(upOwner, paquete, "PACKAGE BODY"));
        fileWriteln(file, "select '/' from dual;");

        //Finalizamos el fichero
        fileWriteln(file, "SPOOL OFF;");
        fileWriteln(file, "exit;");
        file->close();
        Traza::print("writeToFile::Fichero escrito correctamente", W_DEBUG);
    } else {
        Traza::print("writeToFile::Unable to write file", W_ERROR);
        file->close();//Cerramos el fichero
        delete file;
        throw(Excepcion(EFIO));
    }
    delete file;
}

/**
*
*/
string PreparaIOP::makeStrExpPackage(string upOwner, string paquete, string parte){
    return "select case when rownum = 1 THEN '"
    + (PACKAGEWITHESQUEMA ? ("CREATE OR REPLACE " + parte + " " + upOwner + "." + paquete + "'") : ("CREATE OR REPLACE ' || trim(text)")) + " ELSE trim(text) END IF "
    + "from all_source where LOWER(name)= LOWER('" + paquete + "') AND type='" + parte + "'  AND UPPER(owner) = UPPER('" + upOwner + "') order by rownum asc;";
}

/**
*
*/
void PreparaIOP::creaExportProcFunc(string destino, string objeto, string owner, string tipo){
    ofstream *file = new ofstream();
    Constant::lowerCase(&destino);
    Constant::lowerCase(&objeto);

    string salida = destino + "_tmp.sql";

    file->open(salida.c_str(), ios::out | ios::trunc);

    if (file->is_open()){
        fileWriteln(file, "SET LONG 20000000");
        fileWriteln(file, "set linesize 32000");
        fileWriteln(file, "SET ECHO OFF");
        fileWriteln(file, "SET TRIMSPOOL on");
        fileWriteln(file, "SET HEADING OFF");
        fileWriteln(file, "SET PAGESIZE 10000");
        fileWriteln(file, "SET VERIFY OFF");
        fileWriteln(file, "SET FEEDBACK OFF");
        fileWriteln(file, "SET TERMOUT OFF");
        fileWriteln(file, "SPOOL " + destino + ".sql;");
        //Especificamos el comando para obtener el paquete y su body
        //fileWriteln(file, "PROMPT CREATE OR REPLACE;");
        fileWriteln(file, "select case when rownum = 1 THEN 'CREATE OR REPLACE ' || trim(text) ELSE trim(text) END IF from all_source where LOWER(name)= LOWER('"
                            + objeto + "') AND type='" + tipo + "'  AND UPPER(owner) = UPPER('" + owner + "') order by rownum asc;");

        fileWriteln(file, "select '/' from dual;");
        //Finalizamos el fichero
        fileWriteln(file, "SPOOL OFF;");
        fileWriteln(file, "exit;");
        file->close();
        Traza::print("writeToFile::Fichero escrito correctamente", W_DEBUG);
    } else {
        Traza::print("writeToFile::Unable to write file", W_ERROR);
        file->close();//Cerramos el fichero
        delete file;
        throw(Excepcion(EFIO));
    }
    delete file;
}

void PreparaIOP::creaExportTablas(string destino, string tabla){
    creaExportTablas(destino, tabla, true);
}

/**
*
*/
void PreparaIOP::creaExportTablas(string destino, string tabla, bool refConstraints){
    ofstream *file = new ofstream();
    Constant::lowerCase(&destino);
    string salida = destino + "_tmp.sql";
    file->open(salida.c_str(), ios::out | ios::trunc);

    if (file->is_open()){
        fileWriteln(file, "SET LONG 20000000;");
        fileWriteln(file, "set linesize 32000;");
        fileWriteln(file, "SET ECHO OFF;");
        fileWriteln(file, "SET HEADING OFF;");
        fileWriteln(file, "SET PAGESIZE 10000;");
        fileWriteln(file, "SET VERIFY OFF;");
        fileWriteln(file, "SET FEEDBACK OFF;");
        fileWriteln(file, "SET TERMOUT OFF;");
        fileWriteln(file, "SET TRIMSPOOL on;");
        fileWriteln(file, "set longchunksize 200000;");
        fileWriteln(file, "begin");
        //Esto para que no formatee la salida
//        fileWriteln(file, "DBMS_METADATA.SET_TRANSFORM_PARAM(DBMS_METADATA.SESSION_TRANSFORM,'PRETTY',false);");
        //Esto es indispensable para que aparezcan los terminadores de sentencia sql (el punto y coma al final de la sentencia)
        fileWriteln(file, "DBMS_METADATA.SET_TRANSFORM_PARAM(DBMS_METADATA.SESSION_TRANSFORM, 'SQLTERMINATOR',true);");
        //Esto hace que no tenga tanto detalle
        fileWriteln(file, "dbms_metadata.SET_TRANSFORM_PARAM(DBMS_METADATA.SESSION_TRANSFORM, 'SEGMENT_ATTRIBUTES', true ); ");
        fileWriteln(file, "dbms_metadata.SET_TRANSFORM_PARAM(DBMS_METADATA.SESSION_TRANSFORM, 'STORAGE', false);");
        fileWriteln(file, "dbms_metadata.SET_TRANSFORM_PARAM(DBMS_METADATA.SESSION_TRANSFORM, 'CONSTRAINTS_AS_ALTER', true);");
        fileWriteln(file, "dbms_metadata.SET_TRANSFORM_PARAM(DBMS_METADATA.SESSION_TRANSFORM, 'SIZE_BYTE_KEYWORD', true);");
        //No incluimos las constraints porque sino fallara al crear tablas cuyas referencias en las constraints aun no existan
        if (!refConstraints){
            fileWriteln(file, "dbms_metadata.SET_TRANSFORM_PARAM(DBMS_METADATA.SESSION_TRANSFORM, 'REF_CONSTRAINTS', FALSE);");
        }
        fileWriteln(file, "end;");
        fileWriteln(file, "/");
        fileWriteln(file, "SET SERVEROUTPUT ON;");
        fileWriteln(file, "SPOOL " + destino + ".sql;");
        Constant::lowerCase(&tabla);
        //Extraemos las columnas de las tablas
        fileWriteln(file, "SELECT DBMS_METADATA.GET_DDL('TABLE',u.table_name) FROM all_tables u WHERE LOWER(table_name) = LOWER('" + tabla + "');");
        //Extraemos los comentarios de las tablas
        fileWriteln(file, "SELECT 'COMMENT ON COLUMN ' || col.owner || '.' || col.table_name || '.' || col.column_name || ' IS ''' || com.comments || ''';'");
        fileWriteln(file, " FROM all_tab_columns col, all_col_comments com WHERE");
        fileWriteln(file, " col.table_name = com.table_name AND ");
        fileWriteln(file, " col.column_name = com.column_name AND col.owner=com.owner AND");
        fileWriteln(file, " LOWER(col.table_name) = LOWER('" + tabla + "') ORDER BY col.table_name, col.column_id;");
        //Extraemos los indices de las tablas
        //fileWriteln(file, "SELECT DBMS_METADATA.GET_DDL('INDEX',u.index_name) FROM USER_INDEXES u WHERE table_name = '" + tabla + "';");
        //Extraemos los grants de las tablas
        fileWriteln(file, "SELECT DBMS_METADATA.GET_DEPENDENT_DDL('OBJECT_GRANT',UPPER('" + tabla + "')) FROM DUAL;");
//        fileWriteln(file, "SELECT");
//        fileWriteln(file, "CASE WHEN SELECT_PRIV <> 'N' THEN ' GRANT SELECT ON ' || OWNER || '.' || TABLE_NAME || ' TO ' || GRANTEE || ';'  ELSE '' END ||");
//        fileWriteln(file, "CASE WHEN INSERT_PRIV <> 'N' THEN ' GRANT INSERT ON ' || OWNER || '.' || TABLE_NAME || ' TO ' || GRANTEE || ';'  ELSE '' END ||");
//        fileWriteln(file, "CASE WHEN DELETE_PRIV <> 'N' THEN ' GRANT DELETE ON ' || OWNER || '.' || TABLE_NAME || ' TO ' || GRANTEE || ';'  ELSE '' END ||");
//        fileWriteln(file, "CASE WHEN UPDATE_PRIV <> 'N' THEN ' GRANT UPDATE ON ' || OWNER || '.' || TABLE_NAME || ' TO ' || GRANTEE || ';'  ELSE '' END  ");
//        fileWriteln(file, "AS GRANTS");
//        fileWriteln(file, "FROM table_privileges");
//        fileWriteln(file, "where table_name = '" + tabla + "'");
//        fileWriteln(file, "ORDER BY owner, table_name;");
        //Finalizamos el fichero
        fileWriteln(file, "SPOOL OFF;");
        fileWriteln(file, "exit;");
        fileWriteln(file, "");
        file->close();
        Traza::print("writeToFile::Fichero escrito correctamente", W_DEBUG);
    } else {
        Traza::print("writeToFile::Unable to write file", W_ERROR);
        file->close();//Cerramos el fichero
        delete file;
        throw(Excepcion(EFIO));
    }
    delete file;
}

/**
*
*/
void PreparaIOP::creaExportSecuencias(string destino){
    ofstream *file = new ofstream();
    Constant::lowerCase(&destino);
    string salida = destino + "_tmp.sql";
    file->open(salida.c_str(), ios::out | ios::trunc);

    if (file->is_open()){
        fileWriteln(file, "SET LONG 20000000");
        fileWriteln(file, "set linesize 32000");
        fileWriteln(file, "SET ECHO OFF");
        fileWriteln(file, "SET TRIMSPOOL on");
        fileWriteln(file, "SET HEADING OFF");
        fileWriteln(file, "SET PAGESIZE 10000");
        fileWriteln(file, "SET VERIFY OFF");
        fileWriteln(file, "SET FEEDBACK OFF");
        fileWriteln(file, "SET TERMOUT OFF");
        fileWriteln(file, "set longchunksize 200000");
//        fileWriteln(file, "ALTER SESSION SET NLS_LANGUAGE= 'SPANISH';");
//        fileWriteln(file, "ALTER SESSION SET NLS_ISO_CURRENCY = 'SPAIN';");
//        fileWriteln(file, "ALTER SESSION SET NLS_TERRITORY ='SPAIN';");
        fileWriteln(file, "SET SERVEROUTPUT ON");
        fileWriteln(file, "SPOOL " + destino + ".sql;");

//        fileWriteln(file, "select 'CREATE SEQUENCE ' || SEQUENCE_NAME ||");
//        fileWriteln(file, "' START WITH ' || LAST_NUMBER ||");
//        fileWriteln(file, "' INCREMENT BY ' || INCREMENT_BY ||");
//        fileWriteln(file, "' MINVALUE ' || MIN_VALUE ||");
//        fileWriteln(file, "' MAXVALUE ' || MAX_VALUE ||");
//        fileWriteln(file, "case CYCLE_FLAG when 'Y' then ' CYCLE' else ' NOCYCLE' end ||");
//        fileWriteln(file, "CASE WHEN CACHE_SIZE = 0 THEN ' NOCACHE' ELSE ' CACHE ' || CACHE_SIZE END || ';' ||");
//        fileWriteln(file, "CASE WHEN SELECT_PRIV <> 'N' THEN ' GRANT SELECT ON ' || OWNER || '.' || TABLE_NAME || ' TO ' || GRANTEE || ';'  ELSE '' END ||");
//        fileWriteln(file, "CASE WHEN INSERT_PRIV <> 'N' THEN ' GRANT INSERT ON ' || OWNER || '.' || TABLE_NAME || ' TO ' || GRANTEE || ';'  ELSE '' END ||");
//        fileWriteln(file, "CASE WHEN DELETE_PRIV <> 'N' THEN ' GRANT DELETE ON ' || OWNER || '.' || TABLE_NAME || ' TO ' || GRANTEE || ';'  ELSE '' END ||");
//        fileWriteln(file, "CASE WHEN UPDATE_PRIV <> 'N' THEN ' GRANT UPDATE ON ' || OWNER || '.' || TABLE_NAME || ' TO ' || GRANTEE || ';'  ELSE '' END ");
//        fileWriteln(file, "AS GRANTS ");
//        fileWriteln(file, "from USER_SEQUENCES up, table_privileges tb");
//        fileWriteln(file, "where up.sequence_name = tb.table_name;");

        /** DESCARGAMOS LAS SECUENCIAS*/
        fileWriteln(file, "select DISTINCT 'CREATE SEQUENCE ' || SEQUENCE_NAME ||");
        fileWriteln(file, "' START WITH ' || LAST_NUMBER ||");
        fileWriteln(file, "' INCREMENT BY ' || INCREMENT_BY ||");
        fileWriteln(file, "' MINVALUE ' || MIN_VALUE ||");
        fileWriteln(file, "' MAXVALUE ' || MAX_VALUE ||");
        fileWriteln(file, "case CYCLE_FLAG when 'Y' then ' CYCLE' else ' NOCYCLE' end ||");
        fileWriteln(file, "CASE WHEN CACHE_SIZE = 0 THEN ' NOCACHE' ELSE ' CACHE ' || CACHE_SIZE END || ';' AS SECUENCES");
        fileWriteln(file, "from USER_SEQUENCES up, table_privileges tb");
        fileWriteln(file, "where up.sequence_name = tb.table_name ORDER BY SECUENCES;");

        /**DESCARGAMOS LOS GRANTS*/
        fileWriteln(file, "select CASE WHEN SELECT_PRIV <> 'N' THEN ' GRANT SELECT ON ' || OWNER || '.' || TABLE_NAME || ' TO ' || GRANTEE || ';'  ELSE '' END ||");
        fileWriteln(file, "CASE WHEN INSERT_PRIV <> 'N' THEN ' GRANT INSERT ON ' || OWNER || '.' || TABLE_NAME || ' TO ' || GRANTEE || ';'  ELSE '' END ||");
        fileWriteln(file, "CASE WHEN DELETE_PRIV <> 'N' THEN ' GRANT DELETE ON ' || OWNER || '.' || TABLE_NAME || ' TO ' || GRANTEE || ';'  ELSE '' END ||");
        fileWriteln(file, "CASE WHEN UPDATE_PRIV <> 'N' THEN ' GRANT UPDATE ON ' || OWNER || '.' || TABLE_NAME || ' TO ' || GRANTEE || ';'  ELSE '' END ");
        fileWriteln(file, "AS GRANTS ");
        fileWriteln(file, "from USER_SEQUENCES up, table_privileges tb");
        fileWriteln(file, "where up.sequence_name = tb.table_name ORDER BY SEQUENCE_NAME;");

        fileWriteln(file, "SELECT 'exit;' FROM DUAL;");

        //Finalizamos el fichero
        fileWriteln(file, "SPOOL OFF;");
        fileWriteln(file, "exit;");
        file->close();
        Traza::print("writeToFile::Fichero escrito correctamente", W_DEBUG);
    } else {
        Traza::print("writeToFile::Unable to write file", W_ERROR);
        file->close();//Cerramos el fichero
        delete file;
        throw(Excepcion(EFIO));
    }
    delete file;
}

/**
* Extrae los tipos en un unico fichero con sus drops y su secuencia
*/
void PreparaIOP::buscaTipos(string destino, string cadenaConexion){
    ofstream *file = new ofstream();
    Constant::lowerCase(&destino);
    string fListaTipos = destino + "_tmp.sql";
    string fResListaTipos = destino + "_tmp2.sql";
    string fddl = destino + "_tmp3.sql";
    string fResddl = destino + ".sql";

    //Obtenemos los tipos existentes respetando la jerarquia de las clases
    obtenerTiposEnOrden(fListaTipos, fResListaTipos, cadenaConexion);
    //Recorremos el fichero
    listaSimple <string>lista;
    lista.loadStringsFromFile(fResListaTipos);
    file->open(fddl.c_str(), ios::out | ios::trunc);
    fileWriteln(file, "SET LONG 20000000;");
    fileWriteln(file, "set linesize 32000;");
    fileWriteln(file, "SET ECHO OFF;");
    fileWriteln(file, "SET HEADING OFF;");
    fileWriteln(file, "SET PAGESIZE 10000;");
    fileWriteln(file, "SET VERIFY OFF;");
    fileWriteln(file, "SET FEEDBACK OFF;");
    fileWriteln(file, "SET TERMOUT OFF;");
    fileWriteln(file, "SET TRIMSPOOL on;");
    fileWriteln(file, "set longchunksize 200000;");
    fileWriteln(file, "SPOOL '" + fResddl + "';");
    //Primero hacemos los drop en orden inverso
    for (unsigned int i = lista.getSize() - 1; i > 0; i--){
        if (!lista.get(i).empty()){
            fileWriteln(file, "select 'DROP TYPE " + lista.get(i) + ";' from dual;");
        }
    }
    fileWriteln(file, "declare");
    fileWriteln(file, "begin");
    fileWriteln(file, "  DBMS_METADATA.SET_TRANSFORM_PARAM(DBMS_METADATA.SESSION_TRANSFORM, 'SQLTERMINATOR',true);");
    fileWriteln(file, "end;");
    fileWriteln(file, "/");

    //Creamos los tipos
    for (unsigned int i=0; i < lista.getSize(); i++){
        if (!lista.get(i).empty()){
            fileWriteln(file, "select dbms_metadata.get_ddl('TYPE', '" + lista.get(i) + "') As text from dual;");
        }
    }
    fileWriteln(file, "spool off;");
    fileWriteln(file, "exit;");
    file->close();
    lanzaScriptBBDD(fddl, cadenaConexion);
    dir.borrarArchivo(fResListaTipos);
    delete file;
}

/**
*
*/
void PreparaIOP::DescargaTiposEnFicheros(string destino, string cadenaConexion){
    ofstream *file = new ofstream();
    Constant::lowerCase(&destino);
    string fListaTipos = destino + "_tmp.sql";
    string fResListaTipos = destino + "_tmp2.sql";
    string fddl = destino + "_tmp3.sql";

    //Obtenemos los tipos existentes respetando la jerarquia de las clases
    obtenerTiposEnOrden(fListaTipos, fResListaTipos, cadenaConexion);
    //Recorremos el fichero
    listaSimple <string>lista;
    lista.loadStringsFromFile(fResListaTipos);
    file->open(fddl.c_str(), ios::out | ios::trunc);

    fileWriteln(file, "SET LONG 20000000;");
    fileWriteln(file, "set linesize 32000;");
    fileWriteln(file, "SET ECHO OFF;");
    fileWriteln(file, "SET HEADING OFF;");
    fileWriteln(file, "SET PAGESIZE 10000;");
    fileWriteln(file, "SET VERIFY OFF;");
    fileWriteln(file, "SET FEEDBACK OFF;");
    fileWriteln(file, "SET TERMOUT OFF;");
    fileWriteln(file, "SET TRIMSPOOL on;");
    fileWriteln(file, "set longchunksize 200000;");
    for (unsigned int i=0; i < lista.getSize(); i++){
        if (!lista.get(i).empty()){
            fileWriteln(file, "SPOOL '" + dir.getFolder(destino) + FILE_SEPARATOR + lista.get(i) + ".sql';");
            fileWriteln(file, "declare");
            fileWriteln(file, "begin");
            fileWriteln(file, "  DBMS_METADATA.SET_TRANSFORM_PARAM(DBMS_METADATA.SESSION_TRANSFORM, 'SQLTERMINATOR',true);");
            fileWriteln(file, "end;");
            fileWriteln(file, "/");
            if (!lista.get(i).empty()){
                fileWriteln(file, "select dbms_metadata.get_ddl('TYPE', '" + lista.get(i) + "') As text from dual;");
            }
            fileWriteln(file, "spool off;");
        }
    }

    fileWriteln(file, "exit;");
    file->close();
    lanzaScriptBBDD(fddl, cadenaConexion);
    dir.borrarArchivo(fResListaTipos);
    delete file;
}


/**
*
*/
void PreparaIOP::obtenerTiposEnOrden(string fListaTipos, string fResListaTipos, string cadenaConexion){

    ofstream *file = new ofstream();
    file->open(fListaTipos.c_str(), ios::out | ios::trunc);
    if (file->is_open()){




        fileWriteln(file, "SET LONG 20000000");
        fileWriteln(file, "set linesize 32000");
        fileWriteln(file, "SET ECHO OFF");
        fileWriteln(file, "SET TRIMSPOOL on");
        fileWriteln(file, "SET HEADING OFF");
        fileWriteln(file, "SET PAGESIZE 10000");
        fileWriteln(file, "SET VERIFY OFF");
        fileWriteln(file, "SET FEEDBACK OFF");
        fileWriteln(file, "SET TERMOUT OFF");
        fileWriteln(file, "set longchunksize 200000");
        fileWriteln(file, "SET SERVEROUTPUT ON");
        fileWriteln(file, "SPOOL " + fResListaTipos + ";");
        fileWriteln(file, "DECLARE");
        fileWriteln(file, "  CURSOR cPadres IS");
        fileWriteln(file, "    select OBJECT_NAME, esHijo from(");
        fileWriteln(file, "      select OBJECT_NAME, INSTR(dbms_metadata.get_ddl('TYPE', OBJECT_NAME),'UNDER') AS esHijo ");
        fileWriteln(file, "      from user_objects where OBJECT_TYPE = 'TYPE');");
        fileWriteln(file, "  ");
        fileWriteln(file, "  CURSOR cHijos IS");
        fileWriteln(file, "      select OBJECT_NAME, TRIM(SUBSTR(SUBSTR(SUBTEXT, 0, INSTR(SUBTEXT, '(')-1), 6)) AS PADRE, esHijo from(");
        fileWriteln(file, "        select OBJECT_NAME, instr(TEXT,'UNDER') AS esHijo, substr(TEXT, instr(TEXT, 'UNDER'), 255) AS SUBTEXT FROM (");
        fileWriteln(file, "          select OBJECT_NAME, dbms_metadata.get_ddl('TYPE', OBJECT_NAME) AS TEXT ");
        fileWriteln(file, "          from user_objects where OBJECT_TYPE = 'TYPE' ));");
        fileWriteln(file, "  ");
        fileWriteln(file, "  filaHijos cHijos%ROWTYPE;");
        fileWriteln(file, "  filaPadres cPadres%ROWTYPE;");
        fileWriteln(file, "  listaPadres VARCHAR2(500);");
        fileWriteln(file, "  campoPadre VARCHAR2(500);");
        fileWriteln(file, "  campoHijo VARCHAR2(500);");
        fileWriteln(file, "  type tipoArray IS VARRAY(50) OF VARCHAR2(50);");
        fileWriteln(file, "  listaHijos tipoArray := tipoArray();");
        fileWriteln(file, "  totalHijosDepend integer;");
        fileWriteln(file, "BEGIN");
        fileWriteln(file, "   listaPadres := '';");
        fileWriteln(file, "   totalHijosDepend := 1;");
        fileWriteln(file, "   listaHijos.extend(50);");
        fileWriteln(file, "  ");
        fileWriteln(file, "   OPEN cPadres;");
        fileWriteln(file, "     LOOP");
        fileWriteln(file, "     FETCH cPadres INTO filaPadres;");
        fileWriteln(file, "     EXIT WHEN cPadres%NOTFOUND;");
        fileWriteln(file, "      IF filaPadres.esHijo = 0 THEN");
        fileWriteln(file, "        listaPadres := listaPadres || filaPadres.OBJECT_NAME || ';';");
        fileWriteln(file, "        dbms_output.put_line(filaPadres.OBJECT_NAME);");
        fileWriteln(file, "      END IF;");
        fileWriteln(file, "   END LOOP;");
        fileWriteln(file, "   CLOSE cPadres;");
        fileWriteln(file, "   ");
        fileWriteln(file, "   ");
        fileWriteln(file, "   OPEN cHijos;");
        fileWriteln(file, "     LOOP");
        fileWriteln(file, "     FETCH cHijos INTO filaHijos;");
        fileWriteln(file, "     EXIT WHEN cHijos%NOTFOUND;");
        fileWriteln(file, "        IF filaHijos.esHijo > 0 THEN");
        fileWriteln(file, "          campoPadre := substr(filaHijos.PADRE, 0, LENGTH(filaHijos.PADRE)-1);");
        fileWriteln(file, "          campoHijo := filaHijos.OBJECT_NAME;");
        fileWriteln(file, "          IF instr(listaPadres, campoPadre || ';') > 0 THEN");
        fileWriteln(file, "            listaPadres := listaPadres || campoHijo || ';';");
        fileWriteln(file, "            dbms_output.put_line(campoHijo);");
        fileWriteln(file, "          ELSE ");
        fileWriteln(file, "            listaHijos(totalHijosDepend) := campoHijo;");
        fileWriteln(file, "            totalHijosDepend := totalHijosDepend + 1;");
        fileWriteln(file, "          END IF;");
        fileWriteln(file, "        END IF;");
        fileWriteln(file, "   END LOOP;");
        fileWriteln(file, "   CLOSE cHijos;");
        fileWriteln(file, "   ");
        fileWriteln(file, "   --Pintamos las dependencias");
        fileWriteln(file, "   FOR i in 1 .. totalHijosDepend - 1 LOOP");
        fileWriteln(file, "      dbms_output.put_line(listaHijos(i));");
        fileWriteln(file, "   END LOOP;");
        fileWriteln(file, "END;");
        fileWriteln(file, "/");
        fileWriteln(file, "SPOOL OFF;");
        fileWriteln(file, "exit;");
        file->close();
        Traza::print("writeToFile::Fichero escrito correctamente", W_DEBUG);
        lanzaScriptBBDD(fListaTipos, cadenaConexion);
    } else {
        Traza::print("writeToFile::Unable to write file", W_ERROR);
        file->close();//Cerramos el fichero
        delete file;
        throw(Excepcion(EFIO));
    }
    delete file;
}

/**
*
*/
void PreparaIOP::creaExportTipos(string destino){
    ofstream *file = new ofstream();
    Constant::lowerCase(&destino);
    string salida = destino + "_tmp.sql";
    file->open(salida.c_str(), ios::out | ios::trunc);

    if (file->is_open()){
        fileWriteln(file, "SET LONG 20000000");
        fileWriteln(file, "set linesize 32000");
        fileWriteln(file, "SET ECHO OFF");
        fileWriteln(file, "SET TRIMSPOOL on");
        fileWriteln(file, "SET HEADING OFF");
        fileWriteln(file, "SET PAGESIZE 10000");
        fileWriteln(file, "SET VERIFY OFF");
        fileWriteln(file, "SET FEEDBACK OFF");
        fileWriteln(file, "SET TERMOUT OFF");
        fileWriteln(file, "set longchunksize 200000");
        fileWriteln(file, "SET SERVEROUTPUT ON");
        fileWriteln(file, "SPOOL " + destino + ".sql;");
        fileWriteln(file, "DECLARE");
        fileWriteln(file, "      --Cursor para las vistas");
        fileWriteln(file, "      CURSOR cVista IS");
        fileWriteln(file, "        SELECT VIEW_NAME, TEXT");
        fileWriteln(file, "        FROM user_views;");
        fileWriteln(file, "      l_datos_cursor cVista%ROWTYPE;");
        fileWriteln(file, "      ");
        fileWriteln(file, "      --Cursor para los tipos");
        fileWriteln(file, "      CURSOR cTipos IS ");
        fileWriteln(file, "        select OBJECT_NAME from user_objects where OBJECT_TYPE = 'TYPE';");
        fileWriteln(file, "      l_datos_tipos cTipos%ROWTYPE;");
        fileWriteln(file, "      hasBody NUMBER;");
        fileWriteln(file, "      ");
        fileWriteln(file, "      v_clob CLOB;");
        fileWriteln(file, "      ");
        fileWriteln(file, "BEGIN");
        fileWriteln(file, "        OPEN cVista;");
        fileWriteln(file, "        LOOP");
        fileWriteln(file, "          FETCH cVista INTO l_datos_cursor;");
        fileWriteln(file, "          EXIT WHEN cVista%NOTFOUND;");
        fileWriteln(file, "          dbms_output.put_line(dbms_metadata.get_ddl('VIEW',l_datos_cursor.VIEW_NAME));");
        fileWriteln(file, "        END LOOP;");
        fileWriteln(file, "        CLOSE cVista;");
        fileWriteln(file, "        ");
        fileWriteln(file, "        OPEN cTipos;");
        fileWriteln(file, "        LOOP");
        fileWriteln(file, "          FETCH cTipos INTO l_datos_tipos;");
        fileWriteln(file, "          EXIT WHEN cTipos%NOTFOUND;");
        fileWriteln(file, "          dbms_output.put_line(dbms_metadata.get_ddl('TYPE', l_datos_tipos.OBJECT_NAME));");
        fileWriteln(file, "          select count(*) INTO hasBody from user_objects ");
        fileWriteln(file, "          where OBJECT_TYPE = 'TYPE BODY' and OBJECT_NAME = l_datos_tipos.OBJECT_NAME;");
        fileWriteln(file, "          IF hasBody > 0 THEN");
        fileWriteln(file, "             dbms_output.put_line(dbms_metadata.get_ddl('TYPE_BODY', l_datos_tipos.OBJECT_NAME));");
        fileWriteln(file, "          END IF;");
        fileWriteln(file, "        END LOOP;");
        fileWriteln(file, "        CLOSE cTipos;");
        fileWriteln(file, "END;");
        fileWriteln(file, "/");
        //Finalizamos el fichero
        fileWriteln(file, "SPOOL OFF;");
        fileWriteln(file, "exit;");
        file->close();
        Traza::print("writeToFile::Fichero escrito correctamente", W_DEBUG);
    } else {
        Traza::print("writeToFile::Unable to write file", W_ERROR);
        file->close();//Cerramos el fichero
        delete file;
        throw(Excepcion(EFIO));
    }
    delete file;
}

/**
*
*/
void PreparaIOP::lanzaScriptBBDD(string fichero, string cadenaConexion){
    lanzaScriptBBDD(fichero, cadenaConexion, true, false);
}

/**
* lanzaScriptBBDD
*/
void PreparaIOP::lanzaScriptBBDD(string fichero, string cadenaConexion, bool deleteFile, bool showLogs){
    if (dir.existe(fichero) || fichero.empty()){
        string rutaSqlPlus = Constant::getAppDir() + FILE_SEPARATOR + "instantclient_12_1" + FILE_SEPARATOR + "sqlplus.exe";
        string shortRutasqlPlus = dir.GetShortUtilName(rutaSqlPlus);
        if (!shortRutasqlPlus.empty()){
            string comando = shortRutasqlPlus + (!fichero.empty() ? " -S " : " ") + cadenaConexion
                                + (!fichero.empty() ? " @" + fichero : "")
                                + (!fichero.empty() && showLogs ? " > " + dir.getFolder(fichero) + FILE_SEPARATOR + dir.getFileNameNoExt(fichero) + ".log" : "")
                                ;
            dir.changeDirAbsolute(dir.getFolder(rutaSqlPlus).c_str());
            system(comando.c_str());
        }
        if (deleteFile) dir.borrarArchivo(fichero);

    } else {
        cout << "No existe el fichero " << fichero << endl;
    }
}

/**
* Ejecuta todos los scripts de un directorio
*/
void PreparaIOP::ejecutaScriptsDir(string directorio, string cadenaConexion){
    cout << "Ejecutando scripts del directorio: " << directorio << endl;
    vector <FileProps> * files = new vector <FileProps>();

    //Buscamos en el directorio en el que se han modificado las fuentes
    dir.listarDirRecursivo(directorio, files);
    string extension;
    //Para cada fichero modificado, buscamos en el directorio en el que las fuentes estan sin
    //modificar para obtener la ruta del original
    for (unsigned int i=0; i < files->size(); i++){
        cout << files->at(i).filename << endl;
        extension = dir.getExtension(files->at(i).filename);
        if ((extension.compare(".sql") == 0 || extension.compare(".pls") == 0)){
            lanzaScriptBBDD(directorio + Constant::getFileSep() + files->at(i).filename, cadenaConexion, false, true);
        }
    }
    delete files;
    cout << "Fin ejecucion scripts del directorio: " << directorio << endl;
}



/**
* generaRollback
*/
void PreparaIOP::generaRollback(string srcDir, string dstDir,string modFilesDir){

    cout << "Preparando Rollback" << endl;
    int ficherosPreparados = 0;
    vector <FileProps> * files = new vector <FileProps>();

    //Buscamos en el directorio en el que se han modificado las fuentes
    dir.listarDirRecursivo(modFilesDir, files);

    string extension;

    //Para cada fichero modificado, buscamos en el directorio en el que las fuentes estan sin
    //modificar para obtener la ruta del original
    for (unsigned int i=0; i < files->size(); i++){
        cout << files->at(i).filename << endl;
        extension = dir.getExtension(files->at(i).filename);
        if ((extension.compare(".sql") == 0 || extension.compare(".pls") == 0)){
            dir.copyFile(srcDir + Constant::getFileSep() + files->at(i).filename, dstDir + Constant::getFileSep() + files->at(i).filename);
            ficherosPreparados++;
        }
    }
    delete files;
    cout << "Se han copiado: " << ficherosPreparados << " ficheros" << endl;
}

/**
* generaEstructuraPaquetes
*/
void PreparaIOP::generaEstructuraPaquetes(string dirEntrega, string entorno, string fecha){
    string newDir = dirEntrega + tempFileSep + entorno + "_" + fecha;
    if (!dir.existe(newDir)){
        //Creamos los directorios necesarios para la entrega

        dir.mkpath(newDir.c_str(), 0777);
        dir.mkpath(string(newDir + tempFileSep + "fuentes").c_str(), 0777);
        dir.mkpath(string(newDir + tempFileSep + "rollback" + tempFileSep + "packages").c_str(), 0777);
        dir.mkpath(string(newDir + tempFileSep + "rollback" + tempFileSep + "scripts-bd").c_str(), 0777);
        dir.mkpath(string(newDir + tempFileSep + "rollback" + tempFileSep + "types").c_str(), 0777);
        dir.mkpath(string(newDir + tempFileSep + "svn" + tempFileSep + "scripts-bd").c_str(), 0777);
        dir.mkpath(string(newDir + tempFileSep + "svn" + tempFileSep + "packages").c_str(), 0777);
        dir.mkpath(string(newDir + tempFileSep + "svn" + tempFileSep + "types").c_str(), 0777);

        string jvdir = newDir + tempFileSep + "fuentes" + tempFileSep + "JV-XXXXX" + tempFileSep;
        dir.mkpath(string(jvdir + "metadata").c_str(), 0777);
        dir.mkpath(string(jvdir + "metadata_r").c_str(), 0777);
        dir.mkpath(string(jvdir + "tables").c_str(), 0777);
        dir.mkpath(string(jvdir + "tables_r").c_str(), 0777);
        dir.mkpath(string(jvdir + "grants").c_str(), 0777);
        dir.mkpath(string(jvdir + "synonyms").c_str(), 0777);

        //Ponemos en mayusculas el entorno
        string upperEntorno = entorno;
        Constant::upperCase(&upperEntorno);

        //Copiamos el documento correspondiente para el entorno
        string srcFile = Constant::getAppDir() + FILE_SEPARATOR + "plantilla-doc" + FILE_SEPARATOR + "IOP_INTERCAM_" + upperEntorno + "_plantilla.doc";
        string dstFile = newDir + FILE_SEPARATOR + "IOP_INTERCAM_" + upperEntorno + "_plantilla.doc";
        dir.copyFile(srcFile, dstFile);
    }
}

/**
* generaEstructuraJV
*/
void PreparaIOP::generaEstructuraJV(string dirEntrega, string codigoJV){
        string jvdir = dirEntrega + tempFileSep + codigoJV + tempFileSep;
        dir.mkpath(string(jvdir + "metadata").c_str(), 0777);
        dir.mkpath(string(jvdir + "metadata_r").c_str(), 0777);
        dir.mkpath(string(jvdir + "tables").c_str(), 0777);
        dir.mkpath(string(jvdir + "tables_r").c_str(), 0777);
        dir.mkpath(string(jvdir + "grants").c_str(), 0777);
        dir.mkpath(string(jvdir + "synonyms").c_str(), 0777);
}

/**
* exportaDatosTablas
*/
void PreparaIOP::exportaDatosTablas(string destino, string tabla){
    ofstream *file = new ofstream();
    Constant::lowerCase(&destino);
    Constant::lowerCase(&tabla);
    string salida = destino + "_tmp.sql";
    file->open(salida.c_str(), ios::out | ios::trunc);

    /**En primer lugar obtenemos la estructura de las tablas*/
    if (file->is_open()){
        fileWriteln(file, "set linesize 300");
        fileWriteln(file, "SET ECHO OFF");
        fileWriteln(file, "SET HEADING OFF");
        fileWriteln(file, "SET VERIFY OFF");
        fileWriteln(file, "SET FEEDBACK OFF");
        fileWriteln(file, "SET TERMOUT OFF");
        fileWriteln(file, "SPOOL " + destino + "_tmp2.sql;");
        fileWriteln(file, "SELECT table_name, column_name, data_type, owner FROM ALL_TAB_COLUMNS WHERE UPPER(table_name) = UPPER('" + tabla + "') order by column_name asc;");
        //Finalizamos el fichero
        fileWriteln(file, "SPOOL OFF;");
        fileWriteln(file, "exit;");
        file->close();
        Traza::print("writeToFile::Fichero escrito correctamente", W_DEBUG);
    } else {
        Traza::print("writeToFile::Unable to write file", W_ERROR);
        file->close();//Cerramos el fichero
        delete file;
        throw(Excepcion(EFIO));
    }
    delete file;
}

/**
* generaQueryExportTablas
*/
void PreparaIOP::generaQueryExportTablas(string rutaFicheroConEstructura){
    //Seguidamente recorremos el fichero para obtener las columnas y generamos la query
    listaSimple <string>lista;
    lista.loadStringsFromFile(rutaFicheroConEstructura.c_str());
    string linea;
    int colDef = 0;

    string tableName, tableColumn, tableColumnType, tableOwner, queryCols, queryCols2, queryOrder;
    string queryMain;
    string queryEnd;

    queryCols = "";
    queryCols2 = "";

    tablaInfo.columnas.clear();
    tablaInfo.tipoColumnas.clear();
    tablaInfo.tabla = "";

    for (unsigned int i=0; i < lista.getSize(); i++){
        linea = lista.get(i);
        std::vector<std::string> elems = split(linea, ' ');

        for (unsigned int j=0; j < elems.size(); j++){
            if (!elems.at(j).empty()){
                if (colDef == 0){
                    tablaInfo.tabla = elems.at(j);
                } else if (colDef == 1){
                    tableColumn = elems.at(j);
                } else if (colDef == 2){
                    tableColumnType = elems.at(j);
                    Constant::lowerCase(&tableColumnType);
                    //Solo anyadiremos columnas que no sean de tipo clob
                    if (tableColumnType.compare("clob") != 0 && tableColumnType.compare("lob") != 0){
                        tablaInfo.tipoColumnas.push_back(tableColumnType);
                        tablaInfo.columnas.push_back(tableColumn);
                    }
                } else if (colDef == 3){
                    tablaInfo.esquema = elems.at(j);
                }
                colDef++;
                colDef = colDef % 4;
            }
        }
    }
    dir.borrarArchivo(rutaFicheroConEstructura);
}


/**
* Genera un fichero sql con bloque anonimo que exporta los datos de una tabla
* al ser ejecutado mediante sqlplus. Obtiene los datos de la tabla a insertar
* gracias al atributo tablaInfo de la clase PreparaIOP, que debe contener todos
* los datos de la tabla (nombre, columnas y tipo de datos de cada columna)
*/
void PreparaIOP::generaPLSQLExportTablas(string destino){
    ofstream *file = new ofstream();
    Constant::lowerCase(&destino);
    string salida = destino + "_tmp.sql";
    string queryCols2 = "";
    file->open(salida.c_str(), ios::out | ios::trunc);

    if (file->is_open()){
        fileWriteln(file, "SET DEFINE OFF");
        fileWriteln(file, "SET PAGESIZE 0");
        fileWriteln(file, "SET FEEDBACK OFF");
        fileWriteln(file, "SET HEADING OFF");
        fileWriteln(file, "SET VERIFY OFF");
        fileWriteln(file, "SET ECHO OFF");
        fileWriteln(file, "SET TERMOUT OFF");
        fileWriteln(file, "SET LINESIZE 1000");
        fileWriteln(file, "SET TRIMSPOOL on");
        fileWriteln(file, "set longchunksize 1000");

        fileWriteln(file, "ALTER SESSION SET NLS_LANGUAGE= 'SPANISH';");
        fileWriteln(file, "ALTER SESSION SET NLS_ISO_CURRENCY = 'SPAIN';");
        fileWriteln(file, "ALTER SESSION SET NLS_TERRITORY ='SPAIN';");
        fileWriteln(file, "SET SERVEROUTPUT ON");
        fileWriteln(file, "SPOOL " + destino + ".sql");
        fileWriteln(file, "");
        fileWriteln(file, "select 'SET DEFINE OFF' from dual;");
        fileWriteln(file, "select 'ALTER SESSION SET NLS_LANGUAGE= ''SPANISH'';' from dual;");
        fileWriteln(file, "select 'ALTER SESSION SET NLS_ISO_CURRENCY = ''SPAIN'';' from dual;");
        fileWriteln(file, "select 'ALTER SESSION SET NLS_TERRITORY =''SPAIN'';' from dual;");
        fileWriteln(file, "select 'ALTER SESSION SET NLS_TERRITORY =''SPAIN'';' from dual;");
        fileWriteln(file, "select 'set sqlblanklines on;' from dual;");
        fileWriteln(file, "");
        fileWriteln(file, "DECLARE");
        fileWriteln(file, "  v_start TIMESTAMP;");
        fileWriteln(file, "  v_end TIMESTAMP;");
        fileWriteln(file, "  v_clob CLOB;");
        fileWriteln(file, "  v_tmp_clob CLOB;");
        fileWriteln(file, "  output utl_file.file_type;");
        fileWriteln(file, "  ");
        fileWriteln(file, "     CURSOR cursor_sql");
        fileWriteln(file, "     IS");
        string strCol = "";
        string strOrder = "";
        for (unsigned int i=0; i<tablaInfo.columnas.size(); i++){
            if (tablaInfo.tipoColumnas.at(i).find("date") != std::string::npos){
                strCol = strCol + "TO_CHAR(" + tablaInfo.columnas.at(i) + ",'" + FORMATO_FECHA + "') AS " + tablaInfo.columnas.at(i);
            } else {
                strCol = strCol + tablaInfo.columnas.at(i);
            }
            strOrder = strOrder + tablaInfo.columnas.at(i);

            if (i < tablaInfo.columnas.size() - 1){
                strCol = strCol + ",";
                strOrder = strOrder + ",";
            }
        }

        fileWriteln(file, "     SELECT " + strCol);
        fileWriteln(file, "     FROM " + tablaInfo.esquema + "." + tablaInfo.tabla);
        fileWriteln(file, "     ORDER BY " + strOrder + " ASC;");
        fileWriteln(file, "");
        fileWriteln(file, "     l_datos_cursor   cursor_sql%ROWTYPE;");
        fileWriteln(file, "BEGIN");
        fileWriteln(file, "     OPEN cursor_sql;");
        fileWriteln(file, "     LOOP");
        fileWriteln(file, "        FETCH cursor_sql INTO l_datos_cursor;");
        fileWriteln(file, "        EXIT WHEN cursor_sql%NOTFOUND;");
        fileWriteln(file, "        v_tmp_clob := 'insert into " + tablaInfo.esquema + "." + tablaInfo.tabla + " (" + strOrder + ") VALUES (';");

        string strDatosCursor = "";
        string tableColumn = "";
        //Recorremos cada una de las columnas segun su tipo
        for (unsigned int i=0; i<tablaInfo.columnas.size(); i++){

            fileWriteln(file,"      IF l_datos_cursor." + tablaInfo.columnas.at(i) + " is null THEN");
            strDatosCursor = "          v_tmp_clob := v_tmp_clob || 'NULL" + string((i < tablaInfo.columnas.size() - 1) ? ",'" : "'") + ";";
            fileWriteln(file,strDatosCursor);
            fileWriteln(file,"      ELSE");

            if (tablaInfo.tipoColumnas.at(i).find("date") != std::string::npos){
                //Los tipos de datos de fecha deberian tener una conversion especial
                strDatosCursor = "        v_tmp_clob := v_tmp_clob || 'TO_DATE(''' || l_datos_cursor." + tablaInfo.columnas.at(i) + " || ''',''" + FORMATO_FECHA + "'')";
            } else {
                //Reemplazamos las posibles comillas simples con la llamada al replace
                strDatosCursor = "        v_tmp_clob := v_tmp_clob || '''' || REPLACE(l_datos_cursor." + tablaInfo.columnas.at(i) + ",'''','''''') || '''";
            }

            //Introducimos la coma para separar las columnas en los insert
            if (i < tablaInfo.columnas.size() - 1){
                strDatosCursor = strDatosCursor +",'";
            } else {
                strDatosCursor = strDatosCursor +"'";
            }
            //Finalizamos la sentencia sql
            fileWriteln(file, strDatosCursor + ";");

            fileWriteln(file,"      END IF;");
        }
        fileWriteln(file, "        v_tmp_clob := v_tmp_clob || ');';");
        fileWriteln(file, "        DBMS_OUTPUT.put_line (v_tmp_clob);");
        fileWriteln(file, "     END LOOP;");
        fileWriteln(file, "     CLOSE cursor_sql;");
        fileWriteln(file, "END;");
        fileWriteln(file, "");
        fileWriteln(file, "/");
        fileWriteln(file, "select 'commit;' from dual;");
        fileWriteln(file, "select 'exit;' from dual;");
        fileWriteln(file, "SPOOL OFF;");
        fileWriteln(file, "EXIT;");
        file->close();
        Traza::print("writeToFile::Fichero escrito correctamente", W_DEBUG);
    } else {
        Traza::print("writeToFile::Unable to write file", W_ERROR);
        file->close();//Cerramos el fichero
        delete file;
        throw(Excepcion(EFIO));
    }
    delete file;
}

string PreparaIOP::sqlIfNull(string column, string sentence){
    //Concatenando clobs
    if (!column.empty() && column.compare(sentence) == 0){
        return "'|| case  WHEN " + column + " is null THEN 'null' else ''''||" + sentence + "||'''' end|| '";
    } else {
        return "'|| case  WHEN " + column + " is null THEN 'null' else " + sentence + " end|| '";
    }
}


/**
*
*/
std::vector<std::string> PreparaIOP::split(const std::string &s, char delim) {
    std::vector<std::string> elems;
    return split(s, delim, elems);
}

/**
*
*/
std::vector<std::string> &PreparaIOP::split(const std::string &s, char delim, std::vector<std::string> &elems) {
    std::stringstream ss(s);
    std::string item;
    while(std::getline(ss, item, delim)) {
        elems.push_back(item);
    }
    return elems;
}

/**
*
*/
void PreparaIOP::autoGenerarEntrega(string directorio){
    vector<string> *errores = new vector<string>();
    //En primer lugar comprobamos que existan todos los directorios
    comprobarDir(directorio + "\\fuentes", errores);
    comprobarDir(directorio + "\\svn\\packages", errores);
    comprobarDir(directorio + "\\rollback\\packages", errores);
    comprobarDir(directorio + "\\svn\\types", errores);
    comprobarDir(directorio + "\\rollback\\types", errores);

    if (errores->size() == 0){
        //Todo bien, generamos la entrega
        generaOtros(directorio + "\\fuentes", "\\tables\\", directorio + "\\01-script_INT_tables_");
        generaOtros(directorio + "\\fuentes", "\\tables_r\\", directorio + "\\01-script_INT_tables_r_");
        generaPaquetes(directorio + "\\svn\\packages\\", directorio + "\\02-script_INT_packages_");
        generaPaquetes(directorio + "\\rollback\\packages\\", directorio + "\\02-script_INT_packages_r_");
        generaPaquetes(directorio + "\\svn\\types\\", directorio + "\\03-script_INT_types_");
        generaPaquetes(directorio + "\\rollback\\types\\", directorio + "\\03-script_INT_types_r_");
        generaOtros(directorio + "\\fuentes", "\\grants\\", directorio + "\\04-script_INT_grants_");
        generaOtros(directorio + "\\fuentes", "\\synonyms\\", directorio + "\\05-script_INT_synonyms_");
        generaMetadata(directorio + "\\fuentes", "\\metadata\\", directorio + "\\06-script_INT_metadata_");
        generaMetadata(directorio + "\\fuentes", "\\metadata_r\\", directorio + "\\06-script_INT_metadata_r_");
        cout << "Scripts generados. Revise la carpeta indicada: " + directorio << endl;
    } else {
        cout << "La estructura del directorio de entrega no es correcta." << endl;
        cout << "Deben existir los siguientes directorios:" << endl;
        for (unsigned int i = 0; i < errores->size(); i++){
            cout << errores->at(i) << endl;
        }
    }
    //Generamos el bat de la entrega
    generarBatEntrega(directorio, false);
    generarBatEntrega(directorio, true);
}

/**
*
*/
void PreparaIOP::generarBatEntrega(string directorio, bool isRollback){
    listaSimple <FileProps> * files = new listaSimple<FileProps>();
    ofstream *file = new ofstream();
    string tipoBat = "IOP";
    if (isRollback){
        tipoBat = "IOP_RB";
    }
    string fileName = directorio + "\\" + tipoBat + ".bat";
    cout << "directorio: " << directorio << endl;
    string folder = directorio.substr(directorio.find_last_of("\\")+1);
    cout << "folder: " << folder << endl;


    file->open(fileName.c_str(), ios::out | ios::trunc);

    if (file->is_open()){
        fileWriteln(file, "SET DEFINE OFF");
        fileWriteln(file, "@echo off");
        fileWriteln(file, "echo revise " + tipoBat + ".log");
        fileWriteln(file, "title " + tipoBat);
        fileWriteln(file, "del " + tipoBat + ".log");
        fileWriteln(file, "sqlplus " + SQLCONNECTIOP + " @\"99-timestamp.sql\" >> " + tipoBat + ".log");

        //Listamos los ficheros del directorio y los recorremos
        dir.listarDir(directorio.c_str(), files);
        for (int unsigned i=0; i < files->getSize(); i++){
            if (files->get(i).filename.find("-script_") != string::npos){
                //Los ficheros son de iop
                if ((files->get(i).filename.find("_r_") == string::npos && isRollback == false) ||
                    (files->get(i).filename.find("_r_") != string::npos && isRollback == true)){
                        vector <string> datos = Constant::split(files->get(i).filename, "_");
                        fileWriteln(file, "echo.");
                        fileWriteln(file, "echo " + datos.at(2));
                        fileWriteln(file, "sqlplus " + SQLCONNECTIOP + " @\"" + folder + "\\" + files->get(i).filename + "\" >> " + tipoBat + ".log");
                        fileWriteln(file, "echo FIN " + datos.at(2));
                }
            }
        }

        fileWriteln(file, "sqlplus " + SQLCONNECTIOP + " @\"99-timestamp.sql\" >> " + tipoBat + ".log");
        fileWriteln(file, "echo.");
        fileWriteln(file, "pause");
        file->close();
        Traza::print("writeToFile::Fichero escrito correctamente", W_DEBUG);
    } else {
        Traza::print("writeToFile::Unable to write file", W_ERROR);
        file->close();//Cerramos el fichero
        delete file;
        throw(Excepcion(EFIO));
    }
    delete file;
}

/**
*
*/
void PreparaIOP::extraerTodo(string ficheroTablasDDL, string ficheroTablasMetadatos, string ficheroPaquetes, string cadena, string destino){

    string dirTablas = destino + tempFileSep + "1_TABLES";
    string dirSecuencias = destino + tempFileSep + "2_SEQUENCES";
    string dirVistas = destino + tempFileSep + "3_VISTAS";
    string dirTipos = destino + tempFileSep + "4_TYPES";
    string dirPaquetes = destino + tempFileSep + "5_PACKAGES";
    string dirMetadatos = destino + tempFileSep + "6_METADATA";
    string dirConstraints = destino + tempFileSep + "7_CONSTRAINTS";


    //Creamos los directorios que contendran los ficheros
    dir.mkpath(dirTablas.c_str(), 0777);
    dir.mkpath(dirSecuencias.c_str(), 0777);
    dir.mkpath(dirVistas.c_str(), 0777);
    dir.mkpath(dirTipos.c_str(), 0777);
    dir.mkpath(dirPaquetes.c_str(), 0777);
    dir.mkpath(dirMetadatos.c_str(), 0777);
    dir.mkpath(dirConstraints.c_str(), 0777);

    //Realizamos la descarga de todos los puntos
    //Descargando paquetes
    descargaPaquetes(ficheroPaquetes, dirPaquetes, cadena);
    generaPaquetes(dirPaquetes, fileNameScriptPKG);
    catText(dirPaquetes + Constant::getFileSep() + fileNameScriptPKG + Constant::fechaAnyo() + ".sql", "\nexit;");
    //Descargando DDL
    descargaDDLTablas(ficheroTablasDDL, cadena, dirTablas, false);
    generaPaquetes(dirTablas, fileNameScriptDDL);
    catText(dirPaquetes + Constant::getFileSep() + fileNameScriptDDL + Constant::fechaAnyo() + ".sql", "\nexit;");
    //Descargando datos de tablas
    descargaDatosTablas(ficheroTablasMetadatos, dirMetadatos, cadena);
    //Descarga vistas
    descargaVistas(dirVistas, cadena);
    //Descarga secuencias
    descargaSecuencias(dirSecuencias, cadena);
    //Descarga tipos
    descargaTipos(dirTipos, cadena);

    //descargamos las constraints
    creaConstraints(ficheroTablasDDL, dirConstraints);
    string fichDestino = dirConstraints + FILE_SEPARATOR + "Constraints_tmp.sql";
    lanzaScriptBBDD(fichDestino, cadena);
}

/**
*
*/
void PreparaIOP::importarTodo(string ficheroTablasDDL, string ficheroTablasMetadatos, string ficheroPaquetes, string cadena, string destino){
    string dirTablas = destino + tempFileSep + "1_TABLES";
    string dirSecuencias = destino + tempFileSep + "2_SEQUENCES";
    string dirVistas = destino + tempFileSep + "3_VISTAS";
    string dirTipos = destino + tempFileSep + "4_TYPES";
    string dirPaquetes = destino + tempFileSep + "5_PACKAGES";
    string dirMetadatos = destino + tempFileSep + "6_METADATA";
    string dirConstraints = destino + tempFileSep + "7_CONSTRAINTS";

    generarDrops(destino, cadena);
    lanzaScriptBBDD(destino + Constant::getFileSep() + fileNameScriptDROP, cadena, false, true);
    lanzaScriptBBDD(dirTablas + Constant::getFileSep() + fileNameScriptDDL + Constant::fechaAnyo() + ".sql", cadena, false, true);
    lanzaScriptBBDD(dirSecuencias + Constant::getFileSep() + fileNameScriptSECU + ".sql", cadena, false, true);
    lanzaScriptBBDD(dirVistas + Constant::getFileSep() + fileNameScriptVIST +".sql", cadena, false, true);
    lanzaScriptBBDD(dirTipos + Constant::getFileSep() + fileNameScriptTIPO +".sql", cadena, false, true);
    lanzaScriptBBDD(dirPaquetes + Constant::getFileSep() + fileNameScriptPKG + Constant::fechaAnyo() + ".sql", cadena, false, true);
    ejecutaScriptsDir(dirMetadatos, cadena);
    lanzaScriptBBDD(dirConstraints + Constant::getFileSep() + fileNameScriptCONST, cadena, false, true);
}


/**
*
*/
void PreparaIOP::comprobarDir(string ruta,vector<string> *errores){
    if (!dir.existe(ruta))
        errores->push_back(ruta);
}

/**
*
*/
void PreparaIOP::generarDrops(string destino, string cadena){
    ofstream *file = new ofstream();
    Constant::lowerCase(&destino);
    string salida = destino + FILE_SEPARATOR + "drops_tmp.sql";
    file->open(salida.c_str(), ios::out | ios::trunc);
    vector <string> datos = Constant::split(cadena, "/");
    string owner = datos.at(0);


    if (file->is_open()){
        fileWriteln(file, "SET LONG 20000000");
        fileWriteln(file, "set linesize 32000");
        fileWriteln(file, "SET ECHO OFF");
        fileWriteln(file, "SET TRIMSPOOL on");
        fileWriteln(file, "SET HEADING OFF");
        fileWriteln(file, "SET PAGESIZE 10000");
        fileWriteln(file, "SET VERIFY OFF");
        fileWriteln(file, "SET FEEDBACK OFF");
        fileWriteln(file, "SET TERMOUT OFF");
        fileWriteln(file, "set longchunksize 200000");
        fileWriteln(file, "SET SERVEROUTPUT ON");
        fileWriteln(file, "SPOOL " + destino + FILE_SEPARATOR + fileNameScriptDROP +";");
        fileWriteln(file, "");
        fileWriteln(file, "--drops de secuencias");
        fileWriteln(file, "select 'DROP SEQUENCE \"' || OWNER || '\".\"' || sequence_name || '\";'");
        fileWriteln(file, "from USER_SEQUENCES up, table_privileges tb");
        fileWriteln(file, "where up.sequence_name = tb.table_name;");
        fileWriteln(file, "");
        fileWriteln(file, "--drops de tablas");
        fileWriteln(file, "SELECT 'DROP TABLE \"' || OWNER || '\".\"' || OBJECT_NAME || '\" cascade constraints;'");
        fileWriteln(file, "FROM all_objects");
        fileWriteln(file, "WHERE OBJECT_TYPE = 'TABLE' AND UPPER(OWNER) = UPPER('" + owner + "');");
        fileWriteln(file, "");
        fileWriteln(file, "--drops de paquetes");
        fileWriteln(file, "SELECT 'DROP PACKAGE \"' || OWNER || '\".\"' || OBJECT_NAME || '\";'");
        fileWriteln(file, "FROM all_objects");
        fileWriteln(file, "WHERE OBJECT_TYPE = 'PACKAGE' AND UPPER(OWNER) = UPPER('" + owner + "');");
        fileWriteln(file, "");
        fileWriteln(file, "SELECT 'DROP PACKAGE BODY \"' || OWNER || '\".\"' || OBJECT_NAME || '\";'");
        fileWriteln(file, "FROM all_objects");
        fileWriteln(file, "WHERE OBJECT_TYPE = 'PACKAGE BODY' AND UPPER(OWNER) = UPPER('" + owner + "');");
        fileWriteln(file, "");
        fileWriteln(file, "--DROP DE VISTAS");
        fileWriteln(file, "SELECT 'DROP VIEW \"' || OWNER || '\".\"' || OBJECT_NAME || '\";'");
        fileWriteln(file, "FROM all_objects");
        fileWriteln(file, "WHERE OBJECT_TYPE = 'VIEW' AND UPPER(OWNER) = UPPER('" + owner + "');");
        fileWriteln(file, "");
        fileWriteln(file, "--DROP DE FUNCIONES");
        fileWriteln(file, "SELECT 'DROP FUNCTION \"' || OWNER || '\".\"' || OBJECT_NAME || '\";'");
        fileWriteln(file, "FROM all_objects");
        fileWriteln(file, "WHERE OBJECT_TYPE = 'FUNCTION' AND UPPER(OWNER) = UPPER('" + owner + "');");
        fileWriteln(file, "");
        fileWriteln(file, "--DROP DE PROCESOS");
        fileWriteln(file, "SELECT 'DROP PROCEDURE \"' || OWNER || '\".\"' || OBJECT_NAME || '\";'");
        fileWriteln(file, "FROM all_objects");
        fileWriteln(file, "WHERE OBJECT_TYPE = 'PROCEDURE' AND UPPER(OWNER) = UPPER('" + owner + "');");
        fileWriteln(file, "");

        fileWriteln(file, "select '/' from dual;");
        fileWriteln(file, "select 'commit;' from dual;");
        fileWriteln(file, "select 'exit;' from dual;");
        fileWriteln(file, "");
        fileWriteln(file, "SPOOL OFF;");
        fileWriteln(file, "exit;");
        file->close();
        Traza::print("writeToFile::Fichero escrito correctamente", W_DEBUG);
        lanzaScriptBBDD(salida, cadena);

    } else {
        Traza::print("writeToFile::Unable to write file", W_ERROR);
        file->close();//Cerramos el fichero
        delete file;
        throw(Excepcion(EFIO));
    }
    delete file;
}

/**
*
*/
void PreparaIOP::recompilar(string destino, string cadena){
    ofstream *file = new ofstream();
    Constant::lowerCase(&destino);
    string salida = destino + FILE_SEPARATOR + "recompilar.sql";
    file->open(salida.c_str(), ios::out | ios::trunc);
    vector <string> datos = Constant::split(cadena, "/");
    string owner = datos.at(0);
    Constant::upperCase(&owner);

    if (file->is_open()){
        fileWriteln(file, "SET LONG 20000000");
        fileWriteln(file, "set linesize 32000");
        fileWriteln(file, "SET ECHO OFF");
        fileWriteln(file, "SET TRIMSPOOL on");
        fileWriteln(file, "SET HEADING OFF");
        fileWriteln(file, "SET PAGESIZE 10000");
        fileWriteln(file, "SET VERIFY OFF");
        fileWriteln(file, "SET FEEDBACK OFF");
        fileWriteln(file, "SET TERMOUT OFF");
        fileWriteln(file, "set longchunksize 200000");
        fileWriteln(file, "SET SERVEROUTPUT ON");
        fileWriteln(file, "SPOOL " + destino + FILE_SEPARATOR + "Recompilar.log;");
        fileWriteln(file, "");
        fileWriteln(file, "SELECT to_char(sysdate, 'RRRR/MM/DD HH24:MI:SS') as  timestamp");
        fileWriteln(file, " from dual;");
        fileWriteln(file, "SELECT 'Antes de la recompilacion' from dual;");
        fileWriteln(file, "SET PAGESIZE 100");
        fileWriteln(file, "SELECT object_name \"antes de recompilacion\"");
        fileWriteln(file, " from ALL_objects");
        fileWriteln(file, " where status <> 'VALID'");
        fileWriteln(file, " and UPPER(owner) = UPPER('" + owner + "')");
        fileWriteln(file, " and object_type in ('PACKAGE', 'PACKAGE BODY');");
        fileWriteln(file, "exec dbms_utility.compile_schema('" + owner + "', false);");
        fileWriteln(file, "SELECT 'Tras recompilacion' from dual;");
        fileWriteln(file, "SELECT object_name \"tras recompilacion\"");
        fileWriteln(file, " from ALL_objects");
        fileWriteln(file, " where status <> 'VALID'");
        fileWriteln(file, " and UPPER(owner) = UPPER('" + owner + "')");
        fileWriteln(file, " and object_type in ('PACKAGE', 'PACKAGE BODY');");
        fileWriteln(file, "commit;");
        fileWriteln(file, "SELECT to_char(sysdate, 'RRRR/MM/DD HH24:MI:SS') as  timestamp");
        fileWriteln(file, " from dual;");
        fileWriteln(file, "SPOOL OFF;");
        fileWriteln(file, "exit");
        file->close();
        Traza::print("writeToFile::Fichero escrito correctamente", W_DEBUG);
        lanzaScriptBBDD(salida, cadena);
    } else {
        Traza::print("writeToFile::Unable to write file", W_ERROR);
        file->close();//Cerramos el fichero
        delete file;
        throw(Excepcion(EFIO));
    }
    delete file;
}

/**
*
*/
void PreparaIOP::creaFicheroListaObjetos(string destino, string cadenaConexion, string tipo){
    ofstream *file = new ofstream();
    Constant::lowerCase(&destino);
    vector <string> datos = Constant::split(cadenaConexion, "/");
    string owner = datos.at(0);
    Dirutil dir;
    string salida = dir.getFolder(destino) + FILE_SEPARATOR + "lista_tmp.sql";
    file->open(salida.c_str(), ios::out | ios::trunc);

    if (file->is_open()){
        fileWriteln(file, "SET LONG 20000000");
        fileWriteln(file, "set linesize 32000");
        fileWriteln(file, "SET ECHO OFF");
        fileWriteln(file, "SET TRIMSPOOL on");
        fileWriteln(file, "SET HEADING OFF");
        fileWriteln(file, "SET PAGESIZE 10000");
        fileWriteln(file, "SET VERIFY OFF");
        fileWriteln(file, "SET FEEDBACK OFF");
        fileWriteln(file, "SET TERMOUT OFF");
        fileWriteln(file, "set longchunksize 200000");
        fileWriteln(file, "SET SERVEROUTPUT ON");
        fileWriteln(file, "SPOOL " + destino + ";");
        fileWriteln(file, "select OBJECT_NAME FROM all_objects WHERE OBJECT_TYPE = '"
                    + tipo + "' AND UPPER(OWNER) = UPPER('" + owner + "');");
        fileWriteln(file, "");
        fileWriteln(file, "SPOOL OFF;");
        fileWriteln(file, "exit;");
        file->close();
        Traza::print("writeToFile::Fichero escrito correctamente", W_DEBUG);
        lanzaScriptBBDD(salida, cadenaConexion);

    } else {
        Traza::print("writeToFile::Unable to write file", W_ERROR);
        file->close();//Cerramos el fichero
        delete file;
        throw(Excepcion(EFIO));
    }
    delete file;

}

