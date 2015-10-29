#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <stdexcept>

// #include "antColony/ants.h"

/****************** environment struct ******************/

struct AntColony {
    int foo = 1;
};

struct Environment {
    /* ant algorithm parameters
     * n            : matrix size
     * source       : source node of path in graph
     * destination  : destination node of path in graph
     * adjacency    : defining the graph
     * alpha        :  
     * beta         : 
    */
    
    int n, 
        source, 
        destination;
    double ** adjacency,
        alpha = 1,
        beta = 1;
    
};

/****************** const char arrays ******************/

const char * introduction = {
    "\n"
    "########################################################\n"
    "     this program calculates the shortest path from     \n"
    "     any source to any destination in a given graph     \n"
    "     using the ant colony optimization algorithm        \n"
    "########################################################\n"
    "\n"
    };

const char * manual = {
    "man ............................... displays this manual\n"
    "new ........................... create a new environment\n"
    "set ........... set single parameters of the environment\n"
    "  options are:\n"
    "  matrix ............. adjacency matrix\n"
    "  source .................. source node\n"
    "  destination ........ destination node\n"
    "  alpha ............. alpha coefficient\n"
    "  beta ............... beta coefficient\n"
    "environment / env ..... displays the current environment\n"
    "start ...... starts the algorithm on the set environment\n"
    };


/****************** function definitions ******************/

// sets a new environment for the ant algorithm calculations
void newEnvironment (Environment * environment);

// reads shell input and turns it into a matrix
void setMatrix (Environment * environment);

// communicates parameters to the ant algorithm interface
void input (int n, double ** adjacency, int source, int destination);

// prints the values of the set ant algorithm parameters (the environment)
void printEnvironment (Environment * environment);

// draws a square matrix with brackets (oh my glob)
void drawMatrix (int size, double ** matrix);

void drawVector (std::vector<double> vector);
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <vector>
#include <stdexcept>
#include <cstdlib>

// #include "antColony/ants.h"

/****************** environment struct ******************/

struct AntColony {
    int foo = 1;
};

struct Environment {
    /* ant algorithm parameters
     * n            : matrix size
     * source       : source node of path in graph
     * destination  : destination node of path in graph
     * adjacency    : defining the graph
     * alpha        :  
     * beta         : 
    */
    
    int n, 
        source, 
        destination;
    double ** adjacency,
        alpha = 1,
        beta = 1;
    
};

/****************** const char arrays ******************/

const char * introduction = {
    "\n"
    "########################################################\n"
    "     this program calculates the shortest path from     \n"
    "     any source to any destination in a given graph     \n"
    "     using the ant colony optimization algorithm        \n"
    "########################################################\n"
    "\n"
    };

const char * manual = {
    "man ............................... displays this manual\n"
    "new ........................... create a new environment\n"
    "set ........... set single parameters of the environment\n"
    "  options are:\n"
    "  matrix ............. adjacency matrix\n"
    "  source .................. source node\n"
    "  destination ........ destination node\n"
    "  alpha ............. alpha coefficient\n"
    "  beta ............... beta coefficient\n"
    "environment / env ..... displays the current environment\n"
    "start ...... starts the algorithm on the set environment\n"
    };


/****************** function definitions ******************/

// sets a new environment for the ant algorithm calculations
void newEnvironment (Environment * environment, std::fstream & log);

// reads shell input and turns it into a matrix
void setMatrix (Environment * environment, std::fstream & log);

// communicates parameters to the ant algorithm interface
void input (int n, double ** adjacency, int source, int destination);

// prints the values of the set ant algorithm parameters (the environment)
void printEnvironment (Environment * environment, std::fstream & log);

// draws a square matrix with brackets (oh my glob)
void drawMatrix (int size, double ** matrix, std::fstream & log);

void drawVector (std::vector<double> vector);

// starts the ant algorithm on the given environment
void initAntAlgorithm (AntColony * antColony = nullptr);


/****************** main function ******************/

//TODO: write every output in txt-file

std::fstream log ("log.txt", std::ios::out);

int main () {
    bool run = true;
    
    Environment * environment = nullptr;
    AntColony * antColony = nullptr;
    std::vector<std::vector<double>> antMoves;

    std::cout   << introduction;
    log         << introduction;
    
    while (run) {
        std::cout << "> ";
        std::string command;
        std::cin >> command;
        
        log << "> ";
        log << command << std::endl;
        
        try {
            // menu for different commands
            if (command == "man") { // displays a short manual
                std::cout   << manual;
                log         << manual;
            }
            else if (command == "new") { // create a new environment
                environment = new Environment;
                newEnvironment (environment, log);
            }
            else if (command == "set" && !environment) { // set can only be used with set environment
                std::cout << "no environment has been set. use \"new\" to create a new environment." << std::endl;
                log << "no environment has been set. use \"new\" to create a new environment." << std::endl;
            }
            else if (command == "set" && environment) { // set any environment variable
                std::string option;
                std::cin    >> option;
                log         << option << std::endl;
                
                // lvl 2 menu for the menu point "set"
                if (option == "matrix") {
                    setMatrix (environment, log);
                }
                else if (option == "source") {
                    std::cin    >> environment->source;
                    log         << environment->source << std::endl;
                }
                else if (option == "destination") {
                    std::cin    >> environment->destination;
                    log         << environment->destination << std::endl;
                }
                else if (option == "alpha") {
                    std::cin    >> environment->alpha;
                    log         << environment->alpha << std::endl;
                }
                else if (option == "beta") {
                    std::cin    >> environment->beta;
                    log         << environment->beta << std::endl;
                }
                else {
                    std::cout   << "unknown option" << std::endl;
                    log         << "unknown option" << std::endl;
                }
            }
            else if ((command == "environment" || command == "env") && !environment) { // only set environment can be displayed
                std::cout   << "no environment has been set. use \"new\" to create a new environment." << std::endl;
                log         << "no environment has been set. use \"new\" to create a new environment." << std::endl;
            }
            else if ((command == "environment" || command == "env") && environment) { // displays the set environment
                printEnvironment (environment, log);
            }
            else if (command == "start" && environment) { // starts the ant algorithm on set environment
                bool algorithmIsRunning = true;
                initAntAlgorithm (antColony);
                int steps = 1;
                
                while (algorithmIsRunning) {
                    std::string buf;
//                    std::string stepsStr; // just a relict of an old version
                    
                    while (std::getline (std::cin, buf)) {
                        log << buf << std::endl;
                        if (buf.empty()) { // default or last set step amount is taken again
                            break;
                        }            
                        else if (buf == "abort") { // ant algorithms is aborted
                            algorithmIsRunning = false;
                            break;
                        }
                        else{ // buf gets parsed to int, false input will cause an exception
                            try {
                                steps = std::stoi(buf);
                                break;
                            }
                            catch (std::invalid_argument e) {
                                std::cerr   << "enter a number greater or equal 0" << std::endl;
                                log         << "enter a number greater or equal 0" << std::endl;
                            }
                        }
                    }

                    // the steps taken by the ant colony are saved in ant moves
                    antMoves.push_back (antColony.nextStep (steps));

                    if (algorithmIsRunning) { // only a little gimmick, so no output is made after run has been aborted
                        if (steps != 1) {
                            std::cout   << steps << " steps taken." << std::endl;
                            log         << steps << " steps taken." << std::endl;
                        }
                        else {
                            std::cout   << steps << " step taken." << std::endl;
                            log         << steps << " step taken." << std::endl;
                        }
                    }
                }
                
                // run history should be cleared after run especially after abort
            }
            else if (command == "del") { // deletes the set environment
                delete environment;
                environment = nullptr;
            }
            else if (command == "quit") { // terminates the program
                run = false;
                delete environment;
            }
            else {
                std::cout   << "unknown command" << std::endl;
                log         << "unknown command" << std::endl;
            }
        }
        catch (... /*AntException e*/) { // catches  any exception thrown by the ant algorithm
            std::cout   << "Exception caught: " << /*e.message <<*/ std::endl;
            log         << "Exception caught: " << /*e.message <<*/ std::endl;
        }
    }
    
    log.close();
    return 0;
}


/****************** function declarations ******************/

void newEnvironment (Environment * environment, std::fstream & log) {
    // sets adjacency matrix
    setMatrix (environment, log);

    // source input
    std::cout << "path source = ";
    std::cin >> environment->source;
    log << "path source = " << environment->source << std::endl;
    
    // destination input
    std::cout << "path destination = ";
    std::cin >> environment->destination;
    log << "path destination = " << environment->destination << std::endl;
    
    //caling input function which calls interface function
    input (environment->n, environment->adjacency, environment->source, environment->destination);
}

void setMatrix (Environment * environment, std::fstream & log)
{
    // matrix size input
    int n;
    std::cout << "matrix size = ";
    std::cin >> n;
    environment->n = n;
    log << "matrix size = " << n << std::endl;
    
    environment->adjacency = new double * [n];
    for (int i = 0; i < n; i++) {
        environment->adjacency [i] = new double [n];
    }
    
    // two for-loops for adjacency matrix input
    std::cout << "adjacency matrix = " << std::endl;
    log << "adjacency matrix = " << std::endl;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            std::cin >> environment->adjacency [i][j];
            log << environment->adjacency [i][j] << " ";
        }
        
        log << std::endl;
    }
}

void input (int n, double ** adjacency, int source, int destination) {
    std::cout << "void input" << std::endl;
}

void printEnvironment (Environment * environment, std::fstream & log) {
    std::cout   << "matrix size         = " << environment->n << std::endl
                << "adjacency matrix    = " << std::endl;
    log         << "matrix size         = " << environment->n << std::endl
                << "adjacency matrix    = " << std::endl;
    drawMatrix (environment->n, environment->adjacency, log); 
    std::cout   << "source node         = " << environment->source << std::endl
                << "destination node    = " << environment->destination << std::endl
                << "alpha               = " << environment->alpha << std::endl
                << "beta                = " << environment->beta << std::endl;
    log         << "source node         = " << environment->source << std::endl
                << "destination node    = " << environment->destination << std::endl
                << "alpha               = " << environment->alpha << std::endl
                << "beta                = " << environment->beta << std::endl;                
}

void drawMatrix (int size, double ** matrix, std::fstream & log) {
    std::cout << "/";
    for (int i = 0; i < size; i++) {
        std::cout << "  ";
    }
    std::cout << " \\" << std::endl;
    
    for (int i = 0; i < size; i++) {
        std::cout << "|";
        
        for (int j = 0; j < size; j++) {
            std::cout << " " << matrix [i][j];
        }
        std::cout << " |" << std::endl;
    }

    std::cout << "\\";
    for (int i = 0; i < size; i++) {
        log << "  ";
    }
    std::cout << " /" << std::endl;

    // same operations but writing on the file output stream
    log << "/";
    for (int i = 0; i < size; i++) {
        log << "  ";
    }
    log << " \\" << std::endl;
    
    for (int i = 0; i < size; i++) {
        log << "|";
        
        for (int j = 0; j < size; j++) {
            log << " " << matrix [i][j];
        }
        log << " |" << std::endl;
    }

    log << "\\";
    for (int i = 0; i < size; i++) {
        log << "  ";
    }
    log << " /" << std::endl;
}

void initAntAlgorithm (AntColony * antColony) {
    std::cout << "void initAntAlgorithm" << std::endl;
}
// starts the ant algorithm on the given environment
void initAntAlgorithm (AntColony * antColony = nullptr);


/****************** main function ******************/

//TODO: write every output in txt-file

int main () {
    bool run = true;
    
    Environment * environment = nullptr;
    AntColony * antColony = nullptr;
    std::vector<std::vector<int>> antMoves;

    std::cout << introduction;
    
    while (run) {
        std::cout << "> ";
        std::string command;
        std::cin >> command;
        
        try {
            // menu for different commands
            if (command == "man") { // displays a short manual
                std::cout << manual;
            }
            else if (command == "new") { // create a new environment
                environment = new Environment;
                newEnvironment (environment);
            }
            else if (command == "set" && !environment) { // set can only be used with set environment
                std::cout << "no environment has been set. use \"new\" to create a new environment." << std::endl;
            }
            else if (command == "set" && environment) { // set any environment variable
                std::string option;
                std::cin >> option;
                
                // lvl 2 menu for the menu point "set"
                if (option == "matrix") {
                    setMatrix (environment);
                }
                else if (option == "source") {
                    std::cin >> environment->source;
                }
                else if (option == "destination") {
                    std::cin >> environment->destination;
                }
                else if (option == "alpha") {
                    std::cin >> environment->alpha;
                }
                else if (option == "beta") {
                    std::cin >> environment->beta;
                }
                else {
                    std::cout << "unknown option" << std::endl;
                }
            }
            else if ((command == "environment" || command == "env") && !environment) { // only set environment can be displayed
                std::cout << "no environment has been set. use \"new\" to create a new environment." << std::endl;
            }
            else if ((command == "environment" || command == "env") && environment) { // displays the set environment
                printEnvironment (environment);
            }
            else if (command == "start" && environment) { // starts the ant algorithm on set environment
                bool algorithmIsRunning = true;
                initAntAlgorithm (antColony);
                int steps = 1;
                
                while (algorithmIsRunning) {
                    std::string buf;
//                    std::string stepsStr; // just a relict of an old version
                    
                    while (std::getline (std::cin, buf)) {
                        if (buf.empty()) { // default or last set step amount is taken again
                            break;
                        }            
                        else if (buf == "abort") { // ant algorithms is aborted
                            algorithmIsRunning = false;
                            break;
                        }
                        else{ // buf gets parsed to int, false input will cause an exception
                            try {
                                steps = std::stoi(buf);
                                break;
                            }
                            catch (std::invalid_argument e) {
                                std::cerr << "enter a number greater or equal 0" << std::endl;
                            }
                        }
                    }
                    
                    // the steps taken by the ant colony are saved in ant moves
                    antMoves.push_back (antColony.nextStep (steps));

                    if (algorithmIsRunning) { // only a little gimmick, so no output is made after run has been aborted
                        if (steps != 1) {
                            std::cout << steps << " steps taken." << std::endl;
                        }
                        else {
                            std::cout << steps << " step taken." << std::endl;
                        }
                    }
                }
                
                // run history should be cleared after run especially after abort
            }
            else if (command == "del") { // deletes the set environment
                delete environment;
                environment = nullptr;
            }
            else if (command == "quit") { // terminates the program
                run = false;
                delete environment;
            }
            else {
                std::cout << "unknown command" << std::endl;
            }
        }
        catch (... /*AntException e*/) { // catches  any exception thrown by the ant algorithm
            std::cout << "Exception caught: " << /*e.message <<*/ std::endl;
        }
    }
    
    return 0;
}


/****************** function declarations ******************/

void newEnvironment (Environment * environment) {
    // sets adjacency matrix
    setMatrix (environment);

    // source input
    std::cout << "path source = ";
    std::cin >> environment->source;
    
    // destination input
    std::cout << "path destination = ";
    std::cin >> environment->destination;
    
    //caling input function which calls interface function
    input (environment->n, environment->adjacency, environment->source, environment->destination);
}

void setMatrix (Environment * environment)
{
    // matrix size input
    int n;
    std::cout << "matrix size = ";
    std::cin >> n;
    environment->n = n;
    
    environment->adjacency = new double * [n];
    for (int i = 0; i < n; i++) {
        environment->adjacency [i] = new double [n];
    }
    
    // two for-loops for adjacency matrix input
    std::cout << "adjacency matrix = " << std::endl;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            std::cin >> environment->adjacency [i][j];
        }
    }
}

void input (int n, double ** adjacency, int source, int destination) {
    std::cout << "void input" << std::endl;
}

void printEnvironment (Environment * environment) {
    std::cout   << "matrix size         = " << environment->n << std::endl
                << "adjacency matrix    = " << std::endl;
    drawMatrix (environment->n, environment->adjacency); 
    std::cout   << "source node         = " << environment->source << std::endl
                << "destination node    = " << environment->destination << std::endl
                << "alpha               = " << environment->alpha << std::endl
                << "beta                = " << environment->beta << std::endl;
}

void drawMatrix (int size, double ** matrix) {
    std::cout << "/";
    for (int i = 0; i < size; i++) {
        std::cout << "  ";
    }
    std::cout << " \\" << std::endl;
    
    for (int i = 0; i < size; i++) {
        std::cout << "|";
        
        for (int j = 0; j < size; j++) {
            std::cout << " " << matrix [i][j];
        }
        std::cout << " |" << std::endl;
    }

    std::cout << "\\";
    for (int i = 0; i < size; i++) {
        std::cout << "  ";
    }
    std::cout << " /" << std::endl;
}

void initAntAlgorithm (AntColony * antColony) {
    std::cout << "void initAntAlgorithm" << std::endl;
}
