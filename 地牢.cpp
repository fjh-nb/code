#include <iostream>
#include <vector>
#include <memory>
#include <string>
#include <cstdlib>
using namespace std;

// ==================== Entity ====================
class Entity {
private:
    string name_;
    int id_;
public:
    Entity(const string& name, int id) : name_(name), id_(id) {}
    virtual ~Entity() = default;
    string getName() const { return name_; }
    int getId() const { return id_; }
    virtual string getType() const = 0;
};

// ==================== Character ====================
class Character : public Entity {
protected:
    int hp_, atk_, def_;
public:
    Character(const string& name, int id, int hp, int atk, int def)
        : Entity(name, id), hp_(hp), atk_(atk), def_(def) {
    }
    virtual ~Character() = default;

    virtual int getAttackPower() const { return atk_; }

    void takeDamage(int damage) {
        int actual = damage - def_;
        if (actual < 1) actual = 1;
        hp_ -= actual;
        if (hp_ < 0) hp_ = 0;
        cout << getName() << " 受到 " << actual
            << " 点伤害，剩余生命 " << hp_ << endl;
        if (hp_ == 0) cout << getName() << " 倒下了！" << endl;
    }

    void attack(Character& target) {
        cout << getName() << " 攻击 " << target.getName() << "！" << endl;
        target.takeDamage(getAttackPower());
    }

    bool isAlive() const { return hp_ > 0; }
    int getHp() const { return hp_; }
    int getAtk() const { return atk_; }
    int getDef() const { return def_; }

    void heal(int amount) {
        hp_ += amount;
        cout << getName() << " 恢复 " << amount
            << " 点生命，当前生命 " << hp_ << endl;
    }
};

// ==================== Monster ====================
class Monster : public Character {
private:
    int stunTurns_;
public:
    Monster(const string& name, int id, int hp, int atk, int def)
        : Character(name, id, hp, atk, def), stunTurns_(0) {
    }

    string getType() const override { return "怪物"; }

    bool isStunned() const { return stunTurns_ > 0; }
    void stun(int turns) { stunTurns_ = turns; }
    void tickStun() { if (stunTurns_ > 0) stunTurns_--; }
};

// ==================== Hero ====================
class Hero : public Character {
private:
    int cdHeal_, cdStrike_, cdBind_, cdRage_;
    int rageTurns_;
public:
    Hero(const string& name, int id, int hp, int atk, int def)
        : Character(name, id, hp, atk, def),
        cdHeal_(0), cdStrike_(0), cdBind_(0), cdRage_(0), rageTurns_(0) {
    }

    string getType() const override { return "勇者"; }

    int getAttackPower() const override {
        return rageTurns_ > 0 ? atk_ * 2 : atk_;
    }

    bool canHeal() const { return cdHeal_ == 0; }
    bool canStrike() const { return cdStrike_ == 0; }
    bool canBind() const { return cdBind_ == 0; }
    bool canRage() const { return cdRage_ == 0; }

    void useHeal() {
        cout << "勇者释放【治疗术】" << endl;
        heal(30);
        cdHeal_ = 3;
    }

    void useStrike(Character& target) {
        cout << "勇者释放【强力打击】" << endl;
        target.takeDamage(40);  // 固定伤害，无视防御
        cdStrike_ = 2;
    }

    void useBind(Monster& target) {
        cout << "勇者释放【禁锢术】，" << target.getName() << " 被禁锢 1 回合！" << endl;
        target.stun(1);
        cdBind_ = 4;
    }

    void useRage() {
        cout << "勇者释放【狂暴】，下一次攻击造成双倍伤害！" << endl;
        rageTurns_ = 1;
        cdRage_ = 5;
    }

    void tickCooldowns() {
        if (cdHeal_ > 0) cdHeal_--;
        if (cdStrike_ > 0) cdStrike_--;
        if (cdBind_ > 0) cdBind_--;
        if (cdRage_ > 0) cdRage_--;
        if (rageTurns_ > 0) rageTurns_--;
    }

    void showSkills() const {
        cout << "技能：";
        cout << "[1]治疗";
        if (cdHeal_ > 0) cout << "(CD:" << cdHeal_ << ")";
        cout << " [2]强击";
        if (cdStrike_ > 0) cout << "(CD:" << cdStrike_ << ")";
        cout << " [3]禁锢";
        if (cdBind_ > 0) cout << "(CD:" << cdBind_ << ")";
        cout << " [4]狂暴";
        if (cdRage_ > 0) cout << "(CD:" << cdRage_ << ")";
        if (rageTurns_ > 0) cout << " ★狂暴中";
        cout << endl;
    }
};

// ==================== Room ====================
class Room {
private:
    int roomNumber_;
    string description_;
    vector<shared_ptr<Monster>> monsters_;
public:
    Room(int roomNumber, const string& description)
        : roomNumber_(roomNumber), description_(description) {
    }

    void addMonster(shared_ptr<Monster> m) { monsters_.push_back(m); }

    Monster* getFirstAliveMonster() {
        for (auto& m : monsters_)
            if (m->isAlive()) return m.get();
        return nullptr;
    }

    void removeDead() {
        for (auto it = monsters_.begin(); it != monsters_.end(); ) {
            if (!(*it)->isAlive()) it = monsters_.erase(it);
            else ++it;
        }
    }

    void monstersAttack(Hero& hero) {
        for (auto& m : monsters_) {
            if (!m->isAlive() || !hero.isAlive()) continue;
            if (m->isStunned()) {
                cout << m->getName() << " 被禁锢，无法行动！" << endl;
                m->tickStun();
            }
            else {
                m->attack(hero);
            }
        }
    }

    void enter() {
        cout << "\n=== 房间 " << roomNumber_ << "：" << description_ << " ===" << endl;
        if (!monsters_.empty()) {
            cout << "房间里有 " << monsters_.size() << " 个怪物：" << endl;
            for (auto& m : monsters_) {
                cout << "  怪物 " << m->getName()
                    << " | 生命:" << m->getHp()
                    << " 攻击:" << m->getAtk()
                    << " 防御:" << m->getDef() << endl;
            }
        }
        else {
            cout << "房间已空。" << endl;
        }
    }

    bool isCleared() const { return monsters_.empty(); }
    int getRoomNumber() const { return roomNumber_; }
    string getDescription() const { return description_; }
};

// ==================== Dungeon ====================
class Dungeon {
private:
    vector<shared_ptr<Room>> rooms_;
    string name_;
public:
    Dungeon(const string& name) : name_(name) {}
    void addRoom(shared_ptr<Room> room) { rooms_.push_back(room); }
    shared_ptr<Room> getRoom(int index) {
        if (index >= 0 && index < (int)rooms_.size()) return rooms_[index];
        return nullptr;
    }
    int getRoomCount() const { return rooms_.size(); }
    string getName() const { return name_; }
};

// ==================== main ====================
int main() {
    string heroName;
    cout << "请输入勇者的名字: ";
    cin >> heroName;

    Hero hero(heroName, 1, 100, 20, 10);
    Dungeon dungeon("暗影深渊");

    auto goblin = make_shared<Monster>("哥布林", 101, 30, 15, 5);
    auto skeleton = make_shared<Monster>("骷髅兵", 102, 60, 25, 10);
    auto dragon = make_shared<Monster>("巨龙", 103, 100, 30, 15);

    auto room1 = make_shared<Room>(1, "入口大厅");
    room1->addMonster(goblin);
    auto room2 = make_shared<Room>(2, "阴暗走廊");
    room2->addMonster(skeleton);
    auto room3 = make_shared<Room>(3, "龙巢");
    room3->addMonster(dragon);

    dungeon.addRoom(room1);
    dungeon.addRoom(room2);
    dungeon.addRoom(room3);

    int currentRoom = 0;
    auto room = dungeon.getRoom(currentRoom);
    room->enter();

    bool gameOver = false;
    while (!gameOver) {
        cout << "\n勇者 " << hero.getName()
            << " | 生命:" << hero.getHp()
            << " 攻击:" << hero.getAtk()
            << " 防御:" << hero.getDef() << endl;
        hero.showSkills();
        cout << "指令：[a]普攻 [1-4]技能 [w]前进 [q]退出" << endl;
        cout << ">>> ";
        char cmd;
        cin >> cmd;

        bool acted = false;

        switch (cmd) {
        case 'a': {
            Monster* m = room->getFirstAliveMonster();
            if (!m) { cout << "没有活着的怪物！" << endl; break; }
            hero.attack(*m);
            acted = true;
            break;
        }
        case '1':
            if (!hero.canHeal()) { cout << "治疗术冷却中！" << endl; break; }
            hero.useHeal();
            acted = true;
            break;
        case '2': {
            Monster* m = room->getFirstAliveMonster();
            if (!m) { cout << "没有活着的怪物！" << endl; break; }
            if (!hero.canStrike()) { cout << "强力打击冷却中！" << endl; break; }
            hero.useStrike(*m);
            acted = true;
            break;
        }
        case '3': {
            Monster* m = room->getFirstAliveMonster();
            if (!m) { cout << "没有活着的怪物！" << endl; break; }
            if (!hero.canBind()) { cout << "禁锢术冷却中！" << endl; break; }
            hero.useBind(*m);
            acted = true;
            break;
        }
        case '4':
            if (!hero.canRage()) { cout << "狂暴冷却中！" << endl; break; }
            hero.useRage();
            acted = true;
            break;
        case 'w': {
            if (!room->isCleared()) {
                cout << "还没清空这个房间！" << endl;
                break;
            }
            currentRoom++;
            if (currentRoom >= dungeon.getRoomCount()) {
                cout << "\n勇者 " << hero.getName() << " 成功通关地牢！" << endl;
                gameOver = true;
                break;
            }
            system("pause");
            system("cls");
            room = dungeon.getRoom(currentRoom);
            room->enter();
            break;
        }
        case 'q':
            gameOver = true;
            break;
        default:
            cout << "未知指令！" << endl;
            break;
        }

        // ============ 回合结算 ============
        if (acted && hero.isAlive()) {
            // 1. 移除死亡怪物
            room->removeDead();

            // 2. 存活怪物反击
            if (!room->isCleared() && hero.isAlive()) {
                cout << "\n--- 怪物回合 ---" << endl;
                room->monstersAttack(hero);
            }

            // 3. 冷却递减
            hero.tickCooldowns();
        }

        // ============ 死亡检查 ============
        if (!hero.isAlive()) {
            cout << "\n勇者 " << hero.getName() << " 倒在了地牢中..." << endl;
            gameOver = true;
        }
    }

    cout << "\n游戏结束。" << endl;
    return 0;
}