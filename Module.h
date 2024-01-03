#ifndef MODULE_H
#define MODULE_H
#include <iostream>
#include <string>
#include "Course.h"
#include <Wt/WPushButton.h>
#include <Wt/WContainerWidget.h>
#include <Wt/WText.h>

class Module{

public:
    //constructor

    Module();
    Module(std::string moduleName_);
    ~Module();
    //get the class name
    std::string getModuleName(){ return moduleName;}
    
    // get the class year
    int getSize(){ return size;}
    // get the class description

    void addClass(std::string className_, int classYear_, std::string description_, Wt::WContainerWidget *courseContainer, Wt::WText *descriptionText);

    Course getClass(int number);

    Course getClassByName(std::string className);

    int getClassIndex(std::string className);

    void setClassByIndex(Course newCourse,int index);

    Wt::WPushButton* getCourseButton(std::string className);

    Wt::WPushButton* getCourseButton(int number);

    void SetClassxy(int x , int y, std::string className);

    void addPreReq(std::string course1,std::string course2);

    void addReqCourse(std::string course1);

    std::string * getReqCourses(){
        return reqCourses;
    }
    int getReqCourseSize(){return reqCourseSize;}

    std::string getReqCourseIndex(int index){
        return reqCourses[index];
    }
    
    
//attributes 
private: 
    Course* courses;
    int size;
    std::string moduleName;

    std::string * reqCourses;
    int reqCourseSize;


};
#endif // COURSE_H