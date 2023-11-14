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
    void move(Map& map) {
        for (auto& entity : getEntities()) {
            if (entity->getType() == "Human") {
                int current_X = entity->getCoordinates().first;
                int current_Y = entity->getCoordinates().second;
                
                int new_X = current_X + 1;  // Move to the cell on the right
                int new_Y = current_Y;
                // If the new position is occupied or at the edge of the room, move to a random adjacent unoccupied cell
                if (new_X >= map.width || map.map[current_Y][new_X] != '.') {
                    // Generate random direction (up, down, left)
                    int direction = rand() % 3;
                    if (direction == 0) {  // Move up
                        new_X = current_X;
                        new_Y--;
                    } else if (direction == 1) {  // Move down
                        new_X = current_X;
                        new_Y++;
                    } else if (direction == 2) {  // Move left
                        new_X = current_X;
                        new_X--;
                    }
                }
                
                // Update entity coordinates if the new position is valid
                if (new_X >= 0 && new_X < map.width &&  new_Y >= 0 &&  new_Y < map.height && map.map[ new_Y][new_X] == '.' ) {
                    // Check if the new position is not occupied by another entity
                    bool validMove = true;
                    for (const auto& otherEntity : getEntities()) {
                        if (&otherEntity != &entity && otherEntity->getCoordinates() == make_pair(new_X,  new_Y)) {
                            validMove = false;
                            break;
                        }
                    }
                    
                    if (validMove) {
                        // Reduce health by one since the human is moving
                        entity->setHealth(entity->getHealth() - 1);
                        
                        // Update map locations
                        map.new_location(current_X, current_Y, '.', entities);
                        map.new_location(new_X,  new_Y, entity->getSymbol(), entities);
                        
                        // Update entity coordinates
                        entity->setCoordinates(make_pair(new_X,  new_Y));
                    }
                }
            }
            
            if (entity->getType() == "Dragon") {
                int current_X = entity->getCoordinates().first;
                int current_Y = entity->getCoordinates().second;
                
                vector<pair<int, int>> possibleMoves;
                
                // Check the cells adjacent to the current position
                if (current_X - 1 >= 0 && map.map[current_Y][current_X - 1] == '.') {
                    possibleMoves.push_back(make_pair(current_X - 1, current_Y)); // Move left
                }
                if (current_X + 1 < map.width && map.map[current_Y][current_X + 1] == '.') {
                    possibleMoves.push_back(make_pair(current_X + 1, current_Y)); // Move right
                }
                if (current_Y - 1 >= 0 && map.map[current_Y - 1][current_X] == '.') {
                    possibleMoves.push_back(make_pair(current_X, current_Y - 1)); // Move up
                }
                if (current_Y + 1 < map.height && map.map[current_Y + 1][current_X] == '.') {
                    possibleMoves.push_back(make_pair(current_X, current_Y + 1)); // Move down
                }
                
                if (!possibleMoves.empty()) {
                    // Select a random move from the possibleMoves vector
                    int randomIndex = rand() % possibleMoves.size();
                    pair<int, int> newCoordinates = possibleMoves[randomIndex];
                    
                    // Update entity coordinates
                    entity->setCoordinates(newCoordinates);
                    
                    // Update entity health
                    entity->setHealth(entity->getHealth() - rand() % 6);
                    
                    // Update map with the new position
                    map.new_location(current_X, current_Y, '.', entities);
                    map.new_location(newCoordinates.first, newCoordinates.second, entity->getSymbol(), entities);
                }
            }
            
            if (entity->getType() == "Monster") {
                int current_X = entity->getCoordinates().first;
                int current_Y = entity->getCoordinates().second;
                int strength = entity->getStrength();
                
                int new_X = current_X;
                int new_Y = current_Y;
                
                vector<int> direction_store;
                bool move_valid=false;
                
                // Generate random direction (up, down, left, right)
                while(move_valid== false){
                    int direction = rand() % 4;
                    
                    if (direction == 0) {  // Move up
                        if (new_X >= 0 && new_X < map.width &&  current_Y - strength >= 0 && current_Y - strength  < map.height && map.map[ current_Y - strength][new_X] == '.' ){
                            new_Y = current_Y - strength;
                            move_valid = true;
                            direction_store.push_back(direction);
                        }
                    } else if (direction == 1) {  // Move down
                        if (new_X >= 0 && new_X < map.width &&  current_Y + strength >= 0 && current_Y + strength  < map.height && map.map[ current_Y + strength][new_X] == '.' ){
                            new_Y = current_Y + strength;
                            move_valid = true;
                            direction_store.push_back(direction);
                        }
                    } else if (direction == 2) {  // Move left
                        if (current_X - strength >= 0 && current_X - strength < map.width &&  new_Y >= 0 &&  new_Y < map.height && map.map[ new_Y][current_X - strength] == '.' ) {
                            new_X = current_X - strength;
                            move_valid = true;
                            direction_store.push_back(direction);
                        }
                    } else if (direction == 3) {  // Move right
                        if (current_X + strength >= 0 && current_X + strength < map.width &&  new_Y >= 0 &&  new_Y < map.height && map.map[ new_Y][current_X + strength] == '.' ) {
                            new_X = current_X + strength;
                            move_valid = true;
                            direction_store.push_back(direction);
                        }
                    }
                    else if(count(direction_store.begin(), direction_store.end(), 1)>=1 && count(direction_store.begin(), direction_store.end(), 2)>=1 && count(direction_store.begin(), direction_store.end(), 3)>=1 && count(direction_store.begin(), direction_store.end(), 4)>=1){
                        break;
                    }
                }
                
                if (new_X < 0 || new_X >= map.width || new_Y < 0 || new_Y >= map.height) {
                    // Move to a random adjacent unoccupied cell
                    vector<pair<int, int>> adjacentCells;
                    if (current_X - 1 >= 0 && map.map[current_Y][current_X - 1] == '.') {
                        adjacentCells.emplace_back(current_X - 1, current_Y);  // Move left
                    }
                    if (current_X + 1 < map.width && map.map[current_Y][current_X + 1] == '.') {
                        adjacentCells.emplace_back(current_X + 1, current_Y);  // Move right
                    }
                    if (current_Y - 1 >= 0 && map.map[current_Y - 1][current_X] == '.') {
                        adjacentCells.emplace_back(current_X, current_Y - 1);  // Move up
                    }
                    if (current_Y + 1 < map.height && map.map[current_Y + 1][current_X] == '.') {
                        adjacentCells.emplace_back(current_X, current_Y + 1);  // Move down
                    }
                    
                    if (!adjacentCells.empty()) {
                        // Select a random move from the adjacentCells vector
                        int randomIndex = rand() % adjacentCells.size();
                        pair<int, int> randomCell = adjacentCells[randomIndex];
                        new_X = randomCell.first;
                        new_Y = randomCell.second;
                    }
                }
                
                // Reduce health by strength since the monster is moving
                entity->setHealth(entity->getHealth() - strength);
                
                // Update entity coordinates
                entity->setCoordinates(make_pair(new_X, new_Y));
                
                // Update map with the new position
                map.new_location(current_X, current_Y, '.', entities);
                map.new_location(new_X, new_Y, entity->getSymbol(), entities);
            }
        }
    }
};

void creation(){
        Room room;
        int choice;
        
        
    room.addEntity(room.entities);
        Map map(10, 10, room.entities);


        
        map.display();
        while (true) {
            cout << "\n1. Display Room\n2. Move all the animated entities\n3. Display the entity state by coordinates\n4. Reset the room\n5. Finish" << endl;
            cin >> choice;
            
            if (choice == 1) {
                map.display();
            }
            
            else if (choice == 2) {
 
                room.move(map);
                map.display();
            }
            else if (choice == 3) {
                int x, y;
                cout << "Enter the coordinates (x y): ";
                cin >> x >> y;
                pair<int, int> coordinates(x, y);
                
                string info = room.display_info(coordinates);
                
                cout << info;
            } else if (choice == 4) {
                // Reset the room
                room.reset();
                creation();
            }
            else if (choice == 5) {
                break; // Exit the program
                
            } else
            {
                cout << "INVALID" << endl;
            }
        }
    }

int main() {
    srand(static_cast<unsigned int>(time(NULL)));
    creation();
    return 0;
}

