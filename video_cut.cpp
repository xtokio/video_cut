//g++ video_cut.cpp -std=c++11 -o video_cut
#include <iostream>
#include <sstream>

#include <vector>
#include <stdexcept>
#include <stdio.h>
#include <string>

using namespace std;

//Mensajes
void Mensaje(string Mensaje, string Color = "Rojo")
{
    if(Color.compare("Negro")==0)
        Color = "30m";
    if(Color.compare("Rojo")==0)
        Color = "31m";
    if(Color.compare("Verde")==0)
        Color = "32m";
    if(Color.compare("Amarillo")==0)
        Color = "33m";
    if(Color.compare("Azul")==0)
        Color = "34m";
    if(Color.compare("Magenta")==0)
        Color = "35m";
    if(Color.compare("Cyan")==0)
        Color = "36m";
    if(Color.compare("Blanco")==0)
        Color = "37m";

    cout << "\033[0;"+Color << Mensaje << " \033[0m\n";
}
//Mensajes

//Ejecuta comando
string exec(const char* cmd) 
{
    char buffer[128];
    string result = "";
    FILE* pipe = popen(cmd, "r");
    if (!pipe) throw runtime_error("popen() failed!");
    try 
    {
        while (!feof(pipe)) 
        {
            if (fgets(buffer, 128, pipe) != NULL)
                result += buffer;
        }
    } 
    catch (...) 
    {
        pclose(pipe);
        throw;
    }
    pclose(pipe);
    return result;
}
//Ejecuta comando

//string to int
int to_int(string Text)
{
    int Number;
    if ( ! (istringstream(Text) >> Number) ) Number = 0;
    return Number;
}
//string to int

//int to string
string to_str(int Number)
{
     ostringstream ss;
     ss << Number;
     return ss.str();
}
//int to string

//Format Seconds
string to_seconds(int seconds)
{
    string time  = "";
    string sminutes = "";
    string sseconds = "";

    int minutes = 0;
    if(seconds > 60)
    {
        minutes = seconds / 60;
        seconds = seconds % 60;
        if(minutes < 10)
            sminutes = "0"+to_str(minutes);
        else
            sminutes = to_str(minutes);
        
        if(seconds < 10)
            sseconds = "0"+to_str(seconds);
        else
            sseconds = to_str(seconds);

        time = "00:"+sminutes+":"+sseconds;
    }
    else
    {
        if(seconds < 10)
            sseconds = "0"+to_str(seconds);
        else
            sseconds = to_str(seconds);

        time = "00:00:"+sseconds;
    }
        
    return time;        
}

// for string delimiter
vector<string> split (string s, string delimiter) {
    size_t pos_start = 0, pos_end, delim_len = delimiter.length();
    string token;
    vector<string> res;

    while ((pos_end = s.find (delimiter, pos_start)) != string::npos) {
        token = s.substr (pos_start, pos_end - pos_start);
        pos_start = pos_end + delim_len;
        res.push_back (token);
    }

    res.push_back (s.substr (pos_start));
    return res;
}

int main(int argc, char *argv[])
{
    if (argc > 3) 
    {
        string Archivo = argv[1];
        string sDuracion_Inicio = argv[2];
        string sDuracion_Fin = argv[3];
        int Duracion = 0;
        string Comando = "";
        string sDuracion = "";
        vector<string> Archivo_v = split (Archivo, "/");

        //Obtengo duracion de video
        Comando = string("ffprobe -i ")+Archivo+" -show_format -v quiet | sed -n 's/duration=//p'";
        sDuracion = exec(Comando.c_str());

        Duracion = (to_int(sDuracion) - to_int(sDuracion_Inicio)) - to_int(sDuracion_Fin);
        
        //Ejecuto video cut
        Mensaje("Procesando...","Azul");
        Comando = string("ffmpeg  -async 1 -i ")+Archivo+" -ss "+to_seconds(to_int(sDuracion_Inicio))+" -t "+to_seconds(Duracion)+" -async 1 -strict -2 video_cut_"+Archivo_v[Archivo_v.size() - 1];
        string Res = exec(Comando.c_str());
        //Mensaje(Comando.c_str());
        Mensaje("Proceso finalizado","Verde");

    }
    else
        Mensaje("Parametro archivo no proporcionado 'video_cut video.mp4 Segundo_Inicio Segundo_Fin'");
}