//
// Created by u215109 on 25/04/2023.
//

#ifndef EDA2_LAB_COMMON_H
#define EDA2_LAB_COMMON_H

#define MAX_LENGTH 20

#endif //EDA2_LAB_COMMON_H
typedef struct user{
    char[MAX_LENGTH] name;
    char[MAX_LENGTH] surname;
    char[MAX_LENGTH] username;
    int id;
    int birthday;
    int age;

} User;

typedef struct msg{
    char[MAX_LENGTH] text;

} Message;