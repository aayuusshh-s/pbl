#ifndef STAYS_H
#define STAYS_H

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "hashmap.h"

using namespace std;

class Stay {
public:
    string name;
    string type;
    string city;
    int price;
    int guests;
    int bhk;
    string owner;

    Stay() {}

    Stay(string n, string t, string c, int p, int g, int b, string o) {
        name = n;
        type = t;
        city = c;
        price = p;
        guests = g;
        bhk = b;
        owner = o;
    }

    void display() const {
        cout << "\nApartment: " << name << endl;
        cout << "Type: " << type << endl;
        cout << "City: " << city << endl;
        cout << "Price: Rs " << price << "/night" << endl;
        cout << "Maximum Guests: " << guests << endl;
        cout << "BHK: " << bhk << endl;
        cout << "Owner: " << owner << endl;
        cout << "--------------------------\n";
    }
};

class StayStore {
    vector<Stay> stays;
    StayHashMap hashMap;

public:

    StayStore() {
        load();
    }

    bool add(const Stay &s) {

        stays.push_back(s);

        int index = stays.size() - 1;

        hashMap.insert(s.city, index);

        save();
        saveApartmentDetails();

        return true;
    }

    int search(
        string city,
        int budget,
        int guests,
        int bhk
    ) const {

        vector<int> indexes =
            hashMap.searchByCity(city);

        int found = 0;

        for (int index : indexes) {

            const Stay &s = stays[index];

            if (s.price <= budget &&
                s.guests >= guests &&
                s.bhk == bhk) {

                s.display();
                found++;
            }
        }

        return found;
    }

    int listByOwner(string owner) const {

        int found = 0;

        for (const Stay &s : stays) {

            if (s.owner == owner) {

                s.display();
                found++;
            }
        }

        return found;
    }

    void viewApartmentDetails() const {

        ifstream file("apartment_details.txt");

        if (!file) {
            cout << "No apartment details found.\n";
            return;
        }

        string line;

        cout << "\n========== APARTMENT DETAILS ==========\n";

        while (getline(file, line)) {
            cout << line << endl;
        }

        cout << "=======================================\n";

        file.close();
    }

private:

    void save() const {

        ofstream file("stays.txt");

        for (const Stay &s : stays) {

            file << s.name << "|"
                 << s.type << "|"
                 << s.city << "|"
                 << s.price << "|"
                 << s.guests << "|"
                 << s.bhk << "|"
                 << s.owner << "\n";
        }

        file.close();
    }

    void saveApartmentDetails() const {

        ofstream file("apartment_details.txt");

        for (const Stay &s : stays) {

            file << "Apartment: "
                 << s.name << "\n";

            file << "Owner: "
                 << s.owner << "\n";

            file << "City: "
                 << s.city << "\n";

            file << "Type: "
                 << s.type << "\n";

            file << "BHK: "
                 << s.bhk << "\n";

            file << "Price: Rs "
                 << s.price
                 << "/night\n";

            file << "Maximum Guests: "
                 << s.guests << "\n";

            file << "--------------------------------\n";
        }

        file.close();
    }

    void load() {

        ifstream file("stays.txt");

        if (!file)
            return;

        string name;
        string type;
        string city;
        string price;
        string guests;
        string bhk;
        string owner;

        while (getline(file, name, '|') &&
               getline(file, type, '|') &&
               getline(file, city, '|') &&
               getline(file, price, '|') &&
               getline(file, guests, '|') &&
               getline(file, bhk, '|') &&
               getline(file, owner)) {

            Stay s(
                name,
                type,
                city,
                stoi(price),
                stoi(guests),
                stoi(bhk),
                owner
            );

            stays.push_back(s);

            int index = stays.size() - 1;

            hashMap.insert(city, index);
        }

        file.close();
    }
};

#endif