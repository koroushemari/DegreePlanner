#ifndef GRADES_H
#define GRADES_H
#include <iostream>
#include <string>
#include "Module.h"



class Grades{

public:
    Grades();
    
    Grades(std::string className_, int grade_);

    //get the class name
    std::string getClassName(){ return className;}
    
    // get the class year
    int getGrade(){ return grade;}
    
private: 
    std::string className;
    int grade;
};
#endif // GRADES_H