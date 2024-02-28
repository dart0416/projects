#include <iostream>
#include <fstream>

using std::fstream, std::cout, std::cin, std::string, std::endl;


int main()
{
    string name, surname;

    cout << "Podaj imię:  \n";
    cin >> name;
    cout << "Podaj nazwisko:  \n";
    cin >> surname;

    fstream file;
    file.open("file.txt", ios::out);
    
    file << name << endl;
    file << surname << endl;
    
    file.close;




}
