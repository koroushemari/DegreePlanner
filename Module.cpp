#include "Module.h"


//constructor 1
Module::Module(): moduleName(""),size(0){
    courses = new Course[100];
}

//constructor 2
Module::Module(std::string moduleName_): moduleName(moduleName_), size(0){
    courses = new Course[100];
    reqCourses = new std::string[100];
    reqCourseSize = 0;
}
  

// deletes the courses list
Module::~Module(){
    delete[] courses;
}


//returns class By number
Course Module::getClass(int number){


    return courses[number];
}

void Module::setClassByIndex(Course newCourse,int index){
    courses[index] = newCourse;
}

//returns class by name
Course Module::getClassByName(std::string className){
    Course toReturn = courses[0];
    for(int i = 0; i < size+1; i++){
        std::string currCourseName = (courses[i]).getClassName();

        if (currCourseName == className){
            toReturn = courses[i];
        }
    }
    return toReturn;
}

int Module::getClassIndex(std::string className){
    int toReturn = 0;
    for(int i = 0; i < size+1; i++){
        std::string currCourseName = (courses[i]).getClassName();

        if (currCourseName == className){
            toReturn = i;
        }
    }
    return toReturn;
}

//adds new class
void Module::addClass(std::string className_, int classYear_, std::string description_, Wt::WContainerWidget *courseContainer, Wt::WText *descriptionText){
    Course tempCourse(className_, classYear_, description_,courseContainer, descriptionText);
    courses[size] = tempCourse;
    size = size + 1;
}


Wt::WPushButton* Module::getCourseButton(std::string className){
    Course tempCourse = this -> getClassByName(className);
    return tempCourse.getClassButton();
}

Wt::WPushButton* Module::getCourseButton(int number){
    Course tempCourse = this -> getClass(number);
    return tempCourse.getClassButton();
}

void Module::SetClassxy(int x, int y, std::string className){
    int classIndex = getClassIndex(className);

    courses[classIndex].setxy(x,y);
}


void Module::addPreReq(std::string course1,std::string course2){

        courses[getClassIndex(course1)].addPreReq(course2);
    }


void Module::addReqCourse(std::string course1){

        reqCourses[reqCourseSize] = course1;
        ++reqCourseSize;
    }

    