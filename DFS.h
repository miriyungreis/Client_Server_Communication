//
// Created by arye on 21/01/2020.
//

#ifndef CLIENT_SERVER_COMMUNICATION__DFS_H_
#define CLIENT_SERVER_COMMUNICATION__DFS_H_

#include "Searcher.h"
#include "Searchable.h"
#include "SearchableMatrix.h"
#include <string>
#include <algorithm>
#include <queue>
#include <stack>

using namespace std;

template<class T>
class DFS : public Searcher<T> {

 private:
  int nodes_evaluated_counter = 0;

 public:

  /**
* deep copy implementation for DFS.
* @return
*/
  virtual DFS<T> *deepCopy() override {
    DFS<T> *copy = new DFS<T>();
    return copy;
  }

  /**
   * the algorithm DFS
   * @param searchable_obj
   * @return
   */
  vector<State<T> *> search(Searchable<T> *searchable_obj) {
    SearchableMatrix *m = dynamic_cast<SearchableMatrix *>(searchable_obj);
    // our stack for dfs algorithm
    stack<State<T> *> dfs_stack;
    // list of possible neighbours
    vector<State<T> *> successors;
    // list of visited
    vector<State<T> *> visited;
    // start state and goal state
    State<T> *start_state = searchable_obj->getInitialState();
    State<T> *goal_state = searchable_obj->getGoalState();
    //push the first state to the stack and mark it visited
    start_state->setCameFrom(nullptr);
    start_state->setIsVisited(true);
    dfs_stack.push(start_state);
    visited.push_back(start_state);
    this->nodes_evaluated_counter++;
    // loop
    while (!dfs_stack.empty()) {
      // pop the next vertex to work on
      State<T> *element = dfs_stack.top();
      dfs_stack.pop();
      // check if it was visited
      for (State<T> *v1 : visited) {
        if (v1->getState() == element->getState()) {
          element->setIsVisited(true);
        }
      }
      // if was not visited, mark visited
      if (!element->getIsVisited()) {
        element->setIsVisited(true);
        visited.push_back(element);
      }
      this->nodes_evaluated_counter++;
      // get neighbours
      successors = searchable_obj->getAllPossibleStates(element);
      for (State<T> *s : successors) {
        for (State<T> *v : visited) {
          if (v->getState() == s->getState()) {
            s->setIsVisited(true);
          }
        }
        if (!s->getIsVisited()) {
          s->setCameFrom(element);
          s->setCost(element->getCost() + s->getCost());
          s->setIsVisited(true);
          visited.push_back(s);
          dfs_stack.push(s);
        }
        // 3. If n is the goal state,
        if (goal_state->getState() == s->getState()) {
          // backtrace path to n (through recorded parents) and return path.
          return m->backTracePath(s);
        }
      }
    }
    for (State<T> *v : visited) {
      if (v->getState() == goal_state->getState()) {
        return m->backTracePath(v);
      }
    }
  }

  /**
   * getter
   * @return
   */
  int getNumberOfNodesEvaluated() override {
    return this->nodes_evaluated_counter;
  }
};

#endif //CLIENT_SERVER_COMMUNICATION__DFS_H_
