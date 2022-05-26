#pragma once

class Health {
protected:
    sf::Int32 hp_ = 5;

public:
    explicit Health(sf::Int32 hp = 5) : hp_(hp) {}
    
    int gainHP(int n = 1) {
        hp_ += n;
        return HP();
    }

    void setHP(int n) {
        hp_ = n;
    }

    int HP() const {
        return hp_;
    }

    int loseHP(int n = 1) {
        return gainHP(-n);
    }
};

class DamageDealing {
protected:
    sf::Int32 damage_ = 1;

public:
    explicit DamageDealing(sf::Int32 damage = 1) : damage_(damage) {}

    void setDamage(sf::Int32 damage) {
        damage_ = damage;
    }

    sf::Int32 getDamage() const {
        return damage_;
    }

    void dealDamage(Health& obj) {
        obj.loseHP(getDamage());
    }
};

// class Health_and_Damage : public DamageDealing, public Health {};
