#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "interface.h"

struct index* insertIndex(struct index* invertedindex, struct student* newStudent){ /*insert a new record in the invertedIndex*/
	if(invertedindex == NULL){ /*create invertedIndex*/
		invertedindex = malloc(sizeof(struct index));
    	invertedindex->year = newStudent->year;
    	invertedindex->students = malloc(sizeof(struct studentList));
		invertedindex->students->Student = newStudent;
		invertedindex->students->next = NULL;
    	invertedindex->next = NULL;
    	return invertedindex;
	}
	struct index* tempIndex = invertedindex;
	struct studentList* list;
	if(newStudent->year > invertedindex->year){
		tempIndex = malloc(sizeof(struct index));
    	tempIndex->year = newStudent->year;
    	tempIndex->students = malloc(sizeof(struct studentList));
		tempIndex->students->Student = newStudent;
		tempIndex->students->next = NULL;
    	tempIndex->next = invertedindex;
    	return tempIndex;
	}
	struct index* newIndex;
	struct index* previousIndex;
	while(newStudent->year != tempIndex->year){
		if(newStudent->year > tempIndex->year){
			newIndex = malloc(sizeof(struct index));
    		newIndex->year = newStudent->year;
    		newIndex->students = malloc(sizeof(struct studentList));
			newIndex->students->Student = newStudent;
			newIndex->students->next = NULL;
    		newIndex->next = tempIndex;
    		previousIndex->next = newIndex;
    		return invertedindex;
		}
		if(tempIndex->next == NULL){
			tempIndex->next = malloc(sizeof(struct index));
    		tempIndex->next->year = newStudent->year;
    		tempIndex->next->students = malloc(sizeof(struct studentList));
			tempIndex->next->students->Student = newStudent;
			tempIndex->next->students->next = NULL;
    		tempIndex->next->next = NULL;
    		return invertedindex;
		}
		previousIndex = tempIndex;
		tempIndex = tempIndex->next;
	}
	if(tempIndex->students == NULL){
		tempIndex->students = malloc(sizeof(struct studentList));
		tempIndex->students->Student = newStudent;
		tempIndex->students->next = NULL;
	}
	else{
		list = tempIndex->students;
		while(list->next != NULL){
			list = list->next;
		}
		list->next = malloc(sizeof(struct studentList));
		list->next->Student = newStudent;
		list->next->next = NULL;
	}
	return invertedindex;
}

TableEntry* createHashtable(int hashtableNumOfEntries){ /*create a hash table */
	TableEntry* hashtable = malloc(hashtableNumOfEntries*sizeof(TableEntry));
	for(int k = 0 ; k < hashtableNumOfEntries ; k++){
		hashtable[k].key = k;
		hashtable[k].firstStudent = NULL;
	}
	return hashtable;
}

int hash(char* K, int i){ /*hash function for strings*/
  int h=0, a=127;
  for (; *K!='\0'; K++)
    h=(a*h + *K) % i;
  return h;
}

struct student* insertHashtable(int h, struct student* newStudent, TableEntry* studentHashtable){ /*insert a new record in the Hashtable*/
    if(studentHashtable[h].firstStudent == NULL){
    	studentHashtable[h].firstStudent = malloc(sizeof(struct student));
    	studentHashtable[h].firstStudent->studentID = malloc(strlen(newStudent->studentID)+1);
    	strcpy(studentHashtable[h].firstStudent->studentID,newStudent->studentID);
    	
    	studentHashtable[h].firstStudent->lastName = malloc(strlen(newStudent->lastName)+1);
    	strcpy(studentHashtable[h].firstStudent->lastName,newStudent->lastName);
    
    	studentHashtable[h].firstStudent->firstName = malloc(strlen(newStudent->firstName)+1);
    	strcpy(studentHashtable[h].firstStudent->firstName,newStudent->firstName);
    
    	studentHashtable[h].firstStudent->zip = newStudent->zip;
    	studentHashtable[h].firstStudent->year = newStudent->year;
    	studentHashtable[h].firstStudent->gpa = newStudent->gpa;
    	studentHashtable[h].firstStudent->next = NULL;
    }
    else{
    	struct student* tempStudent = studentHashtable[h].firstStudent;
    	while(tempStudent->next != NULL){
    		tempStudent = tempStudent->next;
    	}
    	tempStudent->next = malloc(sizeof(struct student));
    	tempStudent->next->studentID = malloc(strlen(newStudent->studentID)+1);
    	strcpy(tempStudent->next->studentID,newStudent->studentID);
    
    	tempStudent->next->firstName = malloc(strlen(newStudent->firstName)+1);
    	strcpy(tempStudent->next->firstName,newStudent->firstName);
    	
    	tempStudent->next->lastName = malloc(strlen(newStudent->lastName)+1);
    	strcpy(tempStudent->next->lastName,newStudent->lastName);
    
    	tempStudent->next->zip = newStudent->zip;
    	tempStudent->next->year = newStudent->year;
    	tempStudent->next->gpa = newStudent->gpa;
    	tempStudent->next->next = NULL;
    	return tempStudent->next;
    }
    return studentHashtable[h].firstStudent;
}

struct student* lookup(int h, TableEntry* studentHashtable, char* studentID){ /*find a record and return it*/
	struct student* tempStudent = studentHashtable[h].firstStudent;
	if(tempStudent == NULL) return NULL;
	while(strcmp(tempStudent->studentID,studentID) != 0){
		tempStudent = tempStudent->next;
		if(tempStudent == NULL)
			return NULL;
	}
	return tempStudent;
}

void delete(int h, char* studentID, TableEntry* studentHashtable, struct index* invertedIndex){ /*delete a record*/
	struct student* tempStudent = studentHashtable[h].firstStudent;
	int first = 1;
	int year;
	if(tempStudent == NULL){
		printf("Student %s does not exist\n",studentID);
		return;
	}
	struct student* previousStudent = studentHashtable[h].firstStudent;
	while(strcmp(tempStudent->studentID,studentID) != 0){
		first = 0;
		previousStudent = tempStudent;
		tempStudent = tempStudent->next;
		if(tempStudent == NULL){
			printf("Student %s does not exist\n",studentID);
			return;
		}
	}
	year = tempStudent->year;
	
	
	struct index* tempIndex = invertedIndex;
	while(tempIndex->year != year)
		tempIndex = tempIndex->next;	
		
	struct studentList* list = tempIndex->students;
	int firstList = 1;
	struct studentList* previousNode = tempIndex->students;
	while(list->Student->year != year){
		firstList = 0;
		previousNode = list;
		list = list->next;
	}
	if(firstList == 1){
		tempIndex->students = tempIndex->students->next;
	}
	else{
		previousNode->next = list->next;
	}
	
	if(first == 1){
		free(studentHashtable[h].firstStudent->studentID);
		free(studentHashtable[h].firstStudent->firstName);
		free(studentHashtable[h].firstStudent->lastName);
		struct student* deleteStudent = studentHashtable[h].firstStudent;
		studentHashtable[h].firstStudent = studentHashtable[h].firstStudent->next;
		free(deleteStudent);
	}
	else{
		struct student* deleteStudent = tempStudent;
		previousStudent->next = tempStudent->next;
		free(tempStudent->studentID);
		free(tempStudent->firstName);
		free(tempStudent->lastName);
		free(tempStudent);
	}
	
	printf("Record %s deleted\n",studentID);
}

int number(struct index* invertedIndex,int year){ /*returns number of students in a year*/
	int count = 0;
	struct index* tempIndex = invertedIndex;
	while(tempIndex->year != year){
		if(tempIndex->next == NULL) return 0;
		tempIndex = tempIndex->next;
	}
	struct studentList* list = tempIndex->students;
	while(list != NULL){
		count++;
		list = list->next;
	}
	return count;
}

void top(struct index* invertedIndex,int num,int year){/*prints top num students in a year*/
	float max = 0.00;
	int exists = 0;
	struct studentList* newList = malloc(sizeof(struct studentList));
	newList->Student = NULL;
	newList->next = NULL;
	struct student* maxStudent = NULL;
	struct index* tempIndex = invertedIndex;
	while(tempIndex->year != year){
		if(tempIndex->next == NULL){
			printf("No students enrolled in %d",year);
			return;
		}
		tempIndex = tempIndex->next;
	}
	struct studentList* tempList = newList;
	struct studentList* list = tempIndex->students;
	for(int i = 0 ; i < num ; i++){
		list = tempIndex->students;
		while(list != NULL){
			exists = 0;
			if(list->Student->gpa > max){
				tempList = newList;
				while(tempList->Student != NULL){
					if(strcmp(list->Student->studentID,tempList->Student->studentID) == 0){
						exists = 1;
					}
					if(tempList->next == NULL) break;
					tempList = tempList->next;
				}
				if(exists == 0){
					max = list->Student->gpa;
					maxStudent = list->Student;					
				}
			}
			list = list->next;
		}
		printf("%s",maxStudent->studentID);
		if(i != num-1) printf(", ");
		tempList = newList;
		while(tempList->Student != NULL){
			if(tempList->next == NULL){
				tempList->next = malloc(sizeof(struct studentList));
				tempList->next->next = NULL;
				tempList->next->Student = maxStudent;
				break;
			}
			tempList = tempList->next;
		}
		if(newList->Student == NULL) newList->Student = maxStudent;
		max = 0.00;
	}
	struct studentList* deleteList = newList;
	tempList = newList;
	while(deleteList != NULL){
		tempList = deleteList->next;
		free(deleteList);
		deleteList = tempList;
	}
}

void average(struct index* invertedIndex,int year){ /*prints average gpa in a year*/
	struct index* tempIndex = invertedIndex;
	float total = 0.00;
	int count = 0;
	while(tempIndex->year != year){
		if(tempIndex->next == NULL){
			printf("No students enrolled in %d\n",year);
			return;
		}
		tempIndex = tempIndex->next;
	}
	struct studentList* list = tempIndex->students;
	while(list != NULL){
		count++;
		total = total + list->Student->gpa;
		list = list->next;
	}
	float a = total/count;
	printf("%.2f\n",a);
}

void minimum(struct index* invertedIndex, int year){ /*prints worst gpa in a year*/
	struct index* tempIndex = invertedIndex;
	while(tempIndex->year != year){
		if(tempIndex->next == NULL){
			printf("No students enrolled in %d\n",year);
			return;
		}
		tempIndex = tempIndex->next;
	}
	float min = 10.00;
	struct studentList* list = tempIndex->students;
	while(list != NULL){
		if(list->Student->gpa < min){
			min = list->Student->gpa;
		}
		list = list->next;
	}
	list = tempIndex->students;
	while(list != NULL){
		if(list->Student->gpa == min){
			printf("%s ",list->Student->studentID);
		}
		list = list->next;
	}
	printf("\n");
}

void count(struct index* invertedIndex){/*calls number function for every year*/
	struct index* tempIndex = invertedIndex;
	if(invertedIndex == NULL) printf("No students are enrolled\n");
	while(tempIndex != NULL){
		printf("(%d, %d)",tempIndex->year,number(invertedIndex,tempIndex->year));
		if(tempIndex->next != NULL) printf(",");
		tempIndex = tempIndex->next;
	}
	printf("\n");
}

void postalcode(TableEntry* studentHashtable, int rank, int hashtableNumOfEntries){ /*prints rankth most popular postal code*/
	struct student* tempStudent;
	struct postalcodeList* list = NULL;
	struct postalcodeList* tempList = NULL;
	for(int i = 0 ; i < hashtableNumOfEntries ; i++){
		tempStudent = studentHashtable[i].firstStudent;
		while(tempStudent != NULL){
			int found = 0;
			tempList = list;
			while(tempList != NULL){
				if(tempList->postalcode == tempStudent->zip){
					tempList->count++;
					found = 1;
					break;
				}
				tempList = tempList->next;
			}
			if(found == 0){
				if(list == NULL){
					list = malloc(sizeof(struct postalcodeList));
					list->count = 1;
					list->postalcode = tempStudent->zip;
					list->next = NULL;
				}
				else{
					tempList = list;
					while(tempList->next != NULL){
						tempList = tempList->next;
					}
					tempList->next = malloc(sizeof(struct postalcodeList));
					tempList->next->postalcode = tempStudent->zip;
					tempList->next->next = NULL;
					tempList->next->count = 1;
				}
			}
			tempStudent = tempStudent->next;
		}
	}
	struct postalcodeList* rankList = NULL;
	struct postalcodeList* checkList = NULL;
	for(int j = 1 ; j < rank ; j++){
		int max = 0;
		int maxZip;
		int exists = 0;
		tempList = list;
		while(tempList != NULL){
			if(tempList->count > max){
				checkList = rankList;
				exists = 0;
				while(checkList != NULL){
					if(checkList->postalcode == tempList->postalcode){
						exists = 1;
					}
					checkList = checkList->next;
				}
				if(exists == 0){
					max = tempList->count;
					maxZip = tempList->postalcode;
				}
			}
			tempList = tempList->next;
		}
		if(rankList == NULL){
			rankList = malloc(sizeof(struct postalcodeList));
			rankList->postalcode = maxZip;
			rankList->next = NULL;
		}
		else{
			tempList = rankList;
			while(tempList->next != NULL){
				tempList = tempList->next;
			}
			if(tempList->postalcode == maxZip) j--;
			tempList->next = malloc(sizeof(struct postalcodeList));
			tempList->next->postalcode = maxZip;
			tempList->next->next = NULL;
		}	
	}
	int first = 1;
	int max = 0;
	int lastmax = 0;
	while(1){
		lastmax = max;
		max = 0;
		int count = 0;
		int maxZip = 0;
		int exists = 0;
		tempList = list;
		while(tempList != NULL){
			if(tempList->count > max){
				checkList = rankList;
				exists = 0;
				while(checkList != NULL){
					if(checkList->postalcode == tempList->postalcode){
						exists = 1;
						break;
					}
					checkList = checkList->next;
				}
				if(exists == 0){
					max = tempList->count;
					maxZip = tempList->postalcode;
				}
			}
			tempList = tempList->next;
		}
		if(rankList == NULL){
			rankList = malloc(sizeof(struct postalcodeList));
			rankList->postalcode = maxZip;
			rankList->count = max;
			rankList->next = NULL;
		}
		else{
			tempList = rankList;
			while(tempList->next != NULL){
				tempList = tempList->next;
			}
			tempList->next = malloc(sizeof(struct postalcodeList));
			tempList->next->postalcode = maxZip;
			tempList->next->count = max;
			tempList->next->next = NULL;
		}
		if((first == 0) && (lastmax != max)) break;
		printf("%d ",maxZip);
		first = 0;
	}
	tempList = list;
	while(tempList!=NULL){
		checkList = tempList;
		tempList = tempList->next;
		free(checkList);
	}
	tempList = rankList;
	while(tempList!=NULL){
		checkList = tempList;
		tempList = tempList->next;
		free(checkList);
	}
}

void exitFree(TableEntry* studentHashtable,struct index* invertedIndex,int hashtableNumOfEntries){/*free all malloced space*/
	struct index* tempIndex = invertedIndex;
	struct index* deleteIndex;
	struct studentList* list;
	struct studentList* deleteList;
	while(tempIndex != NULL){
		list = tempIndex->students;
		while(list != NULL){
			deleteList = list;
			list = list->next;
			free(deleteList);
			deleteList = NULL;
		}
		deleteIndex = tempIndex;
		tempIndex = tempIndex->next;
		free(deleteIndex);
		deleteIndex = NULL;
	}
	struct student* tempStudent;
	struct student* deleteStudent;
	for(int i = 0 ; i < hashtableNumOfEntries ; i++){
		tempStudent = studentHashtable[i].firstStudent;
		while(tempStudent != NULL){
			deleteStudent = tempStudent;
			tempStudent = tempStudent->next;
			free(deleteStudent->studentID);
			free(deleteStudent->firstName);
			free(deleteStudent->lastName);
			free(deleteStudent);
			deleteStudent = NULL;
		}
	}
	free(studentHashtable);
}