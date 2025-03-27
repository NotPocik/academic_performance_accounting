#include <windows.h>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <iomanip>
#include <typeinfo>

using namespace std;

class User;
class System;
class Teacher;

template <typename T1, typename T2>
int find(vector<T1>& vec, T2& a)
{
	for (int i = 0; i < vec.size(); i++)
	{
		if (vec[i] == a) return i;
	}
	return -1;
}

template <typename T1, typename T2>
int findPtr(vector<T1>& vec, T2* a)
{
	for (int i = 0; i < vec.size(); i++)
	{
		if (*vec[i] == *a) return i;
	}
	return -1;
}

void output(vector<string>& v)
{
	for (int i = 0; i < v.size(); i++)
	{
		cout << i + 1 << ") " << v[i] << endl;
	}
}


class Subject
{
	string subName;
	vector<int> grades;
	int n; // количество оценок

public:
	Subject(string name) : subName(name), n(0) {}

	Subject() : subName(""), n(0) {}

	void gradesOutput()
	{
		if (n == 0) cout << "нет оценок";
		cout << grades[0];
		for (int i = 1; i < grades.size(); i++) cout << ", " << grades[i];
	}

	friend ostream& operator<<(ostream& os, Subject& sub)
	{
		if (sub.n == 0)
		{
			cout << "нет оценок";
			return os;
		}
		cout << sub.grades[0];
		for (int i = 1; i < sub.grades.size(); i++) cout << ", " << sub.grades[i];
		return os;
	}

	bool addGrade(int grade)
	{
		if (grade >= 0 && grade <= 10)
		{
			grades.push_back(grade);
			n++;
			return true;
		}
		else return false;
	}

	bool deleteGrade(int grade)
	{
		int i = grades.size() - 1;
		for (; i >= 0; i--)
		{
			if (grades[i] == grade)
			{
				for (; grades.size() - 1 > i; i++) grades[i] = grades[i + 1];
				grades.pop_back();
				n--;
				return true;
			}
		}
		return false;
	}

	void deleteAllGrades()
	{
		grades.clear();
		n = 0;
	}

	double averageGrade()
	{

		if (n == 0) return -1;
		int sum = 0;
		for (int i = 0; i < n; i++)
		{
			sum += grades[i];
		}
		return (double)sum / n;
	}

	void setSubjectName(string name)
	{
		subName = name;
	}

	string getSubjectName()
	{
		return subName;
	}

	void save_to_file(string file_name)
	{
		ofstream file(file_name);
		file << subName << ':';
		for (int i = 0; i < n; i++)
		{
			file << " " << grades[i];
		}
		file << ",";
	}

	void load_from_file(string file_name)
	{
		ifstream file(file_name);
		getline(file, subName, ':');
		int grade;

		while (file >> grade)
		{
			addGrade(grade);
		}
	}

	void clear()
	{
		subName.clear();
		n = 0;
		grades.clear();
	}

	friend ofstream& operator<<(ofstream& file, Subject& sub)
	{
		file << sub.subName << ':';
		for (int i = 0; i < sub.n; i++)
		{
			file << " " << sub.grades[i];
		}
		file << ",";
		return file;
	}

	friend ifstream& operator>>(ifstream& file, Subject& sub)
	{
		sub.clear();
		getline(file, sub.subName, ':');
		string s;
		char ch;
		int grade;
		file.get(ch);
		while (ch == ' ' && !file.eof())
		{
			file >> grade;
			sub.addGrade(grade);
			file.get(ch);
		}
		return file;
	}
};


class Person
{
	string name;

public:
	Person(string n) : name(n) {}
	Person() : name("") {}

	string getName()
	{
		return name;
	}

	void setName(string n)
	{
		name = n;
	}
};

class Student : public Person
{
	string SchoolClass; // класс учащегося 
	int StudNumber; // порядковый номер в классе
	vector<Subject> marks;
public:
	Student(string name, string ScClass) : Person(name), SchoolClass(ScClass), StudNumber(0) {}
	Student() : Person(), SchoolClass(""), StudNumber(0) {}
	Student(string name, string ScClass, vector<string>& subNames) : Person(name), SchoolClass(ScClass), StudNumber(0)
	{
		for (int i = 0; i < subNames.size(); i++)
		{
			Subject* sub = new Subject(subNames[i]);
			marks.push_back(*sub);
			delete sub;
		}
	}

	string getClass()
	{
		return SchoolClass;
	}

	int getNumber()
	{
		return StudNumber;
	}

	void addSubject(string name)
	{
		Subject s(name);
		marks.push_back(s);
	}

	bool deleteSub(int sub)
	{
		try
		{
			if (sub < 0 || sub >= marks.size()) throw;
			marks.erase(marks.begin() + sub);
			return true;
		}
		catch (...)
		{
			cout << "\nОшибка: номер предмета указан неверно\n";
			return false;
		}
	}

	void setNumber(int number)
	{
		StudNumber = number;
	}

	void setClass(string cl)
	{
		SchoolClass = cl;
	}

	bool addGrade(int sub, int grade)
	{
		if (sub >= 0 && sub < marks.size())
		{
			if (marks[sub].addGrade(grade)) return true;
		}
		return false;
	}

	bool deleteGrade(int sub, int grade)
	{
		if (sub >= 0 && sub < marks.size())
		{
			if (marks[sub].deleteGrade(grade)) return true;
		}
		return false;
	}

	void deleteAllGrades()
	{
		for (int i = 0; i < marks.size(); i++) marks[i].deleteAllGrades();
	}

	double averageGrade(int sub)
	{
		if (sub < 0 || sub >= marks.size()) throw "некорректное указание предмета";
		return marks[sub].averageGrade();
	}

	void SubGradesOutput(int sub)
	{
		cout << getName() << ": " << marks[sub] << "\n   Средний балл: ";
		double g = averageGrade(sub);
		if (g != -1) cout << setprecision(3) << g;
		else cout << "-";
	}

	void allGradesOutput()
	{
		int sum = 0, n = 0;
		for (int i = 0; i < marks.size(); i++)
		{
			cout << marks[i].getSubjectName() << ": " << marks[i] << "\nСредний балл: ";
			double g = averageGrade(i);
			if (g != -1)
			{
				cout << setprecision(3) << g << "\n\n";
				n++;
				sum += round(g);
			}
			else cout << "-\n\n";
		}

		if (n) cout << "Общий среднил балл: " << setprecision(3) << (double)sum / n << "\n\n";
	}

	void clear()
	{
		setName("");
		StudNumber = 0;
		marks.clear();
	}

	friend ofstream& operator<<(ofstream& file, Student& st)
	{
		file << st.getName() << ';' << st.getClass() << ';' << st.getNumber() << ';';
		for (int i = 0; i < st.marks.size(); i++)
		{
			file << st.marks[i];
		}
		file << ";";
		return file;
	}

	friend ifstream& operator>>(ifstream& file, Student& st)
	{
		st.clear();
		Subject sub;
		string s;
		int a;
		getline(file, s, ';');
		st.setName(s);
		getline(file, s, ';');
		st.setClass(s);
		file >> a;
		st.setNumber(a);
		file.ignore();
		while (file.peek() != ';' && !file.eof())
		{
			file >> sub;
			st.marks.push_back(sub);
		}
		file.ignore();
		return file;
	}
};

struct subAndTeacher
{
	string teacherLogin;
	string subject;
};

struct subAndKey
{
	int key;
	string subject;
};

class Class
{
	string className;
	int numberOfStud;
	vector <subAndTeacher> subjects;
	vector <Student> students;

	void addStudent()
	{
		numberOfStud++;
		Student st;
		students.push_back(st);
	}

public:
	Class(string name) : numberOfStud(0), className(name) {}
	Class() : numberOfStud(0), className() {}

	string getClassName()
	{
		return className;
	}

	void setClassName(string name)
	{
		className = name;
	}

	string getSubjectName(int sub)
	{
		return subjects[sub].subject;
	}

	string getStudentName(int st)
	{
		try
		{
			if (st >= numberOfStud || st < 0) throw;
			return students[st].getName();
		}
		catch (...)
		{
			cout << "\nОшибка: номер студента указан неверно\n";
			return "-";
		}
	}

	int getNumberOfStud()
	{
		return numberOfStud;
	}

	void addStudent(string name)
	{
		numberOfStud++;
		vector<string> subs;
		for (int i = 0; i < subjects.size(); i++) subs.push_back(subjects[i].subject);
		Student st(name, className, subs);
		students.push_back(st);
		sortAlph();
	}

	void addSubject(string subName)
	{
		subAndTeacher sub;
		sub.subject = subName;
		sub.teacherLogin = "";
		subjects.push_back(sub);
		for (int i = 0; i < students.size(); i++)
		{
			students[i].addSubject(subName);
		}
	}

	void addTeacher(int sub, Teacher* t);


	string deleteTeacher(int sub)
	{
		if (sub < 0 || sub >= subjects.size())
		{
			cout << "\nОшибка: номер предмета указан неверно\n";
			return "";
		}
		string l = subjects[sub].teacherLogin;
		subjects[sub].teacherLogin = "";
		return l;
	}

	void deleteTeacher(string log)
	{
		for (int i = 0; i < subjects.size(); i++)
		{
			if (subjects[i].teacherLogin == log) subjects[i].teacherLogin = "";
		}
	}

	int findTeacher(string l)
	{
		for (int i = 0; i < subjects.size(); i++)
		{
			if (l == subjects[i].teacherLogin) return i;
		}
		return -1;
	}

	string getSubTeacher(int sub)
	{
		return subjects[sub].teacherLogin;
	}

	vector<subAndKey> getTeacherSubjects(string login)
	{
		vector<subAndKey> subs;
		subAndKey sub;
		for (int i = 0;i < subjects.size();i++)
		{
			if (login == subjects[i].teacherLogin)
			{
				sub.subject = subjects[i].subject;
				sub.key = i;
				subs.push_back(sub);
			}
		}
		return subs;
	}

	bool deleteSubject(int sub)
	{
		try
		{
			if (sub < 0 || sub >= subjects.size()) throw;
			subjects.erase(subjects.begin() + sub);
			for (int i = 0; i < students.size(); i++)
			{
				students[i].deleteSub(sub);
			}
			return true;
		}
		catch (...)
		{
			cout << "\nОшибка удаления предмета\n";
		}
	}

	void sortAlph()
	{
		Student s = students[numberOfStud - 1];
		int i = numberOfStud - 1;
		for (; i > 0; i--)
		{
			if (s.getName() >= students[i - 1].getName()) break;
			students[i] = students[i - 1];
		}
		students[i] = s;
		setStudNumbers(i);
	}

	void setStudNumbers(int i = 0)
	{
		for (; i < numberOfStud; i++) students[i].setNumber(i + 1);
	}

	void studentsOutput()
	{
		for (int i = 0; i < numberOfStud; i++)
		{
			cout << students[i].getNumber() << ") " << students[i].getName() << endl;
		}
	}

	void subjectsOutput()
	{
		for (int i = 0; i < subjects.size(); i++)
		{
			cout << i + 1 << ") " << subjects[i].subject << endl;
		}
	}

	void subjectsAndTeachersOutput()
	{
		for (int i = 0; i < subjects.size(); i++)
		{
			cout << i + 1 << ") " << subjects[i].subject;
			if (subjects[i].teacherLogin != "") cout << " - " << subjects[i].teacherLogin;
			cout << endl;
		}
	}

	void studentsSubGradesOutput(int sub)
	{
		try
		{
			if (sub >= subjects.size() || sub < 0) throw;
			for (int i = 0; i < students.size(); i++)
			{
				cout << i + 1 << ") ";
				students[i].SubGradesOutput(sub);
				cout << "\n\n";
			}
		}
		catch (...)
		{
			cout << "\nОшибка: предмет указан неверно\n";
		}
	}

	void studentAllGradesOutput(int st)
	{
		try
		{
			if (st >= numberOfStud || st < 0) throw;
			students[st].allGradesOutput();
		}
		catch (...)
		{
			cout << "\nОшибка: номер ученика указан неверно\n";
		}
	}

	bool addGrade(int st, int sub, int grade)
	{
		if (st >= 0 && st < students.size())
		{
			if (students[st].addGrade(sub, grade)) return true;
		}
		return false;
	}

	bool deleteGrade(int st, int sub, int grade)
	{
		if (st >= 0 && st < students.size())
		{
			if (students[st].deleteGrade(sub, grade)) return true;
		}
		return false;
	}

	void deleteAllGrades()
	{
		for (int i = 0; i < students.size(); i++) students[i].deleteAllGrades();
	}

	double averageGrade(int st, int sub)
	{
		try
		{
			if (st < 0 || st >= students.size()) throw "некорректное указание учащегося";
			return students[st].averageGrade(sub);
		}
		catch (const char* mes)
		{
			cout << "Ошибка: " << mes << endl;
			return 0;
		}
	}

	bool deleteStudent(int st)
	{
		try
		{
			if (st >= students.size() || st < 0) throw 1;
			students.erase(students.begin() + st);
			numberOfStud--;
			setStudNumbers(st);
			return true;
		}
		catch (int)
		{
			cout << "\nОшибка: номер учащегося введен неверно\n\n" << endl;
			return false;
		}
	}

	void clear()
	{
		className.clear();
		numberOfStud = 0;
		students.clear();
		subjects.clear();
	}

	bool saveToFile(string fileName)
	{
		ofstream file(fileName);
		try
		{
			if (!file.is_open()) throw;

			file << className << ';';
			for (int i = 0; i < subjects.size(); i++)
			{
				file << subjects[i].subject << '~' << subjects[i].teacherLogin << ',';
			}
			file << ";\n";

			for (int i = 0; i < students.size(); i++)
			{
				file << students[i] << '\n';
			}
			file << '.';
			return true;
		}
		catch (...)
		{
			cout << "\nОшибка открытия файла\n";
			return false;
		}
	}

	bool loadFromFile(string fileName)
	{
		clear();
		ifstream file(fileName);
		try
		{
			if (!file.is_open()) throw;

			string s;
			getline(file, className, ';');

			while (file.peek() != ';')
			{
				subAndTeacher sub;
				getline(file, s, '~');
				sub.subject = s;
				getline(file, s, ',');
				sub.teacherLogin = s;
				subjects.push_back(sub);
			}
			file.ignore(2);

			while (file.peek() != '.')
			{
				addStudent();
				file >> students[numberOfStud - 1];
				file.ignore();
				sortAlph();
			}

			return true;
		}
		catch (...)
		{
			cout << "\nОшибка открытия файла\n";
			return false;
		}
	}
};




class Menu
{
	vector<string> options;
public:

	Menu(vector<string> menu_options) : options(menu_options) {}

	int display()
	{
		for (int i = 0; i < options.size(); i++)
		{
			cout << i + 1 << ") " << options[i] << endl;
		}
		cout << "\nВведите ваш выбор:\n";
		int choice;
		cin >> choice;
		return choice;
	}

	void setMenu(vector<string> menu_options/*, void(*f)()*/)
	{
		options = menu_options;
	}

	void operator=(vector<string> menu_options)
	{
		options = menu_options;
	}
};


class User
{
protected:
	string login;
	string password;

public:

	//static Menu mainMenu;
	static Menu classMenu;
	static Menu mainMenu;

	User(string l, string p) : login(l), password(p) {}
	User() : login(""), password("") {}

	virtual string getStatus()
	{
		return "user";
	}

	string getLogin()
	{
		return login;
	}

	string getPassword()
	{
		return password;
	}

	void setLogin(string l)
	{
		login = l;
	}

	void setPassword(string p)
	{
		password = p;
	}

	bool operator==(User& u)
	{
		if (login == u.login) return true;
		else return false;
	}

	bool operator==(string s)
	{
		if (login == s) return true;
		else return false;
	}

	bool checkPassword(string p)
	{
		if (p == password) return true;
		else return false;
	}

	virtual void saveToFile(ofstream& file)
	{
		file << "u " << login << ' ' << password;
	}

	static User* loadFromFile(ifstream& file);


	virtual void classMenuF(string className)
	{
		Class cl;
		cl.loadFromFile(className + ".dat");
		int choice;
		string s;
		system("cls");

		while (true)
		{
			cout << "Класс: " << cl.getClassName() << "\n\n";
			switch (classMenu.display())
			{
			case 1:
			{
				system("cls");

				cout << "Класс: " << cl.getClassName() << "\n\n";
				cout << "Список учеников:\n";
				cl.studentsOutput();


				break;
			}
			case 2:
			{
				system("cls");

				cout << "Класс: " << cl.getClassName() << "\n\n";
				cout << "Список преподаваемых предметов:\n";
				cl.subjectsOutput();

				break;
			}
			case 3:
			{
				system("cls");

				cl.subjectsOutput();
				cout << "\nВведите номер предмета:\n";
				cin >> choice;
				system("cls");

				cout << "Класс: " << cl.getClassName() << "\n\n";
				cout << "Оценки всех учеников по предмету \"" << cl.getSubjectName(choice - 1) << "\":\n\n";
				cl.studentsSubGradesOutput(choice - 1);


				break;
			}
			case 4:
			{
				system("cls");

				cl.studentsOutput();
				cout << "\nВведите номер учащегося:\n";
				cin >> choice;
				s = cl.getStudentName(choice - 1);
				if (s == "-") break;
				system("cls");

				cout << "Класс: " << cl.getClassName() << "\n\n";
				cout << "Все оценки ученика " << s << ":\n\n";
				cl.studentAllGradesOutput(choice - 1);
				break;
			}
			case 5: return;
			default: "Ошибка";
			}
			system("pause");
			system("cls");
		}
	}

	virtual bool mainMenuF(vector<string>& classes, System* sys) //Возвращает true - выход из программы, false - релогин
	{
		int choice;
		string s;
		system("cls");
		while (true)
		{
			cout << "User: " << login << "\n\n";

			switch (mainMenu.display())
			{
			case 1:
			{
				system("cls");

				output(classes);
				cout << "\nВведите номер класса для работы с ним(- для возвращения в главное меню):\n";

				cin >> s;
				if (s == "-") break;
				choice = stoi(s);
				if (choice - 1 < 0 || choice - 1 >= classes.size())
				{
					cout << "\nОшибка: класс указан неверно\n";
					break;
				}
				classMenuF(classes[choice - 1]);
				break;
			}
			case 2:
			{
				system("cls");
				return false;
			}
			case 3:
			{
				system("cls");
				return true;
			}
			}
			system("pause");
			system("cls");
		}
	}
};

Menu User::classMenu(
	{
			"Просмотреть учащихся",
			"Просмотреть преподаваемые предметы",
			"Вывести оценки учащихся по заданному предмету",
			"Вывести все оценки заданного ученика",
			"Назад"
	});

Menu User::mainMenu(
	{
		"Просмотреть классы",
		"Выйти из аккаунта",
		"Выйти"
	});



class Teacher : public User
{
	vector<string> classes;

public:

	static Menu classMenu;
	static Menu mainMenu;

	Teacher(string log, string pas) : User(log, pas) {}
	Teacher(string log, string pas, vector<string>& cls) : User(log, pas), classes(cls) {}

	virtual string getStatus()
	{
		return "teacher";
	}

	virtual void saveToFile(ofstream& file)
	{
		file << "t " << login << ' ' << password;
		for (int i = 0; i < classes.size(); i++) file << ' ' << classes[i];
	}

	virtual void addClass(string cl)
	{
		if (find(classes, cl) != -1)
		{
			return;
		}
		classes.push_back(cl);
	}

	virtual void deleteClass(int cl)
	{
		if (cl < 0 || cl >= classes.size())
		{
			cout << "\nОшибка: номер класса введен неверно\n";
			return;
		}
		classes.erase(classes.begin() + cl);
	}

	virtual void deleteClass(string cl)
	{
		int i = find(classes, cl);
		if (i != -1) classes.erase(classes.begin() + i);
	}

	int findClass(string cl)
	{
		return find(classes, cl);
	}

	vector<string> getClasses()
	{
		return classes;
	}

	virtual void classesOutput()
	{
		for (int i = 0; i < classes.size(); i++) cout << i + 1 << ") " << classes[i] << endl;
	}

	virtual bool mainMenuF(vector<string>& classes1, System* sys) //Возвращает true - выход из программы, false - релогин
	{
		int choice;
		string s;
		system("cls");
		while (true)
		{
			cout << "Teacher: " << login << "\n\n";

			switch (mainMenu.display())
			{
			case 1:
			{
				system("cls");

				output(classes1);
				cout << "\nВведите номер класса для работы с ним(- для возвращения в главное меню):\n";

				cin >> s;
				if (s == "-") break;
				choice = stoi(s);
				if (choice - 1 < 0 || choice - 1 >= classes1.size())
				{
					cout << "\nОшибка: класс указан неверно\n";
					break;
				}
				if(findClass(classes1[choice - 1]) != -1) classMenuF(classes1[choice - 1]);
				else User::classMenuF(classes1[choice - 1]);
				break;
			}
			case 2:
			{
				system("cls");
				cout << "Список классов для учителя " << login << ":\n";
				classesOutput(); 
				break;
			}

			case 3:
			{
				system("cls");
				return false;
			}
			case 4:
			{
				system("cls");
				return true;
			}
			}
			system("pause");
			system("cls");
		}
	}

	virtual void classMenuF(string className)
	{
		Class cl;
		cl.loadFromFile(className + ".dat");
		int choice, st, sub, grade;
		string s;
		vector<subAndKey> subs;
		subs = cl.getTeacherSubjects(login);
		system("cls");
		while (true)
		{
			cout << "Класс: " << cl.getClassName() << "\n\n";
			switch (classMenu.display())
			{
			case 1:
			{
				system("cls");

				cout << "Класс: " << cl.getClassName() << "\n\n";
				cout << "Список учеников:\n";
				cl.studentsOutput();
				break;
			}
			case 2:
			{
				system("cls");

				cout << "Класс: " << cl.getClassName() << "\n\n";
				cout << "Список преподаваемых предметов:\n";
				cl.subjectsOutput();
				break;
			}
			case 3:
			{
				system("cls");

				cl.subjectsOutput();
				cout << "\nВведите номер предмета:\n";
				cin >> choice;
				system("cls");

				cout << "Класс: " << cl.getClassName() << "\n\n";
				cout << "Оценки всех учеников по предмету \"" << cl.getSubjectName(choice - 1) << "\":\n\n";
				cl.studentsSubGradesOutput(choice - 1);
				break;
			}
			case 4:
			{
				system("cls");

				cl.studentsOutput();
				cout << "\nВведите номер учащегося:\n";
				cin >> choice;
				s = cl.getStudentName(choice - 1);
				if (s == "-") break;
				system("cls");

				cout << "Класс: " << cl.getClassName() << "\n\n";
				cout << "Все оценки ученика " << s << ":\n\n";
				cl.studentAllGradesOutput(choice - 1);
				break;
			}
			case 5:
			{
				system("cls");

				cl.studentsOutput();
				cout << "\nВведите номер учащегося(- для отмены операции):\n";
				cin >> s;
				if (s == "-") break;
				st = stoi(s);
				system("cls");
				for (int i = 0; i < subs.size(); i++) cout << i + 1 << ") " << subs[i].subject << endl;

				cout << "\nВведите номер предмета(- для отмены операции):\n";
				cin >> s;
				if (s == "-") break;
				sub = stoi(s);
				if (sub - 1 < 0 || sub - 1 >= subs.size())
				{
					cout << "\nОшибка: номер предмета введен неверно\n";
					break;
				}
				system("cls");
				cout << "Введите оценку(- для отмены операции):\n";
				cin >> s;
				if (s == "-") break;
				grade = stoi(s);
				cl.addGrade(st - 1, subs[sub - 1].key, grade);
				break;
			}
			case 6:
			{
				system("cls");
				for (int i = 0; i < subs.size(); i++) cout << i + 1 << ") " << subs[i].subject << endl;
				cout << "\nВведите номер предмета(- для отмены операции):\n";
				cin >> s;
				if (s == "-") break;
				sub = stoi(s);
				if (sub - 1 < 0 || sub - 1 >= subs.size())
				{
					cout << "\nОшибка: номер предмета введен неверно\n";
					break;
				}
				system("cls");
				cout << "\nВведите оценки для учеников:\n\n";
				for (int i = 0; i < cl.getNumberOfStud(); i++)
				{
					cout << i + 1 << ") " << cl.getStudentName(i) << ": ";
					cin >> s;
					if (s == "-") continue;
					grade = stoi(s);
					cl.addGrade(i, subs[sub - 1].key, grade);
				}
				break;
			}
			case 7:
			{
				system("cls");

				cl.studentsOutput();
				cout << "\nВведите номер учащегося(- для отмены операции):\n";
				cin >> s;
				if (s == "-") break;
				st = stoi(s);
				system("cls");
				cl.subjectsOutput();
				cout << "\nВведите номер предмета(- для отмены операции):\n";
				cin >> s;
				if (s == "-") break;
				sub = stoi(s);
				system("cls");
				cout << "Введите оценку для удаления(- для отмены операции):\n";
				cin >> s;
				if (s == "-") break;
				grade = stoi(s);
				if (!cl.deleteGrade(st - 1, sub - 1, grade)) cout << "Ошибка удаления оценки: данные введены неверно\n";
				break;
			}

			case 8: cl.saveToFile(className + ".dat"); return;
			default: "Ошибка";
			}
			system("pause");
			system("cls");
		}
		cl.saveToFile(className + ".dat");
	}
};

Menu Teacher::classMenu(
	{
			"Просмотреть учащихся",
			"Просмотреть преподаваемые предметы",
			"Вывести оценки учащихся по заданному предмету",
			"Вывести все оценки заданного ученика",
			"Добавить оценку ученику",
			"Добавить оценку всему классу по заданному предмету",
			"Удалить оценку ученика",
			"Назад"
	});

Menu Teacher::mainMenu(
	{
		"Просмотреть классы",
		"Просмотреть список ваших классов",
		"Выйти из аккаунта",
		"Выйти"
	});

class Admin : public User
{
public:

	static Menu classMenu;
	static Menu mainMenu;

	Admin(string l, string p) : User(l, p) {}

	virtual string getStatus()
	{
		return "admin";
	}

	virtual void saveToFile(ofstream& file)
	{
		file << "a " << login << ' ' << password;
	}

	virtual void classMenuF(string className, System* sys);
	

	virtual bool mainMenuF(vector<string>& classes, System* sys); //Возвращает true - выход из программы, false - релогин

};

Menu Admin::classMenu(
	{
			"Просмотреть учащихся",
			"Просмотреть преподаваемые предметы",
			"Вывести оценки учащихся по заданному предмету",
			"Вывести все оценки заданного ученика",
			"Добавить оценку ученику",
			"Добавить оценку всему классу по заданному предмету",
			"Удалить оценку ученика",
			"Добавить ученика",
			"Удалить ученика",
			"Добавить предмет",
			"Удалить предмет",
			"Связать учительский аккаунт с заданным предметом",
			"Отвязать учительский аккаунт от предмета",
			"Сбросить все оценки класса",
			"Назад"
	});

Menu Admin::mainMenu(
	{
		"Просмотреть классы",
		"Добавить класс",
		"Удалить класс",
		"Добавить пользователя",
		"Удалить пользователя",
		"Просмотреть список всех пользователей",
		"Выйти из аккаунта",
		"Выйти"
	});

User* User::loadFromFile(ifstream& file)
{
	char ch;
	string log, pas;
	file >> ch >> log >> pas;
	switch (ch)
	{
	case 'u':
	{
		User* user = new User(log, pas);
		return user;
	}

	case 'a':
	{
		Admin* user = new Admin(log, pas);
		return user;
	}

	case 't':
	{
		vector<string> cls;
		string cl;
		while (file.peek() != '\n')
		{
			file >> cl;
			cls.push_back(cl);
		}
		Teacher* user = new Teacher(log, pas, cls);
		return user;
	}
	}
}


class System
{
	vector<string> classes;
	const string dataFile;
	vector<User*> users;
	User* curUser;

public:

	System(string file) : dataFile(file)
	{
		loadData();
	}

	~System()
	{
		for (auto it = users.begin(); it != users.end(); it++) delete* it;
	}

	int findUser(string& l)
	{
		for (int i = 0; i < users.size(); i++)
		{
			if (users[i]->getLogin() == l) return i;
		}
		return -1;
	}

	void addClass(string className)
	{
		try
		{
			if (find(classes, className) != -1) throw;
			Class cl1(className);
			cl1.saveToFile(className + ".dat");
			classes.push_back(className);
		}
		catch (...)
		{
			cout << "\nОшибка: такой класс уже существует\n";
		}
	}

	void deleteClass(int sub)
	{
		try
		{
			if (sub < 0 || sub >= classes.size()) throw;

			for (int i = 0;i < users.size();i++)
			{
				if (typeid(*users[i]) == typeid(Teacher))
				{
					Teacher* t = dynamic_cast<Teacher*>(users[i]);
					int cl = t->findClass(classes[sub]);
					if (cl != -1)
					{
						t->deleteClass(cl);
					}
				}
			}

			classes.erase(classes.begin() + sub);
		}
		catch (...)
		{
			cout << "\nОшибка: номер класса введен неверно\n";
		}
	}

	bool addUser(string log, string pas, char ch)
	{
		try
		{
			if (findUser(log) != -1) throw;

			switch (ch)
			{
			case 'u':
			{
				User* user = new User(log, pas);
				users.push_back(user);
				return true;
			}

			case 'a':
			{
				Admin* user = new Admin(log, pas);
				users.push_back(user);
				return true;
				break;
			}
			case 't':
			{
				Teacher* user = new Teacher(log, pas);
				users.push_back(user);
				return true;
				break;
			}
			default: return false;
			}
		}
		catch (...)
		{
			cout << "\nОшибка: пользователь с таким именем уже существует\n";
			return false;
		}

	}

	bool deleteUser(int user)
	{
		try
		{
			if (user < 0 || user >= users.size()) throw;
			if (typeid(*users[user]) == typeid(Teacher))
			{
				Teacher* t = dynamic_cast<Teacher*>(users[user]);
				vector<string> cls = t->getClasses();
				for (int i = 0; i < cls.size(); i++)
				{
					Class cl;
					cl.loadFromFile(cls[i] + ".dat");
					cl.deleteTeacher(t->getLogin());
					cl.saveToFile(cls[i] + ".dat");
				}
			}
			delete users[user];
			users.erase(users.begin() + user);
			return true;
		}
		catch (...)
		{
			cout << "\nОшибка: номер пользователя введен неверно\n";
			return false;
		}
	}

	User* getUserPtr(int num)
	{
		return users[num];
	}

	void usersOutput()
	{
		for (int i = 0; i < users.size(); i++)
		{
			cout << i + 1 << ") " << users[i]->getLogin() << " - " << users[i]->getStatus() << endl;
		}
	}

	vector<Teacher*> returnTeachers()
	{
		vector<Teacher*> t;
		for (int i = 0; i < users.size(); i++)
		{
			if (typeid(*users[i]) == typeid(Teacher)) t.push_back((Teacher*)users[i]);
		}
		return t;
	}

	void saveData()
	{
		ofstream file(dataFile);
		for (int i = 0; i < users.size(); i++)
		{
			users[i]->saveToFile(file);
			file << '\n';
		}
		file << '.';
		for (int i = 0; i < classes.size(); i++)
		{
			file << classes[i] << ' ';
		}
		file << '.';
	}

	void loadData()
	{
		ifstream file(dataFile);
		if (!file.is_open())
		{
			saveData();
			file.open(dataFile);
		}
		string s;
		while (file.peek() != '.' && !file.eof())
		{
			users.push_back(User::loadFromFile(file));
			file.ignore();
		}
		file.ignore();
		while (file.peek() != '.' && !file.eof())
		{
			file >> s;
			classes.push_back(s);
			file.ignore();
		}
	}

	bool login()
	{
		string log, pas, reqPas;
		int userNum;
		if (users.empty()) if (!createFirstUser()) return false;
		while (true)
		{
			system("cls");
			cout << "Вход в систему\n\nВведите логин(- для выхода из программы):\n";
			cin >> log;
			if (log == "-") return false;
			userNum = findUser(log);
			if (userNum == -1)
			{
				cout << "\nТакого пользователя не существует!\n";
				system("pause");
				continue;
			}
			cout << "\nВведите пароль:\n";
			cin >> pas;
			if (!users[userNum]->checkPassword(pas))
			{
				cout << "\nПароль введен неверно!\n";
				system("pause");
				continue;
			}
			curUser = users[userNum];
			return true;
		}
	}

	bool createFirstUser()
	{
		string log, pas;
		cout << "В системе не зарегистрировано ни одного пользователя.\nПридумайте и ведите данные первого пользователя.\nЭтому пользователю будет присвоен статус администратора.\n\n";

		system("pause");
		system("cls");

		cout << "\nВведите логин(-для выхода из программы):\n";
		cin >> log;
		if (log == "-") return false;
		cout << "\nВведите пароль(-для выхода из программы):\n";
		cin >> pas;
		if (pas == "-") return false;
		addUser(log, pas, 'a');
		return true;
	}

	void menu()
	{
		if (!login()) return;
		while (!curUser->mainMenuF(classes, this))
		{
			saveData();
			if (!login()) return;
		}
		saveData();
	}
};

bool Admin::mainMenuF(vector<string>& classes, System* sys) //Возвращает true - выход из программы, false - релогин
{
	int choice;
	string s, s2, s3;
	system("cls");
	while (true)
	{
		cout << "Admin: " << login << "\n\n";

		switch (mainMenu.display())
		{
		case 1:
		{
			system("cls");

			output(classes);
			cout << "\nВведите номер класса для работы с ним(- для возвращения в главное меню):\n";

			cin >> s;
			if (s == "-") break;
			choice = stoi(s);
			if (choice - 1 < 0 || choice - 1 >= classes.size())
			{
				cout << "\nОшибка: класс указан неверно\n";
				break;
			}
			classMenuF(classes[choice - 1], sys);
			break;
		}
		case 2:
		{
			system("cls");

			cout << "Введите название нового класса(- для отмены операции):\n";
			cin >> s;
			if (s == "-") break;
			sys->addClass(s);
			break;
		}
		case 3:
		{
			system("cls");

			output(classes);
			cout << "\nВведите номер класса для удаления(- для отмены операции):\n";
			cin >> s;
			if (s == "-") break;
			choice = stoi(s);
			sys->deleteClass(choice - 1);

			break;
		}
		case 4:
		{
			system("cls");

			cout << "Введите логин нового пользователя(- для отмены операции):\n";
			cin >> s;
			if (s == "-") break;
			cout << "Введите пароль нового пользователя(- для отмены операции):\n";
			cin >> s2;
			if (s2 == "-") break;
			cout << "Выберите статус нового пользователя(- для отмены операции):\n1) User\n2) Teacher\n3) Admin\n\n";
			cin >> s3;
			if (s3 == "-") break;
			choice = stoi(s3);
			if (choice < 1 || choice > 3)
			{
				cout << "\nОшибка: номер статуса введен неверно\n";
				break;
			}
			switch (choice)
			{
			case 1: sys->addUser(s, s2, 'u'); break;
			case 2: sys->addUser(s, s2, 't'); break;
			case 3: sys->addUser(s, s2, 'a'); break;
			}
			break;
		}
		case 5:
		{
			system("cls");

			sys->usersOutput();
			cout << "\nВведите номер пользователя для удаления(- для отмены операции):\n";
			cin >> s;
			if (s == "-") break;
			choice = stoi(s);
			if (sys->getUserPtr(choice - 1) == this)
			{
				cout << "\nНевозможно удалить текущего пользователя\n";
				break;
			}
			sys->deleteUser(choice - 1);

			break;
		}
		case 6:
		{
			system("cls");

			cout << "Список всех пользователей:\n";
			sys->usersOutput();
			cout << endl;
			break;
		}

		case 7:
		{
			system("cls");
			return false;
		}
		case 8:
		{
			system("cls");
			return true;
		}
		}
		system("pause");
		system("cls");
	}
}


void Admin::classMenuF(string className, System* sys)
{
	Class cl;
	cl.loadFromFile(className + ".dat");
	int choice, st, sub, grade;
	string s;
	vector<Teacher*> teachers;
	system("cls");
	while (true)
	{
		cout << "Класс: " << cl.getClassName() << "\n\n";
		switch (classMenu.display())
		{
		case 1:
		{
			system("cls");

			cout << "Класс: " << cl.getClassName() << "\n\n";
			cout << "Список учеников:\n";
			cl.studentsOutput();
			break;
		}
		case 2:
		{
			system("cls");

			cout << "Класс: " << cl.getClassName() << "\n\n";
			cout << "Список преподаваемых предметов, a также учительские аккаунты с доступом к предмету:\n";
			cl.subjectsAndTeachersOutput();
			break;
		}
		case 3:
		{
			system("cls");

			cl.subjectsOutput();
			cout << "\nВведите номер предмета:\n";
			cin >> choice;
			system("cls");

			cout << "Класс: " << cl.getClassName() << "\n\n";
			cout << "Оценки всех учеников по предмету \"" << cl.getSubjectName(choice - 1) << "\":\n\n";
			cl.studentsSubGradesOutput(choice - 1);
			break;
		}
		case 4:
		{
			system("cls");

			cl.studentsOutput();
			cout << "\nВведите номер учащегося:\n";
			cin >> choice;
			s = cl.getStudentName(choice - 1);
			if (s == "-") break;
			system("cls");

			cout << "Класс: " << cl.getClassName() << "\n\n";
			cout << "Все оценки ученика " << s << ":\n\n";
			cl.studentAllGradesOutput(choice - 1);
			break;
		}
		case 5:
		{
			system("cls");

			cl.studentsOutput();
			cout << "\nВведите номер учащегося(- для отмены операции):\n";
			cin >> s;
			if (s == "-") break;
			st = stoi(s);
			system("cls");
			cl.subjectsOutput();
			cout << "\nВведите номер предмета(- для отмены операции):\n";
			cin >> s;
			if (s == "-") break;
			sub = stoi(s);
			system("cls");
			cout << "Введите оценку(- для отмены операции):\n";
			cin >> s;
			if (s == "-") break;
			grade = stoi(s);
			cl.addGrade(st - 1, sub - 1, grade);
			break;
		}
		case 6:
		{
			system("cls");
			cl.subjectsOutput();
			cout << "\nВведите номер предмета(- для отмены операции):\n";
			cin >> s;
			if (s == "-") break;
			sub = stoi(s);
			system("cls");
			cout << "\nВведите оценки для учеников:\n\n";
			for (int i = 0; i < cl.getNumberOfStud(); i++)
			{
				cout << i + 1 << ") " << cl.getStudentName(i) << ": ";
				cin >> s;
				if (s == "-") continue;
				grade = stoi(s);
				cl.addGrade(i, sub - 1, grade);
			}
			break;
		}
		case 7:
		{
			system("cls");

			cl.studentsOutput();
			cout << "\nВведите номер учащегося(- для отмены операции):\n";
			cin >> s;
			if (s == "-") break;
			st = stoi(s);
			system("cls");
			cl.subjectsOutput();
			cout << "\nВведите номер предмета(- для отмены операции):\n";
			cin >> s;
			if (s == "-") break;
			sub = stoi(s);
			system("cls");
			cout << "Введите оценку для удаления(- для отмены операции):\n";
			cin >> s;
			if (s == "-") break;
			grade = stoi(s);
			if (!cl.deleteGrade(st - 1, sub - 1, grade)) cout << "Ошибка удаления оценки: данные введены неверно\n";
			break;
		}

		case 8:
		{
			system("cls");
			string name, surname;
			cout << "Введите фамилию ученика(- для отмены операции):\n";
			cin >> surname;
			if (surname == "-") break;
			cout << "Введите имя ученика(- для отмены операции):\n";
			cin >> name;
			if (name == "-") break;
			cl.addStudent(surname + ' ' + name);
			break;
		}
		case 9:
		{
			system("cls");

			cout << "Список учеников:\n";
			cl.studentsOutput();
			cout << "Введите номер учениика для удаления(- для отмены операции):\n";
			cin >> s;
			if (s == "-") break;
			st = stoi(s);
			cl.deleteStudent(st - 1);
			break;
		}
		case 10:
		{
			system("cls");
			string n;
			cout << "Введите название предмета(- для отмены операции):\n";
			cin >> n;
			getline(cin, s, '\n');
			n += s;
			if (n == "-") break;
			cl.addSubject(n);
			break;
		}

		case 11:
		{
			system("cls");
			cl.subjectsOutput();

			cout << "\nВыберите предмет для удаления(- для отмены операции):\n";
			cin >> s;
			if (s == "-") break;
			sub = stoi(s);
			cl.deleteSubject(sub - 1);
			break;
		}

		case 12:
		{
			system("cls");

			teachers = sys->returnTeachers();
			cl.subjectsAndTeachersOutput();

			cout << "\nВыберите предмет(- для отмены операции):\n";
			cin >> s;
			if (s == "-") break;
			sub = stoi(s);
			cout << "\n";
			for (int i = 0; i < teachers.size(); i++)
			{
				cout << i + 1 << ") " << teachers[i]->getLogin() << endl;
			}

			cout << "\nВыберите учительский аккаунт для связывания(- для отмены операции):\n";
			cin >> s;
			if (s == "-") break;
			choice = stoi(s);

			if (choice - 1 < 0 || choice - 1 >= teachers.size())
			{
				cout << "\nОшибка: номер аккаунта выбран неверно\n";
				break;
			}

			cl.addTeacher(sub - 1, teachers[choice - 1]);
			break;
		}

		case 13:
		{
			system("cls");

			teachers = sys->returnTeachers();
			cl.subjectsAndTeachersOutput();

			cout << "\nВыберите предмет(- для отмены операции):\n";
			cin >> s;
			if (s == "-") break;
			sub = stoi(s);

			string t = cl.deleteTeacher(sub - 1);
			if (t != "" && cl.findTeacher(t) == -1)
			{
				for (int i = 0; i < teachers.size(); i++)
				{
					if (teachers[i]->getLogin() == t)
					{
						teachers[i]->deleteClass(cl.getClassName());
						break;
					}
				}
			}
			break;
		}

		case 14:
		{
			system("cls");

			cout << "Вы уверены, что хотите удалить все оценки класса? Если да, введите слово \"да\", если нет - любой другой символ:" << endl;
			cin >> s;
			if (s == "да")
			{
				cl.deleteAllGrades();
				cout << "\nВсе оценки класса были удалены\n";
			}

			break;
		}

		case 15: cl.saveToFile(className + ".dat"); return;


		default: "Ошибка";
		}
		system("pause");
		system("cls");
	}
	cl.saveToFile(className + ".dat");
}

void Class::addTeacher(int sub, Teacher* t)
{
	if (sub < 0 || sub >= subjects.size())
	{
		cout << "\nОшибка: номер предмета указан неверно\n";
		return;
	}
	subjects[sub].teacherLogin = t->getLogin();
	t->addClass(className);

}

int main()
{
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);

	System sys("data.dat");
	sys.menu();

	return 0;
}


