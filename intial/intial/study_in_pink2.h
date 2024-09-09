/*
 * Ho Chi Minh City University of Technology
 * Faculty of Computer Science and Engineering
 * Initial code for Assignment 1
 * Programming Fundamentals Spring 2023
 * Author: Vu Van Tien
 * Date: 02.02.2023
 */

// The library here is concretely set, students are not allowed to include any other libraries.
#ifndef _H_STUDY_IN_PINK_2_H_
#define _H_STUDY_IN_PINK_2_H_

#include "main.h"

////////////////////////////////////////////////////////////////////////
/// STUDENT'S ANSWER BEGINS HERE
/// Complete the following functions
/// DO NOT modify any parameters in the functions.
////////////////////////////////////////////////////////////////////////

// Forward declaration
class MovingObject;
class Position;
class Configuration;
class Map;

class Criminal;
class RobotS;
class RobotW;
class RobotSW;
class RobotC;

class ArrayMovingObject;
class StudyPinkProgram;

class BaseItem;
class BaseBag;
class SherlockBag;
class WatsonBag;

class TestStudyInPink;

enum ItemType
{
    MAGIC_BOOK,
    ENERGY_DRINK,
    FIRST_AID,
    EXCEMPTION_CARD,
    PASSING_CARD
};
enum ElementType
{
    PATH,
    WALL,
    FAKE_WALL
};
enum RobotType
{
    C = 0,
    S,
    W,
    SW
};

class MapElement
{
protected:
    ElementType type;

public:
    MapElement(ElementType in_type) : type(in_type) {}
    virtual ~MapElement() {}
    virtual ElementType getType() const { return type; }
};

class Path : public MapElement
{
public:
    Path() : MapElement(PATH) {}
};

class Wall : public MapElement
{
public:
    Wall() : MapElement(WALL) {}
};

class FakeWall : public MapElement
{
private:
    int req_exp;

public:
    FakeWall(int in_req_exp) : MapElement(FAKE_WALL)
    {
        this->req_exp = in_req_exp;
    }
    int getReqExp() const { return req_exp; }
};

class Map
{
    friend class TestStudyInPink;

private:
    int nr;
    int nc;

public:
    int num_rows;
    int num_cols;
    MapElement ***map;

    Map(int nr,
        int nc,
        int num_walls,
        Position *array_walls,
        int num_fake_walls,
        Position *array_fake_walls);
    bool isValid(const Position &pos, MovingObject *mv_obj) const;
    ~Map();
};

class Position
{
    friend class TestStudyInPink;

private:
    int r, c;

public:
    Position(const string &str_pos);
    Position(int r = 0, int c = 0) : r(r), c(c){};
    int getRow() const;
    int getCol() const;
    void setRow(int r);
    void setCol(int c);
    string str() const;
    bool operator!=(const Position &other) const;
    bool operator==(const Position &other) const;
    bool isEqual(int ir, int ic) const;
    bool isEqual(const Position &other) const;
    static const Position npos;
    int calculateManhattanDistance(const Position &other) const
    {
        return abs(r - other.r) + abs(c - other.c);
    }
};

class MovingObject
{
    friend class TestStudyInPink;

protected:
    int index;
    Map *map;
    string name;
    int exp;
    int hp;

public:
    Position pos;
    MovingObject(int index, const Position pos, Map *map, const string &name = "");
    string getName() const { return name; }
    int getExp() const { return exp; }
    virtual Position getNextPosition() = 0;
    virtual Position getCurrentPosition() const { return pos; }
    virtual void move() = 0;
    virtual string str() const = 0;
    int getEXP() const { return exp; }
    void setEXP(int exp) { this->exp = exp; }
    int getHP() const { return hp; }
    void setHP(int hp) { this->hp = hp; }
    virtual ~MovingObject(){};
};
class Character : public MovingObject
{
    friend class TestStudyInPink;

public:
    Character(int index, const Position init_pos, Map *map, const string &name);
    int hp;
    int exp;
    virtual string getName() const = 0;
    int getEXP() const;
    void setEXP(int exp);
    int getHP() const;
    void setHP(int hp);
};
class Sherlock : public Character
{
    friend class TestStudyInPink;

private:
    string moving_rule;
    size_t current_move_index;

public:
    Sherlock(int index, const string &moving_rule, const Position &init_pos, Map *map, int init_hp, int init_exp);
    Position getNextPosition();
    virtual Position getCurrentPosition() const;
    void move();
    string str() const;
    string getName() const;
};

class Watson : public Character
{
    friend class TestStudyInPink;

private:
    string moving_rule;
    size_t current_move_index;

public:
    Watson(int index, const string &moving_rule, const Position &init_pos, Map *map, int init_hp, int init_exp);
    Position getNextPosition();
    Position getCurrentPosition() const;
    void move();
    string str() const;
    string getName() const;
};

class Criminal : public Character
{
    friend class TestStudyInPink;

private:
    ArrayMovingObject *arr_mv_objs;
    Sherlock *sherlock;
    Watson *watson;
    Position prevpos;
    int steps;
    ArrayMovingObject *mvb_obj_1;
    int robotsCreated;
    int calculateManhattanDistance(const Position &targetPosition) const;

public:
    Criminal(int index, const Position &init_pos, Map *map, Sherlock *sherlock, Watson *watson);
    Position getNextPosition();
    Position getCurrentPosition() const;
    void move();
    string str() const;
    Position getprevpos();
    void createRobot();
    bool move_1 = false;
    string getName() const;
};

class ArrayMovingObject
{
    friend class TestStudyInPink;

private:
    MovingObject **arr_mv_objs;
    int count;
    int capacity;

public:
    ArrayMovingObject(int capacity);
    bool isFull() const;
    bool add(MovingObject *mv_obj);
    MovingObject *get(int index) const;
    int size() const;
    string str() const;
    ~ArrayMovingObject();
};

class Configuration
{
    friend class StudyPinkProgram;

private:
    int map_num_rows;
    int map_num_cols;
    int max_num_moving_objects;
    int num_walls;
    Position *arr_walls;
    int num_fake_walls;
    Position *arr_fake_walls;
    string sherlock_moving_rule;
    Position sherlock_init_pos;
    int sherlock_init_hp;
    int sherlock_init_exp;
    string watson_moving_rule;
    Position watson_init_pos;
    int watson_init_hp;
    int watson_init_exp;
    Position criminal_init_pos;
    int num_steps;

public:
    Configuration(const string &filepath);
    ~Configuration();
    string str() const;
};

// Robot, BaseItem, BaseBag, ...
class Robot : public MovingObject
{
    friend class TestStudyInPink;

private:
    string type;

protected:
    RobotType robot_type;
    BaseItem *item;
    Criminal *criminal;

public:
    Robot(int index, const Position &init_pos, Map *map, Criminal *criminal);
    virtual Position getNextPosition() = 0;
    virtual void move() = 0;
    string str() const = 0;
    RobotType getRobotType() const;
    BaseItem *getItem() const;
    string getType() const
    {
        return type;
    };
    void createItem(int i, int j);
    virtual ~Robot();
};

class RobotC : public Robot
{
    friend class TestStudyInPink;

public:
    RobotC(int index, const Position &init_pos, Map *map, Criminal *criminal);
    Position getNextPosition();
    Position getCurrentPosition() const;
    void move();
    string str() const;
};

class RobotS : public Robot
{
    friend class TestStudyInPink;

private:
    Sherlock *sherlock;

public:
    RobotS(int index, const Position &init_pos, Map *map, Criminal *criminal, Sherlock *sherlock);
    Position getNextPosition();
    void move();
    string str() const;
    long getDistance() const;
};

class RobotW : public Robot
{
    friend class TestStudyInPink;

private:
    Watson *watson;

public:
    RobotW(int index, const Position &init_pos, Map *map, Criminal *criminal, Watson *watson);
    Position getNextPosition();
    void move();
    string str() const;
    long getDistance() const;
};

class RobotSW : public Robot
{
    friend class TestStudyInPink;

private:
    Sherlock *sherlock;
    Watson *watson;

public:
    RobotSW(int index, const Position &init_pos, Map *map, Criminal *criminal, Sherlock *sherlock, Watson *watson);
    Position getNextPosition();
    void move();
    long getDistance() const;
    string str() const;
};

class BaseItem
{
    friend class TestStudyInPink;

public:
    virtual bool canUse(Character *obj, Robot *robot) = 0;
    virtual void use(Character *obj, Robot *robot) = 0;
    virtual ItemType getType() const = 0;
    virtual string getItemName() const = 0;
};

class MagicBook : public BaseItem
{
    friend class TestStudyInPink;

public:
    bool canUse(Character *obj, Robot *robot);
    void use(Character *obj, Robot *robot);
    ItemType getType() const
    {
        return ItemType::MAGIC_BOOK;
    }
    string getItemName() const  { return "MagicBook"; }
};

class EnergyDrink : public BaseItem
{
    friend class TestStudyInPink;

public:
    bool canUse(Character *obj, Robot *robot);
    void use(Character *obj, Robot *robot);
    ItemType getType() const
    {
        return ItemType::ENERGY_DRINK;
    }
    string getItemName() const  { return "EnergyDrink"; }
};

class FirstAid : public BaseItem
{
    friend class TestStudyInPink;

public:
    bool canUse(Character *obj, Robot *robot);
    void use(Character *obj, Robot *robot);
    ItemType getType() const
    {
        return ItemType::FIRST_AID;
    }
    string getItemName() const  { return "FirstAid"; }
};

class ExcemptionCard : public BaseItem
{
    friend class TestStudyInPink;

public:
    bool canUse(Character *obj, Robot *robot);
    void use(Character *obj, Robot *robot);
    ItemType getType() const
    {
        return ItemType::EXCEMPTION_CARD;
    }
     string getItemName() const  { return "ExcemptionCard"; }
};

class PassingCard : public BaseItem
{
    friend class TestStudyInPink;

private:
    string challenge;

public:
    PassingCard(const string &challenge);
    bool canUse(Character *obj, Robot *robot);
    void use(Character *obj, Robot *robot);
    string getCardType() const;
    ItemType getType() const
    {
        return ItemType::PASSING_CARD;
    }
    string getItemName() const  { return "PassingCard"; }
};

class BaseBag
{
    friend class TestStudyInPink;

protected:
    Character *obj;
    Robot *robot;
    struct Node
    {
        BaseItem *item;
        Node *next;
        Node(BaseItem *i, Node *n = nullptr) : item(i), next(n) {}
    };
    Node *head;
    int item_count;
    int max_items;

public:
    BaseBag(Character *obj);
    virtual bool insert(BaseItem *item);
    virtual BaseItem *get();
    virtual BaseItem *get(ItemType itemType);
    virtual string str() const;
    virtual int getCount() const;
    virtual string listname() const;
    ~BaseBag();
};

class SherlockBag : public BaseBag
{
    friend class TestStudyInPink;

protected:
    struct Node
    {
        BaseItem *item;
        Node *next;
        Node(BaseItem *i, Node *n = nullptr) : item(i), next(n) {}
    };
    Node *head;
    int item_count;
    int max_items;

public:
    SherlockBag(Sherlock *sherlock);
    bool insert(BaseItem *item);
    BaseItem *get();
    BaseItem *get(ItemType itemType);
    void exchangeWithW(WatsonBag &watsonBag);
    string str() const;
    int getCount() const;
    string listname() const;
    ~SherlockBag();
};

class WatsonBag : public BaseBag
{
    friend class TestStudyInPink;

protected:
    struct Node
    {
        BaseItem *item;
        Node *next;
        Node(BaseItem *i, Node *n = nullptr) : item(i), next(n) {}
    };
    Node *head;
    int item_count;
    int max_items;

public:
    WatsonBag(Watson *watson);
    bool insert(BaseItem *item);
    BaseItem *get();
    BaseItem *get(ItemType itemType);
    void exchangewithS(SherlockBag &sherlockBag);
    string str() const;
    int getCount() const;
    string listname() const;
    ~WatsonBag();
};
class StudyPinkProgram
{
    friend class TestStudyInPink;

private:
    // Sample attributes
    Configuration *config;
    Sherlock *sherlock;
    Watson *watson;
    Criminal *criminal;
    Map *map;
    ArrayMovingObject *arr_mv_objs;

public:
    StudyPinkProgram(const string &config_file_path)
    {
        config = new Configuration(config_file_path);
        map = new Map(config->map_num_rows, config->map_num_cols, config->num_walls, config->arr_walls, config->num_fake_walls, config->arr_fake_walls);
        sherlock = new Sherlock(0, config->sherlock_moving_rule, config->sherlock_init_pos, map, config->sherlock_init_hp, config->sherlock_init_exp);
        watson = new Watson(1, config->watson_moving_rule, config->watson_init_pos, map, config->watson_init_hp, config->watson_init_exp);
        criminal = new Criminal(2, config->criminal_init_pos, map, sherlock, watson);
        arr_mv_objs = new ArrayMovingObject(config->max_num_moving_objects);
        arr_mv_objs->add(sherlock);
        arr_mv_objs->add(watson);
        arr_mv_objs->add(criminal);
    };

    bool isStop() const;

    void printResult() const
    {
        if (sherlock->getCurrentPosition().isEqual(criminal->getCurrentPosition()))
        {
            cout << "Sherlock caught the criminal" << endl;
        }
        else if (watson->getCurrentPosition().isEqual(criminal->getCurrentPosition()))
        {
            cout << "Watson caught the criminal" << endl;
        }
        else
        {
            cout << "The criminal escaped" << endl;
        }
    }

    void printStep(int si) const
    {
        cout << "Step: " << setw(4) << setfill('0') << si
             << "--"
             << sherlock->str() << "--|--" << watson->str() << "--|--" << criminal->str() << endl;
    }

    void run(bool verbose);
    ~StudyPinkProgram();
};

////////////////////////////////////////////////
/// END OF STUDENT'S ANSWER
////////////////////////////////////////////////
#endif /* _H_STUDY_IN_PINK_2_H_ */