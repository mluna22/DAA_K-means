/**
 * Universidad de La Laguna
 * Escuela Superior de Ingeniería y Tecnología
 * Grado en Ingeniería Informática
 * Diseño y Análisis de Algoritmos
 * 
 * @author Miguel Luna García
 * @since 18 Mar 2023
 * @file problem.h
 * @brief Problem Class
 *        This file contains the Problem class declaration
 */

#ifndef PROBLEM_H
#define PROBLEM_H

#include <vector>
#include <cmath>
#include "utilities.h"

/**
 * @brief Defines the clustering problem (localization problem) 
*/
class Problem {
 public:
  /**
   * @brief Creates a new problem
   * @param n Number of points
   * @param d Number of dimensions
  */
  Problem(int n, int d) {
    for (int i{0}; i < n; ++i) {
      points_.push_back(Point(d));
    }
  }

  const Point& operator[](int i) const {
    return points_[i];
  }

  Point& operator[](int i) {
    return points_[i];
  }

  const int size() const {
    return points_.size();
  }

  const int dimensions() const {
    return points_[0].size();
  }
 private:
  std::vector<Point> points_;
};

#endif  // PROBLEM_H