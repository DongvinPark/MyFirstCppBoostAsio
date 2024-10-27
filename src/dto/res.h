//
// Created by user on 2024-10-27.
//

#ifndef RES_H
#define RES_H
#include <string>


class res {
  private:
  int key = 0;
  std::string val;
  public:
  res(const int k, const std::string &v) : key(k), val(v) {}
  int getKey() const {return key;}
  [[nodiscard]] std::string getVal() const {return val;}
};



#endif //RES_H
