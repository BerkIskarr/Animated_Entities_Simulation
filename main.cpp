#include <iostream>
#include <random>
#include <vector>
#include <set>
#include <unistd.h>
#include <ctime>

using namespace std;

// Generate random coordinates for entities
class entity;


class Map;

class entity {
public:
    int health = 100;
    int strength;
    char symbol;
    string type;
    string name;
    pair<int,int> coordinates;

public:

    char getSymbol() const {
        return symbol;
    }

    string getType() const {
        return type;
    }

    int getHealth()  {
        return health;
    }
    string getName() const{
        return name;
    }
    int getStrength() const{
        return strength;
    }
    
    pair<int,int> getCoordinates(){
        return coordinates;
    }
    
    void setCoordinates(const pair<int, int>& newCoordinates) {
        coordinates = newCoordinates;
    }
    
    void setHealth(int newhealth){
        health = newhealth;
    }

   virtual string getInfo() {
        return ("Type: " + type + "\nName: " + name + "\nHealth: " + to_string(health));
    }
    virtual ~entity() {}
};


void populate(vector<entity*> &entities){
    vector<pair<int, int>> coordinates;
    int counter=0;
     // Generate random coordinates
     set<pair<int, int>> randomCoordinates;
     while (randomCoordinates.size() < 12) {
         int x = rand() % 10;
         int y = rand() % 10;
         randomCoordinates.insert(make_pair(x, y));
     }

     // Copy random coordinates to the vector
     for (const auto& coord : randomCoordinates) {
         coordinates.push_back(coord);
     }

     // Generate remaining coordinates for the grid

     while (coordinates.size() < 100) {
         int x = rand() % 10;
         int y = rand() % 10;
         pair<int, int> newCoord = make_pair(x, y);

         // Check if the coordinate already exists in the vector
         if (find(coordinates.begin(), coordinates.end(), newCoord) == coordinates.end()) {
             coordinates.push_back(newCoord);
         }
     }
    for (const auto& entity: entities){
        entity->setCoordinates(coordinates[counter]);
        counter++;
    }
}

class Map {

public:
    int width;
    int height;
    vector<vector<char>> map;
    Map(int width, int height, vector<entity*> &entities) : width(width), height(height), map(height, vector<char>(width, '.')) {
        for (const auto& entity : entities) {
            const auto& coordinates = entity->getCoordinates();
            location(coordinates.first, coordinates.second, entity->getSymbol(), entities);
        }
    }

    void display() {
        cout << "  ";
        for (int i = 0; i < width; i++) {
            cout << i << ' ';
        }
        cout << endl;
        for (int i = 0; i < height; i++) {
            cout << i << ' ';
            for (int j = 0; j < width; j++) {
                cout << map[i][j] << ' ';
            }
            cout << endl;
        }
    }
    
    void location(int x, int y, char symbol, vector<entity*>& entities) {
        if (x >= 0 && x < width && y >= 0 && y < height && map[y][x] == '.') {
            bool occupied = true;
            for (const auto& entity : entities) {
                if (entity->getCoordinates().first == x && entity->getCoordinates().second == y) {
                    occupied = false;
                    break;
                }
            }
            if (!occupied) {
                map[y][x] = symbol;
            }
        }
    }

    void new_location(int x, int y, char symbol, vector<entity*>& entities){
       
            map[y][x] = symbol;
        
    }
};


class Human : public entity {
public:
    Human(const string& n){
        name = n;
        symbol = '@';
        type = "Human";
    }
    string getInfo()  {
         return ("Type: " + type + "\nName: " + name + "\nHealth: " + to_string(health));
     }
};
    
    

class Dragon : public entity {
public:
    Dragon(string n){
        symbol = '#';
        name = n;
        type = "Dragon";
    }

};

class Monster : public entity {
 
public:
    Monster(string n) {
        symbol = '*';
        strength = rand() % 6;
        name = n;
        type = "Monster";
    }
 
    string getInfo()  {
         return ("Type: " + type + "\nName: " + name + "\nHealth: " + to_string(health) + "\nStrength: " + to_string(strength));
     }

    
};

class Stone: public entity {

public:
    Stone(){
        symbol = 'S';
        type = "Stone";
    }
    string getInfo()  {
         return ("Type: " + type);
     }


};

class Hole: public entity {
private:
int depth;
public:
    Hole(){
        symbol = 'O';
        type = "Hole";
        depth = rand() % 21;
    }
    string getInfo()  {
         return ("Type: " + type +"\nDepth: " + to_string(depth));
     }
};


class Room {

    
public:
    vector<entity*> entities;
    set<pair<int, int>> takenCoordinates;
    
    void addEntity(vector<entity*>& entities) {

        entities.push_back(new Human("Harold"));
        entities.push_back(new Human("David"));
        entities.push_back(new Human("Clare"));

        entities.push_back(new Dragon("Wyvern"));
        entities.push_back(new Dragon("Smaug"));

        entities.push_back(new Monster("Vampire"));
        entities.push_back(new Monster("Zombie"));
        entities.push_back(new Monster("Minotaur"));

        entities.push_back(new Hole());
        entities.push_back(new Hole());

        entities.push_back(new Stone());
        entities.push_back(new Stone());
        populate(entities);

    }
    
    string display_info(const pair<int, int>& coordinates)  {
        string charInfo;
        bool foundEntity = false;
        
        for (const auto& e : entities) {
            if (e->getCoordinates() == coordinates) {
                charInfo += e->getInfo() + "\n\n";
                foundEntity = true;
            }
        }
        
        if (!foundEntity) {
            charInfo = "No entity found at coordinates";
        }
        
        return charInfo;
    }
    vector<entity*> getEntities() const {
        return entities;
    }
    set<pair<int, int>>& getTakenCoordinates()  {
        return takenCoordinates;
    }
    void reset() {
        entities.clear();
        takenCoordinates.clear();
        
    }
