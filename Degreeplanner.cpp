
#include "DegreePlanner.h"
#include <sqlite3.h>

bool createUserTable() {
    sqlite3* db; // Declare a pointer to a sqlite3 object, representing the SQLite database.

    int rc; // Declare an integer to store the return code from SQLite functions.

    rc = sqlite3_open("Degreedatabase.db", &db);
    if (rc) {
        return false;
    }


    std::string sql = "CREATE TABLE IF NOT EXISTS user ("
                      "email TEXT PRIMARY KEY, "
                      "password TEXT NOT NULL, "
                      "name TEXT NOT NULL);";


    rc = sqlite3_exec(db, sql.c_str(), 0, 0, 0);
    if (rc != SQLITE_OK) {


        sqlite3_close(db);
        return false;
    }


    sqlite3_close(db);
    return true;
}

bool createCoursesTable() {
    sqlite3* db;
    char* errMsg = 0;
    int rc;


    rc = sqlite3_open("Degreedatabase.db", &db);


    if (rc) {
        std::cerr << "Can't open database: " << sqlite3_errmsg(db) << std::endl;
        return false;
    }

    std::string sql = "CREATE TABLE IF NOT EXISTS Courses ("
                      "coursename TEXT PRIMARY KEY, "
                      "courseindex INT, "
                      "prereqsize INT, "
                      "courseYear INT, "
                      "graded INT, "
                      "description TEXT, "
                      "prereq_names TEXT);"; // Storing prereq_names as a TEXT, consider JSON or CSV format

    rc = sqlite3_exec(db, sql.c_str(), 0, 0, &errMsg);
    if (rc != SQLITE_OK) {
        std::cerr << "SQL error: " << errMsg << std::endl;
        sqlite3_free(errMsg);
        sqlite3_close(db);
        return false;
    }

    sqlite3_close(db);
    return true;
}

bool createGradesTable() {
    sqlite3* db;
    char* errMsg = 0;
    int rc;

    rc = sqlite3_open("Degreedatabase.db", &db);
    if (rc) {
        std::cerr << "Can't open database: " << sqlite3_errmsg(db) << std::endl;
        return false;
    }
    std::string sql1 = "DROP TABLE IF EXISTS Grades;";

    // Execute the SQL statement
    rc = sqlite3_exec(db, sql1.c_str(), 0, 0, &errMsg);

    std::string sql = "CREATE TABLE IF NOT EXISTS Grades ("
                      "UserEmail TEXT , "
                      "Grade INT, "
                      "Course TEXT);";

    rc = sqlite3_exec(db, sql.c_str(), 0, 0, &errMsg);
    if (rc != SQLITE_OK) {
        std::cerr << "SQL error: " << errMsg << std::endl;
        sqlite3_free(errMsg);
        sqlite3_close(db);
        return false;
    }

    sqlite3_close(db);
    return true;
}

bool createModuleTable() {
    sqlite3* db;
    char* errMsg = 0;
    int rc;

    rc = sqlite3_open("Degreedatabase.db", &db);
    if (rc) {
        std::cerr << "Can't open database: " << sqlite3_errmsg(db) << std::endl;
        return false;
    }

    std::string sql = "CREATE TABLE IF NOT EXISTS Modules ("
                      "ModuleName TEXT, "
                      "ModuleCourses TEXT, "
                      "ModuleReqCourses TEXT, "
                      "ModuleReqCourseSize INT, "
                      "ModuleCourseSize INT);";

    rc = sqlite3_exec(db, sql.c_str(), 0, 0, &errMsg);
    if (rc != SQLITE_OK) {
        std::cerr << "SQL error: " << errMsg << std::endl;
        sqlite3_free(errMsg);
        sqlite3_close(db);
        return false;
    }

    sqlite3_close(db);
    return true;
}



bool addUser(const std::string& email, const std::string& name, const std::string& password) {
    sqlite3* db;
    char* errMsg = 0;
    int rc;

    rc = sqlite3_open("Degreedatabase.db", &db);

    if (rc) {
        std::cerr << "Can't open database: " << sqlite3_errmsg(db) << std::endl;
        return false;
    }

    std::ostringstream sql;
    sql << "INSERT INTO user (email, name, password) VALUES ("
        << "'" << email << "', "
        << "'" << name << "', "
        << "'" << password << "');";

    rc = sqlite3_exec(db, sql.str().c_str(), 0, 0, &errMsg);
    if (rc != SQLITE_OK) {
        std::cerr << "SQL error: " << errMsg << std::endl;
        sqlite3_free(errMsg);
        sqlite3_close(db);
        return false;
    }

    sqlite3_close(db);
    return true;
}

bool addGrade1(const std::string& userEmail, int grade, const std::string& course) {
    sqlite3* db;
    char* errMsg = 0;
    int rc;

    rc = sqlite3_open("Degreedatabase.db", &db);
    if (rc) {
        std::cerr << "Can't open database: " << sqlite3_errmsg(db) << std::endl;
        return false;
    }

    std::string sql = "INSERT INTO Grades (UserEmail, Grade, Course) VALUES (?, ?, ?);";

    sqlite3_stmt *stmt;
    rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        std::cerr << "SQL error: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_close(db);
        return false;
    }


    sqlite3_bind_text(stmt, 1, userEmail.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 2, grade);
    sqlite3_bind_text(stmt, 3, course.c_str(), -1, SQLITE_STATIC);

    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE) {
        std::cerr << "Insertion failed: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return false;
    }


    sqlite3_finalize(stmt);
    sqlite3_close(db);
    return true;
}

bool removeGrade1(const std::string& userEmail, const std::string& course) {
    sqlite3* db;
    char* errMsg = 0;
    int rc;

    // Open the database
    rc = sqlite3_open("Degreedatabase.db", &db);
    if (rc) {
        std::cerr << "Can't open database: " << sqlite3_errmsg(db) << std::endl;
        return false;
    }

    // Prepare the SQL delete statement
    std::string sql = "DELETE FROM Grades WHERE UserEmail = ? AND Course = ?;";

    sqlite3_stmt *stmt;
    rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        std::cerr << "SQL error: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_close(db);
        return false;
    }

    // Bind the email and course parameters to the prepared statement
    sqlite3_bind_text(stmt, 1, userEmail.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, course.c_str(), -1, SQLITE_STATIC);

    // Execute the delete statement
    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE) {
        std::cerr << "Deletion failed: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return false;
    }

    // Finalize and close the database
    sqlite3_finalize(stmt);
    sqlite3_close(db);
    return true;
}

std::string getPassword(const std::string& email) {
    sqlite3* db;
    sqlite3_stmt* stmt;
    int rc;

    rc = sqlite3_open("Degreedatabase.db", &db);
    if (rc) {
        std::cerr << "Can't open database: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_close(db);
        throw std::runtime_error("Can't open database");
    }

    std::string sql = "SELECT password FROM user WHERE email = ?";
    rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, 0);
    if (rc != SQLITE_OK) {
        std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_close(db);
        throw std::runtime_error("Failed to prepare statement");
    }

    sqlite3_bind_text(stmt, 1, email.c_str(), -1, SQLITE_STATIC);
    std::string password;
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        password = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
    } else {
        std::cerr << "Email not found." << std::endl;
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);
    return password;
}
int getGrade(const std::string& email, const std::string& course) {
    int grade = 0 ;
    sqlite3* db;
    sqlite3_stmt* stmt;
    int rc;

    rc = sqlite3_open("Degreedatabase.db", &db);
    if (rc) {
        std::cerr << "Can't open database: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_close(db);
        throw std::runtime_error("Can't open database");
    }

    std::string sql = "SELECT Grade FROM Grades WHERE UserEmail = ? AND Course = ?";
    rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, 0);
    if (rc != SQLITE_OK) {
        std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_close(db);
        throw std::runtime_error("Failed to prepare statement");
    }

    sqlite3_bind_text(stmt, 1, email.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, course.c_str(), -1, SQLITE_STATIC);

    if (sqlite3_step(stmt) == SQLITE_ROW) {
        grade = sqlite3_column_int(stmt, 0);
    } else {
        std::cerr << "Email not found." << std::endl;
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);
    return grade;
}

bool getGradesAndCourses(const std::string& userEmail, std::vector<int>& grades, std::vector<std::string>& courses) {
    sqlite3* db;
    char* errMsg = 0;
    int rc;

    // Open the database
    rc = sqlite3_open("Degreedatabase.db", &db);
    if (rc) {
        std::cerr << "Can't open database: " << sqlite3_errmsg(db) << std::endl;
        return false;
    }

    // Prepare the SQL select statement
    std::string sql = "SELECT Grade, Course FROM Grades WHERE UserEmail = ?;";
    sqlite3_stmt *stmt;
    rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        std::cerr << "SQL error: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_close(db);
        return false;
    }

    // Bind the user email parameter to the prepared statement
    sqlite3_bind_text(stmt, 1, userEmail.c_str(), -1, SQLITE_STATIC);

    // Execute the select statement and process the rows
    while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
        int grade = sqlite3_column_int(stmt, 0);
        std::string course = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));

        grades.push_back(grade);
        courses.push_back(course);
    }

    if (rc != SQLITE_DONE) {
        std::cerr << "Query failed: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return false;
    }

    // Finalize and close the database
    sqlite3_finalize(stmt);
    sqlite3_close(db);
    return true;
}



Wt::WText *description ;
Wt::WText *description1 ;
Wt::WText *courseName ;
Wt::WLineEdit* gradeInput;
Wt::WTable* table;
std::string preReqs ="";

int currentRequiredMark = 50;


std::string honSpecString = "This is the program that will give you the most concentrated background and deep knowledge of almost all areas of computer science. In order to receive the Honors designation, you must maintain a 70% average and you must get at least a 60% in every required course.";
std::string specString = "This program will give a strong background in almost all areas of computer science. You could learn about graphics, networks, security, databases, the internet, computer law. This module offers a wealth of possiblities!";
std::string majorString ="This program provides a solid background in computer science, covering the foundational elements of the discipline. While perhaps best used as part of a Double Major (see Add-On Modules for Other Degrees), this module can be used as a basis for a degree in Computer Science as well.";
std::string minorString ="Consider this program if you are planning another degree and you want a solid background in computer science principles.";
unsigned gradeCounter;
unsigned removed;

std::string courseTake__ = "Course Taken? Add the course with your grade, make sure it is over : ";


std::string honSpecName ="Honors Specialization in Computer Science";
std::string specName = "Specialization in Computer Science";
std::string majorName = "Major in Computer Science";
std::string minorName = "Minor in Computer Science";
std::string requiredC ="";


std::string newD;
std::string* newD_ = new std::string[100];


//
namespace {
    struct Employee {
        std::string firstName;
        std::string lastName;
        double pay;

        Employee(const std::string& aFirstName,
                 const std::string& aLastName,
                 double aPay)
                : firstName(aFirstName),
                  lastName(aLastName),
                  pay(aPay) { }
    };

    Employee employees[] = {
            Employee("CS1027", "0.5", 100),
            Employee("CS1033", "0.5", 50),
            Employee("CS2210", "0.5", 10)
    };
}
using namespace Wt;
//









Module module("MAIN MODULE");

Module honSpecM("HonSpec");
Module specM("Spec");
Module majorM("Major");
Module minorM("Minor");


void addButtonWithClickHandler(Wt::WContainerWidget* container, std::string ButtonName,std::string css_, std::string tempD,Wt::WText* descriptionText) {


    auto button = container->addWidget(std::make_unique<Wt::WPushButton>(ButtonName));
    button ->setAttributeValue("style",css_);
    button->clicked().connect([=] {
        descriptionText -> setText(tempD);
        return;
    });
}

void DegreePlanner::buttonClickHandler(int x, Wt::WContainerWidget* con){

    auto button = con->widget(x);
    button->setStyleClass("courseButtonReq");

}

void DegreePlanner::defaultButton(int x, Wt::WContainerWidget* con){
    auto button = con->widget(x);
    button->setStyleClass("courseButton");
}


//sets buttons to orange
void DegreePlanner::courseTakenButton(int x, Wt::WContainerWidget* con){

    auto button = con->widget(x);
    button->setStyleClass("courseButtonTaken");
}


void DegreePlanner::autoCourseTakenButton(Wt::WContainerWidget * con){
    int courseSize = currentUser -> returnGradeSize();
    for(int i = 0; i < courseSize; i++){

        courseTakenButton((module.getClassByName((currentUser -> returnGradeByIndex(i)).getClassName())).getButtonIndex(),con);
    }
}

void moduleButtonHandler(Wt::WPushButton* button, Wt::WContainerWidget* con1, Wt::WContainerWidget* con2){
    button->clicked().connect([=]() {
        for(int i = 0; i<honSpecM.getReqCourseSize();i++){
        }
        con1 -> setHidden(false);
        con2 -> setHidden(true);
    });
}

int graded = 0;
//Wt::WPushButton * grade, Wt::WPushButton * removeGrade

void courseButtonOnClick(Wt::WPushButton* button,Wt::WText* descriptionText, std::string tempD, std::string className,Wt::WContainerWidget* container, int index,Wt::WPushButton * grade, Wt::WPushButton * removeGrade ) {

    button->clicked().connect([=] {

        preReqs ="";

        for(int i = 0; i< (module.getClassByName(className)).getPreReqSize(); i++){
            preReqs = preReqs + " " + (module.getClassByName(className)).getPreReq(i);
        }
        if((module.getClassByName(className)).getGraded() == 0){
            removeGrade -> setHidden(true);
            grade -> setHidden(false);
            gradeInput -> setHidden(false);

        }
        if((module.getClassByName(className)).getGraded() == 1){
            grade -> setHidden(true);
            removeGrade ->setHidden(false);
            gradeInput -> setHidden(true);
        }
        courseName -> setText(className);
        descriptionText -> setText(tempD);
        description1 -> setText(preReqs);
        container -> setHidden(false);


    });
}



void makeWeb(Wt::WContainerWidget* container, Course course1, Course course2) {


    int x = course2.getX()+10 - course1.getX();
    int y = course2.getY()+10 - course1.getY();


    int length = ceil(sqrt(pow(x,2)+pow(y,2)));

    float angle = asin(float(y)/length);
    auto web = container->addWidget(std::make_unique<Wt::WContainerWidget>());
    web -> setStyleClass("honspec");
    std::string width = std::to_string(length);


    std::string angle_ = std::to_string(angle);

    std::string top = std::to_string(course1.getY()+50);
    std::string left = std::to_string(course1.getX()+150);
    web -> setAttributeValue("style", "transform-origin: top left;position:fixed; width:"+width+"px; top:"+top+"px; left:"+left+"px;transform: rotate("+angle_+"rad); height: 1px; background-color: black;");

}



// LOGIN METHOD

void DegreePlanner::login(Wt::WContainerWidget * con){


    // Title

}


//global variables.
std::vector<int> memory;

std::vector<std::string> words;

std::string correctWord ;

DegreePlanner::DegreePlanner(const Wt::WEnvironment& env)
        : WApplication(env)
{



    Wt::WApplication::instance()->useStyleSheet("main.css");
    createGradesTable();
    createUserTable();
    createModuleTable();
    createCoursesTable();

    addGrade1("Koroush", 87, "Biology");




    // initializes the main Container (private instance of Degree planner)
    mainContainer = root()->addNew<Wt::WContainerWidget>();
    mainContainer -> setStyleClass("mainContainer");


    setTitle("Degree Planner");
    Wt::WApplication::instance()->useStyleSheet("main.css");



    /*
    *
    *
    *
    *
    *
    *
    *
    */

    // CODE FOR MODULES


    // Code for user input


    Wt::WContainerWidget* container = root() ->addNew<Wt::WContainerWidget>();
    container->setStyleClass("logs-container");

    auto title = container->addNew<Wt::WText>("Western Login");
    title->setStyleClass("title");

    // Username input
    auto userNameEdit = container->addNew<Wt::WLineEdit>();
    userNameEdit->setStyleClass("form-group");
    userNameEdit->setPlaceholderText("Username or email");

    // Password input
    auto passwordEdit = container->addNew<Wt::WLineEdit>();
    passwordEdit->setStyleClass("form-group");
    passwordEdit->setPlaceholderText("Password");
    passwordEdit->setEchoMode(Wt::EchoMode::Password);

    auto loginButton = container->addNew<Wt::WPushButton>("Log in");
    Wt::WContainerWidget *SignUpContainer = root()->addNew<Wt::WContainerWidget>();
    SignUpContainer-> setStyleClass("signs-container");
    auto SignUpButton = container->addNew<Wt::WPushButton>("Sign Up");
    auto emailLabel = SignUpContainer->addWidget(std::make_unique<Wt::WText>("Email: "));
    auto emailEdit = SignUpContainer->addWidget(std::make_unique<Wt::WLineEdit>());
    auto usernameWarning = SignUpContainer->addWidget(std::make_unique<Wt::WText>(""));
    emailEdit->setPlaceholderText("Enter your email");

// Create and add username input field
    auto usernameLabel = SignUpContainer->addWidget(std::make_unique<Wt::WText>("Username: "));
    auto usernameEdit = SignUpContainer->addWidget(std::make_unique<Wt::WLineEdit>());

    usernameEdit->setPlaceholderText("Enter your username");



// Create and add password input field
    auto passwordLabel = SignUpContainer->addWidget(std::make_unique<Wt::WText>("Password: "));
    auto passwordEdit2 = SignUpContainer->addWidget(std::make_unique<Wt::WLineEdit>());
    passwordEdit2->setPlaceholderText("Enter your password");
    passwordEdit2->setEchoMode(Wt::EchoMode::Password);
    SignUpContainer->setHidden(true);
    currentUser = new User("","");

    loginButton->clicked().connect([=] {
        // Implement authentication logic here
        std::string email = userNameEdit->text().toUTF8();
        Wt::WString password = passwordEdit->text();
        if (getPassword(email) == password) { // Placeholder for actual authentication
            currentUser -> setName(userNameEdit->text().toUTF8());
            currentUser -> setPassword(passwordEdit->text().toUTF8());

            userNameEdit->setText("");
            passwordEdit->setText("");
            mainContainer->setHidden(false);
            container ->setHidden(true);

            std::vector<int> tempGrades_;
            std::vector<std::string> tempCourses_;
            std::cout<<currentUser -> getUsername()<<std::endl;


            getGradesAndCourses(email,tempGrades_,tempCourses_);

            std::cout<<tempGrades_.size()<<std::endl;

            for (size_t i = 0; i < tempGrades_.size(); ++i) {

                currentUser -> addNewGrade(tempCourses_[i],tempGrades_[i]);
                std::cout<<tempCourses_[i]<<std::endl;

            }








        } else {
            // Login failed
        }
    })
            ;
    auto inSignUpButton = SignUpContainer->addWidget(std::make_unique<Wt::WPushButton>("Sign Up"));
    auto CloseButton = SignUpContainer->addWidget(std::make_unique<Wt::WPushButton>("Close"));



    SignUpButton->clicked().connect([=] {
        usernameWarning->setText("");


        // Click handler for the newly created inner button

        inSignUpButton->clicked().connect([=] {
            //usernameWarning->setText("");

            std::string email = emailEdit->text().toUTF8();
            std::string user = usernameEdit->text().toUTF8();
            std::string pass = passwordEdit2->text().toUTF8();

            bool success = addUser(email, user, pass);

            if (success){
                //usernameWarning->setText("");
                emailEdit->setText("");
                usernameEdit->setText("");
                passwordEdit2->setText("");
                SignUpContainer->setHidden(true);
            }else{
                emailEdit->setText("");
                usernameEdit->setText("");
                passwordEdit2->setText("");
                usernameWarning->setText("Email Already Exists");


            }

        });

        CloseButton->clicked().connect([=] {
            usernameWarning->setText("");
            SignUpContainer->setHidden(true);

        });



        SignUpContainer->setHidden(false);

    });


    //login(root());


    Wt::WContainerWidget * moduleContainer = mainContainer->addNew<Wt::WContainerWidget>();


    moduleContainer -> setStyleClass("moduleContainer");

    Wt::WPushButton *honSpec = moduleContainer->addNew<Wt::WPushButton>("Honor Specialization");
    Wt::WPushButton *spec = moduleContainer->addNew<Wt::WPushButton>("Specialization");
    Wt::WPushButton *major = moduleContainer->addNew<Wt::WPushButton>("Major");
    Wt::WPushButton *minor = moduleContainer->addNew<Wt::WPushButton>("Minor");




    honSpec -> setStyleClass("honspec");
    spec -> setStyleClass("spec");
    major -> setStyleClass("majorButton");
    minor -> setStyleClass("minorButton");





    mainContainer->setHidden(true);


    // iniialize courses taken if already existing






    // MODULE CODE ENDS
    /*
    *
    *
    *
    *
    *
    *
    *
    *
    */
    //REQUIRED COURSES CONATINER


    // This following section sets up the different Year buttons




    // This is the end of the section above


    // This follows section sets up the display the courses in the left navigation bar as well as the main course display

    gradeCounter = 0;
    int average = 0 ;


    Wt::WContainerWidget *reqCoursesContainer = mainContainer->addNew<Wt::WContainerWidget>();

    reqCoursesContainer -> setStyleClass("reqCoursesContainer");

    //Course Description Section  pop up

    Wt::WContainerWidget *coursesDescription = mainContainer->addNew<Wt::WContainerWidget>();

    coursesDescription -> setStyleClass("coursesDescription");

    Wt::WContainerWidget *coursesDescription_ = coursesDescription->addNew<Wt::WContainerWidget>();
    coursesDescription_ -> setStyleClass("coursesDescription_");

    coursesDescription -> setAttributeValue("style", "position: fixed; top: 50%; left: 50%; transform: translate(-50%, -50%);");
    coursesDescription -> setHidden(true);
    //coursesDescription -> setHidden(true);
    Wt::WPushButton *back__ = coursesDescription->addNew<Wt::WPushButton>("Back");
    back__ -> setStyleClass("courseYearButton");
    back__ ->setAttributeValue("style", "position:fixed; top:0%; left:90%; height:5%; width: 10%; font-size: 10px;");
    back__->clicked().connect([=]() {
        coursesDescription -> setHidden(true);
    });
    courseName = coursesDescription_ -> addNew<Wt::WText>("#");
    courseName -> setAttributeValue("style","top:20%");

    coursesDescription -> addNew <Wt::WBreak>();

    Wt::WText *addText1 = coursesDescription->addNew<Wt::WText>("Prerequisites");
    addText1 -> setAttributeValue("style","postion:relative;transform: translate(-50%, 50%);");
    coursesDescription -> addNew <Wt::WBreak>();


    Wt::WContainerWidget *preReqList = coursesDescription->addNew<Wt::WContainerWidget>();

    preReqList -> setStyleClass("coursesDescription_");
    description1 = preReqList -> addNew<Wt::WText>("PREREQS");


    coursesDescription -> addNew <Wt::WBreak>();
    coursesDescription -> addNew <Wt::WBreak>();

    Wt::WText *addText = coursesDescription->addNew<Wt::WText>("Course Taken? Add the course with your grade, make sure it is over : ");

    coursesDescription_ -> addNew <Wt::WBreak>();
    description = coursesDescription_ -> addNew<Wt::WText>("#");

    gradeInput = coursesDescription->addNew<Wt::WLineEdit>("0");

    gradeInput -> setStyleClass("gradeInput");
    gradeInput -> setAttributeValue("style","position:relative;");
    coursesDescription -> addNew <Wt::WBreak>();

    coursesDescription -> addNew <Wt::WBreak>();

    Wt::WPushButton *addGrade = coursesDescription->addNew<Wt::WPushButton>("Add Course and Grade");
    addGrade -> setStyleClass ("addGradeButton");
    addGrade -> setAttributeValue("style","position:relative;");

    Wt::WPushButton *removeGrade = coursesDescription->addNew<Wt::WPushButton>("Remove Course and Grade");
    removeGrade -> setStyleClass ("addGradeButton");
    removeGrade -> setAttributeValue("style","position:relative;");


    addText -> setAttributeValue("style","postion:relative;transform: translate(-50%, 50%);");

    removeGrade -> setHidden(true);
    addGrade -> setHidden(true);



    //


    //Containers for each Module's courses separated by year

    // The Following is the code for the courses in the computer science program

    module.addClass("CS1026",1,"CS 1026 provides an introduction to the basic concepts of computer programming and program design. It is intended for students who have interest in learning basic programming skills including those who intend to study Computer Science. This course assumes no previous programming background, although having some experience with programming is an asset. Programming skills will be developed using the Python programming language. ",reqCoursesContainer,description);
    module.addClass("CS1027",1,"Course topics include: data types, variables, expressions, program constructs, strings (text), functions, basic data structures (lists, tuples, sets dictionaries), objects, object‐oriented design, classes, modularity, and problem‐solving techniques. Programming examples and assignments are taken from a variety of disciplines.",reqCoursesContainer,description);
    module.addClass("CALC1000",1,"Course topics include: data types, variables, expressions, program constructs, strings (text), functions, basic data structures (lists, tuples, sets dictionaries), objects, object‐oriented design, classes, modularity, and problem‐solving techniques. Programming examples and assignments are taken from a variety of disciplines.",reqCoursesContainer,description);
    module.addClass("MATH1600",1,"Course topics include: data types, variables, expressions, program constructs, strings (text), functions, basic data structures (lists, tuples, sets dictionaries), objects, object‐oriented design, classes, modularity, and problem‐solving techniques. Programming examples and assignments are taken from a variety of disciplines.",reqCoursesContainer,description);
    module.addClass("CALC1500",1,"Course topics include: data types, variables, expressions, program constructs, strings (text), functions, basic data structures (lists, tuples, sets dictionaries), objects, object‐oriented design, classes, modularity, and problem‐solving techniques. Programming examples and assignments are taken from a variety of disciplines.",reqCoursesContainer,description);
    module.addClass("CALC1501",1,"Course topics include: data types, variables, expressions, program constructs, strings (text), functions, basic data structures (lists, tuples, sets dictionaries), objects, object‐oriented design, classes, modularity, and problem‐solving techniques. Programming examples and assignments are taken from a variety of disciplines.",reqCoursesContainer,description);


    module.addClass("CS2208",2,"The purpose of this course is to give students an understanding of how a computer works (organization) and what a computer does (architecture). The course covers the internal representation of various data types, including characters, integers, and floating points. This course focuses on the architectural components of computers, how these components are interconnected, and the nature of the information flow between them. Assembly language will be used to reinforce these issues.",reqCoursesContainer,description);
    module.addClass("CS2209",2,"The course is an introduction to logic for computer science. We will cover propositional and predicate logic; representing static and dynamic properties of real-world systems; logic as a tool for representation, reasoning and calculation; logic and programming.",reqCoursesContainer,description);
    module.addClass("CS2210",2,"The purpose of this course is to provide the students with solid foundations in the basic concepts of programming: data structures and algorithms. The main objective of the course is to teach the students how to select and design data structures and algorithms that are appropriate for problems that they might encounter. This course is also about showing the correctness of algorithms and studying their computational complexities. This course offers the students a mixture of theoretical knowledge and practical experience. The study of data structures and algorithms is carried out within an object-oriented framework. When implementations are considered, the Java programming language is used. Topics covered in this course include:  Analysis of algorithms, Dictionaries, hash tables, Trees, binary search trees, AVL trees, multi-way search trees, (2,4)-trees, B-trees, Graphs, graph traversals, graph algorithms, Sorting.",reqCoursesContainer,description);
    module.addClass("CS2211",2,"This course provides an introduction to software tools and systems level programming. Topics include: understanding how programs run (compilation, linking, and loading), an introduction to a complex operating system (UNIX), scripting languages, and the C programming language. As time permits, other topics will be chosen from: system calls, memory management, libraries, multi-component program organization and builds, version control, debuggers and profilers.",reqCoursesContainer,description);
    module.addClass("CS2212",2,"The informal approaches that most individual programmers use when writing small programs do not work very well when applied to the development of large pieces of software and team programming situations. Software engineering is a discipline that applies principles of traditional engineering to improve software, as well as its development and maintainability. In this course, we will examine the stages of the software engineering process, including requirements gathering, specification, design, implementation, and testing. The principles of object---oriented design and analysis and user interface design will be stressed, while a term project completed within a team of 4 students will serve to reinforce concepts learned and give students practical experience developing software in a team environment. UML (Unified Modeling Language), the standard tool for expressing designs in software engineering, will be introduced.	 All programming for this course will be done in Java.",reqCoursesContainer,description);
    module.addClass("CS2214",2,"This course presents an introduction to the mathematical foundations of computer science, with an emphasis on mathematical reasoning, combinatorial analysis, discrete structures, applications and modeling, and algorithmic thinking. Topics may include sets, functions, relations, algorithms, number theory, matrices, mathematical reasoning, counting, graphs and trees. ",reqCoursesContainer,description);
    module.addClass("W2101",2,"This course presents an introduction to the mathematical foundations of computer science, with an emphasis on mathematical reasoning, combinatorial analysis, discrete structures, applications and modeling, and algorithmic thinking. Topics may include sets, functions, relations, algorithms, number theory, matrices, mathematical reasoning, counting, graphs and trees. ",reqCoursesContainer,description);
    module.addClass("W2111",2,"This course presents an introduction to the mathematical foundations of computer science, with an emphasis on mathematical reasoning, combinatorial analysis, discrete structures, applications and modeling, and algorithmic thinking. Topics may include sets, functions, relations, algorithms, number theory, matrices, mathematical reasoning, counting, graphs and trees. ",reqCoursesContainer,description);
    module.addClass("W2125",2,"This course presents an introduction to the mathematical foundations of computer science, with an emphasis on mathematical reasoning, combinatorial analysis, discrete structures, applications and modeling, and algorithmic thinking. Topics may include sets, functions, relations, algorithms, number theory, matrices, mathematical reasoning, counting, graphs and trees. ",reqCoursesContainer,description);
    module.addClass("W2131",2,"This course presents an introduction to the mathematical foundations of computer science, with an emphasis on mathematical reasoning, combinatorial analysis, discrete structures, applications and modeling, and algorithmic thinking. Topics may include sets, functions, relations, algorithms, number theory, matrices, mathematical reasoning, counting, graphs and trees. ",reqCoursesContainer,description);
    module.addClass("MATH2155",2,"This course presents an introduction to the mathematical foundations of computer science, with an emphasis on mathematical reasoning, combinatorial analysis, discrete structures, applications and modeling, and algorithmic thinking. Topics may include sets, functions, relations, algorithms, number theory, matrices, mathematical reasoning, counting, graphs and trees. ",reqCoursesContainer,description);
    module.addClass("STATS2857",2,"This course presents an introduction to the mathematical foundations of computer science, with an emphasis on mathematical reasoning, combinatorial analysis, discrete structures, applications and modeling, and algorithmic thinking. Topics may include sets, functions, relations, algorithms, number theory, matrices, mathematical reasoning, counting, graphs and trees. ",reqCoursesContainer,description);
    module.addClass("STATS2244",2,"This course presents an introduction to the mathematical foundations of computer science, with an emphasis on mathematical reasoning, combinatorial analysis, discrete structures, applications and modeling, and algorithmic thinking. Topics may include sets, functions, relations, algorithms, number theory, matrices, mathematical reasoning, counting, graphs and trees. ",reqCoursesContainer,description);


    module.addClass("CS3305",3,"CS 1026 provides an introduction to the basic concepts of computer programming and program design. It is intended for students who have interest in learning basic programming skills including those who intend to study Computer Science. This course assumes no previous programming background, although having some experience with programming is an asset. Programming skills will be developed using the Python programming language. ",reqCoursesContainer,description);
    module.addClass("CS3307",3,"Course topics include: data types, variables, expressions, program constructs, strings (text), functions, basic data structures (lists, tuples, sets dictionaries), objects, object‐oriented design, classes, modularity, and problem‐solving techniques. Programming examples and assignments are taken from a variety of disciplines.",reqCoursesContainer,description);
    module.addClass("CS3331",3,"The purpose of this course is to provide the students with solid foundations in the basic concepts of programming: data structures and algorithms. The main objective of the course is to teach the students how to select and design data structures and algorithms that are appropriate for problems that they might encounter. This course is also about showing the correctness of algorithms and studying their computational complexities. This course offers the students a mixture of theoretical knowledge and practical experience. The study of data structures and algorithms is carried out within an object-oriented framework. When implementations are considered, the Java programming language is used. Topics covered in this course include:  Analysis of algorithms, Dictionaries, hash tables, Trees, binary search trees, AVL trees, multi-way search trees, (2,4)-trees, B-trees, Graphs, graph traversals, graph algorithms, Sorting.",reqCoursesContainer,description);
    module.addClass("CS3319",3,"The purpose of this course is to give students an understanding of how a computer works (organization) and what a computer does (architecture). The course covers the internal representation of various data types, including characters, integers, and floating points. This course focuses on the architectural components of computers, how these components are interconnected, and the nature of the information flow between them. Assembly language will be used to reinforce these issues.",reqCoursesContainer,description);
    module.addClass("CS3340",3,"This course provides an introduction to software tools and systems level programming. Topics include: understanding how programs run (compilation, linking, and loading), an introduction to a complex operating system (UNIX), scripting languages, and the C programming language. As time permits, other topics will be chosen from: system calls, memory management, libraries, multi-component program organization and builds, version control, debuggers and profilers.",reqCoursesContainer,description);
    module.addClass("CS3342",3,"The informal approaches that most individual programmers use when writing small programs do not work very well when applied to the development of large pieces of software and team programming situations. Software engineering is a discipline that applies principles of traditional engineering to improve software, as well as its development and maintainability. In this course, we will examine the stages of the software engineering process, including requirements gathering, specification, design, implementation, and testing. The principles of object---oriented design and analysis and user interface design will be stressed, while a term project completed within a team of 4 students will serve to reinforce concepts learned and give students practical experience developing software in a team environment. UML (Unified Modeling Language), the standard tool for expressing designs in software engineering, will be introduced.	 All programming for this course will be done in Java.",reqCoursesContainer,description);
    module.addClass("CS3350",3,"The course is an introduction to logic for computer science. We will cover propositional and predicate logic; representing static and dynamic properties of real-world systems; logic as a tool for representation, reasoning and calculation; logic and programming.",reqCoursesContainer,description);


    module.addClass("CS4000",4,"CS 1026 provides an introduction to the basic concepts of computer programming and program design. It is intended for students who have interest in learning basic programming skills including those who intend to study Computer Science. This course assumes no previous programming background, although having some experience with programming is an asset. Programming skills will be developed using the Python programming language. ",reqCoursesContainer,description);
    module.addClass("CS4001",4,"Course topics include: data types, variables, expressions, program constructs, strings (text), functions, basic data structures (lists, tuples, sets dictionaries), objects, object‐oriented design, classes, modularity, and problem‐solving techniques. Programming examples and assignments are taken from a variety of disciplines.",reqCoursesContainer,description);
    module.addClass("CS4002",4,"The purpose of this course is to provide the students with solid foundations in the basic concepts of programming: data structures and algorithms. The main objective of the course is to teach the students how to select and design data structures and algorithms that are appropriate for problems that they might encounter. This course is also about showing the correctness of algorithms and studying their computational complexities. This course offers the students a mixture of theoretical knowledge and practical experience. The study of data structures and algorithms is carried out within an object-oriented framework. When implementations are considered, the Java programming language is used. Topics covered in this course include:  Analysis of algorithms, Dictionaries, hash tables, Trees, binary search trees, AVL trees, multi-way search trees, (2,4)-trees, B-trees, Graphs, graph traversals, graph algorithms, Sorting.",reqCoursesContainer,description);
    module.addClass("CS4003",4,"The purpose of this course is to give students an understanding of how a computer works (organization) and what a computer does (architecture). The course covers the internal representation of various data types, including characters, integers, and floating points. This course focuses on the architectural components of computers, how these components are interconnected, and the nature of the information flow between them. Assembly language will be used to reinforce these issues.",reqCoursesContainer,description);
    module.addClass("CS4004",4,"This course provides an introduction to software tools and systems level programming. Topics include: understanding how programs run (compilation, linking, and loading), an introduction to a complex operating system (UNIX), scripting languages, and the C programming language. As time permits, other topics will be chosen from: system calls, memory management, libraries, multi-component program organization and builds, version control, debuggers and profilers.",reqCoursesContainer,description);
    module.addClass("CS4005",4,"The informal approaches that most individual programmers use when writing small programs do not work very well when applied to the development of large pieces of software and team programming situations. Software engineering is a discipline that applies principles of traditional engineering to improve software, as well as its development and maintainability. In this course, we will examine the stages of the software engineering process, including requirements gathering, specification, design, implementation, and testing. The principles of object---oriented design and analysis and user interface design will be stressed, while a term project completed within a team of 4 students will serve to reinforce concepts learned and give students practical experience developing software in a team environment. UML (Unified Modeling Language), the standard tool for expressing designs in software engineering, will be introduced.	 All programming for this course will be done in Java.",reqCoursesContainer,description);
    module.addClass("CS4006",4,"The course is an introduction to logic for computer science. We will cover propositional and predicate logic; representing static and dynamic properties of real-world systems; logic as a tool for representation, reasoning and calculation; logic and programming.",reqCoursesContainer,description);
    module.addClass("CS4490Z",4,"This course presents an introduction to the mathematical foundations of computer science, with an emphasis on mathematical reasoning, combinatorial analysis, discrete structures, applications and modeling, and algorithmic thinking. Topics may include sets, functions, relations, algorithms, number theory, matrices, mathematical reasoning, counting, graphs and trees. ",reqCoursesContainer,description);

    module.addPreReq("CS1027","CS1026");

    module.addPreReq("CS2208","CS1027");
    module.addPreReq("CS2209","CS1027"); module.addPreReq("CS2209","CALC1000"); module.addPreReq("CS2209","CALC1500"); module.addPreReq("CS2209","CALC1501");  module.addPreReq("CS2209","MATH1600");
    module.addPreReq("CS2210","CS1027"); module.addPreReq("CS2210","CALC1000"); module.addPreReq("CS2210","CALC1500"); module.addPreReq("CS2210","CALC1501");  module.addPreReq("CS2210","MATH1600");
    module.addPreReq("CS2211","CS1027");
    module.addPreReq("CS2212","CS2210"); module.addPreReq("CS2212","CS2211");

    module.addPreReq("CS2214","CALC1500"); module.addPreReq("CS2214","CALC1501");  module.addPreReq("CS2214","MATH1600");
    module.addPreReq("STATS2857","CALC1500"); module.addPreReq("STATS2857","CALC1501");  module.addPreReq("STATS2857","MATH1600");
    module.addPreReq("STATS2244","CALC1500"); module.addPreReq("STATS2244","CALC1501");  module.addPreReq("STATS2244","MATH1600");
    module.addPreReq("MATH2155","CALC1500"); module.addPreReq("MATH2155","CALC1501");  module.addPreReq("MATH2155","MATH1600");

    module.addPreReq("CS3331","CS2214"); module.addPreReq("CS3331","MATH2155");
    module.addPreReq("CS3307","CS2212"); module.addPreReq("CS3307","CS2210");  module.addPreReq("CS3307","CS2211");

    module.addPreReq("CS3305","CS2208"); module.addPreReq("CS3305","CS2210");  module.addPreReq("CS3305","CS2211");
    module.addPreReq("CS3340","CS2210"); module.addPreReq("CS3340","CS2211");  module.addPreReq("CS3340","CS2214"); module.addPreReq("CS3340","MATH2155");


    module.addPreReq("CS3342","CS2211");

    module.addPreReq("CS3350","CS2210"); module.addPreReq("CS3350","CS2211");  module.addPreReq("CS3350","CS2209"); module.addPreReq("CS3350","CS2208");



    honSpecM.addReqCourse("CS2209"); honSpecM.addReqCourse("CS2208"); honSpecM.addReqCourse("CS2210"); honSpecM.addReqCourse("CS2211"); honSpecM.addReqCourse("CS2212"); honSpecM.addReqCourse("CS2214"); honSpecM.addReqCourse("CS3331"); honSpecM.addReqCourse("CS3305");
    honSpecM.addReqCourse("CS3307"); honSpecM.addReqCourse("CS3340"); honSpecM.addReqCourse("CS3342"); honSpecM.addReqCourse("CS3350"); honSpecM.addReqCourse("CS4490Z"); honSpecM.addReqCourse("W2101"); honSpecM.addReqCourse("STATS2244");

    specM.addReqCourse("CS2209"); specM.addReqCourse("CS2208"); specM.addReqCourse("CS2210"); specM.addReqCourse("CS2211"); specM.addReqCourse("CS2212");   specM.addReqCourse("CS3305"); specM.addReqCourse("CS3307"); specM.addReqCourse("CS3331"); specM.addReqCourse("CS3342"); specM.addReqCourse("CS3350");
    specM.addReqCourse("CS2214"); specM.addReqCourse("W2101"); specM.addReqCourse("STATS2244");

    majorM.addReqCourse("CS2208"); majorM.addReqCourse("CS2209"); majorM.addReqCourse("CS2210"); majorM.addReqCourse("CS2211"); majorM.addReqCourse("CS2212"); majorM.addReqCourse("CS3305"); majorM.addReqCourse("CS3307"); majorM.addReqCourse("CS2214");

    minorM.addReqCourse("CS2208");  minorM.addReqCourse("CS2208"); minorM.addReqCourse("CS2208"); minorM.addReqCourse("CS2210"); minorM.addReqCourse("CS2211"); minorM.addReqCourse("CS2214");
    // End of the courses section


    // The following is the code for the positoning of each course button in the main screen

    int first_ = 0;
    int second_ = 0;
    int third_ = 0;
    int fourth_ = 0;

    for (int i = 0; i<module.getSize(); i++){

        int year = (module.getClass(i)).getClassYear();
        if ( year%4 == 1){
            ++first_;
            module.SetClassxy(200,first_*200,(module.getClass(i)).getClassName());
        }
        if ( year%4 == 2){
            ++second_;
            module.SetClassxy(700,second_*200,(module.getClass(i)).getClassName());
        }

        if ( year%4 == 3){
            ++third_ ;
            module.SetClassxy(1200,third_*200,(module.getClass(i)).getClassName());
        }

        if ( year%4 == 0){
            ++fourth_;
            module.SetClassxy(1700,fourth_*200,(module.getClass(i)).getClassName());

        }

    }


    for(int i = 0 ; i<module.getSize(); i++){
        for(int j = 0; j<  (module.getClass(i)).getPreReqSize() ;j++){
            makeWeb(reqCoursesContainer,module.getClassByName((module.getClass(i)).getPreReq(j)),module.getClass(i));
        }
    }

    autoCourseTakenButton(reqCoursesContainer);



    for (int i = 0; i<module.getSize(); i++){


        int year = (module.getClass(i)).getClassYear();


        if ( year%4 == 1){
            std::string space_ = std::to_string((module.getClass(i)).getY());
            std::string css_ =  "position:fixed; left: 200px;top:"+space_+"px";
            Wt::WPushButton* tempButton = reqCoursesContainer ->addNew<Wt::WPushButton>((module.getClass(i)).getClassName());
            tempButton -> setStyleClass("courseButton");
            tempButton -> setAttributeValue("style",css_);

            Course tempCourse = (module.getClass(i));
            tempCourse.setCourseButton(tempButton);

            tempCourse.setCourseButtonIndex((reqCoursesContainer->count())-1);

            module.setClassByIndex(tempCourse,i);
            courseButtonOnClick(tempButton,description,(module.getClass(i)).getDescription(),(module.getClass(i)).getClassName(),coursesDescription,(module.getClass(i)).getGraded(),addGrade,removeGrade);
        }

        (module.getClass(i)).getClassYear();
        if ( year%4 == 2){
            std::string space_ = std::to_string((module.getClass(i)).getY());
            std::string css_ =  "position:fixed;left: 700px; top:"+space_+"px;";

            Wt::WPushButton* tempButton = reqCoursesContainer ->addNew<Wt::WPushButton>((module.getClass(i)).getClassName());
            tempButton -> setStyleClass("courseButton");
            tempButton -> setAttributeValue("style",css_);
            Course tempCourse = (module.getClass(i));
            tempCourse.setCourseButton(tempButton);

            tempCourse.setCourseButtonIndex((reqCoursesContainer->count())-1);

            module.setClassByIndex(tempCourse,i);


            courseButtonOnClick(tempButton,description,(module.getClass(i)).getDescription(),(module.getClass(i)).getClassName(),coursesDescription,(module.getClass(i)).getGraded(),addGrade,removeGrade);
        }

        if ( year%4 == 3){
            std::string space_ = std::to_string((module.getClass(i)).getY());
            std::string css_ =  "position:fixed; left: 1200px; top:"+space_+"px;";
            Wt::WPushButton* tempButton =reqCoursesContainer ->addNew<Wt::WPushButton>((module.getClass(i)).getClassName());
            tempButton -> setStyleClass("courseButton");
            tempButton -> setAttributeValue("style",css_);
            Course tempCourse = (module.getClass(i));
            tempCourse.setCourseButton(tempButton);

            tempCourse.setCourseButtonIndex((reqCoursesContainer->count())-1);

            module.setClassByIndex(tempCourse,i); //

            courseButtonOnClick(tempButton,description,(module.getClass(i)).getDescription(),(module.getClass(i)).getClassName(),coursesDescription,(module.getClass(i)).getGraded(),addGrade,removeGrade);
        }

        if ( year%4 == 0){
            std::string space_ = std::to_string((module.getClass(i)).getY());
            std::string css_ =  "position:fixed; left: 1700px; top:"+space_+"px;";
            Wt::WPushButton* tempButton = reqCoursesContainer->addNew<Wt::WPushButton>((module.getClass(i)).getClassName());
            tempButton -> setStyleClass("courseButton");
            tempButton -> setAttributeValue("style",css_);
            Course tempCourse = (module.getClass(i));
            tempCourse.setCourseButton(tempButton);

            tempCourse.setCourseButtonIndex((reqCoursesContainer->count())-1);

            module.setClassByIndex(tempCourse,i);

            courseButtonOnClick(tempButton,description,(module.getClass(i)).getDescription(),(module.getClass(i)).getClassName(),coursesDescription,(module.getClass(i)).getGraded(),addGrade,removeGrade);

        }

    }

    // End of the years //


    //Module Function Code

    Wt::WContainerWidget *coursesContainer = mainContainer->addNew<Wt::WContainerWidget>();
    coursesContainer -> setStyleClass("coursesContainer");
    coursesContainer -> setHidden(false);
    Wt::WContainerWidget *moduleDescription = coursesContainer->addNew<Wt::WContainerWidget>();
    moduleDescription -> setStyleClass("moduleDescription");
    Wt::WContainerWidget *moduleDescription_ = moduleDescription->addNew<Wt::WContainerWidget>();
    moduleDescription_ -> setStyleClass("moduleDescription_");

    Wt::WPushButton *back_ = coursesContainer->addNew<Wt::WPushButton>("Back");
    back_ -> setStyleClass("courseYearButton");
    back_ ->setAttributeValue("style", "position:fixed; top:95%; left:0%; height: 5%; width: 100%;");

    Wt::WPushButton *gradesButton = coursesContainer->addNew<Wt::WPushButton>("See Your Grades");

    gradesButton -> setStyleClass("courseYearButton");
    gradesButton ->setAttributeValue("style", "position:fixed; top:80%; left:0%; height: 10%; width: 100%;");


    // Nor hence hoped her after other known defer his. For county now sister engage had season

    Wt::WText *addText3 = moduleDescription_->addNew<Wt::WText>("HELLO");
    moduleDescription_ -> addNew <Wt::WBreak>();
    moduleDescription_ -> addNew <Wt::WBreak>();

    addText3 -> setAttributeValue("style","postion:relative;transform: translate(-50%, 50%);font-size:30px; color: black;");

    Wt::WText *addText2 = moduleDescription_->addNew<Wt::WText>("");
    addText2 -> setAttributeValue("style","postion:relative;transform: translate(-50%, 50%);");

    moduleDescription_ -> addNew <Wt::WBreak>();
    moduleDescription_ -> addNew <Wt::WBreak>();

    Wt::WText *addText5 = moduleDescription_->addNew<Wt::WText>("Required Courses");
    addText5 -> setAttributeValue("style","postion:relative;transform: translate(-50%, 50%);font-size:30px; color:black;");

    moduleDescription_ -> addNew <Wt::WBreak>();
    moduleDescription_ -> addNew <Wt::WBreak>();

    Wt::WText *addText4 = moduleDescription_->addNew<Wt::WText>("");
    addText4 -> setAttributeValue("style","postion:relative;transform: translate(-50%, 50%);");



    coursesContainer -> setHidden(true);

    honSpec->clicked().connect([=]() {
        for(int i = 0; i<honSpecM.getReqCourseSize();i++){


            buttonClickHandler((module.getClassByName((honSpecM.getReqCourses())[i])).getButtonIndex(),reqCoursesContainer);
        }
        addText2 ->setText(honSpecString);
        addText3 ->setText(honSpecName);


        requiredC ="";

        for(int i = 0; i< honSpecM.getReqCourseSize(); i++){
            requiredC = requiredC + " " + honSpecM.getReqCourseIndex(i);
        }

        std::string tempTEXT = courseTake__ + std::to_string(70)+ "%.";

        addText -> setText(tempTEXT);
        currentRequiredMark = 70;

        addText4 ->setText(requiredC);
        coursesContainer -> setHidden(false);
        moduleContainer -> setHidden(true);
        autoCourseTakenButton(reqCoursesContainer);

    });
    spec->clicked().connect([=]() {
        for(int i = 0; i<specM.getReqCourseSize();i++){
            buttonClickHandler((module.getClassByName((specM.getReqCourses())[i])).getButtonIndex(),reqCoursesContainer);
        }
        addText2 ->setText(specString);
        addText3 ->setText(specName);

        requiredC ="";

        for(int i = 0; i< specM.getReqCourseSize(); i++){
            requiredC = requiredC + " " + specM.getReqCourseIndex(i);
        }

        std::string tempTEXT = courseTake__ + std::to_string(60)+ "%.";
        currentRequiredMark = 60;

        addText -> setText(tempTEXT);
        addText4 ->setText(requiredC);
        coursesContainer -> setHidden(false);
        moduleContainer -> setHidden(true);
        autoCourseTakenButton(reqCoursesContainer);
    });

    major->clicked().connect([=]() {
        for(int i = 0; i< majorM.getReqCourseSize();i++){
            buttonClickHandler((module.getClassByName((majorM.getReqCourses())[i])).getButtonIndex(),reqCoursesContainer);
        }
        addText2 ->setText(majorString);
        addText3 ->setText(majorName);

        requiredC ="";

        for(int i = 0; i< majorM.getReqCourseSize(); i++){
            requiredC = requiredC + " " + majorM.getReqCourseIndex(i);
        }


        currentRequiredMark = 60;
        addText4 ->setText(requiredC);
        coursesContainer -> setHidden(false);
        moduleContainer -> setHidden(true);
        autoCourseTakenButton(reqCoursesContainer);
        std::string tempTEXT = courseTake__ + std::to_string(60)+ "%.";

        addText -> setText(tempTEXT);
    });

    minor ->clicked().connect([=]() {
        for(int i = 0; i< minorM.getReqCourseSize();i++){
            buttonClickHandler((module.getClassByName((minorM.getReqCourses())[i])).getButtonIndex(),reqCoursesContainer);
        }
        addText2 ->setText(minorString);
        addText3 ->setText(minorName);

        requiredC ="";

        for(int i = 0; i< minorM.getReqCourseSize(); i++){
            requiredC = requiredC + " " +  minorM.getReqCourseIndex(i);
        }

        addText4 ->setText(requiredC);
        coursesContainer -> setHidden(false);
        moduleContainer -> setHidden(true);
        currentRequiredMark = 60;
        autoCourseTakenButton(reqCoursesContainer);
        std::string tempTEXT = courseTake__ + std::to_string(60)+ "%.";

        addText -> setText(tempTEXT);
    });

    back_->clicked().connect([=]() {
        coursesContainer -> setHidden(true);
        moduleContainer -> setHidden(false);
        coursesContainer -> setHidden(true);
        for(int i = 0; i<module.getSize();i++){
            defaultButton((module.getClass(i)).getButtonIndex(),reqCoursesContainer);
        }
        autoCourseTakenButton(reqCoursesContainer);


        std::string tempTEXT = courseTake__ +  "%.";

        addText -> setText(tempTEXT);
    });







    //Year Button Code Ends




    //REQUIRED COURSES CONTAINER
    /*
    *
    *
    *
    *
    *
    *
    *
    *
    *
    *
    *
    */
    //Grades Container
    Wt::WContainerWidget *gradeContainer = mainContainer->addNew<Wt::WContainerWidget>();

    gradeContainer -> setStyleClass("gradeContainer");
    gradeContainer -> setAttributeValue("style", "position: fixed; top: 50%; left: 50%; transform: translate(-50%, -50%);");

    gradeContainer -> setHidden(true);

    Wt::WPushButton *backGrade = gradeContainer->addNew<Wt::WPushButton>("Back");
    backGrade -> setStyleClass("courseYearButton");
    backGrade ->setAttributeValue("style", "position:fixed; top:0%; left:80%; height:10%; width: 20%; font-size: 10px; color: white;");
    backGrade->clicked().connect([=]() {
        gradeContainer -> setHidden(true);
    });

    gradesButton->clicked().connect([=]() {

        gradeContainer -> setHidden(false);
    });


    removed = 00;

    auto avergaeText = gradeContainer->addWidget(std::make_unique<Wt::WText>("average"));
    avergaeText ->setStyleClass("averageText");
    table = gradeContainer -> addNew<Wt::WTable>();
    table->setHeaderCount(1);
    table->setWidth(Wt::WLength("100%"));

    table->elementAt(0, 0)->addNew<Wt::WText>("#");
    table->elementAt(0, 1)->addNew<Wt::WText>("Course");
    table->elementAt(0, 2)->addNew<Wt::WText>("Weight");
    table->elementAt(0, 3)->addNew<Wt::WText>("Grade");

    gradeCounter = 1;
    int totalcount ;




    avergaeText->setText("Average: " + std::to_string(0));
    addGrade->clicked().connect([=]()mutable {

        int num = std::stoi(gradeInput ->text());
        if(num>=currentRequiredMark && 100>=num){
            table->elementAt(gradeCounter, 0)
                    ->addNew<Wt::WText>(Wt::WString("{1}").arg("-"));
            table->elementAt(gradeCounter, 1)
                    ->addNew<Wt::WText>(courseName ->text());
            table->elementAt(gradeCounter, 2)
                    ->addNew<Wt::WText>("0.5");
            table->elementAt(gradeCounter, 3)
                    ->addNew<Wt::WText>(gradeInput -> text());

            std::string str = (avergaeText->text()).toUTF8();
            str = str.substr(str.length() - 2);
            int num_ = std::stoi(str);
            totalcount = (num_)*(gradeCounter-removed) +num ;

            average = totalcount /(gradeCounter-removed) ;

            // Update the text of the WText widget
            avergaeText->setText("Average: " + std::to_string(average));
            gradeCounter = gradeCounter +1;


            std::string changeGrade_ = (courseName ->text()).toUTF8();

            Course temp = module.getClassByName(changeGrade_);

            temp.setGraded();
            currentUser -> addNewGrade(changeGrade_,num);
            module.setClassByIndex(temp,module.getClassIndex(changeGrade_));
            coursesDescription -> setHidden(true);
            courseTakenButton((module.getClassByName(changeGrade_)).getButtonIndex(),reqCoursesContainer);

            addGrade1("user",num,changeGrade_);
            std::cout<<currentUser -> getUsername()<<std::endl;
            std::cout<<num<<std::endl;
            std::cout<<changeGrade_<<std::endl;
        }



    });

    removeGrade -> clicked().connect([=]()mutable {

        for (int i = 0 ; i< gradeCounter; i++){

            Wt::WText* gradedClassName = dynamic_cast<Wt::WText*>((table->elementAt(i, 1))->widget(0));
            std::string changeGrade_ = (gradedClassName ->text()).toUTF8();
            if((courseName ->text())== changeGrade_){
                Wt::WTableRow *row_ = table->rowAt(i);
                row_->hide();
                gradedClassName->setText("");
                gradedClassName = dynamic_cast<Wt::WText*>((table->elementAt(i, 3))->widget(0));

                changeGrade_ = (gradedClassName ->text()).toUTF8();

                std::string str = (avergaeText->text()).toUTF8();
                str = str.substr(str.length() - 2);
                ++removed;
                //std::cout<<removed<<std::endl;

                average = ((std::stoi(str)*(gradeCounter-removed))-std::stoi(changeGrade_))/(gradeCounter-removed);
                totalcount = totalcount-std::stoi(changeGrade_);
                if(average <10){

                    avergaeText->setText("Average: " +std::to_string(0)+ std::to_string(average));

                }
                else{
                    avergaeText->setText("Average: " + std::to_string(average));
                }
                currentUser -> deleteGrade(changeGrade_);
                std::cout<< currentUser -> returnGradeSize()<<std::endl;
                std::cout<< (currentUser -> returnGradeByIndex(0)).getClassName()<<std::endl;
                //removeGrade1("user",changeGrade_);

            }
        }
        //gradeCounter = gradeCounter -1;
        std::string changeGrade_ = (courseName ->text()).toUTF8();

        Course temp = module.getClassByName(changeGrade_);

        temp.setUngraded();
        module.setClassByIndex(temp,module.getClassIndex(changeGrade_));
        coursesDescription -> setHidden(true);
        defaultButton((module.getClassByName(changeGrade_)).getButtonIndex(),reqCoursesContainer);


    });

    //gradeContainer->addWidget(std::move(table));




    //Grades Container
    /*
    *
    *
    *
    *
    *
    *
    *
    *
    *
    *
    *
    *
    */

    // Nav Bar

    // log out button
    Wt::WContainerWidget *navContainer = mainContainer->addNew<Wt::WContainerWidget>();
    navContainer -> setStyleClass("navContainer");

    Wt::WContainerWidget *navSep = navContainer->addNew<Wt::WContainerWidget>();
    navSep -> setStyleClass("navSep");

    Wt::WPushButton *logout = navContainer->addNew<Wt::WPushButton>("Logout");
    logout ->setStyleClass("logout");

    logout -> setAttributeValue("style", "position:fixed; top:0%; left:0%; height:100%; width: 15%; font-size: 10px; color: white; background-color: black");
    logout->clicked().connect([=] {

        mainContainer->setHidden(true);
        container ->setHidden(false);


    });

    // Nav Bar
    /*
    *
    *
    *
    *
    *
    *
    *
    */
    Wt::WContainerWidget *adminPageContainer = navContainer->addNew<Wt::WContainerWidget>();
    adminPageContainer->setStyleClass("adminContainer");
    //adminPageContainer->setHidden(true);
    Wt::WText *warningMessage = adminPageContainer->addNew<Wt::WText>("WARNING! ADMINISTRATOR MODE: ANY CHANGES MADE HERE WILL BE APPLIED TO ALL USERS\t");
    warningMessage->setStyleClass("adminText");
    adminPageContainer->addNew<Wt::WBreak>();
    Wt::WPushButton *addClassAdmin = adminPageContainer->addNew<Wt::WPushButton>("Add Class");
    addClassAdmin ->setStyleClass("adminButton");
    Wt::WPushButton *removeClassAdmin = adminPageContainer->addNew<Wt::WPushButton>("Remove Class");
    removeClassAdmin ->setStyleClass("adminButton");
    //adminPageContainer->setHidden(false);
    //help Container

    Wt::WPushButton *help = moduleContainer->addNew<Wt::WPushButton>("Help");
    help -> setStyleClass("helpButton");
    Wt::WContainerWidget *helpContainer = mainContainer->addNew<Wt::WContainerWidget>();

    helpContainer -> setStyleClass("helpContainer");

    //help ->setAttributeValue("style", "position:fixed; top:0%; left:80%; height:10%; width: 20%; font-size: 10px; color: white;");
    Wt::WPushButton *helpClose = helpContainer->addNew<Wt::WPushButton>("Close");

    helpContainer-> setHidden(true);


    help->clicked().connect([=]() {
        helpContainer-> setHidden(false);

    });

    helpClose->clicked().connect([=]() {
        helpContainer-> setHidden(true);

    });

    // help container ends

}

void DegreePlanner::greet()
{
    /*
     * Update the text, using text input into the nameEdit_ field.
     */

}