#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "interface.h"

void main(int argc, char *argv[]){
	FILE* inputfile = NULL;
	FILE* configfile = NULL;
	char delim[] = " \n";
	int hashtableNumOfEntries = 10;
	char *line_buf = NULL;
	size_t line_buf_size = 0;
	for(int i = 1 ; i < argc ; i++){
		if(strcmp(argv[i],"-i")==0){
			inputfile = fopen(argv[i+1],"r");
		}
		if(strcmp(argv[i],"-c")==0){
			configfile = fopen(argv[i+1],"r");
			getline(&line_buf, &line_buf_size, configfile);
			char* ptr = strtok(line_buf, delim);
	   		if(strcmp(ptr,"hashtableNumOfEntries") == 0){
	   			ptr = strtok(NULL, delim);
	   			hashtableNumOfEntries = atoi(ptr);
	   		}
	   		fclose(configfile);
		}
	}
	int line_count = 0;
	ssize_t line_size = -1;
	if(inputfile != NULL) line_size = getline(&line_buf, &line_buf_size, inputfile);
	struct student* tempStudent;
	
	int first = 1;
	int duplicates = 0;

	TableEntry* studentHashtable = createHashtable(hashtableNumOfEntries);

    struct student* firstStudent = NULL;
    struct index* invertedindex = NULL;
    while (line_size >= 0){
	    line_count++;

	    char *ptr = strtok(line_buf, delim);

		tempStudent = malloc(sizeof(struct student));

    	tempStudent->studentID = malloc(strlen(ptr)+1);
    	strcpy(tempStudent->studentID,ptr);

		ptr = strtok(NULL, delim);
	   	tempStudent->lastName = malloc(strlen(ptr)+1);
	   	strcpy(tempStudent->lastName,ptr);

    	ptr = strtok(NULL, delim);
    	tempStudent->firstName = malloc(strlen(ptr)+1);
    	strcpy(tempStudent->firstName,ptr);

    	ptr = strtok(NULL, delim);
    	tempStudent->zip = atoi(ptr);

	    ptr = strtok(NULL, delim);
	    tempStudent->year = atoi(ptr);

    	ptr = strtok(NULL, delim);
    	tempStudent->gpa = atof(ptr);

	   	tempStudent->next = NULL;
	   	
	   	int h = hash(tempStudent->studentID, hashtableNumOfEntries);
	    
	    struct student* checkStudent = studentHashtable[h].firstStudent;
	    int found = 0;
	   	while(checkStudent != NULL){
	   		if(strcmp(checkStudent->studentID,tempStudent->studentID) == 0){
	   			duplicates++;
	   			found = 1;
	   		}
	   		checkStudent = checkStudent->next;
	   	}
	   	if(found == 0){
	    	struct student* newStudent = insertHashtable(h,tempStudent,studentHashtable);
	    	invertedindex = insertIndex(invertedindex, newStudent);
		}
	    
		free(tempStudent->studentID);
		free(tempStudent->firstName);
		free(tempStudent->lastName);
		free(tempStudent);
	   	
	    line_size = getline(&line_buf, &line_buf_size, inputfile);
	}
	if(inputfile != NULL) fclose(inputfile);
	printf("%d duplicated records found and not inserted\n",duplicates);
	char buf[80];
	char* ptr;
	while(1){
		fgets(buf,80,stdin);
		ptr = strtok(buf, delim);
		if(strcmp(ptr,"exit") == 0){
			exitFree(studentHashtable,invertedindex,hashtableNumOfEntries);
			printf("exit program\n");
			break;
		}
		else if(strcmp(ptr,"i") == 0){ ////insert
			tempStudent = malloc(sizeof(struct student));
			ptr = strtok(NULL, delim);
			tempStudent->studentID = malloc(strlen(ptr)+1);
    		strcpy(tempStudent->studentID,ptr);
    		
    		ptr = strtok(NULL, delim);
	   		tempStudent->lastName = malloc(strlen(ptr)+1);
	   		strcpy(tempStudent->lastName,ptr);
			
			ptr = strtok(NULL, delim);
			tempStudent->firstName = malloc(strlen(ptr)+1);
    		strcpy(tempStudent->firstName,ptr);

    		ptr = strtok(NULL, delim);
    		tempStudent->zip = atoi(ptr);

	    	ptr = strtok(NULL, delim);
	    	tempStudent->year = atoi(ptr);

    		ptr = strtok(NULL, delim);
    		tempStudent->gpa = atof(ptr);

	   		tempStudent->next = NULL;
	   	
	   		int h = hash(tempStudent->studentID, hashtableNumOfEntries);
	   		struct student* checkStudent = studentHashtable[h].firstStudent;
	   		int found = 0;
	   		while(checkStudent != NULL){
	   			if(strcmp(checkStudent->studentID,tempStudent->studentID) == 0){
	   				printf("Student %s exists\n",tempStudent->studentID);
	   				found = 1;
	   			}
	   			checkStudent = checkStudent->next;
	   		}
	   		if(found == 0){
	    		struct student* newStudent = insertHashtable(h,tempStudent,studentHashtable);
	    		invertedindex = insertIndex(invertedindex, newStudent);
	    		printf("Student %s inserted\n",tempStudent->studentID);
			}
			free(tempStudent->studentID);
			free(tempStudent->firstName);
			free(tempStudent->lastName);
			free(tempStudent);
		}
		else if(strcmp(ptr,"l") == 0){
			char* studentID = NULL;
			ptr = strtok(NULL, delim);
			studentID = malloc(strlen(ptr)+1);
    		strcpy(studentID,ptr);
    		int h = hash(studentID, hashtableNumOfEntries);
    		struct student* tempStudent = lookup(h,studentHashtable,studentID);
    		if(tempStudent == NULL){
    			("Student %s does not exist\n",studentID);
    		}
    		else
    			printf("%s %s %s %d %d %.2f\n",tempStudent->studentID,tempStudent->lastName,tempStudent->firstName,tempStudent->zip,tempStudent->year,tempStudent->gpa);
    		free(studentID);
		}
		else if(strcmp(ptr,"d") == 0){
			char* studentID = NULL;
			ptr = strtok(NULL, delim);
			studentID = malloc(strlen(ptr)+1);
    		strcpy(studentID,ptr);
    		int h = hash(studentID, hashtableNumOfEntries);
    		
    		delete(h,studentID,studentHashtable,invertedindex);
    		free(studentID);
		}
		else if(strcmp(ptr,"n") == 0){
			int year;
			int count = 0;
			ptr = strtok(NULL, delim);
			year = atoi(ptr);
    		count = number(invertedindex,year);
    		if(count == 0)
    			printf("No students enrolled in %d\n",year);
    		else
    			printf("%d student(s) in %d\n",count, year);
    		
		}
		else if(strcmp(ptr,"t") == 0){
			int num,year;
			ptr = strtok(NULL, delim);
			num = atoi(ptr);
			
			ptr = strtok(NULL, delim);
			year = atoi(ptr);
			
			top(invertedindex,num,year);
			printf("\n");
		}
		else if(strcmp(ptr,"a") == 0){
			int year;
			
			ptr = strtok(NULL, delim);
			year = atoi(ptr);
			
			average(invertedindex,year);
		}
		else if(strcmp(ptr,"m") == 0){
			int year;
			
			ptr = strtok(NULL, delim);
			year = atoi(ptr);
			
			minimum(invertedindex,year);
		}
		else if(strcmp(ptr,"c") == 0){
			
			count(invertedindex);
		}
		else if(strcmp(ptr,"p") == 0){
			int rank;
			
			ptr = strtok(NULL, delim);
			rank = atoi(ptr);
			
			postalcode(studentHashtable,rank,hashtableNumOfEntries);
			printf("is/are %d most popular\n",rank);
		}
	}	
	
}















