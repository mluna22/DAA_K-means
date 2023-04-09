/**
 * Universidad de La Laguna
 * Escuela Superior de Ingeniería y Tecnología
 * Grado en Ingeniería Informática
 * Diseño y Análisis de Algoritmos
 * 
 * @author Miguel Luna García
 * @since 18 Mar 2023
 * @file problem.h
 * @brief Solution Class
 *        This file contains the Solution class declaration
 */

#ifndef SOLUTION_H
#define SOLUTION_H

#include <vector>
#include <cmath>
#include "problem.h"

/**
 * @brief Defines a solution to the clustering problem
*/
class Solution {
 public:
  /**
   * @brief Creates a new solution
   * @param d Number of dimensions
  */
  Solution(int d) {
    dimensions_ = d;
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

  /**
   * @brief Calculates the sum of squared errors of the solution
   */
  const double evaluate(const Problem& problem) {
    double pmedian{0};
    for (int i{0}; i < problem.size(); ++i) {  // for each point
      double min_distance{euclidean_distance(problem[i], points_[0])};
      for (int j{1}; j < points_.size(); ++j) {  // for each centroid
        double distance{euclidean_distance(problem[i], points_[j])};
        if (distance < min_distance && distance > 0) {
          min_distance = distance;
        }
      }
      pmedian += min_distance;
    }
    return pmedian;
  }

  const bool operator==(const Solution& other) {
    if (points_.size() != other.points_.size()) return false;
    for (int i{0}; i < points_.size(); ++i) { // for each point
      if (points_[i].size() != other.points_[i].size()) return false;
      for (int j{0}; j < points_[i].size(); ++j) { // for each dimension
        if (fabs(points_[i][j] - other.points_[i][j]) > 0.001) return false;
      }
    }
    return true;
  }

  const bool operator!=(const Solution& other) {
    return !(*this == other);
  }

  void push_back(Point point) {
    points_.push_back(point);
  }

  const int dimensions() {
    return dimensions_;
  }
  
 private:
  // Centroids if kmeans, points of service if grasp
  std::vector<Point> points_;
  int dimensions_;
};

#endif  // SOLUTION_H