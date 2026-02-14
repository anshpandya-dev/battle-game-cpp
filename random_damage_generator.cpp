/*
Project: Battle Game C++
File: battle_logic_test.cpp
Author: GULSHAN JATWA
Date: 14-02-2026
Description: Testing random damage range using rand() and srand().
*/

#include <iostream>
#include <cstdlib> 
#include <ctime>   

using namespace std;

int main() {
    // Ye line magic hai,kyuki  ise likhna jaruri he taki har bar alag random number mile pc ke clock ke hisab se, agar is line ko nahi likhenge to har bar same random number milega
    srand(time(0)); 

    int randomDamage = rand() % 90 + 10; // 10 se 100 ke beech damage generate karega
    cout << "your Random Damage is: " << randomDamage << endl;

    return 0;
}
    // rand() ek funtion he c++ me biult in funtion jo ki random number detha he
    // srand() ek funtion he c++ me biult in funtion jo ki random number generator ko seed karta he, iska use karne se har bar alag random number milega, agar is line ko nahi likhenge to har bar same random number milega