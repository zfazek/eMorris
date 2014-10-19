#pragma once

#include <vector>
#include <string>

class Mill;

class Move {

    private:
        static const int nActions = 10;
        static constexpr double epsilon = 1e-6;
        std::vector<Move*> children;
        double nVisits = 0;
        double totValue = 0;
        int idx = 0;
        Mill *mill;


    public:

        Move(Mill *mill);
        ~Move();
        std::vector<Move*> getChildren();
        void selectAction();
        int arity();
        Move *getBest();
        void print();

        std::string currMove;

    private:

        void expand();
        Move *select();
        bool isLeaf();
        int evaluate();
        void updateStats(int value);
        double nextDouble();

};

