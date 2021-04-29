struct student{
	char* studentID;
	char* lastName;
	char* firstName;
	int zip;
	int year;
	float gpa;
	struct student* next;
};

struct postalcodeList{
	int postalcode;
	int count;
	struct postalcodeList* next;
};

struct index {
	int year;
	struct index* next;
	struct studentList* students;
};

struct studentList {
	struct student* Student;
	struct studentList* next;
};

typedef struct {
	int key;
	struct student* firstStudent;
} TableEntry;