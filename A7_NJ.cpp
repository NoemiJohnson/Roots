//--------------------------------------------------------//
// Program: Course Storage System & Expanded PFI GPA      // 
// Calculator with Functions and File Input Capabilities  //
//  with BSCE & University Core Degree Audit              //
//  Source File: A7.cpp on TURING                         //
//               A7_NJ.cpp on BlackBoard                  //
//                                                        //
// Author: Noemi Johnson                                  //
//                                                        //
// Last Modified: December 8th, 2016                      //
//                                                        //
// Class: CSCE 2004  with Section 4 LAB                   //
//--------------------------------------------------------//
#include <cstdlib>
#include <iostream> 
#include <fstream>  
#include <cstring>  
#include <string>   
#include <iomanip> 
using namespace std;

class Course 
{
public:
Course(); // default constructor

void get(string& name, string& time, string& number, char& g, int& h) const; 

void set(string name, string time, string number, char g,int h);

void print()const;

private:
	string courseName;
	string courseTime;
	string courseNumber;
	char grade;
	int hours;
};

Course::Course() //constructor
{	
	//Feedback from the TA said that the Course Constructor does not initialize variables. 
 
}

 void Course::get(string& name, string& time, string& number, char& g, int& h)const
{
	name = courseName;
	time = courseTime;
	number = courseNumber;
	g = grade;
	h = hours;
}

void Course::set(string name, string time, string number, char g,int h)
{	
	courseName = name;	
	courseTime = time;	
    	courseNumber = number;
	grade = g;
	hours = h;
}

void Course::print()const
{
    cout << courseName << endl;
    cout << courseTime << endl;
    cout << courseNumber << endl;
    cout << grade << endl;
    cout << hours << endl;		
}

class Requirement
{
public:
  Requirement();
  Requirement(const Requirement & original);
   
  ~Requirement();

   void print() const;
   void read();
   void compare(const Course courses[], int n);

private:
   /* Array to store the Requirement Courses is below*/
   static const int MAX = 50;
   Course mCourses[MAX]; 

   int mNumCourses; 
   string group;
   string subGroup;
   int reqHours;
   int hoursTaken;
};

// Requirement Class. The values have to be in here else my code will not work!
Requirement::Requirement()
{
   mNumCourses = 0;
   group = "default group";
   subGroup = "default Subgroup";
   reqHours = -1;
   hoursTaken = 0;

}
//Copy Constructor
/*Requirements array mCourses is shown below*/
Requirement::Requirement(const Requirement & original)
{
	for (int index = 0; index < mNumCourses; index++)
	{
	    mCourses[index] = Course();
	}
} 

Requirement::~Requirement()
{
   //I learned this from lab!
}

void Requirement::print() const 
{
    	cout << group << endl;
   	cout << subGroup << endl;
    	cout << reqHours << endl;

	for (int index = 0; index < mNumCourses; index++)
	{
	    	string name;
		string number;
		string time;
		char g;
		int h;

		mCourses[index].get(name, time, number, g,  h);
		cout << number <<  " " << h << endl;
	}
	cout << endl;
}

/*Requirement::read() reads the BSCE Req File.txt */
void Requirement::read()
{
	ifstream inputFile;
	inputFile.open("BSCE Req File.txt");
	static int linePos = 0; 
	string linePosTemp;
	
	//This loop puts the curses back into the correct position:
	for (int i = 0; i < linePos; i++) 
	{
		getline (inputFile, linePosTemp);
	}

	if (inputFile)
	{
		string name = "namedefault";
		string time = "timedefault";
		string number = "numberdefault" ;
		char g = 'D';
		int h = 0;
	
		//linePos keeps track where where the user is in the infile:
		getline (inputFile, group);
		linePos++; 
		getline (inputFile, subGroup);
		linePos++;
		inputFile >> reqHours;
		linePos++;
		inputFile.ignore();

		string holder = "hold this for me"; 
		//This is used to check for a blank to end the object
		getline (inputFile, holder); 
		linePos++;
		while (true)
		{
			if (holder == "")
			{
			//The way out, the "" marks the end of a requirement object
			return; 
			}

			else
			{
			//If it doesn't find a blank in holder it assigns it to number, because we need it
			number = holder; 
			//a non-blank 'holder' is always followed by credit hours
			inputFile >> h; 
			linePos++;
			inputFile.ignore();
					
			//Made a temporary object to store the values before assigning it to the object array
			Course temp; 
			temp.set(name,  time, number ,g , h);
					
			mCourses[mNumCourses] = temp;
					
			//Keeps track of how many courses are in this array
			mNumCourses++; 
			//Keeps getting input, otherwise we are stuck in this loop, the "" will happen eventually.
			getline(inputFile, holder); 
			linePos++;
			}
		}
	}
		else
		{
			cout << "Error" << endl; // error message
		}
	inputFile.close();  

}
/*The Compare Function under the Requirement class will match the submitted courses against the BSCE File*/
void Requirement::compare(const Course courses[], int n )
{
	//Variables for user's submited course name, course semester, course number, grade and course's hours are below:
	string name; 
	string time; 
	string number; 
	char g; 
	int h;

	//Variables for the Requirement course name, course number, course grade and course hours are below.
	//Not all of these are needed but must be initialized in order to use the course.get function. 
	string rName;
	string rTime;
	string rNumber; 
	char rG;
	int rH;
	
	/*This loop controls the user's courses, setting it up for comparison*/
	for (int i = 0; i < n; i++)
	{
		courses[i].get(name, time, number, g, h); 

		/*This inner loop contols the requirement courses and then compares the values*/
		for (int i = 0; i < mNumCourses; i++) 
		{							
			mCourses[i].get(rName, rTime, rNumber, rG, rH);
			//This will compare every requirement course to the user's course object before it goes back to the outer loop
			if (number == rNumber)
			{
				if (g == 'W' || g == 'I' || g == 'F' || g == 'w' || g == 'i' || g == 'f' )
				{
				//A course with a W, F or I cannot be used, so it will not show the requiement as met	
				}

				else
				{
					//Accumumator for hours taken by the user
					hoursTaken += rH; 	
				}
			}
			else
			{
				//Do Nothing.
			}
		}
	}
	
	//If there are zero hours in the comparison, the requirement was not been met AT ALL.
	if (hoursTaken <= 0)
	
		cout << group << " " << subGroup << " requirement is not met at all." << endl;
	
	/*If the hours taken by the user are more than 0 , but less than than the 
	 required hours for a particular subgroup, 
	 the program will show the user by how many hours that requirement is met.*/
	/*Another comparison through the courses array is made*/
	else if (hoursTaken < reqHours && hoursTaken > 0) 
	{
		cout << group << " " << subGroup << " requirement is partially met by (hours still needed " << (reqHours - hoursTaken) << "):" << endl;
		
		for (int i = 0; i < n; i++)
		{
			//gets information from the courses array
			courses[i].get(name, time, number, g, h); 

			for (int i = 0; i <mNumCourses; i++)
			{
				//gets information from the mCourses array (requirements)
				mCourses[i].get(rName, rTime, rNumber, rG, rH);

				if (number == rNumber)
				{
					if (g == 'W' || g == 'I' || g == 'F' || g == 'w' || g == 'i' || g == 'f' )
					{
						//A course with a W, F or I cannot be used, so it will not show the requiement as partially met. 
					}
			
					else 
					{
					//This is my attempt to get the program to choose the highest grades in order to meet the requirements:
						if ((number == number && g != 'C'))
						{
							cout << name << endl;
							cout << time << endl;
							cout << number << endl;
							cout << g << endl;
							cout << h << endl;
						}
					
					}
				}
			}   
		}
		
	}
	
	/*If the hours taken by the user are more than the requirement's hours (reqHours) , the program will show the user the requirement is met.*/
	else if (hoursTaken >= reqHours)
	{ 
		cout << group << " " << subGroup << " requirement is met by: " << endl; 

		for (int i = 0; i < n; i++)
		{
			courses[i].get(name, time, number, g, h); 

			for (int i = 0; i <mNumCourses; i++) 
			{							
				mCourses[i].get(rName, rTime, rNumber, rG, rH);

				if (number == rNumber)
				{
					if (g == 'W' || g == 'I' || g == 'F' || g == 'w' || g == 'i' || g == 'f') 
					{
						//A course with a W, F or I cannot be used, so it will not show the requiement as partially met.
					}
					else
					{
						//This prints out the results of the comparison based on the Sample Audit Text File:
						cout << name << endl;
						cout << time << endl;
						cout << number << endl;
						cout << g << endl;
						cout << h << endl;
						
					}
					
				}
			}
		}
	}

return;
}

double gpa (int n, string name,  string time, string number,  char grade,  int hours, Course courses[] )
{
	double totalGwa = 0;
	double totalHours = 0;
		
	for (int i = 0; i < n; ++i)
	{
	double GRADE_POINT_P_HOUR=0;
	courses[i].get(name, time, number,grade, hours);
		if (grade == 'A')
		{ 
			GRADE_POINT_P_HOUR = 4;
			double gradePoints = GRADE_POINT_P_HOUR * hours;
			totalGwa += gradePoints;
			totalHours += hours;
		}
		else if (grade == 'B')
		{
			GRADE_POINT_P_HOUR = 3;
			double gradePoints = GRADE_POINT_P_HOUR * hours;
			totalGwa += gradePoints;
			totalHours += hours;
		}
		else if (grade == 'C')
		{
			GRADE_POINT_P_HOUR = 2;
			double gradePoints = GRADE_POINT_P_HOUR * hours;
			totalGwa += gradePoints;
			totalHours += hours;
		}
		else if (grade == 'D')
		{
			GRADE_POINT_P_HOUR = 1;
			double gradePoints = GRADE_POINT_P_HOUR * hours;
			totalGwa += gradePoints;
			totalHours += hours;
		}
		else if (grade == 'F' )
		{
			GRADE_POINT_P_HOUR = 0;
			double gradePoints = GRADE_POINT_P_HOUR * hours;
			totalGwa += gradePoints;
			totalHours += hours;
		}
		else if (grade == 'I' || grade == 'W') 
		{
			// I or W do not count towards hours.
			totalHours += 0;
		}
		else
		{
			cout << "You did not enter A or B or C or D or F or W or I, please try again" << endl;
		}
	} 			
		// The modified formula for calculating GPA is below. 
		// The 3.0 comparison is also shown to the user.
		double gpa = (totalGwa)/(totalHours);

		if (totalHours > 0)
		{
			return gpa;
		}
		else
		{
		 	cout <<"Cannot divide by zero. Please try again." << endl;
		}		
	return gpa;
}

/*Function B (double semesterGPA) calculates total GPA average based 
* on a user's requested semester & returns semesterGpa.
*/
double semesterGpa (int n, string name,  string time, string number,  char grade,  int hours, Course courses[],  string semester)
{	  
	double semesterGPA =0;
    	double totalGwa = 0;
	double totalHours = 0;
	
	for (int i = 0; i < n; i++)
	{
		courses[i].get(name, time, number, grade, hours);
		if (semester == time)
		{
			//Grade letter to grade point conversion begins below.
			double GRADE_POINT_P_HOUR = 0;

			if (grade == 'A')
			{ 
				GRADE_POINT_P_HOUR = 4;
				double gradePoints = GRADE_POINT_P_HOUR * hours;
				totalGwa += gradePoints;
				totalHours += hours;	
			}
			else if (grade == 'B')
			{
				GRADE_POINT_P_HOUR = 3;
				double gradePoints = GRADE_POINT_P_HOUR * hours;
				totalGwa += gradePoints;
				totalHours += hours;
			}
			else if (grade == 'C')
			{
				GRADE_POINT_P_HOUR = 2;
				double gradePoints = GRADE_POINT_P_HOUR * hours;
				totalGwa += gradePoints;
				totalHours += hours;	
			}
			else if (grade == 'D')
			{
				GRADE_POINT_P_HOUR = 1;
				double gradePoints = GRADE_POINT_P_HOUR * hours;
				totalGwa += gradePoints;
				totalHours += hours;					
			}
			else if (grade == 'F' )
			{
				GRADE_POINT_P_HOUR = 0;
				double gradePoints = GRADE_POINT_P_HOUR * hours;
				totalGwa += gradePoints;
				totalHours += hours;	
			}
			// I or W do not count towards hours.
			else if (grade == 'I' || grade == 'W') 
			{
				GRADE_POINT_P_HOUR = 0;
				double gradePoints = GRADE_POINT_P_HOUR * hours;
				totalGwa += gradePoints;
				totalHours += hours;
			}						  
		}
	}
	
	if (totalHours <= 0)
	{
		cout << "You did not complete course(s) in " << semester << "."<<endl; 
	}

	else if (totalHours > 0)
	{
		double semesterGPA = (totalGwa)/(totalHours);
					
		if (semesterGPA >=3.0)
		{
			//The GPA average for the semester was set to two decimal spaces.
			cout << "Congratulations. Your GPA for "<< semester << " was " << showpoint << fixed <<setprecision(2) << semesterGPA << endl;
		}
		else
		{
			cout << "Sorry dude, your GPA was " << semesterGPA <<" for "<< semester << endl;
		}		
	}
return semesterGPA;
}	

/*Function C (int DRule) shows the user the total class hours with Ds & returns DRule*/   
int DRule (int n, string name,  string time, string number,  char grade,  int hours, Course courses[])
{
	int DRule = 0;
	int numCourses = n;

	for (int i = 0; i <numCourses; i++)
	{
		courses[i].get(name, time, number, grade, hours);

		if (grade == 'D')
		{ 
			// Formula for calculating total class hours with Ds: 
			DRule = DRule + hours;
		}	
	}
return DRule;
}

/*Function D (print) lists all course information provided by the user in the order it was entered.
* It does not return a value
*/
void print(int n, string name , string time , string number , char grade , int hours, Course courses[] )
{   
	int numCourses = n;
	cout << numCourses << endl;

	for (int i = 0; i < n; i++)
	{
		courses[i].get(name, time, number, grade, hours);
		cout << name << endl;
		cout << time << endl;
		cout << number << endl;
		cout << grade << endl;
		cout << hours << endl;
	}
}

/* Function E (getCourse) gets specific course information from the user
* and stores them in the arrays declared in int main.It does not return a value.
*/
void getCourse(string& name, string& time, string& number, char& grade, int& hours, int n)
{	
	cout << "Enter course name (e.g Programming Foundations I) for your class number " << (n + 1) << endl;
	getline(cin, name); 
	
	cout << "Enter the time taken (e.g. Spring 2014) for your class number " << (n + 1) << endl;
	getline(cin, time);
	
	cout << "Enter course number (e.g. CSCE 2004) for your class number " << (n + 1) << endl;
	getline(cin, number);
	
	cout << "Enter grade letter (A,B,C,D,F,W or I) for your class number " << (n + 1) << endl;
	cin >> grade;
	cin.ignore();

	// User input validation for grade letter input begins below.
	for ( ;grade != 'A' && grade != 'B' && grade!= 'C' && grade!= 'D' && grade != 'F' && grade !='I' && grade !='W';  )
	{
		cout << "You did not enter A or B or C or D or F or W or I, please try again" << endl;
		cout << "Enter grade letter (A,B,C,D,F,W or I) for class " << (n + 1) << endl;
		cin >> grade;
		cin.ignore();
	}
		
		cout << "Enter number of hours (1-5) for your class " << (n + 1) << endl;
		cin >> hours;		
		cin.ignore();
		
	// User input validation for class hours begins below.
	for ( ;hours != 1 && hours != 2 && hours != 3 && hours != 4 && hours != 5; )
	{
		cout <<"Course hours cannot exceed 5 per course" << endl;
		cout <<"Enter number of hours (1-5) for your class " << (n + 1) << endl;
		cin >> hours;	
		cin.ignore();
	}	     
}

/*Function F (menu) shows the the menu & shows the user's menu choice & returns choice*/
char menu()
{ 
	char choice = '0';
	
	cout << "" << endl;
	cout << "Welcome to the interactive menu-driven part of the system" << endl;
	cout << "Here are your options:" << endl;
	cout << "A(a). Compute the GPA of all courses "<< endl;
	cout << "B(b). List all courses "<< endl;
	cout << "C(c). Compute the total credit hours of the courses with grade D "<< endl;
	cout << "D(d). Compute the GPA for a particular semester" << endl;
	cout << "E(e). Add another course to the course list " << endl;
	cout << "F(f). Delete a course from the course list" << endl;
	cout << "Q(q). Quit the program" << endl;
	cout << "" << endl;
	cout << "Please choose one of the above" << endl;
	cout << "" << endl;
		
	cin >> choice;
	cin.ignore();

	//Input validation for menu choices below:
	while (choice != 'A' && choice != 'a' && choice != 'B' && choice != 'b' && choice != 'C'&& choice != 'c' && choice != 'D' && choice != 'd' && choice != 'E' && choice != 'e' && choice != 'F' && choice != 'f' && choice != 'Q' && choice != 'q')
	{
		cout << "Invalid Choice. Please try again" << endl; 
		cin >> choice;
		cin.ignore(); 
	}
	 
	return choice;
}	

/*Function deleteCourse doesn't return anything. It just deletes a course from the list*/
void deleteCourse (string name,  string time, string number, char grade, int hours , int &n, int capacity, Course courses [])
//if user chooses menu option F or f, it will delete a course from the array courses.
{
	//Int variable deleteCourse is what the user enters to choose which course to delete.
	int deleteCourse = 0;
	cout << "Select from 1 to " << n << " for the course to be deleted" << endl;
	cin >> deleteCourse;

	
	// Int variable deleteCourseIndex is the new position in each
	// course information array after a course is deleted.
	int deleteCourseIndex = deleteCourse - 1;

	cout << "Here is the course you have selected for deletion:" << endl;
	cout << "Course number " << deleteCourse << endl;

	//Accesses the course from the courseTaken array
	//Shows the user the course name, time, course number, 
	//grade letter and hours that the user chose to delete.
	courses[deleteCourseIndex].get(name, time, number, grade, hours);
	cout << name << endl;
	cout << time << endl;
	cout << number << endl;
	cout << grade << endl;
	cout << hours << endl;


	//Asks confirmation of deleting the selected course before deleting it.
	cout << "Would you like to delete the course above?" <<endl;
	cout << "Please enter y for yes and n for no" << endl;
	
	char yesNoChoice = '0';

	cin >> yesNoChoice;
	cin.ignore();

		// If the user chooses yes, the selected course gets deleted.
		if (yesNoChoice == 'y' || yesNoChoice == 'Y')
		{
			int from = deleteCourse;
			int to = deleteCourseIndex;

			/* The for loop below will help move the non-deleted 
			/ courses vertically after a course has been deleted 
			/ for the sake of neatness*/
			for (;to < n; to++, from++)
			{
				courses[to] = courses[from];
			}
			n--;
		}
	
		// If the user chooses no, then the program goes back to the menu.
		else if (yesNoChoice == 'n' || yesNoChoice == 'N')
		{
		 	/*Function call for going back to the menu*/
			 menu();
		}

		else 
		{
			cout << "Invalid input. Please try again." << endl;
			/*A failed input makes the program go back to the menu.*/
		   	 menu();
		}
}

bool reading(const char filename[], string &name, string &time, string &number, char &grade, int &hours, int& n, int capacity, Course courses[])
{
	ifstream inputFile;
	inputFile.open(filename);
	if (inputFile.fail())
	return false;

	else
	{
		inputFile >> n; // n = numCourses upon Return
		inputFile.ignore();
		for (int i = 0; i < capacity; i++)
		{
			getline(inputFile,name);
			getline(inputFile,time);
			getline(inputFile,number);
			inputFile >> grade ;
			inputFile.ignore();
			inputFile >> hours;
			inputFile.ignore();
			courses[i].set(name, time, number, grade,hours);
		}
		inputFile.close();
		return true;
	}
}
bool writing (char filename [], string name, string time, string number, char grade, int hours , int n, Course courses [])
{ 
	ofstream outfile;
	outfile.open("checking.txt");
	if (outfile.fail())
	return false;

	else
	{
		outfile << n << endl; // n = numCourses upon Return
		
		for (int i = 0; i < n; i++)
		{
			courses[i].get(name, time, number, grade, hours);
			outfile << name << endl;
			outfile << time << endl;
			outfile << number << endl;
			outfile << grade  << endl;
			outfile << hours << endl;	
		}
		outfile.close();
		return true;
	}
}

int main()
{
	// Variables declared below:
	string name;
	string time;
	string number;
	char grade;
	int hours;

    	string group = "default group";
   	string subGroup = "default Subgroup";
    	int reqHours = -1;
    	int hoursTaken = 0;
	int capacity = 30;
	int numCourses = 0;

	//Course course array:
	const int MAX_COURSES = 30;
	Course courses[MAX_COURSES];

	//Requirement Classes with their Objects:
	Requirement English;
	Requirement FineArts;
	Requirement USHistory;
	Requirement Humanities;
	Requirement Math;
	Requirement Science;
	Requirement Social;

	//University SubGroups calling the requirement read functions to perform the audit
	English.read();
	FineArts.read();
	USHistory.read();
	Humanities.read();
	Math.read();
	Science.read();
	Social.read();

	cout <<""<< endl;
	cout << "Welcome to the PFI course management system v1" << endl;
	cout << "and BSCE Degree Audit " << endl;
	cout << "Would you like to read the courses taken from a file?" << endl;
	cout << "Please enter y for yes and n for no" << endl;

	char option = '0';
	cin >> option;
	cin.ignore();

	//User chose to use default file "courses.txt".
	if (option == 'y')
	{
		cout << "Would you like to use the default file?" << endl;
		cout << "Please enter y for yes and n for no" << endl;

		 char option= '0';
	     	 cin >> option;
		 cin.ignore();

		 if (option == 'y' || option == 'Y') 
		 {
			char fileName[] = "courses.txt";
			cout << "File input was successful. Please review the courses" << endl;
		
			 reading(fileName, name, time, number, grade, hours, numCourses, MAX_COURSES, courses);
			 for (int i = 0; i < numCourses; i++)
			 {
				courses[i].print();
			 }
		 }
		 //When the user chooses no, the program asks the user to input a file name to read.
		 else if (option == 'n' || option == 'N') 
		 {
			cout << "Then enter the filename of the text file you wish to use" <<endl;
			char fileName [25];
			cin >> fileName;
			cin.ignore();
			 
			/* Calls function reading to read the text file after user inputs filename*/
			bool readSuccess = reading(fileName, name, time, number, grade, hours, numCourses, MAX_COURSES, courses);
			
			 //Input validation for the user-entered filename
			 if (readSuccess)
			 {
				cout << "File input was successful." << endl;
				cout << "Please review the courses. " << endl;
			 }
			 else
			 {
				cout << "File input was unsuccessful." << endl;
			 }
			 /* Calls function print to list courses loaded from the courses array*/
			 for (int i = 0; i < numCourses; i++)
			 {
				courses[i].print();
			 }		
		 }
		 else 
		 {
			 cout << "Invalid input. Please enter y for yes or n for no" << endl;
			 cin >> option;
			 cin.ignore();
		 }
	 }
	//When the user chooses no, the program will ask the user to manually enter course information, starting with the number of courses.
	else if (option == 'n' || option == 'N') 
	{
		cout <<""<< endl;
		cout << "Welcome to the PFI course management System " <<endl;
		cout << "and University BSCE Degree Audit, v1 " << endl;
		cout <<""<<endl;
		cout << "Enter the number of classes for the system: " << endl;

		cin >> numCourses;
		cin.ignore(); 
	
		/*Function call for Function E gets course information from user before user sees the menu*/
		/*Then puts that information into the courses array*/
	
		for (int n = 0; n < numCourses; n++)
		{
			getCourse(name, time, number, grade, hours, n);
			courses[n].set(name, time, number, grade, hours);
		}
	}
	
	/*The user's choice is a char value and it is equivalent to calling Function F to access the menu*/
	char choice = menu();
	
	while (choice != 'Q' || choice != 'q')
	{	
		if (choice == 'A' || choice == 'a')
		{
			/*Function call for Function A is below*/
			double GPA = gpa(numCourses, name, time, number, grade, hours, courses);
		
			//GPA comparison stayed the same.
			if (GPA >= 3.0)
			{
				cout << "Congratulations. Your GPA was " << GPA << endl;
			}
			else
			{
				cout << "Sorry dude, your GPA was " << GPA << endl;	
			}
		}	
		else if (choice == 'B' || choice == 'b')
		{
			/*Function call for Function D is below*/
			cout << "The following are your courses in input format: " << endl; 
			print(numCourses, name, time, number, grade, hours, courses);	
		}

		else if (choice == 'C' || choice == 'c')
		{
			/*Function call for Function C is below*/
			cout << "Total hours with D grades are " << DRule(numCourses, name, time, number, grade, hours, courses);	
			cout << ""<<endl; //Added space//	 	
		}

		else if (choice == 'D' || choice == 'd')
		{    
	   		//semester string is initialized & will be provided by the user to search the classSemester array.
			string semester;
		
			cout << "Please enter the semester time (e.g Fall 2014) " << endl; 
		   	 getline(cin, semester);
		
			/*Function call for function B (semesterGPA) is below*/
			semesterGpa(numCourses, name, time, number, grade, hours, courses, semester);	
		}

		else if (choice == 'E' || choice == 'e')
		{   
			/*Function call for Function E is called below, as long as user input doesn't exceed 10 courses*/ 
			if (numCourses < MAX_COURSES)
			{	
				getCourse(name, time, number, grade, hours, numCourses);
				courses[numCourses].set(name, time, number, grade, hours);
			
				numCourses++;
				
				/*Function call for Function D is below*/
				cout << "The following are your courses in input format:" << endl;
				print(numCourses, name, time, number, grade, hours, courses);		
			}
			else 
			{
				//Reminds user about the 30 course limit.
				cout << "You cannot enter more than 30 courses" << endl; 
			}
		}

		else if (choice == 'F' || choice == 'f')
		{
		 	 /*Function call for deleteCourse allows user to delete a course from the list of courses in the program*/
		   	deleteCourse (name, time, number, grade, hours, numCourses, MAX_COURSES, courses);

		 	cout << "Please review the updated courses below" << endl;
			
			//Shows the user the updated course list after a course was deleted. 
		 	for (int i = 0; i < numCourses; i++)
			{
				courses[i].get(name, time, number, grade, hours);
				cout << name << endl;
				cout << time << endl;
				cout << number << endl;
				cout << grade << endl;
				cout << hours << endl;
			}
		}
	
		else if (choice == 'Q' || choice == 'q')
		{	
			/*Function writing is called when user quits the program.
			/The function writes the courses onto "checking.txt" before completely exiting the program.*/
			char fileName [25] = {"checking.txt"};

			/*Function calls the function writingSuccess to write the current courses to the textfile "checking.txt"*/
			bool writingSuccess = writing (fileName, name, time, number, grade, hours, numCourses, courses);

				//If the writing the file fails, then it ends the program.
				if (!writingSuccess)
				{	
					cout << "Something went wrong with your .txt" << endl;

					return 0;
				}
				else 
				{	
					//The compare function is called under each SubGroup, accessing courses and numCourses:
					cout << "Running Degree Audit :" << endl;

					English.compare(courses, numCourses);
					Math.compare(courses, numCourses);
					FineArts.compare(courses, numCourses);
					USHistory.compare(courses, numCourses);
					Social.compare(courses, numCourses);
					Humanities.compare(courses, numCourses);
					Science.compare(courses, numCourses);

					cout << "Thank you for using the system, terminating.." << endl;	
	
		 			return 0;
				}
		}
	 choice = menu();
	}   
}



	
