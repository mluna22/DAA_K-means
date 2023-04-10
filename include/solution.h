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
   * @brief Calculates the sum of distances of the solution
   */
  const double evaluate(const Problem& problem) {
    double sum_of_distances{0};
    for (int i{0}; i < problem.size(); ++i) {  // por cada punto
      double min_distance{INFINITY};
      for (int j{0}; j < points_.size(); ++j) {  // por cada punto de servicio
        double distance{euclidean_distance(problem[i], points_[j])};
        if (distance < min_distance) {
          min_distance = distance;
        }
      }
      sum_of_distances += min_distance;
    }
    return sum_of_distances;
  }

  /**
   * @brief Calculates the sum of distances of the solution and the distances of each point to the solution
   */
  const double evaluate(const Problem& problem, std::vector<std::pair<double, int>>& distances) {
    double sum_of_distances{0};
    distances = std::vector<std::pair<double, int>>(problem.size(), {0, 0});
    for (int i{0}; i < problem.size(); ++i) {  // por cada punto
      double min_distance{INFINITY};
      for (int j{0}; j < points_.size(); ++j) {  // por cada punto de servicio
        double distance{euclidean_distance(problem[i], points_[j])};
        if (distance < min_distance) {
          min_distance = distance;
          distances[i] = {distance, j};
        }
      }
      sum_of_distances += min_distance;
    }
    double result_check{0};
    for (int i{0}; i < problem.size(); ++i) {
      result_check += distances[i].first;
    }
    
    return sum_of_distances;
  }

  const bool operator==(const Solution& other) {
    if (points_.size() != other.points_.size()) return false;
    for (int i{0}; i < points_.size(); ++i) { // por cada punto
      if (points_[i].size() != other.points_[i].size()) return false;
      for (int j{0}; j < points_[i].size(); ++j) { // por cada dimension
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

  Solution local_search(const Problem& problem) {
    // Intercambio, inserción y eliminación
    Solution best_solution(*this);
    Solution new_solution(*this);
    std::vector<std::pair<double, int>> distances;
    double best_solution_value{evaluate(problem, distances)};
    double new_solution_value{best_solution_value};
    do {
      best_solution = new_solution;
      best_solution_value = new_solution_value;

      new_solution = best_solution.exchange_search(problem);
      new_solution_value = new_solution.evaluate(problem);
      if (new_solution_value < best_solution_value) {
        continue;
      }
      new_solution = best_solution.insertion_search(problem);
      new_solution_value = new_solution.evaluate(problem);
      if (new_solution_value < best_solution_value) {
        continue;
      }
      new_solution = best_solution.elimination_search(problem);
      new_solution_value = new_solution.evaluate(problem);
    } while (new_solution_value < best_solution_value);
    return best_solution;
  }
  
 private:
  // Centroids if kmeans, points of service if grasp
  std::vector<Point> points_;
  int dimensions_;
  Solution insertion_search(const Problem& problem) {
    Solution best_solution(*this);
    double best_solution_value{evaluate(problem)};
    float penalty{0.2};  // 20% de penalización
    for (int j{0}; j < problem.size(); ++j) { // por cada punto
      Solution new_solution(*this);
      new_solution.push_back(problem[j]);
      if (new_solution.evaluate(problem) < best_solution_value * (1 - penalty)) {
        best_solution = new_solution;
        best_solution_value = new_solution.evaluate(problem);
      }
    }
    return best_solution;
  }

  Solution elimination_search(const Problem& problem) {
    Solution best_solution(*this);
    double best_solution_value{evaluate(problem)};
    float boost{0.05};  // 5% de aumento
    for (int i{0}; i < points_.size(); ++i) { // por cada punto de la solución
      Solution new_solution(*this);
      new_solution.points_.erase(new_solution.points_.begin() + i);
      if (new_solution.evaluate(problem) * (1 - boost) < best_solution_value) {
        best_solution = new_solution;
        best_solution_value = new_solution.evaluate(problem);
      }
    }
    return best_solution;
  }

  Solution exchange_search(const Problem& problem) {
    Solution best_solution(*this);
    double best_solution_value{evaluate(problem)};
    for (int i{0}; i < points_.size(); ++i) { // por cada punto de la solución
      for (int j{0}; j < problem.size(); ++j) { // por cada punto
        Solution new_solution(*this);
        new_solution[i] = problem[j];
        if (new_solution.evaluate(problem) < best_solution_value) {
          best_solution = new_solution;
          best_solution_value = new_solution.evaluate(problem);
        }
      }
    }
    return best_solution;
  }
};

#endif  // SOLUTION_H