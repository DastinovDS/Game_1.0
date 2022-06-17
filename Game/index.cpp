#include <iostream>
#include <cstdlib>
#include <string.h>
#include <cctype>
#include <time.h>
#include <vector>
#include <iterator>
#include <vector>

using namespace std;

class Monster;

class Unit{
protected:
    string name;
    int hp;
    int atk;
    int def;
public:
    Unit(int h, int a, int d, string n) : hp(h), atk(a), def(d), name(n){}
    Unit(){}
    string getName(){return name;}
    int getHp(){return hp;}
    int getAtk(){return atk;}
    int getDef(){return def;}
    void output_info(){
        cout << "Name: " << getName() << endl;
        cout << "HP: " << getHp() << endl;
        cout << "ATK: " << getAtk() << endl;
        cout << "DEF: " << getDef() << endl;
    }
};

int find_hero_pos(int* array, int size){
    for(int i = 0; i < size*size; i++){
        if(array[i] == 5) return i;
    }
    return 0;
}

class Hero : public Unit{
private:
    int max_hp = 30;
    int level = 1;
    int gold = 0;
    int exp_for_new_lvl = 5;
    int exp = 0;
public:
    Hero(string n) : Unit(30, 3, 1, n){}
    friend void fight(Hero &h, vector<Monster> &list, Monster &m);
    void new_lvl(){
        if(exp >= exp_for_new_lvl){
            if(exp > exp_for_new_lvl){
                exp-= exp_for_new_lvl;
            }
            else exp = 0;
            level++;
            cout << "You have reached LVL " << level << "!" << endl;
            max_hp += 5;
            hp = max_hp;
            atk++;
            exp_for_new_lvl *= 2;
        }
    }
};

class Monster : public Unit{
private:
    int ID;
    int gain_gold;
    int gain_exp;
public:
    Monster(int type){
        if(type == 1){
            name = "Ork";
            hp = 15;
            atk = 3;
            def = 1;
            gain_exp = 3;
            gain_gold = 2;
        }
        else if(type == 2){
            name = "Skeleton";
            hp = 5;
            atk = 5;
            def = 0;
            gain_exp = 2;
            gain_gold = 1;
        }
        else if(type == 3){
            name = "Spider";
            hp = 10;
            atk = 2;
            def = 2;
            gain_exp = 5;
            gain_gold = 3;
        }
        else{
            name = "Dragon";
            hp = 20;
            atk = 5;
            def = 3;
            gain_exp = 10;
            gain_gold = 5;
        }
    }
    void setID(int index){
        ID = index;
    }
    int getID(){return ID;}
    friend void fight(Hero &h, vector<Monster> &list, Monster &m);
};

bool bool_find_monster(int index, vector<Monster> list){
    vector<Monster> :: iterator i;
    for(i = list.begin(); i != list.end(); i++){
        if(i->getID() == index) return true;
    }
    return false;
}

void index_arr(int* array, int size){
    for(int x = 0; x < size; x++){
        cout << endl;
        for(int y = 0; y < size; y++){
            int i = x + y * size;
            array[i] = i;
            cout << array[i] << " ";
        }
    }
    cout << endl << endl;
}

void gen_map(int* array, int size){
    int i;
    for(i = 0; i < size*size; i++){
        array[i] = 0;
    }
    i = rand() % (size*size);
    array[i] = 5;
}

void spawn_monsters(vector<Monster> &monster_list, int* array, int map_size){
    for(int i = 0; i < map_size*map_size; i++){
        if(array[i] != 5){
            int gen = rand() % 100 + 1;
            if(gen == 1){
                Monster monster(4);
                monster.setID(i);
                monster_list.push_back(monster);
            }
            else if(gen > 1 && gen <= 3){
                Monster monster(1);
                monster.setID(i);
                monster_list.push_back(monster);
            }
            else if(gen > 3 && gen <= 8){
                Monster monster(2);
                monster.setID(i);
                monster_list.push_back(monster);
            }
            else if(gen > 8 && gen <= 10){
                Monster monster(3);
                monster.setID(i);
                monster_list.push_back(monster);
            }
        }
    }
}

void show_monsters(vector<Monster> list){
    vector<Monster> :: iterator it;
    for(it = list.begin(); it != list.end(); it++){
        cout << "Monster ID: " << it->getID() << endl;
        it->output_info();
        cout << "\n\n";
    }
}

void show_map_without_monsters(int* array, int size){
    for(int x = 0; x < size; x++){
        cout << endl;
        for(int y = 0; y < size; y++){
            int i = x + y * size;
            if(array[i] == 0) cout << "*" << " ";
            else cout << "@" << " ";
        }
    }
    cout << "\n\n";
}

void show_map_with_monsters(int* array, int size, vector<Monster> list){
    vector<Monster> :: iterator i;
    for(int x = 0; x < size; x++){
        cout << endl;
        for(int y = 0; y < size; y++){
            int ind = x + y * size;
            for(i = list.begin(); i != list.end(); i++){
                if(i->getID() == ind){
                    if(i->getName() == "Ork") cout << "$" << " ";
                    else if(i->getName() == "Skeleton") cout << "%" << " ";
                    else if(i->getName() == "Spider") cout << "&" << " ";
                    else if(i->getName() == "Dragon") cout << "!" << " ";
                    break;
                }
            }
            if(array[ind] == 5) cout << "@" << " ";
            else if(array[ind] == 0 && bool_find_monster(ind, list) == false) cout << "*" << " ";
        }
    }
    cout << "\n\n";
}

void erase_dead_monster(vector<Monster> &list){
    vector<Monster> :: iterator i;
    for(i = list.begin(); i != list.end(); i++){
        if(i->getHp() <= 0){
            list.erase(i);
            break;
        }
    }
}

bool hero_dead(Hero h){
    if(h.getHp() <= 0) return true;
    else return false;
}

void fight(Hero &h, vector<Monster> &list, Monster &m){
    while(1){
        if(h.atk <= m.def && m.atk <= h.def){
            cout << "No one can win" << endl;
            break;
        }
        else{
            int dmg;
            if(h.atk <= m.def) cout << h.name << " do no damage!" << endl;
            else{
                dmg = h.atk - m.def;
                cout << h.name << " do " << dmg << " damage to " << m.name << endl;
                m.hp -= dmg;
                if(m.hp <= 0){
                    cout << h.name << " kill " << m.name << "!" << endl;
                    h.exp += m.gain_exp;
                    h.gold += m.gain_gold;
                    h.new_lvl();
                    erase_dead_monster(list);
                    break;
                }
                else cout << m.name << "'s HP: " << m.hp << endl;
            }
            cout << "\n\n";
            if(m.atk <= h.def) cout << m.name << " do no damage!" << endl;
            else{
                dmg = m.atk - h.def;
                cout << m.name << " do " << dmg << " damage to " << h.name << endl;
                h.hp -= dmg;
                if(h.hp <= 0){
                    cout << "You are dead" << endl;
                    break;
                }
                else cout << h.name << "'s HP: " << h.hp << endl;
                cout << "\n\n";
            }
        }
    }
}

void movement(int* array, int size, vector<Monster> &list, Hero &h){
    int choice;
    vector<Monster> :: iterator i;
    while(1){
        cout << "Choose where you want to go:\n1)Up\n2)Down\n3)Left\n4)Right\n";
        cin >> choice;
        if(choice > 0 || choice < 5) break;
    }
    int pos = find_hero_pos(array, size);
    switch(choice){
        case 1:
            if(bool_find_monster(pos - 1, list) == true && pos % size != 0){
                for(i = list.begin(); i != list.end(); i++){
                    if(i->getID() == pos - 1){
                        fight(h, list, *i);
                        break;
                    }
                }
            }
            else{
                if(pos % size != 0){
                    array[pos - 1] = 5;
                    array[pos] = 0;
                }
            }
            break;
        case 2:
            if(bool_find_monster(pos + 1, list) == true && (pos + 1) % size != 0){
                for(i = list.begin(); i != list.end(); i++){
                    if(i->getID() == pos + 1){
                        fight(h, list, *i);
                        break;
                    }
                }
            }
            else{
                if((pos + 1) % size != 0){
                    array[pos + 1] = 5;
                    array[pos] = 0;
                }
            }
            break;
        case 3:
            if(bool_find_monster(pos - size, list) == true && (pos - size) > 0){
                for(i = list.begin(); i != list.end(); i++){
                    if(i->getID() == pos - size){
                        fight(h, list, *i);
                        break;
                    }
                }
            }
            else{
                if(pos - size > 0){
                    array[pos - size] = 5;
                    array[pos] = 0;
                }
            }
            break;
        case 4:
            if(bool_find_monster(pos + size, list) == true && (pos + size) < size*size){
                for(i = list.begin(); i != list.end(); i++){
                    if(i->getID() == (pos + size)){
                        fight(h, list, *i);
                        break;
                    }
                }
            }
            else{
                if(pos + size < size*size){
                    array[pos + size] = 5;
                    array[pos] = 0;
                }
            }
            break;
    }
}

int main(int argc, const char * argv[]) {
    srand(time(0));
    vector<Monster> monster_list;
    int map_size = 10;
    int* map = new int[map_size*map_size];
    index_arr(map, map_size);
    gen_map(map, map_size);
    show_map_without_monsters(map, map_size);
    spawn_monsters(monster_list, map, map_size);
    show_monsters(monster_list);
    show_map_with_monsters(map, map_size, monster_list);
    Hero hero("Johan");
    while(hero_dead(hero) == false){
        movement(map, map_size, monster_list, hero);
        show_map_with_monsters(map, map_size, monster_list);
    }
}