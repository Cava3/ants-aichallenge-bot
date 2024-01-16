#pragma once

#include <fstream>

#ifndef DEBUG
    #define DEBUG
#endif

/*
    struct de debuggage
    Pour l'utiliser, il suffit de déclarer une variable de type Bug et de l'ouvrir
    avec le nom du fichier de sortie en paramètre de la méthode open.
    Ensuite, on peut utiliser l'opérateur << comme pour un flux de sortie.
    Par exemple:
        Bug bug;
        bug.open("./debug.txt");
        bug << state << endl;
        bug << "testing" << 2.0 << '%' << endl;
        bug.close();
*/
struct Bug
{
    std::ofstream file;

    Bug() {};

    // Ouvre le fichier spécifié
    inline void open(const std::string &filename)
    {
        #ifdef DEBUG
            file.open(filename.c_str());
        #endif
    }

    // Ferme le fichier
    inline void close()
    {
        #ifdef DEBUG
            file.close();
        #endif
    }
};

// Surcharge de l'opérateur << 
inline Bug& operator<<(Bug &bug, std::ostream& (*manipulator)(std::ostream&))
{
    #ifdef DEBUG
        bug.file << manipulator;
    #endif

    return bug;
};

// Surcharge de l'opérateur <<
template <class T>
inline Bug& operator<<(Bug &bug, const T &t)
{
    #ifdef DEBUG
        bug.file << t;
    #endif

    return bug;
};
