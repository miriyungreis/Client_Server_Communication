//
// Created by arye on 20/01/2020.
//

#ifndef CLIENT_SERVER_COMMUNICATION__SEARCHABLEMATRIX_H_
#define CLIENT_SERVER_COMMUNICATION__SEARCHABLEMATRIX_H_


#include "Matrix.h"
#include "Searchable.h"
#include <algorithm>
#include <string>
#include <list>

using namespace std;

class SearchableMatrix : public Searchable<Point> {

 private:
  Matrix matrix;


 public:
  State<Point> *getInitialState() override ;
  State<Point> *getGoalState() override ;
  bool isGoalState(State<Point>*) override ;
  vector<State<Point>*> getAllPossibleStates(State<Point>*) override ;
};

#endif //CLIENT_SERVER_COMMUNICATION__SEARCHABLEMATRIX_H_
