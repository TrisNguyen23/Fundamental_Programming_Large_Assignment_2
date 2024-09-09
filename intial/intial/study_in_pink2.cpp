#include "study_in_pink2.h"

////////////////////////////////////////////////////////////////////////
/// STUDENT'S ANSWER BEGINS HERE
/// Complete the following functions
/// DO NOT modify any parameters in the functions.
////////////////////////////////////////////////////////////////////////

Map::Map(int nr, int nc, int num_walls, Position *array_walls, int num_fake_walls, Position *array_fake_walls)
    : num_rows(nr), num_cols(nc)
{
    map = new MapElement **[num_rows];
    for (int i = 0; i < num_rows; ++i)
    {
        map[i] = new MapElement *[num_cols];
        for (int j = 0; j < num_cols; ++j)
        {
            map[i][j] = new Path();
        }
    }

    for (int i = 0; i < num_walls; ++i)
    {
        int row = array_walls[i].getRow();
        int col = array_walls[i].getCol();
        map[row][col] = new Wall();
    }

    for (int i = 0; i < num_fake_walls; ++i)
    {
        int row = array_fake_walls[i].getRow();
        int col = array_fake_walls[i].getCol();
        map[row][col] = new FakeWall((row * 257 + col * 139 + 89) % 900 + 1);
    }
}

Map::~Map()
{

    for (int i = 0; i < num_rows; ++i)
    {
        for (int j = 0; j < num_cols; ++j)
        {
            delete map[i][j];
        }
        delete[] map[i];
    }
    delete[] map;
}

bool Map::isValid(const Position &pos, MovingObject *mv_obj) const
{
    int row = pos.getRow();
    int col = pos.getCol();

    if (row < 0 || row >= num_rows || col < 0 || col >= num_cols)
    {
        return false;
    }

    if (map[row][col]->getType() == WALL)
    {
        return false;
    }

    if (mv_obj != nullptr)
    {
        if (mv_obj->getName() == "Sherlock" && map[row][col]->getType() == FAKE_WALL)
        {
            return true;
        }
        else if (mv_obj->getName() == "Watson" && map[row][col]->getType() == FAKE_WALL)
        {
            FakeWall *fakeWall = dynamic_cast<FakeWall *>(map[row][col]);
            if (fakeWall != nullptr)
            {
                int required_exp = fakeWall->getReqExp();
                return mv_obj->getExp() >= required_exp;
            }
        }
    }

    return true;
}

const Position Position::npos = Position(-1, -1);

bool Position::operator!=(const Position &other) const
{
    return (r != other.r) || (c != other.c);
}

bool Position::operator==(const Position &other) const
{
    return (r == other.r) || (c == other.c);
}

Position::Position(const string &str_pos)
{
    size_t bat_dau = str_pos.find('(');
    size_t dau_phay = str_pos.find(',');
    size_t het = str_pos.find(')');

    if (bat_dau != string::npos && dau_phay != string::npos && het != string::npos)
    {
        r = stoi(str_pos.substr(bat_dau + 1, dau_phay - bat_dau - 1));
        c = stoi(str_pos.substr(dau_phay + 1, het - dau_phay - 1));
    }
    else
    {
        r = 0;
        c = 0;
    }
}

int Position::getRow() const { return r; }

int Position::getCol() const { return c; }

void Position::setRow(int r) { this->r = r; }

void Position::setCol(int c) { this->c = c; }

string Position::str() const { return "(" + to_string(r) + "," + to_string(c) + ")"; }

bool Position::isEqual(int in_r, int in_c) const
{
    return (r == in_r && c == in_c);
}
bool Position::isEqual(const Position &other) const
{
    return r == other.r && c == other.c;
}

MovingObject::MovingObject(int index, const Position pos, Map *map, const string &name)
    : index(index), pos(pos), map(map), name(name), exp(exp) {}
Character::Character(int index, const Position init_pos, Map *map, const string &name) : MovingObject(index, init_pos, map, name)
{
    hp = 0;
    exp = 0;
}
Sherlock::Sherlock(int index, const string &moving_rule, const Position &init_pos, Map *map, int init_hp, int init_exp)
    : Character(index, init_pos, map, "Sherlock")
{
    current_move_index = 0;
    hp = init_hp;
    exp = init_exp;
    this->moving_rule = moving_rule;
    if (init_hp < 0)
        init_hp = 0;
    else if (init_hp > 500)
        init_hp = 500;

    if (init_exp < 0)
        init_exp = 0;
    else if (init_exp > 900)
        init_exp = 900;
}
Position Sherlock::getNextPosition()
{
    if (!map || current_move_index < 0 || current_move_index >= moving_rule.size())
    {
        return Position::npos;
    }

    char direction = moving_rule[current_move_index];
    int row = pos.getRow();
    int col = pos.getCol();

    switch (direction)
    {
    case 'L':
        col--;
        break;
    case 'R':
        col++;
        break;
    case 'U':
        row--;
        break;
    case 'D':
        row++;
        break;
    default:
        return Position::npos;
    }

    Position next_position(row, col);
    if (!map->isValid(next_position, this))
    {
        return Position::npos;
    }
    current_move_index++;
    return next_position;
}

Position Sherlock::getCurrentPosition() const { return pos; }

void Sherlock::move()
{
    Position nextPos = getNextPosition();
    if (nextPos != Position::npos)
    {
        pos = nextPos;
    }
}

string Sherlock::str() const
{
    return "Sherlock[index=" + to_string(index) + ";pos=" + pos.str() + ";moving_rule=" + moving_rule + "]";
}

Watson::Watson(int index, const string &moving_rule, const Position &init_pos, Map *map, int init_hp, int init_exp)
    : Character(index, init_pos, map, "Watson")
{
    this->moving_rule = moving_rule;
    this->hp = init_hp;
    this->exp = init_exp;
    if (init_hp < 0)
        init_hp = 0;
    else if (init_hp > 500)
        init_hp = 500;
    else
        init_hp = init_hp;

    if (init_exp < 0)
        init_exp = 0;
    else if (init_exp > 900)
        init_exp = 900;
    else
        init_exp = init_exp;
}

Position Watson::getNextPosition()
{
    if (!map || current_move_index < 0 || current_move_index >= moving_rule.size())
    {
        return Position::npos;
    }

    char direction = moving_rule[current_move_index];
    int row = pos.getRow();
    int col = pos.getCol();

    switch (direction)
    {
    case 'L':
        col--;
        break;
    case 'R':
        col++;
        break;
    case 'U':
        row--;
        break;
    case 'D':
        row++;
        break;
    default:
        return Position::npos;
        break;
    }

    Position next_position(row, col);
    if (!map->isValid(next_position, this))
    {
        return Position::npos;
    }
    current_move_index++;
    return next_position;
}

Position Watson::getCurrentPosition() const
{
    return pos;
}

void Watson::move()
{
    Position nextPos = getNextPosition();
    if (nextPos != Position::npos)
    {
        pos = nextPos;
    }
}
string Watson::str() const
{
    return "Watson[index=" + to_string(index) + ";pos=" + pos.str() + ";moving_rule=" + moving_rule + "]";
}
Criminal::Criminal(int index, const Position &init_pos, Map *map, Sherlock *sherlock, Watson *watson)
    : Character(index, init_pos, map, "Criminal"), sherlock(sherlock), watson(watson)
{
    prevpos = pos;
    steps = 0;
    robotsCreated = 0;
}

Position Criminal::getNextPosition()
{
    Position currentPosition = getCurrentPosition();
    int maxDistance = -1;
    Position nextPosition = Position();

    char directions[] = {'U', 'L', 'D', 'R'};
    int dx[] = {0, -1, 0, 1};
    int dy[] = {1, 0, -1, 0};

    for (int i = 0; i < 4; ++i)
    {
        int newRow = currentPosition.getRow() + dy[i];
        int newCol = currentPosition.getCol() + dx[i];
        Position nextPos(newRow, newCol);

        if (map->isValid(nextPos, this))
        {
            int totalDistance = calculateManhattanDistance(nextPos);
            if (totalDistance > maxDistance)
            {
                maxDistance = totalDistance;
                nextPosition = nextPos;
            }
        }
    }

    return nextPosition;
}

Position Criminal::getCurrentPosition() const { return pos; }

void Criminal::createRobot()
{
    if (mvb_obj_1 != nullptr && !mvb_obj_1->isFull())
    {
        MovingObject *robot = nullptr;
        long int disttoS = abs(sherlock->getCurrentPosition().getRow() - prevpos.getRow()) +
                           abs(sherlock->getCurrentPosition().getCol() - prevpos.getCol());
        long int disttoW = abs(watson->getCurrentPosition().getRow() - prevpos.getRow()) +
                           abs(watson->getCurrentPosition().getCol() - prevpos.getCol());

        if (robotsCreated == 0)
        {
            robot = new RobotC(index, getprevpos(), map, this);
        }
        else if (disttoS < disttoW)
        {
            robot = new RobotS(index, getprevpos(), map, this, sherlock);
        }
        else if (disttoW < disttoS)
        {
            robot = new RobotW(index, getprevpos(), map, this, watson);
        }
        else
        {
            robot = new RobotSW(index, getprevpos(), map, this, sherlock, watson);
        }
        if (robot != nullptr)
        {
            mvb_obj_1->add(robot);
            robotsCreated++;
        }
    }
}
void Criminal::move()
{
    Position nextPos = getNextPosition();

    if (map->isValid(nextPos, this) && nextPos != Position::npos)
    {
        prevpos = pos;
        pos = nextPos;
        ++steps;
        move_1 = true;
        if (steps % 3 == 0)
        {
            createRobot();
        }
        if (arr_mv_objs != nullptr)
        {
            for (int i = 0; i < arr_mv_objs->size(); ++i)
            {
                if (arr_mv_objs->get(i) != nullptr)
                {
                    arr_mv_objs->get(i)->move();
                }
            }
        }
    }
}

int Criminal::calculateManhattanDistance(const Position &targetPosition) const
{
    if (sherlock == nullptr || watson == nullptr)
    {
        return -1;
    }

    int sherlockDistance = abs(sherlock->getCurrentPosition().getRow() - targetPosition.getRow()) +
                           abs(sherlock->getCurrentPosition().getCol() - targetPosition.getCol());

    int watsonDistance = abs(watson->getCurrentPosition().getRow() - targetPosition.getRow()) +
                         abs(watson->getCurrentPosition().getCol() - targetPosition.getCol());

    return sherlockDistance + watsonDistance;
}

Position Criminal::getprevpos()
{
    return prevpos;
}

string Criminal::str() const
{
    return "Criminal[index=" + to_string(index) + ";pos=" + pos.str() + "]";
}
ArrayMovingObject::ArrayMovingObject(int capacity) : capacity(capacity), count(0)
{
    arr_mv_objs = new MovingObject *[capacity];
}

bool ArrayMovingObject::isFull() const { return count == capacity; }

bool ArrayMovingObject::add(MovingObject *mv_obj)
{
    if (!isFull())
    {
        arr_mv_objs[count++] = mv_obj;
        return true;
    }
    return false;
}

MovingObject *ArrayMovingObject::get(int index) const
{
    if (index >= 0 && index < count)
    {
        return arr_mv_objs[index];
    }
    else
    {
        return nullptr;
    }
}

int ArrayMovingObject::size() const { return count; }

string ArrayMovingObject::str() const
{
    stringstream array_moving_object;
    array_moving_object << "ArrayMovingObject[count=" << count << ";capacity=" << capacity;
    bool have_element = false;
    if (count > 0)
    {
        for (int i = 0; i < count; ++i)
        {
            array_moving_object << (have_element ? ";" : ";");
            array_moving_object << arr_mv_objs[i]->str();
            have_element = true;
        }
        array_moving_object << "]";
    }
    else
    {
        array_moving_object << "]";
    }
    return array_moving_object.str();
}

ArrayMovingObject::~ArrayMovingObject()
{
    delete[] arr_mv_objs;
}
Configuration::Configuration(const string &filepath)
    : map_num_rows(0), map_num_cols(0), max_num_moving_objects(0),
      num_walls(0), arr_walls(nullptr), num_fake_walls(0),
      arr_fake_walls(nullptr), sherlock_init_pos(Position(0, 0)),
      sherlock_init_hp(0), sherlock_init_exp(0), watson_init_pos(Position(0, 0)),
      watson_init_hp(0), watson_init_exp(0), criminal_init_pos(Position(0, 0)),
      num_steps(0)
{
    ifstream file(filepath);
    if (file.is_open())
    {
        string line;
        while (getline(file, line))
        {
            stringstream configura(line);
            string key, value;
            getline(configura, key, '=');
            getline(configura, value);

            key.erase(0, key.find_first_not_of(" \t\r\n"));
            key.erase(key.find_last_not_of(" \t\r\n") + 1);
            value.erase(0, value.find_first_not_of(" \t\r\n"));
            value.erase(value.find_last_not_of(" \t\r\n") + 1);

            if (key == "MAP_NUM_ROWS")
            {
                map_num_rows = stoi(value);
            }
            else if (key == "MAP_NUM_COLS")
            {
                map_num_cols = stoi(value);
            }
            else if (key == "MAX_NUM_MOVING_OBJECTS")
            {
                max_num_moving_objects = stoi(value);
            }

            else if (key == "ARRAY_WALLS")
            {
                int count = 0;
                for (int i = 0; i < value.length(); i++)
                {
                    if (value[i] == ',')
                        count++;
                }
                num_walls = count;

                arr_walls = new Position[num_walls];
                for (int i = 0; i < num_walls; i++)
                {
                    size_t position = value.find(';');
                    size_t mo_ngoac = value.find('(');
                    size_t dong_ngoac = value.find(')');
                    size_t dau_phay = value.find(',');
                    string row = value.substr(mo_ngoac + 1, dau_phay - mo_ngoac);
                    int r = stoi(row);
                    string col = value.substr(dau_phay + 1, dong_ngoac - dau_phay);
                    int c = stoi(col);
                    arr_walls[i] = Position(r, c);
                    value = value.substr(position + 1);
                    position = value.find(';');
                }
            }
            else if (key == "ARRAY_FAKE_WALLS")
            {
                int count = 0;
                for (char character : value)
                {
                    if (character == ',')
                    {
                        ++count;
                    }
                }
                num_fake_walls = count;

                arr_fake_walls = new Position[num_fake_walls];

                for (int i = 0; i < num_fake_walls; i++)
                {
                    size_t position = value.find(';');
                    size_t mo_ngoac = value.find('(');
                    size_t dong_ngoac = value.find(')');
                    size_t dau_phay = value.find(',');
                    string row = value.substr(mo_ngoac + 1, dau_phay - mo_ngoac);
                    int r = stoi(row);
                    string col = value.substr(dau_phay + 1, dong_ngoac - dau_phay);
                    int c = stoi(col);
                    arr_fake_walls[i] = Position(r, c);
                    value = value.substr(position + 1);
                }
            }
            else if (key == "SHERLOCK_MOVING_RULE")
            {
                sherlock_moving_rule = value;
            }
            else if (key == "SHERLOCK_INIT_POS")
            {
                size_t dau_phay = value.find(',');
                if (dau_phay != string::npos && dau_phay > 0 && dau_phay < value.length() - 1)
                {
                    int row = stoi(value.substr(1, dau_phay - 1));
                    int col = stoi(value.substr(dau_phay + 1, value.length() - dau_phay - 2));
                    sherlock_init_pos = Position(row, col);
                }
            }
            else if (key == "SHERLOCK_INIT_HP")
            {
                sherlock_init_hp = stoi(value);
            }
            else if (key == "SHERLOCK_INIT_EXP")
            {
                sherlock_init_exp = stoi(value);
            }
            else if (key == "WATSON_MOVING_RULE")
            {
                watson_moving_rule = value;
            }
            else if (key == "WATSON_INIT_POS")
            {
                size_t dau_phay = value.find(',');
                if (dau_phay != string::npos && dau_phay > 0 && dau_phay < value.length() - 1)
                {
                    int row = stoi(value.substr(1, dau_phay - 1));
                    int col = stoi(value.substr(dau_phay + 1, value.length() - dau_phay - 2));
                    watson_init_pos = Position(row, col);
                }
            }
            else if (key == "WATSON_INIT_HP")
            {
                watson_init_hp = stoi(value);
            }
            else if (key == "WATSON_INIT_EXP")
            {
                watson_init_exp = stoi(value);
            }
            else if (key == "CRIMINAL_INIT_POS")
            {
                size_t dau_phay = value.find(',');
                if (dau_phay != string::npos && dau_phay > 0 && dau_phay < value.length() - 1)
                {
                    int row = stoi(value.substr(1, dau_phay - 1));
                    int col = stoi(value.substr(dau_phay + 1, value.length() - dau_phay - 2));
                    criminal_init_pos = Position(row, col);
                }
            }
            else if (key == "NUM_STEPS")
            {
                num_steps = stoi(value);
            }
        }
        file.close();
    }
}

Configuration::~Configuration()
{
    delete[] arr_walls;
    delete[] arr_fake_walls;
}

string Configuration::str() const
{
    string result = "Configuration[\n";
    result += "MAP_NUM_ROWS=" + to_string(map_num_rows) + "\n";
    result += "MAP_NUM_COLS=" + to_string(map_num_cols) + "\n";
    result += "MAX_NUM_MOVING_OBJECTS=" + to_string(max_num_moving_objects) + "\n";
    result += "NUM_WALLS=" + to_string(num_walls) + "\n";
    result += "ARRAY_WALLS=[";
    for (int i = 0; i < num_walls; ++i)
    {
        result += "(" + to_string(arr_walls[i].getRow()) + "," + to_string(arr_walls[i].getCol()) + ")";
        if (i != num_walls - 1)
        {
            result += ";";
        }
    }
    result += "]\n";

    result += "NUM_FAKE_WALLS=" + to_string(num_fake_walls) + "\n";
    result += "ARRAY_FAKE_WALLS=[";
    for (int i = 0; i < num_fake_walls; ++i)
    {
        result += "(" + to_string(arr_fake_walls[i].getRow()) + "," + to_string(arr_fake_walls[i].getCol()) + ")";
        if (i != num_fake_walls - 1)
        {
            result += ";";
        }
    }
    result += "]\n";

    result += "SHERLOCK_MOVING_RULE=" + sherlock_moving_rule + "\n";
    result += "SHERLOCK_INIT_POS=(" + to_string(sherlock_init_pos.getRow()) + "," + to_string(sherlock_init_pos.getCol()) + ")\n";
    result += "SHERLOCK_INIT_HP=" + to_string(sherlock_init_hp) + "\n";
    result += "SHERLOCK_INIT_EXP=" + to_string(sherlock_init_exp) + "\n";

    result += "WATSON_MOVING_RULE=" + watson_moving_rule + "\n";
    result += "WATSON_INIT_POS=(" + to_string(watson_init_pos.getRow()) + "," + to_string(watson_init_pos.getCol()) + ")\n";
    result += "WATSON_INIT_HP=" + to_string(watson_init_hp) + "\n";
    result += "WATSON_INIT_EXP=" + to_string(watson_init_exp) + "\n";

    result += "CRIMINAL_INIT_POS=(" + to_string(criminal_init_pos.getRow()) + "," + to_string(criminal_init_pos.getCol()) + ")\n";
    result += "NUM_STEPS=" + to_string(num_steps) + "\n";

    result += "]\n";
    return result;
}

Robot::Robot(int index, const Position &init_pos, Map *map, Criminal *criminal)
    : MovingObject(index, init_pos, map, "Robot"), robot_type(robot_type), criminal(criminal)
{
    createItem(init_pos.getRow(), init_pos.getCol());
}

Robot::~Robot() {}

RobotType Robot::getRobotType() const
{
    return robot_type;
}
BaseItem *Robot::getItem() const
{
    return item;
}

RobotC::RobotC(int index, const Position &init_pos, Map *map, Criminal *criminal)
    : Robot(index, init_pos, map, criminal)
{
    this->robot_type = RobotType::C;
}

Position RobotC::getNextPosition()
{
    if (criminal->move_1)
    {
        return criminal->getprevpos();
    }
    else
        return Position::npos;
}

void RobotC::move()
{
    Position next_pos = criminal->getprevpos();
    if (map->isValid(next_pos, this) && pos != next_pos)
    {
        pos = next_pos;
    }
}
Position RobotC::getCurrentPosition() const
{
    return this->pos;
}

string RobotC::str() const
{
    return "Robot[pos=" + pos.str() + ";type=C;dist=]";
}

RobotS::RobotS(int index, const Position &init_pos, Map *map, Criminal *criminal, Sherlock *sherlock)
    : Robot(index, init_pos, map, criminal), sherlock(sherlock)
{
    this->robot_type = RobotType::S;
}

Position RobotS::getNextPosition()
{
   Position sherlockPos = sherlock->getCurrentPosition();
    Position cands[4] = {
        Position(pos.getRow() - 1, pos.getCol()),
        Position(pos.getRow(), pos.getCol() + 1),
        Position(pos.getRow() + 1, pos.getCol()),
        Position(pos.getRow(), pos.getCol() - 1)
    };
    Position bestPos = Position::npos;
    int minDist = INT_MAX;
    for (int i = 0; i < 4; ++i)
    {
        const Position &p = cands[i];
        if (map->isValid(p, this))
        {
            int dist = p.calculateManhattanDistance(sherlockPos);
            if (dist < minDist)
            {
                minDist = dist;
                bestPos = p;
            }
        }
    }
    return bestPos;
}

long RobotS::getDistance() const
{
    return pos.calculateManhattanDistance(sherlock->getCurrentPosition());
}

void RobotS::move()
{
    Position next_pos = getNextPosition();
    if (map->isValid(pos, sherlock) && pos != sherlock->getCurrentPosition())
    {
        pos = next_pos;
    }
}

string RobotS::str() const
{
    return "Robot[pos=" + pos.str() + ";type=S;dist=" + to_string(pos.calculateManhattanDistance(criminal->getCurrentPosition())) + "]";
}

RobotW::RobotW(int index, const Position &init_pos, Map *map, Criminal *criminal, Watson *watson)
    : Robot(index, init_pos, map, criminal), watson(watson)
{
    this->robot_type = RobotType::W;
}

Position RobotW::getNextPosition()
{
    Position watsonPos = watson->getCurrentPosition();
    Position cands[4] = {
        Position(pos.getRow() - 1, pos.getCol()),
        Position(pos.getRow(), pos.getCol() + 1),
        Position(pos.getRow() + 1, pos.getCol()),
        Position(pos.getRow(), pos.getCol() - 1)
    };
    Position bestPos = Position::npos;
    int minDist = INT_MAX;
    for (int i = 0; i < 4; ++i)
    {
        const Position &p = cands[i];
        if (map->isValid(p, this))
        {
            int dist = p.calculateManhattanDistance(watsonPos);
            if (dist < minDist)
            {
                minDist = dist;
                bestPos = p;
            }
        }
    }
    return bestPos;
}

void RobotW::move()
{
    Position next_pos = getNextPosition();
    if (map->isValid(next_pos, watson) && next_pos != watson->getCurrentPosition())
    {
        pos = next_pos;
    }
}
long RobotW::getDistance() const
{
    return pos.calculateManhattanDistance(watson->getCurrentPosition());
}

string RobotW::str() const
{
    return "Robot[pos=" + pos.str() + ";type=W;dist=" + to_string(pos.calculateManhattanDistance(criminal->getCurrentPosition())) + "]";
}

RobotSW::RobotSW(int index, const Position &init_pos, Map *map, Criminal *criminal, Sherlock *sherlock, Watson *watson)
    : Robot(index, init_pos, map, criminal), sherlock(sherlock), watson(watson)
{
    this->robot_type = RobotType::SW;
}

Position RobotSW::getNextPosition()
{
    Position criminalPos = criminal->getCurrentPosition();
    Position cands[8] = {
        Position(criminalPos.getRow() - 2, criminalPos.getCol()),
        Position(criminalPos.getRow() - 1, criminalPos.getCol() + 1),
        Position(criminalPos.getRow(), criminalPos.getCol() + 2),
        Position(criminalPos.getRow() + 1, criminalPos.getCol() + 1),
        Position(criminalPos.getRow() + 2, criminalPos.getCol()),
        Position(criminalPos.getRow() + 1, criminalPos.getCol() - 1),
        Position(criminalPos.getRow(), criminalPos.getCol() - 2),
        Position(criminalPos.getRow() - 1, criminalPos.getCol() - 1),
    };

    Position bestPos = Position::npos;
    int minDist = INT_MAX;
    for (int i = 0; i < 8; ++i)
    {
        const Position &p = cands[i];
        if (map->isValid(p, this))
        {
            int dist = min(p.calculateManhattanDistance(sherlock->getCurrentPosition()),
                           p.calculateManhattanDistance(watson->getCurrentPosition()));
            if (dist < minDist)
            {
                minDist = dist;
                bestPos = p;
            }
        }
    }
    return bestPos;
}
string RobotSW::str() const
{
    return "Robot[pos=" + pos.str() + ";type=SW;dist=" + to_string(pos.calculateManhattanDistance(criminal->getCurrentPosition())) + "]";
}
long RobotSW::getDistance() const
{
    long distToSherlock = pos.calculateManhattanDistance(sherlock->getCurrentPosition());
    long distToWatson = pos.calculateManhattanDistance(watson->getCurrentPosition());
    return min(distToSherlock, distToWatson);
}
void RobotSW::move()
{
    Position next_pos = getNextPosition();
    if (map->isValid(next_pos, this) && pos != next_pos)
    {
        pos = next_pos;
    }
}

int Character::getEXP() const
{
    return exp;
}
void Character::setEXP(int exp)
{
    this->exp = exp;
}
int Character::getHP() const
{
    return hp;
}
void Character::setHP(int hp)
{
    this->hp = hp;
}
bool MagicBook::canUse(Character *obj, Robot *robot)
{
    if (obj)
    {
        return (dynamic_cast<Sherlock *>(obj) || dynamic_cast<Watson *>(obj)) && obj->getEXP() <= 350;
    }
    return false;
}

void MagicBook::use(Character *obj, Robot *robot)
{
    if (canUse(obj, robot))
    {
        int newExp = obj->getEXP() * 1.25;
        obj->setEXP(newExp);
    }
}

bool EnergyDrink::canUse(Character *obj, Robot *robot)
{
    if (obj)
    {
        return obj->getHP() <= 100;
    }
    return false;
}

void EnergyDrink::use(Character *obj, Robot *robot)
{
    if (canUse(obj, robot))
    {
        int newHp = obj->getHP() * 1.2;
        obj->setHP(newHp);
    }
}

bool FirstAid::canUse(Character *obj, Robot *robot)
{
    if (obj)
    {
        return obj->getHP() <= 100 || obj->getEXP() <= 350;
    }
    return false;
}

void FirstAid::use(Character *obj, Robot *robot)
{
    if (canUse(obj, robot))
    {
        int newHp = obj->getHP() * 1.5;
        obj->setHP(newHp);
    }
}

bool ExcemptionCard::canUse(Character *obj, Robot *robot)
{
    if (obj && dynamic_cast<Sherlock *>(obj))
    {
        return obj->getHP() % 2 != 0;
    }
    return false;
}

void ExcemptionCard::use(Character *obj, Robot *robot)
{
    if (canUse(obj, robot))
    {
        obj->setHP(obj->getHP());
        obj->setEXP(obj->getEXP());
    }
}

PassingCard::PassingCard(const string &challenge) : challenge(challenge) {}

bool PassingCard::canUse(Character *obj, Robot *robot)
{
    if (obj && dynamic_cast<Watson *>(obj))
    {
        return obj->getHP() % 2 == 0;
    }
    return false;
}

void PassingCard::use(Character *obj, Robot *robot)
{
    if (challenge == "all" || (robot && robot->getType() == challenge))
    {
        return;
    }
    else
    {
        obj->setEXP(obj->getExp() - 50);
    }
}

string PassingCard::getCardType() const
{
    return challenge;
}
void Robot::createItem(int i, int j)
{
    int p = i * j;
    int s = 0;
    while (p > 0)
    {
        s += p % 10;
        p /= 10;
    }
    s = s > 9 ? s - 9 * (s / 9) : s;

    if (s >= 0 && s <= 1)
    {
        item = new MagicBook();
    }
    else if (s >= 2 && s <= 3)
    {
        item = new EnergyDrink();
    }
    else if (s >= 4 && s <= 5)
    {
        item = new FirstAid();
    }
    else if (s >= 6 && s <= 7)
    {
        item = new ExcemptionCard();
    }
    else if (s >= 8 && s <= 9)
    {
        int t = (i * 11 + j) % 4;
        switch (t)
        {
        case 0:
            item = new PassingCard("RobotS");
            break;
        case 1:
            item = new PassingCard("RobotC");
            break;
        case 2:
            item = new PassingCard("RobotSW");
            break;
        default:
            item = new PassingCard("all");
            break;
        }
    }
}

string Sherlock::getName() const
{
    return "Sherlock";
}

string Watson::getName() const
{
    return "Watson";
}

string Criminal::getName() const
{
    return "Criminal";
}

BaseBag::BaseBag(Character *obj) : head(nullptr), item_count(0), obj(obj), robot(robot) {}
BaseBag::~BaseBag()
{
    Node *current = head;
    while (current)
    {
        Node *next = current->next;
        delete current->item;
        delete current;
        current = next;
    }
}
bool BaseBag::insert(BaseItem *item)
{
    if (item_count < max_items)
    {
        head = new Node(item, head);
        item_count++;
        return true;
    }
    return false;
}

BaseItem *BaseBag::get()
{
    Node *current = head;
    Node *previous = nullptr;
    while (current)
    {
        if (current->item->canUse(obj, robot))
        {
            BaseItem *item = current->item;
            if (previous)
            {
                previous->next = current->next;
                current->next = head;
                head = current;
            }
            else
            {
                head = head->next;
            }
            item_count--;
            return item;
        }
        previous = current;
        current = current->next;
    }
    return nullptr;
}

BaseItem *BaseBag::get(ItemType itemType)
{
    Node *current = head;
    Node *previous = nullptr;
    while (current)
    {
        if (current->item->getType() == itemType && current->item->canUse(obj, robot))
        {
            BaseItem *item = current->item;
            if (previous)
            {
                previous->next = current->next;
                current->next = head;
                head = current;
            }
            else
            {
                head = head->next;
            }
            item_count--;
            return item;
        }
        previous = current;
        current = current->next;
    }
    return nullptr;
}

string BaseBag::listname() const
{
    string result;
    Node *current = head;
    bool first = true;
    while (current)
    {
        if (first)
        {
            result += ";" + current->item->getItemName();
            first = false;
        }
        else
        {
            result += "," + current->item->getItemName();
        }
        current = current->next;
    }
    return result;
}

string BaseBag::str() const
{
    return "Bag[count=" + to_string(item_count) + listname() + "]";
}

int BaseBag::getCount() const
{
    return item_count;
}

int SherlockBag::getCount() const
{
    return item_count;
}

int WatsonBag::getCount() const
{
    return item_count;
}

SherlockBag::SherlockBag(Sherlock *sherlock) : BaseBag(sherlock), head(nullptr), item_count(0), max_items(13) {}

SherlockBag::~SherlockBag()
{
    while (head != nullptr)
    {
        Node *temp = head;
        head = head->next;
        delete temp->item;
        delete temp;
    }
}

bool SherlockBag::insert(BaseItem *item)
{
    if (item_count >= max_items)
    {
        return false;
    }

    Node *newNode = new Node(item);
    newNode->next = head;
    head = newNode;
    item_count++;
    return true;
}

BaseItem *SherlockBag::get()
{
    if (item_count == 0)
    {
        return nullptr;
    }

    Node *temp = head;
    BaseItem *item = temp->item;
    head = head->next;
    delete temp;
    item_count--;
    return item;
}

BaseItem *SherlockBag::get(ItemType itemType)
{
    if (item_count == 0)
    {
        return nullptr;
    }

    Node *prev = nullptr;
    Node *current = head;

    while (current != nullptr)
    {
        if (current->item->getType() == itemType)
        {
            if (prev == nullptr)
            {
                head = current->next;
            }
            else
            {
                prev->next = current->next;
                current->next = head;
                head = current;
                prev = head;
            }
            BaseItem *item = current->item;
            delete current;
            item_count--;
            return item;
        }
        prev = current;
        current = current->next;
    }

    return nullptr;
}

void SherlockBag::exchangeWithW(WatsonBag &watsonBag)
{
    BaseItem *item = nullptr;
    while ((item = get(ItemType::PASSING_CARD)) != nullptr)
    {
        watsonBag.insert(item);
    }

    while ((item = watsonBag.get(ItemType::EXCEMPTION_CARD)) != nullptr)
    {
        insert(item);
    }
}

string SherlockBag::listname() const
{
    string result;
    Node *current = head;
    bool first = true;
    while (current)
    {
        if (first)
        {
            result += ";" + current->item->getItemName();
            first = false;
        }
        else
        {
            result += "," + current->item->getItemName();
        }
        current = current->next;
    }
    return result;
}

string SherlockBag::str() const
{
    return "Bag[count=" + to_string(item_count) + listname() + "]";
}

WatsonBag::WatsonBag(Watson *watson) : BaseBag(watson), head(nullptr), item_count(0), max_items(15) {}

WatsonBag::~WatsonBag()
{
    while (head != nullptr)
    {
        Node *temp = head;
        head = head->next;
        delete temp->item;
        delete temp;
    }
}

bool WatsonBag::insert(BaseItem *item)
{
    if (item_count >= max_items)
    {
        return false;
    }

    Node *newNode = new Node(item);
    newNode->next = head;
    head = newNode;
    item_count++;
    return true;
}

BaseItem *WatsonBag::get()
{
    if (item_count == 0)
    {
        return nullptr;
    }

    Node *temp = head;
    BaseItem *item = temp->item;
    head = head->next;
    delete temp;
    item_count--;
    return item;
}

BaseItem *WatsonBag::get(ItemType itemType)
{
    if (item_count == 0)
    {
        return nullptr;
    }

    Node *prev = nullptr;
    Node *current = head;

    while (current != nullptr)
    {
        if (current->item->getType() == itemType)
        {
            if (prev == nullptr)
            {
                head = current->next;
            }
            else
            {
                prev->next = current->next;
                current->next = head;
                head = current;
                prev = head;
            }
            BaseItem *item = current->item;
            delete current;
            item_count--;
            return item;
        }
        prev = current;
        current = current->next;
    }

    return nullptr;
}

void WatsonBag::exchangewithS(SherlockBag &sherlockBag)
{
    BaseItem *item = nullptr;
    while ((item = get(ItemType::EXCEMPTION_CARD)) != nullptr)
    {
        sherlockBag.insert(item);
    }

    while ((item = sherlockBag.get(ItemType::PASSING_CARD)) != nullptr)
    {
        insert(item);
    }
}

string WatsonBag::listname() const
{
    string result;
    Node *current = head;
    bool first = true;
    while (current)
    {
        if (first)
        {
            result += ";" + current->item->getItemName();
            first = false;
        }
        else
        {
            result += "," + current->item->getItemName();
        }
        current = current->next;
    }
    return result;
}

string WatsonBag::str() const
{
    return "Bag[count=" + to_string(item_count) + listname() + "]";
}
bool StudyPinkProgram::isStop() const
{
    return sherlock->getHP() == 1 || watson->getHP() == 1 || sherlock->getCurrentPosition().isEqual(criminal->getCurrentPosition()) || watson->getCurrentPosition().isEqual(criminal->getCurrentPosition());
}

void StudyPinkProgram::run(bool verbose)
{
    for (int istep = 0; istep < config->num_steps; ++istep)
    {
        for (int i = 0; i < arr_mv_objs->size(); ++i)
        {
            arr_mv_objs->get(i)->move();
            if (isStop())
            {
                printStep(istep);
                break;
            }
            if (verbose)
            {
                printStep(istep);
            }

            if (arr_mv_objs->get(i)->getName() == "Sherlock")
            {
                Position sherlock_pos = arr_mv_objs->get(i)->getCurrentPosition();

                for (int j = 0; j < arr_mv_objs->size(); ++j)
                {
                    if (i != j && (arr_mv_objs->get(j)->getName() == "RobotS" || arr_mv_objs->get(j)->getName() == "RobotSW" || arr_mv_objs->get(j)->getName() == "RobotC" || arr_mv_objs->get(j)->getName() == "RobotW"))
                    {
                        Position robotPos = arr_mv_objs->get(j)->getCurrentPosition();
                        if (sherlock_pos == robotPos)
                        {
                            if (arr_mv_objs->get(j)->getName() == "RobotW")
                            {
                                RobotW *robotW = dynamic_cast<RobotW *>(arr_mv_objs->get(j));
                                BaseItem *item = robotW->getItem();
                                if (item != nullptr)
                                {
                                    SherlockBag sherlockBag(sherlock);
                                    sherlockBag.insert(item);
                                }
                            }
                            else if (arr_mv_objs->get(j)->getName() == "RobotS")
                            {
                                RobotS *robotS = dynamic_cast<RobotS *>(arr_mv_objs->get(j));
                                if (arr_mv_objs->get(i)->getExp() > 400)
                                {
                                    BaseItem *item = robotS->getItem();
                                    if (item != nullptr)
                                    {
                                        SherlockBag sherlockBag(sherlock);
                                        sherlockBag.insert(item);
                                    }
                                }
                                else
                                {
                                    arr_mv_objs->get(i)->setEXP(arr_mv_objs->get(i)->getExp() * 0.9);
                                }
                            }
                            else if (arr_mv_objs->get(j)->getName() == "RobotSW")
                            {
                                RobotSW *robotSW = dynamic_cast<RobotSW *>(arr_mv_objs->get(j));
                                if (arr_mv_objs->get(i)->getExp() > 300 && arr_mv_objs->get(i)->getHP() > 335)
                                {
                                    BaseItem *item = robotSW->getItem();
                                    if (item != nullptr)
                                    {
                                        SherlockBag sherlockBag(sherlock);
                                        sherlockBag.insert(item);
                                    }
                                }
                                else
                                {
                                    arr_mv_objs->get(i)->setHP(arr_mv_objs->get(i)->getHP() * 0.85);
                                    arr_mv_objs->get(i)->setEXP(arr_mv_objs->get(i)->getExp() * 0.85);
                                }
                            }
                            else if (arr_mv_objs->get(j)->getName() == "RobotC")
                            {
                                RobotC *robotC = dynamic_cast<RobotC *>(arr_mv_objs->get(j));
                                if (arr_mv_objs->get(i)->getExp() > 500)
                                {
                                    return;
                                }
                                else
                                {
                                    BaseItem *item = robotC->getItem();
                                    if (item != nullptr)
                                    {
                                        SherlockBag sherlockBag(sherlock);
                                        sherlockBag.insert(item);
                                    }
                                }
                            }
                        }
                    }
                }
            }

            else if (arr_mv_objs->get(i)->getName() == "Watson")
            {
                Position watson_pos = arr_mv_objs->get(i)->getCurrentPosition();

                for (int j = 0; j < arr_mv_objs->size(); ++j)
                {
                    if (i != j && (arr_mv_objs->get(j)->getName() == "RobotS" || arr_mv_objs->get(j)->getName() == "RobotSW" || arr_mv_objs->get(j)->getName() == "RobotC" || arr_mv_objs->get(j)->getName() == "RobotW"))
                    {
                        Position robotPos = arr_mv_objs->get(j)->getCurrentPosition();
                        if (watson_pos == robotPos)
                        {
                            if (arr_mv_objs->get(j)->getName() == "RobotW")
                            {
                                RobotW *robotW = dynamic_cast<RobotW *>(arr_mv_objs->get(j));
                                if (arr_mv_objs->get(i)->getHP() > 350)
                                {
                                    BaseItem *item = robotW->getItem();
                                    if (item != nullptr)
                                    {
                                        WatsonBag watsonBag(watson);
                                        watsonBag.insert(item);
                                    }
                                }
                                else
                                {
                                    arr_mv_objs->get(i)->setHP(arr_mv_objs->get(i)->getHP() * 0.95);
                                }
                            }
                            else if (arr_mv_objs->get(j)->getName() == "RobotS")
                            {
                            }
                            else if (arr_mv_objs->get(j)->getName() == "RobotSW")
                            {
                                RobotSW *robotSW = dynamic_cast<RobotSW *>(arr_mv_objs->get(j));
                                if (arr_mv_objs->get(i)->getExp() > 600 && arr_mv_objs->get(i)->getHP() > 165)
                                {
                                    BaseItem *item = robotSW->getItem();
                                    if (item != nullptr)
                                    {
                                        WatsonBag watsonBag(watson);
                                        watsonBag.insert(item);
                                    }
                                }
                                else
                                {
                                    arr_mv_objs->get(i)->setHP(arr_mv_objs->get(i)->getHP() * 0.85);
                                    arr_mv_objs->get(i)->setEXP(arr_mv_objs->get(i)->getExp() * 0.85);
                                }
                            }
                            else if (arr_mv_objs->get(j)->getName() == "RobotC")
                            {
                                RobotC *robotC = dynamic_cast<RobotC*>(arr_mv_objs->get(j));

                                BaseItem *item = robotC->getItem();
                                if (item != nullptr)
                                {
                                    WatsonBag watsonBag(watson);
                                    watsonBag.insert(item);
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    printResult();
}

StudyPinkProgram::~StudyPinkProgram()
{
    delete arr_mv_objs;
    delete criminal;
    delete watson;
    delete sherlock;
    delete map;
    delete config;
}

////////////////////////////////////////////////
/// END OF STUDENT'S ANSWER
////////////////////////////////////////////////
